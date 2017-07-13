/* Includes ------------------------------------------------------------------*/
#include "led_matrix.h"
#include "stm32f7xx_hal.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "stm32f7xx_hal_adc.h"
#include "stm32f7xx_hal_adc_ex.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

ADC_HandleTypeDef adc_handle;
ADC_ChannelConfTypeDef adc_ch_conf;

osStatus status;
GPIO_InitTypeDef led_matrix;

typedef struct {
	GPIO_TypeDef *pin_letter;
	uint16_t pin_number;
}BlinkerTypeDef;

// Each LED state is stored in this 2D array
GPIO_PinState led_matrix_state[LED_MATRIX_ROWS][LED_MATRIX_COLS];

// Mutex definition
osMutexDef(LED_MATRIX_MUTEX_DEF);

// Mutex global variable
osMutexId led_matrix_mutex_id;

/* Private function prototypes -----------------------------------------------*/
void led_matrix_set(uint8_t row, uint8_t col, uint8_t state);
/* Private functions ---------------------------------------------------------*/
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
	// 1 A0 PA0 ADC3_IN0
	__HAL_RCC_ADC3_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	led_matrix.Pin = GPIO_PIN_0;
	led_matrix.Speed = GPIO_SPEED_FAST;
	//led_matrix.Pull = GPIO_NOPULL;
	led_matrix.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA, &led_matrix);
}

uint16_t adc_measure()
{
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

// TODO:
// Write this function!
void led_matrix_set(uint8_t row, uint8_t col, uint8_t state) {
	// TODO:
	// Wait for the mutex
	if (led_matrix_mutex_id != NULL)  {
		status = osMutexWait(led_matrix_mutex_id, osWaitForever);
		if (status != osOK)  {
			printf("MutexWait Error"); // handle failure code
		}
	}

	// TODO:
	// Change the LED in the selected row and col to the specified state
	// Use the led_matrix_state 2D array variable!
	if (state) {
		led_matrix_state[row][col] = 0;
	} else {
		led_matrix_state[row][col] = 1;
	}

	// TODO:
	// Release the mutex
	if (led_matrix_mutex_id != NULL)  {
		status = osMutexRelease(led_matrix_mutex_id);
		if (status != osOK)  {
			printf("MutexRelease Error"); // handle failure code
		}
	}
}

// TODO:
// Write this function!
void led_matrix_update_thread(void const *argument)
{
	// TODO:
	// Initialize the pins as outputs and the led_matrix_state 2D array
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	led_matrix.Mode = GPIO_MODE_OUTPUT_PP;
	led_matrix.Pull = GPIO_NOPULL;

	led_matrix.Pin = GPIO_PIN_7;			//D0, 13-as l�b
	HAL_GPIO_Init(GPIOC, &led_matrix);

	led_matrix.Pin = GPIO_PIN_8;			//D10, 9-as l�b
	HAL_GPIO_Init(GPIOA, &led_matrix);

	led_matrix.Pin = GPIO_PIN_4;			//D3, 3-as l�b
	HAL_GPIO_Init(GPIOB, &led_matrix);

	led_matrix.Pin = GPIO_PIN_6;			//D1, 10-as l�b
	HAL_GPIO_Init(GPIOC, &led_matrix);

	led_matrix.Pin = GPIO_PIN_6;			//D2, 4-as l�b
	HAL_GPIO_Init(GPIOG, &led_matrix);

	led_matrix.Pin = GPIO_PIN_7;			//D4, 6-as l�b
	HAL_GPIO_Init(GPIOG, &led_matrix);

	led_matrix.Pin = GPIO_PIN_15;			//D11, 14-as l�b
	HAL_GPIO_Init(GPIOB, &led_matrix);

	led_matrix.Pin = GPIO_PIN_15;			//D9, 8-as l�b
	HAL_GPIO_Init(GPIOA, &led_matrix);

	led_matrix.Pin = GPIO_PIN_3;			//D7, 5-as l�b
	HAL_GPIO_Init(GPIOI, &led_matrix);

	led_matrix.Pin = GPIO_PIN_0;			//D5, 1-as l�b
	HAL_GPIO_Init(GPIOI, &led_matrix);

	led_matrix.Pin = GPIO_PIN_2;			//D8, 7-as l�b
	HAL_GPIO_Init(GPIOI, &led_matrix);

	led_matrix.Pin = GPIO_PIN_6;			//D6, 2-as l�b
	HAL_GPIO_Init(GPIOH, &led_matrix);


	/*
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1);		//1-as oszlop
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);		//2-as oszlop
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, 1);		//3-as oszlop
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 1);		//4-as oszlop
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, 1);		//5-as oszlop
	*/

	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);		//1-as sor
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);		//2-as sor
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1);		//3-as sor
	//HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, 1);		//4-as sor
	//HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, 1);		//5-as sor
	//HAL_GPIO_WritePin(GPIOI, GPIO_PIN_2, 1);		//6-as sor
	//HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, 1);		//7-as sor

	BlinkerTypeDef ledblinking [] = {
		{GPIOC, GPIO_PIN_7},
		{GPIOB, GPIO_PIN_4},
		{GPIOG, GPIO_PIN_6},
		{GPIOC, GPIO_PIN_6},
		{GPIOG, GPIO_PIN_7},
		{GPIOA, GPIO_PIN_8},
		{GPIOB, GPIO_PIN_15},
		{GPIOA, GPIO_PIN_15},
		{GPIOI, GPIO_PIN_3},
		{GPIOI, GPIO_PIN_0},
		{GPIOI, GPIO_PIN_2},
		{GPIOH, GPIO_PIN_6},
	};

	for (int k = 5; k < LED_MATRIX_ROWS + 5; k++) {
		HAL_GPIO_WritePin(ledblinking[k].pin_letter, ledblinking[k].pin_number, 1);
	}

	// TODO:
	// Create a mutex
	// Use the LED_MATRIX_MUTEX_DEF
	led_matrix_mutex_id = osMutexCreate(osMutex(LED_MATRIX_MUTEX_DEF));

	LCD_UsrLog("led_matrix - initialized\n");

	// Infinite loop
	while (1) {
		// TODO:
		// Implement the led matrix updater functionality

		// Step 1:
		// Iterate through every column or row
		for (int i = 0; i < LED_MATRIX_COLS; i++) {


			// Step 2:
			// Wait for the mutex
			osMutexWait(led_matrix_mutex_id, osWaitForever);

			// Step 3:
			// Turn on the column or row
			//led_matrix_set(0, 0, 1);
			HAL_GPIO_WritePin(ledblinking[i].pin_letter, ledblinking[i].pin_number, 1);

			// Step 4:
			// Turn on the leds in that column or row
			for (int j = 5; j < LED_MATRIX_ROWS + 5; j++) {
				HAL_GPIO_WritePin(ledblinking[j].pin_letter, ledblinking[j].pin_number, 0);
				osDelay(100);
				HAL_GPIO_WritePin(ledblinking[j].pin_letter, ledblinking[j].pin_number, 1);
			}

			// Step 5:
			// Release the mutex
			osMutexRelease(led_matrix_mutex_id);

			// Step 6:
			// Delay
			osDelay(50);

			// Step 7:
			// Turn off the column or row
			HAL_GPIO_WritePin(ledblinking[i].pin_letter, ledblinking[i].pin_number, 0);
		}
	}

	// Terminating thread
	while (1) {
		LCD_ErrLog("led_matrix - terminating...\n");
		osThreadTerminate(NULL);
	}
}

