/********************************************************************************************
 	 	 	 SERIE EMBARCADOS DSP

 	 	 	 Arquivo: EmbarcadosDSP.c

 	 	     Desc.: Arquivo que contem as funcoes principais para execucao
 	 	     	 	do algoritmo DSP que sera apresentado em cada parte da
 	 	     	 	sŽrie.

 	 	     Autor: FSN - 2014

 ********************************************************************************************/


/********************************************************************************************
							Includes
 ********************************************************************************************/
#include "stm32f4xx.h"
#include "EmbarcadosDSP.h"
/********************************************************************************************
							Protipos de funcoes internas:
 ********************************************************************************************/
void vSystickInit(uint32_t dLoad);
void vADCInit(void);
void vDACInit(void);
uint16_t wADCRead(void);
void vDACWrite(Dac_t *pxDac);
/********************************************************************************************
							Variaveis locais
 ********************************************************************************************/
//Estrutura de controle para escrita nos DACS.
Dac_t xAnalog;

//Iremos fazer a filtragem em tempo real, logo os buffers
//possuem comprimento unitario:
int16_t wInput = 0;
int16_t wOutput = 0;

//Por default vamos utilizar filtragem EMA.
EMAStatus_t xGlobalStatus;
EMAFilter_t xEmaFilter;

/********************************************************************************************
							Variaveis publicas
 ********************************************************************************************/
/********************************************************************************************
 	 Funcao: vSystickInit()
 	 Desc : Inicializa o timer periodico e sua interrupcao.
 	 Param: dLoad - Valor da recarga do systick
 	 Ret  : N/A
 ********************************************************************************************/
void vSystickInit(uint32_t dLoad)
{
	//Derruba o systick primeiro:
	SysTick->CTRL = 0x00;

	//Coloca o valor de recarga:
	SysTick->LOAD = dLoad;

	//Habilita timer e sua interrupcao:
	SysTick->CTRL |= 0x07;

}
/********************************************************************************************
 	 Funcao: vADCInit()
 	 Desc : Inicializa o conversor A/D para conversao em modo continuo.
 	 Param: N/A
 	 Ret  : N/A
 ********************************************************************************************/
void vADCInit(void)
{
	//Primeiro configura PA0 como entrada anal—gica:

	//Habilita clock do GPIO:
	RCC->AHB1ENR |= 0x01;

	//COnfigura direcao GPIOA como input analogico:
	GPIOA->MODER |= (0x03 );

	//COnfigura o conversor A/D:

	//Habilita clock do periferico:
	RCC->APB2ENR |= (1 << 8);

	//Desliga conversor A/D:
	ADC1->CR2 &= ~(0x01);

	//COnfigura ADC no canal 0 e em modo de convesao continua:

	ADC1->CR1  = 0x00000000;
	ADC1->CR2 |= (0x01 << 1);

	//Tempo de amostragem:
	ADC1->SMPR2 &= ~(0x07);

	ADC1->SQR1 = 0x00000000;

	ADC->CCR |= (0x01 << 16) ;

	//Volta ao ADC1 e selciona o canal 0:
	ADC1->SQR3 &= ~(0x1F);

	//Canal configurado, habilia conversor A/D:
	ADC1->CR2 |= 0x01;

	//Inicia conversao:
	ADC1->CR2 |= (0x01 << 30);

}
/********************************************************************************************
 	 Funcao: vDACInit()
 	 Desc : Inicializa o conversor D/A.
 	 Param: N/A
 	 Ret  : N/A
 ********************************************************************************************/
void vDACInit(void)
{
	//Habilita clock do GPIO usado para saida do D/A:
	RCC->AHB1ENR |= 0x01;

	//COnfigura PA4:5 para funcao de saida analogica:
	GPIOA->MODER |= (0x03 << 8) | (0x03 << 10);

	//COnfigura DAC1:2 sem FIFO para conversao Direta:

	//Habilia clock do periferico:
	RCC->APB1ENR |= (1 << 29);

	//Derruba o DAC:
	DAC->CR &= ~(0xFFFFFFFF);

	//Sobe o DAC:
	DAC->CR |= (1 << 16) | (0x01 << 0);

	//Esta pronto para uso.

}
/********************************************************************************************
 	 Funcao: wADCRead()
 	 Desc : Efetua a leitura do registro que possui o resultado do A/D.
 	 Param: N/A
 	 Ret  : Resultado de 12bits ja escalado.
 ********************************************************************************************/
