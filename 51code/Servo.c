#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Servo_Init(void)
{
	PWM_Init();
}

//0		45		90		135		180		
//5		10		15		20		25

void Servo_SetAngle(float Angle)
{
		PWM_SetCompare2(Angle/9+5);
}
