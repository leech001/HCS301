## A simple C library (STM32 HAL) for reading and sending code for HCS301 433Mhz Keeloq 66 bit.

### This library use for control automation garage door and road barriers.

### Receive code

#### Config you RX433 PIN with "GPIO_EXTI":
 - External Interrupt Mode with Rising/Falling edge trigger detection;
 - No pull up no pull down.

and add NVIC global interrupt for you RXPIN.

Copy the library header and source file to the appropriate project directories (Inc, Src).

Config you RX433 and TX433 PORT and PIN on hcs301.h file
```
#define RX_PORT         You_RX_GPIO_Port
#define RX_PIN          You_Rx_Pin
```
In the main file of your project (main.c), include the header files.

Micros library download from https://github.com/leech001/micros
```
/ * USER CODE BEGIN Includes * /
#include "micros.h"		      
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
	if (GPIO_Pin == You_Rx_Pin){
	    HCS301_Int()
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

## Send code
#### Config you TX433 PIN with "Output Push Pull":
 - Speed Very High;
 - No pull up no pull down.
 
Copy the library header and source file to the appropriate project directories (Inc, Src).

Config you RX433 and TX433 PORT and PIN on hcs301.h file
```
#define TX_PORT         You_TX_GPIO_Port
#define TX_PIN          You_TX_Pin
```
In the main file of your project (main.c), include the header files.

Micros library download from https://github.com/leech001/micros
```
/ * USER CODE BEGIN Includes * /
#include "micros.h"		      
#include "hcs301.h"
/ * USER CODE END Includes * /
```
#### For send code HCS301 use function
```
HCS301_Send(you_code);
```