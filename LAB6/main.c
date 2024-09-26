/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Programa principal
  ******************************************************************************
  * @attention
  *
  * Derechos de autor (c) 2024 STMicroelectronics.
  * Todos los derechos reservados.
  *
  * Este software está licenciado bajo términos que se encuentran en el archivo LICENSE
  * en el directorio raíz de este software.
  * Si no viene con un archivo LICENSE, se proporciona "COMO ES".
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t rx_buffer[1];
uint8_t tx_message[10];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  Punto de entrada de la aplicación.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* Configuración del MCU ---------------------------------------------------*/

  /* Reinicia todos los periféricos, inicializa la interfaz Flash y el Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configura el reloj del sistema */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Inicializa todos los periféricos configurados */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* Inicia la recepción por interrupción en UART1 */
  HAL_UART_Receive_IT(&huart1, rx_buffer, 1);

  /* USER CODE END 2 */

  /* Bucle infinito */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* Aquí puedes agregar código si es necesario */
  }
  /* USER CODE END 3 */
}

/**
  * @brief Configuración del reloj del sistema
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configura el voltaje de salida del regulador interno principal
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Inicializa los osciladores RCC según los parámetros especificados
  * en la estructura RCC_OscInitTypeDef.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Inicializa los relojes CPU, AHB y APB
  */
  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                   | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Función de inicialización de USART1
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance          = USART1;
  huart1.Init.BaudRate     = 9600;
  huart1.Init.WordLength   = UART_WORDLENGTH_8B;
  huart1.Init.StopBits     = UART_STOPBITS_1;
  huart1.Init.Parity       = UART_PARITY_NONE;
  huart1.Init.Mode         = UART_MODE_RX;
  huart1.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */
}

/**
  * @brief Función de inicialización de USART2
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{
  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance          = USART2;
  huart2.Init.BaudRate     = 9600;
  huart2.Init.WordLength   = UART_WORDLENGTH_8B;
  huart2.Init.StopBits     = UART_STOPBITS_1;
  huart2.Init.Parity       = UART_PARITY_NONE;
  huart2.Init.Mode         = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */
}

/**
  * @brief Función de inicialización de GPIO
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* Habilita el reloj para los puertos GPIO */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* Aquí puedes inicializar otros pines si es necesario */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief Callback de recepción UART
  * @param huart: puntero a la estructura UART_HandleTypeDef
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    switch (rx_buffer[0])
    {
      case 'A':
        strcpy((char*)tx_message, "Arriba ");
        break;
      case 'B':
        strcpy((char*)tx_message, "Abajo ");
        break;
      case 'C':
        strcpy((char*)tx_message, "Derecha ");
        break;
      case 'D':
        strcpy((char*)tx_message, "Izquierda ");
        break;
      case 'E':
        strcpy((char*)tx_message, "PAUSA ");
        break;
      case 'F':
        strcpy((char*)tx_message, "REINICIO ");
        break;
      default:
        strcpy((char*)tx_message, "Desconocido ");
        break;
    }

    /* Envía el mensaje por UART2 */
    HAL_UART_Transmit(&huart2, tx_message, strlen((char*)tx_message), HAL_MAX_DELAY);

    /* Limpia el buffer del mensaje */
    memset(tx_message, 0, sizeof(tx_message));

    /* Reinicia la recepción por interrupción */
    HAL_UART_Receive_IT(&huart1, rx_buffer, 1);
  }
}
/* USER CODE END 4 */

/**
  * @brief  Función en caso de error.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* Puedes agregar tu propia implementación para reportar el estado de error */
  __disable_irq();
  while (1)
  {
    /* Bucle infinito en caso de error */
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reporta el nombre del archivo fuente y el número de línea donde ocurrió el error.
  * @param  file: puntero al nombre del archivo fuente
  * @param  line: número de línea donde ocurrió el error
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* Puedes agregar tu propia implementación para reportar el nombre del archivo y la línea */
  /* printf("Error en el archivo %s en la línea %d\r\n", file, line); */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

