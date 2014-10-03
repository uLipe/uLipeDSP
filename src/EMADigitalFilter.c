/*
 * EMADigitalFilter.c
 *
 *  Created on: 31/07/2014
 *      Author: felipe.neves
 */

/*
 * 	Dependencias sao incluidas aqui:
 */
#include "Filters/EMADigitalFilter.h"

/*
 *  Variaveis globais:
 */
static uint8_t bFilterOn;

/*
 * enum, estado de operacao da engine de filtro:
 */
enum FilterOp
{
	kEMAOn,
	kEMAOff,
};

/*
 * 		implementação das funcoes:
 *
 */

/*
 * 	EMA_xFilterInit()
 */
EMAStatus_t EMA_xFilterInit(EMAFilterPtr_t pxEma, int16_t *pwInput, int16_t *pwOutput,
							uint16_t wBlockSize)
{
	EMAStatus_t xRet;
	uint32_t dI = 0;

	//Checa argumentos:
	if(0 == pxEma)
	{
		//nao pode inicializar ponteiro nulo:
		xRet = kEMAFilterInvalidParam;

		return(xRet);

	}

	if(0 == pwInput)
	{
		//nao pode inicializar ponteiro nulo:
		xRet = kEMAFilterInvalidParam;

		return(xRet);

	}

	if(0 == pwOutput)
	{
		//nao pode inicializar ponteiro nulo:
		xRet = kEMAFilterInvalidParam;

		return(xRet);

	}

	/*
	 * 	Todo : checar se precisa testar o valor do blocksize
	 */


	//Derruba filtro, engine nao pode executar enquanto inicializamos alguem:

	bFilterOn = kEMAOff;


	//inciacilza buffers:
	pxEma->pwInput = pwInput;
	pxEma->pwOutput = pwOutput;

	//Alpha e beta sao por default 1 e 0
	pxEma->wAplha = 32768;
	pxEma->wBeta  = 0;

	//Coloca os estados iniciais do filtro num valor conhecido:
	for(dI = 0; dI < AVG_SIZE ; dI ++)
	{
		pxEma->awPrevInState[dI] = 0;
	}

	pxEma->awPrevOutState[0] = 0;
	pxEma->awPrevOutState[1] = 0;

	//Inicializa tamanho do bloco a ser filtrado:
	pxEma->wBlockSize = wBlockSize;

	//Buffer inicializado, pode ligar a engine EMA:
	bFilterOn = kEMAOn;


	//tudo correu bem:
	xRet = kEMAFilterOk;

	return(xRet);
}

/*
 * 	EMA_xCoefCalc()
 *
 */
EMAStatus_t EMA_xCoefCalc(EMAFilterPtr_t pxEma, float fAlpha)
{
	EMAStatus_t xRet;

	//Checa argumentos:
	if(0 == pxEma)
	{
		//nao pode inicializar ponteiro nulo:
		xRet = kEMAFilterInvalidParam;

		return(xRet);

	}

	//Derruba filtro:
	bFilterOn = kEMAOff;


	//Calcula alpha e beta para um valor escalado:
	pxEma->wAplha = (int16_t)(32768.0 * fAlpha);
	pxEma->wBeta  = (int16_t)(32768.0 * ( 1.0 - fAlpha));


	//Pode acionar o filtro:
	bFilterOn = kEMAOn;

	xRet = kEMAFilterOk;


	return(xRet);
}

/*
 * 	EMA_xDoFilter()
 *
 */
EMAStatus_t EMA_xDoFilter(EMAFilterPtr_t pxEma)
{
	uint32_t dI = 0;
	uint32_t dJ = 0;
	uint32_t dAverage = 0;
	EMAStatus_t xRet;


	//Checa argumentos:
	if(0 == pxEma)
	{
		//nao pode inicializar ponteiro nulo:
		xRet = kEMAFilterInvalidParam;

		return(xRet);

	}

	if(bFilterOn != kEMAOn)
	{
		//Nao pode filtrar se a engine estiver desligada:
		xRet = kEMAFilterOff;

		return(xRet);
	}


	//Executa nucleo de filtragem:
	for(dI = 0; dI < pxEma->wBlockSize ; ((dI ++) & (pxEma->wBlockSize - 1)))
	{

		//pega primeira amostra:
		pxEma->awPrevInState[0] =
				pxEma->pwInput[dI];

		//calcula média móvel:
		for(dJ = 0; dJ < AVG_SIZE; dJ++)
		{
			dAverage += (AVG_COEF *  pxEma->awPrevInState[dJ]);
		}

		//Executa engine EMA:
		pxEma->awPrevOutState[0] = (dAverage >> 15);

		pxEma->pwOutput[dI] = ((pxEma->awPrevOutState[0] * pxEma->wAplha) +
							 	 	 (pxEma->awPrevOutState[1] * pxEma->wBeta )) >> 15;

		//Atualiza historico da engine
		pxEma->awPrevOutState[1] = pxEma->awPrevOutState[0];


		//E da media movel:
		for(dJ = (AVG_SIZE - 1); dJ != 0; dJ--)
		{
			pxEma->awPrevInState[dJ] = pxEma->awPrevInState[dJ - 1];
		}

		dAverage = 0;
	}


	//Tudo ok com a filtragem:
	xRet = kEMAFilterOk;

	return(xRet);
}
