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
  * @brief ��������
  */
int main(void)
{
  /* ʹ��L1 Cache */
  CPU_CACHE_Enable();
  
  /* STM32H7xx HAL���ʼ����
        - ��ʱϵͳ�õĻ���H7�Դ���64MHz HSIʱ��:
        - ���ú���HAL_InitTick����ʼ���δ�ʱ���ж�Ϊ1���롣
        - ����NVIVȺ�����ȼ�����Ϊ4��
  */
  HAL_Init();
  
  /* ����ϵͳʱ�ӵ�400MHz */
  SystemClock_Config();
  
  while (1)
  {

  }
}

/**
  * @brief  ϵͳʱ������
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
  * @param  ��
  * @retval ��
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;
  
  /* ʹ��Supply configuration update */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /* ��סSupply configuration update */
//  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
  
  /* ѡ��LDO��ѹ������ĵ�ѹ��Χ
        - ��ѡVOS1��VOS2��VOS3����ͬ��Χ��Ӧ��ͬ��Flash���ٶ�
        - ����ѡ��ʹ��VOS1����ѹ��Χ1.15V - 1.26V */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  
  /* ʹ��HSE����ѡ��HSE��ΪPLLʱ��Դ */
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
  
  /* ѡ��PLL��Ϊϵͳʱ��Դ�����ø�����ʱ�ӷ�Ƶ�� */
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
  
  /* IO����ģʽ */
//  __HAL_RCC_CSI_ENABLE() ;
//  __HAL_RCC_SYSCFG_CLK_ENABLE() ;
//  HAL_EnableCompensationCell();
}

/**
* @brief  ʹ��CPU L1-Cache��
* @param  ��
* @retval ��
*/
static void CPU_CACHE_Enable(void)
{
  /* ʹ�� I-Cache */
  SCB_EnableICache();
  
  /* ʹ�� D-Cache */
  SCB_EnableDCache();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval ��
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