uint16_t wADCRead(void)
{
	uint16_t wResultado = 0;

	//Aguarda termino da conversao, se estiver em progresso:
	while(!(ADC1->SR & (1 << 1)));


	//Efetua a leitura do A/D:
	wResultado = ADC1->DR;

	//Inicia conversao:
	ADC1->CR2 |= (0x01 << 30);

	//Retorna valor:
	return(wResultado);
}
/********************************************************************************************
 	 Funcao: wDACWrite()
 	 Desc : Escreve o dado DAC.
 	 Param: N/A
 	 Ret  : Resultado de 12bits ja escalado.
 ********************************************************************************************/

void vDACWrite(Dac_t *pxDac)
{


	//satura valores de entrada:
	if(xAnalog.wAnalog_1 > 4095) xAnalog.wAnalog_1 = 4095;

	if(xAnalog.wAnalog_2 > 4095) xAnalog.wAnalog_2 = 4095;


	switch(pxDac->bChannel)
	{
	case CANAL_1:
		DAC->DHR12R1 = pxDac->wAnalog_1;
	//	for(dI = 0; dI < 0xFF; dI++);
	break;

	case CANAL_2:
		DAC->DHR12R2 = pxDac->wAnalog_2;
	//	for(dI = 0; dI < 0xFF; dI++);
	break;

	default:
		  return;
	}

}
/********************************************************************************************
 	 Funcao: main()
 	 Desc : Rotina principal do sistema.
 	 Param: N/A
 	 Ret  : N/A
 ********************************************************************************************/
int main(void)
{

	//Coloca o microcontrolador em Debug mode:
	DBGMCU->CR |= 0x03;

	//Estou admitindo que seu MCU ja esteja com o clock configurado
	//No startup do GCC ele ja o vem configurado, caso nao seja o caso
	//Caro leitor configure o clock e wdt antes de prosseguir.


	//Inicializamos o nosso filtro EMA:
	xGlobalStatus = EMA_xFilterInit(&xEmaFilter, &wInput, &wOutput, 1 );
	if(kEMAFilterOk != xGlobalStatus) while(1);

	//Configuramos o valor de alpha:
	xGlobalStatus =  EMA_xCoefCalc(&xEmaFilter, 0.156f);

	//COnfigura ADC:
	vADCInit();

	//Configura DAC:
	vDACInit();


	//Inicializa o systick para ele interromper no valor da taxa de
	//amostragem desejada:
	vSystickInit((uint32_t)(CLOCK / (TAXA_AMOSTRAGEM)));

	//
	for(;;);
}

/********************************************************************************************
 	 Funcao: Systick_Handler()
 	 Desc : ISR responsavel por coletar dados vindos da entrada analogica
 	 	 	e deposito de dados processados na saida analogica, executado
 	 	 	periodicamente dependendo da taxa de amostragem.

 	 Param: N/A
 	 Ret  : N/A
 ********************************************************************************************/
void DSPTick(void)
{
	//Aqui efetuamos o processamento:

	//Efetua leitura do ADC e escala o valor adicionando 4 lsbs:
	wInput = (int16_t)(wADCRead() << 4);

	//VAlor computado, procedemos com a filtragem:
	xGlobalStatus = EMA_xDoFilter(&xEmaFilter);

	//O valor filtrado estta em output, podendo ser escalado:
	xAnalog.wAnalog_1 = wOutput;

	//Coloca o valor no DAC:
	vDACWrite(&xAnalog);

}

/********************************************************************************************
				Fim do arquivo.
 ********************************************************************************************/


