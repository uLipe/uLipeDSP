/*
 * AverageDigitalFilter.c
 *
 *  Created on: 30/07/2014
 *      Author: felipe.neves
 */


/*
 * Dependencias desse modulo
 */

#include "Filters/AverageDigitalFilter.h"

/*
 * Modo de divisao do filtro:
 */
enum CoefMode
{
	kAVGDivByShift = 0, //divisao usando shift (default)
	kAVGDivByCustom,	//usa codigo de divisao do compilador
};


enum FilterEnabled
{
	kAVGFilterOff = 0,
	kAVGFilterOn,
};

/*
 *  Variaveis locais:
 */

static uint8_t bCoefMode = kAVGDivByShift;
static uint8_t bFilterOn;

/*
 *  Implementacao:
 */

/*
 *	AVG_xFilterInit()
 */
AVGStatus AVG_xFilterInit(AVGFilterPtr_t pxFilter, int16_t *pwInputBuffer,
						  int16_t *pwOutputBuffer, uint16_t wBlockSize)
{
	AVGStatus xRet;
	uint32_t  dI = 0;

	//Checa argumentos:
	if(pwInputBuffer == 0)
	{
		xRet = kAVGFilterInvalidParam;

		return(xRet);
	}

	if(pwOutputBuffer == 0)
	{
		xRet = kAVGFilterInvalidParam;

		return(xRet);
	}

	if(pxFilter == 0)
	{
		xRet = kAVGFilterInvalidParam;

		return(xRet);
	}

	//tamanho do bloco deve ser >= ao numero de estados:
	/*
	 * TODO, precisa mesmo?
	 */

	//Desliga o filtro durante inicializacao:
	bFilterOn = kAVGFilterOff;


	//Inicializa buffers de entrada e saida:
	pxFilter->pwInput  = pwInputBuffer;
	pxFilter->pwOutput = pwOutputBuffer;

	//Checa modo de divisao:
	/*
	 * TODO, implementacao futura
	 */

	//coloca filtro num estado incial conhecido:
	for(dI = 0; dI < FILTER_SIZE; dI++)
	{
		pxFilter->awInputPrevState[dI] = 0;
	}

	//Coloca tamanho do bloco a ser filtrado:
	pxFilter->wBlockSize = wBlockSize;

	//Bloco inicializado, filtro pode operar:
	bFilterOn = kAVGFilterOn;

	xRet = kAVGFilterOk;

	return(xRet);

}
/*
 * 	AVG_xDoFilter
 */
AVGStatus AVG_xDoFilter(AVGFilterPtr_t  pxFilter)
{
	uint32_t dI = 0, dJ = 0;
	uint32_t dAcum = 0;
	AVGStatus xRet;


	//checa argumentos:
	if(pxFilter == 0)
	{
		xRet = kAVGFilterInvalidParam;

		return(xRet);
	}

	//So pode rodar o filtro se ninguem estiver sendo
	//inicializado:
	if(bFilterOn != kAVGFilterOn)
	{
		xRet = kAVGFilterIsOff;

		return(xRet);
	}

	//Filtra todos os elementos de 1 blockSize:
	for(dI = 0; dI < pxFilter->wBlockSize; ((dI++) & (pxFilter->wBlockSize - 1)))
	{
		//Coleta amostra corrente:
		pxFilter->awInputPrevState[0] =
				pxFilter->pwInput[dI];

		//Calcula nucleo do filtro:
		for(dJ = 0; dJ < FILTER_SIZE; dJ++)
		{
			dAcum += (FILTER_COEF * pxFilter->awInputPrevState[dJ]);
		}

		//Coloca na saida:
		pxFilter->pwOutput[dI] = (int16_t)(dAcum >> 15);

		/*
		 * TODO: saturar e controle de overflow
		 */

		//Zera acumulador e atualiza historico:
		dAcum = 0;

		for(dJ = (FILTER_SIZE - 1); dJ != 0 ; dJ--)
		{
			pxFilter->awInputPrevState[dJ] =
					pxFilter->awInputPrevState[dJ - 1];
		}

	}

	//Acao concluida com sucesso:
	xRet = kAVGFilterOk;

	return(xRet);

}
