# STM32 HAL library for reading code from HCS301 433Mhz Keeloq

## English note
A simple C library (STM32 HAL) for for reading code from HCS301 433Mhz Keeloq

Config you RX433 PIN with GPIO_EXTI:
 - External Interrupt Mode with Rising/Falling edge trigger detection;
 - no pull up no pull down.
and add NVIC global interrupt for you PIN.

Copy the library header and source file to the appropriate project directories (Inc, Src).

Config you RX433 PORT and PIN on hcs301.h file
```
// ======= Config section =============
#define HCS_RECIEVER_PORT	GPIOC
#define HCS_RECIEVER_PIN  	GPIO_PIN_15
// ====================================
```
In the head file of your project (main.c), include the header file
```
/ * USER CODE BEGIN Includes * /
#include "micros.h"		// https://github.com/leech001/micros
#include "hcs301.h"
/ * USER CODE END Includes * /
```
add HCS301 structure to the section
```
/ * USER CODE BEGIN PV * /
HCS301_t hcs301;
/ * USER CODE END PV * /
```
add RX433 receive interrupt
```
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch(GPIO_Pin){
		case HCS_RECIEVER_PIN:
		{
			HCS_interrupt(&hcs301);
		}
	}
}
/* USER CODE END 0 */
```
add in main function section for initial initialization of the DWT (Data Watchpoint and Trace unit) (https://github.com/leech001/micros) 
```
/ * USER CODE BEGIN 2 * /
  DWT_Init();
/ * USER CODE END 2 * /
```
On this project setup is ready.
After starting the program, information from RX433(HCS301) will be available through the hcs301 structure
```
...
hcs301.SerialNum
...
```
HCS301 algorithm used from http://arduino.ru/forum/apparatnye-voprosy/biblioteka-chteniya-id-brelkov-signalizatsiii-hcs301-keeloq#comment-35573

## Russian note
Простая библиотека на Си (STM32 HAL) для считывания кодов от пультов (433 МГц) (ворота, шлагбаумы, сигнализации) основанных на микросхеме HCS301 с кодировкой Keeloq (пример https://ironlogic.ru/il_new.nsf/htm/ru_il100).

Сконфигурируйте ваш порт и пин куда подключен модуль для приема кодов как порт с внешним прерыванием (GPIO_EXTI):
 - External Interrupt Mode with Rising/Falling edge trigger detection;
 - no pull up no pull down.
Включите глобальное прерывание для данного порта (NVIC).

Скопируйте заголовочный и исходный файл библиотеки в соответствующие директории проекта (Inc, Src).

Определение ваши PORT и PIN порты куда подключен считыватель в файле hcs301.h file
```
// ======= Config section =============
#define HCS_RECIEVER_PORT	GPIOC
#define HCS_RECIEVER_PIN  	GPIO_PIN_15
// ====================================
```
В головном файл вашего проекта (main.c) подключите заголовочные файлы.
```
/* USER CODE BEGIN Includes */
#include "micros.h"		// https://github.com/leech001/micros
#include "hcs301.h"
/* USER CODE END Includes */
```
в секцию добавьте структуру
```
/* USER CODE BEGIN PV */
HCS301_t hcs301;
/* USER CODE END PV */
```
в секцию добавьте вызов прерывания для вашего пина
```
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch(GPIO_Pin){
		case HCS_RECIEVER_PIN:
		{
			HCS_interrupt(&hcs301);
		}
	}
}
/* USER CODE END 0 */
```
добавьте в секцию функции int main(void) инициализацию DWT (Data Watchpoint and Trace unit) (https://github.com/leech001/micros)
```
/* USER CODE BEGIN 2 */
  DWT_Init();
/* USER CODE END 2 */
```
На этом настройка проекта закончена.
После запуска программы информация о кодах от пультов поступающих при нажатии кнопок будет доступна через структуру hcs301.
```
...
hcs301.SerialNum
...
```
Алгоритм работы с HCS301 использован с данного ресурса http://arduino.ru/forum/apparatnye-voprosy/biblioteka-chteniya-id-brelkov-signalizatsiii-hcs301-keeloq#comment-35573
 