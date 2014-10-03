/********************************************************************************************
 	 	 	 SERIE EMBARCADOS DSP

 	 	 	 Arquivo: EmbarcadosDSP.h

 	 	     Desc.: Arquivo que contem as intefaces para funcoes que executarao
 	 	     	 	os algoritmos DSP que serao apresentado em cada parte da
 	 	     	 	série.

 	 	     Autor: FSN - 2014

 ********************************************************************************************/

#ifndef __EMBARCADOSDSP_H_
#define __EMBARCADOSDSP_H_ //Previne compilacao recursiva.
/********************************************************************************************
							Includes
 ********************************************************************************************/
#include <stdint.h>
#include "Filters/AverageDigitalFilter.h"
#include "Filters/EMADigitalFilter.h"


/********************************************************************************************
							Defines
 ********************************************************************************************/
#define CLOCK			  168000000 //clock da CPU em Hz,

						  //Para o primeiro projeto vamos usar as definicoes do DDS.
#define TAXA_AMOSTRAGEM   100000 //Frequencia de amostragem em Hz

//Canais do DAC:
#define CANAL_1			  0x01
#define CANAL_2			  0x02

/********************************************************************************************
  	  	  	  	  	  	  	 Structs
 ********************************************************************************************/
typedef struct
{
	uint16_t wAnalog_1;
	uint16_t wAnalog_2;
	uint8_t  bChannel;
}Dac_t;


/*
 * DSPTick();
 */
extern void DSPTick(void);

/********************************************************************************************
							Fim do arquivo.
 ********************************************************************************************/
#endif
