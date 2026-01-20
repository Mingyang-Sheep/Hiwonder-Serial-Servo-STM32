#ifndef __USART_H
#define __USART_H
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/5/27
//版本：V1.3
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
////////////////////////////////////////////////////////////////////////////////// 
extern u8 USART_RX_BUF[64];     //接收缓冲,最大63个字节.末字节为换行符 
extern u8 USART_RX_STA;         //接收状态标记	
extern uint8_t UART2_RX_BUF[16];

// 初始化
void uart2Init(uint32_t bound);
void uart2NVICInit(void);

// 发送相关
void uart2WriteBuf(uint8_t *buf, uint8_t len);
void USART2_SendByte(uint8_t byte);
void USART2_SendArray(uint8_t *arr, uint16_t len);

// 中断服务函数（要在 startup_stm32f10x_xx.s 里映射）
void USART2_IRQHandler(void);
void uartInit(u32 bound);
void uartWriteBuf(uint8_t *buf, uint8_t len);

#endif
