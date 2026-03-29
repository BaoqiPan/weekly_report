#ifndef __MENU_H
#define __MENU_H

//首页
void Show_Main_UI(void);
uint8_t First_Page_Home(void);

//模式选择页
void Show_Mode_Select_UI(void);
uint8_t Mode_Select_Home();

//单次模式
void Show_Single_Mode_UI(void);
uint8_t Single_Mode_Home(void);
//多次模式
void Show_Multi_Mode_UI(void);
uint8_t Multi_Mode_Home(void);
//单,多次模式历史数据
//1代表单次模式，0代表多次模式
void Show_Data_UI(uint8_t mode);
uint8_t Single_Data_Home(uint8_t mode);
//显示数据模式选择
void Show_Select_Data_UI(void);
uint8_t Select_Data_Home(void);


//计算结果，单次和多次
void Calculate_Result(uint8_t Mode, uint16_t Distance);


#endif
