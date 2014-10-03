/*
 * EMADigitalFilter.h
 *
 *  Created on: 30/07/2014
 *      Author: felipe.neves
 */

#ifndef EMADIGITALFILTER_H_
#define EMADIGITALFILTER_H_

#include <stdint.h>

/*
 * Constantes:
 */

#define AVG_SIZE 8
#define AVG_COEF (uint16_t)(32768 / AVG_SIZE)

/*
 * Estados de operacao do filtro:
 */
enum EMA_OpStates
{
	kEMAFilterOk,
	kEMAFilterInvalidParam,
	kEMAFilterOn,
	kEMAFilterOff
};

typedef enum EMA_OpStates EMAStatus_t;

/*
 * Estrutura de controle do filtro EMA
 */
struct EMAFilter_
{
	//buffers de entrada e saida
	int16_t *pwInput;
	int16_t *pwOutput;

	//Estado de saida anterior:
	int16_t awPrevInState[AVG_SIZE];
	int16_t awPrevOutState[2];

	//Coeficientes:
	int16_t wAplha;
	int16_t wBeta;

	//tamanho do bloco:
	uint16_t wBlockSize;

};

typedef struct EMAFilter_  EMAFilter_t;
typedef struct EMAFilter_* EMAFilterPtr_t;

/*
 * 	Prototipos das funcoes
 */


/*!
 * 	EMA_xFilterInit()
 * 	\brief Inicializa estrutura de filtragem digital
 *
 * 	\param pxEma - ponteiro para estrutura de controle do filtro (nao pode ser 0)
 * 	\param pwInput - ponteiro para buffer de amostras nao filtrada
 * 	\param pwOutput - ponteiro para buffer de amostras filtradas
 * 	\param wBlockSize
 *
 * 	\return kEMAFilterOk em caso de sucesso ou erros, vide enum EMAOpStates
 *
 * 	\note O tamanho do buffer pwInput e pwOutput DEVE ser potencia de 2
 */
EMAStatus_t EMA_xFilterInit(EMAFilterPtr_t pxEma, int16_t *pwInput, int16_t *pwOutput,
							uint16_t wBlockSize);

/*!
 * 	EMA_xCoefCalc()
 * 	\brief Escala coeficiente alpha, que determina a frequencia de corte
 * 	\param pxEma - ponteiro para estrutura de controle dofiltro
 * 	\param fAlpha - coeficiente alpha do filtro, deve ser ponto flutuante
 *
 * 	\return kEMAFilterOk em caso de sucesso ou erros, vide enum EMAOpStates
 */
EMAStatus_t EMA_xCoefCalc(EMAFilterPtr_t pxEma, float fAlpha);


/*!
 * 	EMA_xDoFilter()
 * 	\brief Filtra uma ou um conjunto de amostras usando o algoritmo EMA
 * 	\param pxEma - ponteiro para estrutura de controle do filtro
 *
 * 	\return kEMAFilterOk em caso de sucesso ou erros, vide enum EMAOpStates
 */
EMAStatus_t EMA_xDoFilter(EMAFilterPtr_t pxEma);


/*
 * Fim do arquivo.
 */
#endif /* EMADIGITALFILTER_H_ */
