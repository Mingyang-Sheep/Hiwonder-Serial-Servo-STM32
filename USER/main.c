#include "delay.h"
#include "uart.h"
#include "LobotServoController.h"
#include "bool.h"

void delay_s(int s)
{
	for(;s>0;s--)
		delay_ms(1000);
}

LobotServo servos[3];

/*********************************************************************************
 * Author:      Liu Mingyang  Southern University of Science and Technology
 * Introduction:这份代码即保留了原来传统舵机的位置模式，也就是固定转角，同时
								开发了新的模式，电机模式(motor)能够让舵机拥有和电机一样的功能。
								代码配置了串口1和串口2，具体的看LobotServoController.c文件当中
								函数最后发送的代码，修改1或者2即可（如果有需要也可自行更改）
 **********************************************************************************/

 int main(void)
 {
 	SystemInit();//系统时钟等初始化
	delay_init(72);	     //延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uartInit(115200);//串口初始化为115200
	 uart2Init(115200);

		while(1){
		setServoMotorMode(1, 0,500);
		setServoMotorMode(2, 0,500);
		delay_ms(1000); 
		setServoMotorMode(3, 0,500);
		delay_ms(1000); 

		

		}
}
