/* !
 * AverageDigitalFilter.h
 * \brief Interface de acesso as rotinas de filtragem digital.
 *
 *  Created on: 30/07/2014
 *      Author: felipe.neves
 */

#ifndef AVERAGEDIGITALFILTER_H_
#define AVERAGEDIGITALFILTER_H_

#include <stdint.h>

/*
 * Constantes do filtro media
 */
#define FILTER_SIZE 8 //Numero de esttados anteriores
#define FILTER_COEF (uint16_t)(32768 / FILTER_SIZE)

/*
 * Estados de operaco do filtro;
 */
enum FilterOpState
{
	kAVGFilterOk = 0,
	kAVGFilterOverflow,
	kAVGFilterSat,
	kAVGFilterInvalidParam,
	kAVGFilterIsOff,
};

typedef enum FilterOpState AVGStatus;


/*
 * 	Estrutura de controle do filtro digital:
 */

struct AVGFilter_
{
	//Buffers de entrada e saida:
	int16_t *pwInput;
	int16_t *pwOutput;

	//Memoria dos estados anteriores do filtro:
	int16_t  awInputPrevState[FILTER_SIZE];

	//Tamanho do bloco a ser filtrado:
	uint16_t wBlockSize;

};


typedef struct AVGFilter_ AVGFilter_t;
typedef struct AVGFilter_* AVGFilterPtr_t;

/*
 * Protototipos das funcoes:
 */


/*!
 * 	AVG_xFilterInit()
 * 	\brief Funcao que inicializa o filtro digital
 *
 * 	\param pxFilter - ponteiro para estrutura de filtro
 *  \param pwInputBUffer - ponteiro para dados de entrada
 * 	\param pwOutputBuffer - ponteiro de destino para os dados filtrados
 * 	\param wBlockSize - tamanho do bloco a ser filtrado
 *
 * 	\return kAVGFilterOk em caso de acao correta ou erros, vide enum acima
 */
AVGStatus AVG_xFilterInit(AVGFilterPtr_t pxFilter, int16_t *pwInputBuffer,
						  int16_t *pwOutputBuffer, uint16_t wBlockSize);

/*!
 * 	AVG_xDoFilter()
 * 	\brief Funcao que filtra os dados usado o algoritmo de media movel
 *
 * 	\param pxFilter - ponteiro para estrutura de controle do filtro
 *
 * 	\return kAVGFilterOk em caso de acao correta ou erros, vide enum acima
 */
AVGStatus AVG_xDoFilter(AVGFilterPtr_t  pxFilter);



/*
 * Fim do arquivo
 */
#endif /* AVERAGEDIGITALFILTER_H_ */
