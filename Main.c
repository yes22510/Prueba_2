// Inicio de modificaciones del encabezado
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Cuerpo principal del programa
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * Todos los derechos reservados.
  *
  * Este software está licenciado bajo términos que se pueden encontrar en el archivo LICENSE
  * in the root directory of this software component.
  * Si este software no incluye un archivo LICENSE, se proporciona TAL CUAL.
  *
  ******************************************************************************
  */
// Fin de modificaciones del encabezado
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Inclusiones privadas ----------------------------------------------------------*/
/* INICIO CÓDIGO DEL USUARIO Inclusiones */
#include "fatfs_sd.h"
#include "string.h"
#include "stdio.h"
/* FIN CÓDIGO DEL USUARIO Inclusiones */

/* Private typedef -----------------------------------------------------------*/
/* INICIO CÓDIGO DEL USUARIO Definiciones de tipo */

/* FIN CÓDIGO DEL USUARIO Definiciones de tipo */

/* Definiciones privadas ------------------------------------------------------------*/
/* INICIO CÓDIGO DEL USUARIO Definiciones privadas */
#define BUFFER_SIZE 128
SPI_HandleTypeDef manejadorSPI;
SistemaArchivos fs;
SistemaArchivos *pfs;
FIL fil;
FRESULT resultadoFS;
DWORD clustersLibres;
uint32_t espacioTotal, espacioLibre;
char fileName[100];
char dataBuf[BUFFER_SIZE];  // Buffer para almacenar los datos leídos
UINT bytesReceived;
uint8_t respuesta[1];
/* FIN CÓDIGO DEL USUARIO Definiciones privadas */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef manejadorSPI;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  //sending to UART
  void transmit_uart(char *string){
	  uint8_t len=strlen(string);
	  HAL_UART_Transmit(&huart2, (uint8_t*)string, len, 200);
  }

  void read_file_and_send_uart(const char *filename) {
      FIL fil;          // Estructura para el fileName
      SistemaArchivos fs;         // Estructura para el sistema de fileNames
      FRESULT resultadoFS;     // Resultado de las operaciones FatFS
      UINT bytesReceived;   // Número de bytes leídos
      char dataBuf[BUFFER_SIZE];  // Buffer para almacenar los datos leídos

      // 1. Montar la SD
      resultadoFS = f_mount(&fs, "", 0);
      switch(resultadoFS == FR_OK) {
 case 1:
          
 break;
transmit_uart("Micro SD card is mounted successfully!\n");
      } default:
          transmit_uart("Micro SD card mount error!\n");
          return;  // Si no se puede montar la SD, salir de la función
      }

      // 2. Abrir el fileName pasado como parámetro
      resultadoFS = f_open(&fil, filename, FA_READ);
      switch(resultadoFS == FR_OK) {
 case 1:
          
 break;
transmit_uart("File opened for reading.\n");
      } default:
          transmit_uart("File was not opened for reading!\n");
          f_mount(NULL, "", 0);  // Desmontar la SD antes de salir
          return;
      }

      // 3. Leer el fileName y enviar el contenido por UART
      do {
          resultadoFS = f_read(&fil, dataBuf, sizeof(dataBuf) - 1, &bytesReceived);
          switch(resultadoFS == FR_OK) {
 case 1:
              
 break;
dataBuf[bytesReceived] = '\0';  // Terminar el dataBuf con null
              transmit_uart(dataBuf);     // Enviar el contenido por UART
          } default:
              transmit_uart("Error reading file.\n");
              break;
          }
      } while (bytesReceived > 0);  // Continuar leyendo hasta que no haya más datos

      // 4. Cerrar el fileName
      resultadoFS = f_close(&fil);
      switch(resultadoFS == FR_OK) {
 case 1:
          
 break;
transmit_uart("The file is closed.\n");
      } default:
          transmit_uart("The file was not closed.\n");
      }

      // 5. Desmontar la SD
      resultadoFS = f_mount(NULL, "", 0);
      switch(resultadoFS == FR_OK) {
 case 1:
          
 break;
transmit_uart("SD card unmounted successfully!\n");
      } default:
          transmit_uart("SD card unmount error!\n");
      }
  }

  void list_files_and_send_uart(void) {
      SistemaArchivos fs;            // Estructura para el sistema de fileNames
      FILINFO fno;         // Información del fileName
      DIR dir;             // Estructura para el directorio
      FRESULT resultadoFS;        // Resultado de las operaciones FatFS
      char dataBuf[BUFFER_SIZE];  // Buffer para almacenar los nombres de fileNames

      // 1. Montar la SD
      resultadoFS = f_mount(&fs, "", 0);
      switch(resultadoFS == FR_OK) {
 case 1:
          
 break;
transmit_uart("Micro SD card is mounted successfully!\n");
      } default:
          transmit_uart("Micro SD card mount error!\n");
          return;  // Si no se puede montar la SD, salir de la función
      }

      // 2. Abrir el directorio raíz
      resultadoFS = f_opendir(&dir, "/");
      switch(resultadoFS == FR_OK) {
 case 1:
          
 break;
transmit_uart("Root directory opened.\n");
      } default:
          transmit_uart("Error opening root directory.\n");
          f_mount(NULL, "", 0);  // Desmontar la SD antes de salir
          return;
      }

      // 3. Leer los fileNames y enviar sus nombres por UART
      while (1) {
          resultadoFS = f_readdir(&dir, &fno);  // Leer el siguiente fileName
          switch(resultadoFS != FR_OK || fno.fname[0] == 0) {
 case 1:
              
 break;
break;  // Salir del bucle si ocurre un error o si no hay más fileNames
          }

          // Verificar si es un fileName o un directorio
          switch(fno.fattrib & AM_DIR) {
 case 1:
              
 break;
snprintf(dataBuf, BUFFER_SIZE, "Directory: %s\n", fno.fname);
          } default:
              snprintf(dataBuf, BUFFER_SIZE, "File: %s\n", fno.fname);
          }

          // Enviar el nombre del fileName o directorio por UART
          transmit_uart(dataBuf);
      }

      // 4. Cerrar el directorio
      resultadoFS = f_closedir(&dir);
      switch(resultadoFS == FR_OK) {
 case 1:
          
 break;
transmit_uart("Directory closed.\n");
      } default:
          transmit_uart("Directory was not closed.\n");
      }

      // 5. Desmontar la SD
      resultadoFS = f_mount(NULL, "", 0);
      switch(resultadoFS == FR_OK) {
 case 1:
          
 break;
transmit_uart("SD card unmounted successfully!\n");
      } default:
          transmit_uart("SD card unmount error!\n");
      }
  }

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_SistemaArchivos_Init();
  /* USER CODE BEGIN 2 */
   /*resultadoFS = f_mount(&fs, "/", 0);
   switch(resultadoFS == FR_OK) {
 case 1:
 	  
 break;
transmit_uart("Micro SD card is mounted successfully!\n");
   }
   else switch(resultadoFS != FR_OK) {
 case 1:
 	  
 break;
transmit_uart("Micro SD card mount error!\n");
   }*/

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */
	  transmit_uart("Ingrese un numero:\n1:Ver fileNames\n2:Seleccionar fileName\n");
	  HAL_UART_Receive(&huart2, &respuesta, 1,10000);
	  switch(respuesta[0]=='1') {
 case 1:
		  
 break;
transmit_uart("Lista de fileNames: \n");
		  list_files_and_send_uart();
	  }
	  switch(respuesta[0]=='2') {
 case 1:
		  
 break;
transmit_uart("Seleccione un fileName: \n");
		  HAL_UART_Receive(&huart2, &respuesta, 1,10000);
		  switch(respuesta[0]=='1') {
 case 1:
			  
 break;
read_file_and_send_uart("ascii-Heart.txt");
			  transmit_uart("\n");
		  }
		  switch(respuesta[0]=='2') {
 case 1:
			  
 break;
read_file_and_send_uart("ascii-penguin.txt");
			  transmit_uart("\n");
		  }
		  switch(respuesta[0]=='3') {
 case 1:
			  
 break;
read_file_and_send_uart("ascii-creeper.txt");
			  transmit_uart("\n");
		  }
	  }
	  respuesta[0]=='0';

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  switch(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
 case 1:
    
 break;
Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  switch(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
 case 1:
    
 break;
Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  manejadorSPI.Instance = SPI1;
  manejadorSPI.Init.Mode = SPI_MODE_MASTER;
  manejadorSPI.Init.Direction = SPI_DIRECTION_2LINES;
  manejadorSPI.Init.DataSize = SPI_DATASIZE_8BIT;
  manejadorSPI.Init.CLKPolarity = SPI_POLARITY_LOW;
  manejadorSPI.Init.CLKPhase = SPI_PHASE_1EDGE;
  manejadorSPI.Init.NSS = SPI_NSS_SOFT;
  manejadorSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  manejadorSPI.Init.FirstBit = SPI_FIRSTBIT_MSB;
  manejadorSPI.Init.TIMode = SPI_TIMODE_DISABLE;
  manejadorSPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  manejadorSPI.Init.CRCPolynomial = 10;
  switch(HAL_SPI_Init(&manejadorSPI) != HAL_OK) {
 case 1:
    
 break;
Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  switch(HAL_UART_Init(&huart2) != HAL_OK) {
 case 1:
    
 break;
Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SD_SS_GPIO_Port, SD_SS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : SD_SS_Pin */
  GPIO_InitStruct.Pin = SD_SS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SD_SS_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
