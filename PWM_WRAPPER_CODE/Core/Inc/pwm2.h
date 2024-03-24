/*
 * PWM.H
 *
 *  Created on: Mar 24, 2024
 *      Author: Newton Nadar
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef enum{
	PWM_OK=0,
	PWM_ERROR

}StatusCodes_t;

typedef enum{

	Motor1=0,
	Motor2

}Motor_t;

typedef enum{
	LeftAlign=0,
	RightAlign=1,
	CenterAlign=2

}MotorAlignements_t;

StatusCodes_t PWM_Init();            /*Function will configure the OC channel of timer as PWM*/
StatusCodes_t PWM_UpdateDutyCycle(uint8_t dutycycle,Motor_t Motor) ; /*This function will configure the Duty Cycle for PWM*/
StatusCodes_t PWM_Start(Motor_t Motor);			/*This function will start the respective PWM*/
StatusCodes_t PWM_Stop(Motor_t Motor);            /*This functionw will stop the respective PWM*/


static void MX_TIM1_Init(void);
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base);



#endif /* INC_PWM_H_ */