// This thread is a waterfall type animation
void led_matrix_waterfall_thread(void const *argument)
{
	while (1) {
		for (uint8_t r = 0; r < LED_MATRIX_ROWS; r++) {
			for (uint8_t c = 0; c < LED_MATRIX_COLS; c++) {
				led_matrix_set(r, c, 1);
				osDelay(50);
				led_matrix_set(r, c, 0);
			}
		}
	}

	while (1) {
		LCD_ErrLog("led_matrix_waterfall - terminating...\n");
		osThreadTerminate(NULL);
	}
}

void adc_measurement_thread(void const *argument)
{
	//HAL_ADC_MspInit();

	adc_handle.State = HAL_ADC_STATE_RESET;
	adc_handle.Instance = ADC3;
	adc_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	adc_handle.Init.Resolution = ADC_RESOLUTION_12B;
	adc_handle.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	adc_handle.Init.DMAContinuousRequests = DISABLE;
	adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	adc_handle.Init.ContinuousConvMode = DISABLE;
	adc_handle.Init.DiscontinuousConvMode = DISABLE;
	adc_handle.Init.ScanConvMode = DISABLE;
	HAL_ADC_Init(&adc_handle);

	adc_ch_conf.Channel = ADC_CHANNEL_0;
	adc_ch_conf.Offset = 0;
	adc_ch_conf.Rank = 1;
	adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);

	while (1) {
		float ref = (float)adc_measure() / 4095 * 4500;
		uint8_t buffer[100];
		sprintf((char*)buffer, "ref   %.f", ref);
		BSP_LCD_ClearStringLine(6);
		BSP_LCD_DisplayStringAtLine(6, buffer);
		HAL_Delay(10);
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/