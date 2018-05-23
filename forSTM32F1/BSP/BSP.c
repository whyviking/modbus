#include "bsp.h"
#include "os_cpu.h"
#include "stm32f10x_rcc.h"
#include "os_cfg.h"
#include "stm32f10x.h"

static void RCC_Configuration(void);
static void Init_LED(void);
static void Init_USART_IO(void);
static void USART_Configuration(uint16_t up_baudrate,uint16_t down_baudrate);
static void NVIC_Configuration(void);
/*
*********************************************************************************************************
*                                           BSP_IntDisAll()
*
* Description : Disable ALL interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntDisAll(void) //关总中断
{
    CPU_IntDis();
}

/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;


    RCC_GetClocksFreq(&rcc_clocks);
    return ((INT32U)rcc_clocks.HCLK_Frequency);
}


/*
*********************************************************************************************************
*                                       BSP_CPU_TickInit()
*
* Description : This function reads CPU registers to determine the CPU clock frequency of the chip in KHz.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  BSP_CPU_TickInit(void)
{
    INT32U  cpu_clk_freq;
    INT32U  cnts;

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */
#if (OS_VERSION >= 30000u)
    cnts         = (cpu_clk_freq / OSCfg_TickRate_Hz);          /* Determine nbr SysTick increments                     */
#else
    cnts         = (cpu_clk_freq / OS_TICKS_PER_SEC);
#endif
    OS_CPU_SysTickInit(cnts);                                   /* Initialize the SysTick.                            */
}


void BSP_Init(void)
{
	RCC_Configuration();
	Init_LED();
	Init_USART_IO();
    USART_Configuration(9600, 9600);
    NVIC_Configuration();
}

/*** 时钟配置函数 ***/
static void RCC_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	
}

static void Init_LED(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    
	/* PA15 运行指示 */
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	    //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //50M时钟速度
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);
	GPIO_SetBits(LED1_PORT, LED1_PIN);                      //turn off led1
    
	/* PB8 通讯指示 */
	GPIO_InitStructure.GPIO_Pin = LED2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	    //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //50M时钟速度
	GPIO_Init(LED2_PORT, &GPIO_InitStructure);
    GPIO_SetBits(LED2_PORT, LED2_PIN);                      //turn off led2
}

/*** usart io init ***/
static void Init_USART_IO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//串口1发送管脚
	GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_PORT, &GPIO_InitStructure);
	//串口1接收管脚
	GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART1_PORT, &GPIO_InitStructure);
	
	
	//串口2发送管脚
	GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART2_PORT, &GPIO_InitStructure);
	//串口2接收管脚
	GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART2_PORT, &GPIO_InitStructure);

	//串口3发送管脚
	GPIO_InitStructure.GPIO_Pin = USART3_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART3_PORT, &GPIO_InitStructure);
	//串口3接收管脚
	GPIO_InitStructure.GPIO_Pin = USART3_RX_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART3_PORT, &GPIO_InitStructure);
	
	
	//串口4发送管脚
	GPIO_InitStructure.GPIO_Pin = USART4_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART4_PORT, &GPIO_InitStructure);
	//串口4接收管脚
	GPIO_InitStructure.GPIO_Pin = USART4_RX_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART4_PORT, &GPIO_InitStructure);
	
	//串口5发送管脚
	GPIO_InitStructure.GPIO_Pin = USART5_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART5_TX_PORT, &GPIO_InitStructure);
	//串口5接收管脚
	GPIO_InitStructure.GPIO_Pin = USART5_RX_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART5_RX_PORT, &GPIO_InitStructure);

    
}

/*** usart config ***/
static void USART_Configuration(uint16_t up_baudrate,uint16_t down_baudrate)
{
	USART_InitTypeDef USART_InitStructure;
    
    //////////////////////////////////////////////////////////////////////////////
	////串口一中断模式
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate            = up_baudrate;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;  
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	 
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE); 
		
	//////////////////////////////////////////////////////////////////////////////
	//串口二中断模式
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate            = down_baudrate;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_2;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);

	//////////////////////////////////////////////////////////////////////////////
	//串口三中断模式
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate            = down_baudrate;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);

	///////////////////////////////////////////////////////////////////////////////
	//串口四中断模式
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate            = down_baudrate;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(UART4, &USART_InitStructure);
	
   // USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	//USART_Cmd(UART4, ENABLE);
	
    //////////////////////////////////////////////////////////////////////////////
	//串口五中断模式
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate            = down_baudrate;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(UART5, &USART_InitStructure);
	
   // USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);  
	//USART_Cmd(UART5, ENABLE);
}


/*** 中断向量配置函数 ***/
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;  
      
    /* Configure one bit for preemption priority -------------------------------- */  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  
      

    /* UART1 -------------------------------------------------------------------- */  
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority  = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  

    /* UART2 -------------------------------------------------------------------- */  
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority  = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
    
    /* UART3 -------------------------------------------------------------------- */  
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority  = 2;    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  

    /* UART4 -------------------------------------------------------------------- */  
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority  = 3;    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
 //   NVIC_Init(&NVIC_InitStructure);  
    
    /* UART5 -------------------------------------------------------------------- */  
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority  = 4;    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  //  NVIC_Init(&NVIC_InitStructure);  

}
