/****************************************************************************
**
**
**
**
**
**
****************************************************************************/

#include "main.h"

void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);

/**
  * @brief 主函数。
  */
int main(void)
{
  /* 使能L1 Cache */
  CPU_CACHE_Enable();
  
  /* STM32H7xx HAL库初始化：
        - 此时系统用的还是H7自带的64MHz HSI时钟:
        - 调用函数HAL_InitTick，初始化滴答时钟中断为1毫秒。
        - 设置NVIV群组优先级分组为4。
  */
  HAL_Init();
  
  /* 配置系统时钟到400MHz */
  SystemClock_Config();
  
  while (1)
  {

  }
}

/**
  * @brief  系统时钟设置
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 160
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  无
  * @retval 无
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;
  
  /* 使能Supply configuration update */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /* 锁住Supply configuration update */
//  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
  
  /* 选择LDO稳压器输出的电压范围
        - 可选VOS1，VOS2和VOS3，不同范围对应不同的Flash读速度
        - 这里选择使用VOS1，电压范围1.15V - 1.26V */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  
  /* 使能HSE，并选择HSE作为PLL时钟源 */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
   
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* 选择PLL作为系统时钟源，设置各总线时钟分频器 */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK   | \
                                  RCC_CLOCKTYPE_HCLK    | \
                                  RCC_CLOCKTYPE_D1PCLK1 | \
                                  RCC_CLOCKTYPE_PCLK1   | \
                                  RCC_CLOCKTYPE_PCLK2   | \
                                  RCC_CLOCKTYPE_D3PCLK1);
  
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
  
  /* IO高速模式 */
//  __HAL_RCC_CSI_ENABLE() ;
//  __HAL_RCC_SYSCFG_CLK_ENABLE() ;
//  HAL_EnableCompensationCell();
}

/**
* @brief  使能CPU L1-Cache。
* @param  无
* @retval 无
*/
static void CPU_CACHE_Enable(void)
{
  /* 使能 I-Cache */
  SCB_EnableICache();
  
  /* 使能 D-Cache */
  SCB_EnableDCache();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval 无
  */
void Error_Handler(void)
{

}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
