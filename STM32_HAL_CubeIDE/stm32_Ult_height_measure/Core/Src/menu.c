#include "key.h"
#include "OLED.h"
#include "OLED_Data.h"
#include "main.h"


extern uint8_t Key_Num;
extern uint32_t distance;
extern float single_height_data[3];
extern uint8_t shutdown_flag;
extern uint8_t Home_Flag;



uint8_t single_flag = 0;

extern float multi_height_data[3];
float multi_height_data_Buffer[3];
uint8_t multi_flag = 0;

uint8_t show_data_flag = 0;

uint8_t KeyNum;


//首页
void Show_Main_UI(void)
{
	OLED_ShowString(32, 2, "身高测量", OLED_8X16);
	OLED_ShowString(8, 40, "开始", OLED_8X16);
	OLED_ShowString(48, 40, "查看", OLED_8X16);
	OLED_ShowString(88, 40, "退出", OLED_8X16);
}

uint8_t mainflag = 1;

uint8_t First_Page_Home(void)
{
	while(1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			mainflag--;
			if (mainflag <= 0)
				mainflag = 3;
		}
		else if (KeyNum == 2)
		{
			mainflag++;
			if (mainflag >= 4)
				mainflag = 1;
		}
		else if (KeyNum == 3)
		{
			OLED_Clear();
			return mainflag;
		}
		switch(mainflag)
		{
			case 1:
				Show_Main_UI();
				OLED_ReverseArea(8, 40, 32, 16);
				OLED_Update();
				break;
			case 2:
				Show_Main_UI();
				OLED_ReverseArea(48, 40, 32, 16);
				OLED_Update();
				break;
			case 3:
				Show_Main_UI();
				OLED_ReverseArea(88, 40, 32, 16);
				OLED_Update();
				break;
		}
		if (shutdown_flag == 1)
		{
			Home_Flag = shutdown;
			return -1;
		}
	}
}

//模式选择页
void Show_Mode_Select_UI(void)
{
	OLED_ShowImage(0, 0, 16, 16, Return);
	OLED_ShowString(24, 2, "请选择模式", OLED_8X16);
	OLED_ShowString(16, 40, "单次", OLED_8X16);
	OLED_ShowString(80, 40, "多次", OLED_8X16);
}

uint8_t modeflag = 1;
uint8_t Mode_Select_Home()
{
	while(1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			modeflag--;
			if (modeflag <= 0)
				modeflag = 3;
		}
		else if (KeyNum == 2)
		{
			modeflag++;
			if (modeflag >= 4)
				modeflag = 1;
		}
		else if (KeyNum == 3)
		{
			OLED_Clear();
			return modeflag;
		}
		switch(modeflag)
		{
			case 1:
				Show_Mode_Select_UI();
				OLED_ReverseArea(0, 0, 16, 16);
				OLED_Update();
				break;
			case 2:
				Show_Mode_Select_UI();
				OLED_ReverseArea(16, 40, 32, 16);
				OLED_Update();
				break;
			case 3:
				Show_Mode_Select_UI();
				OLED_ReverseArea(80, 40, 32, 16);
				OLED_Update();
				break;
		}
		if (shutdown_flag == 1)
		{
			Home_Flag = shutdown;
			return -1;
		}
	}
}


//单次模式
void Show_Single_Mode_UI(void)
{
	OLED_ShowImage(0, 0, 16, 16, Return);
	OLED_ShowString(24, 18, "身高：", OLED_8X16);
	OLED_ShowImage(8, 40, 16, 16, Start);
	OLED_ShowString(32, 40, "查看历史", OLED_8X16);
	OLED_ShowImage(104, 40, 16, 16, Reset);
	if (show_data_flag == 1)
	{
		OLED_ShowFloatNum(72, 18, single_height_data[single_flag - 1], 3, 1, OLED_8X16);
	}
	else if (show_data_flag == 0)
	{
		OLED_ClearArea(72, 18, 48, 16);
	}

//	OLED_ShowFloatNum(0,16,single_height_data[0], 3, 1, OLED_8X16);
//	OLED_ShowFloatNum(0,32,single_height_data[1], 3, 1, OLED_8X16);
//	OLED_ShowFloatNum(0,48,single_height_data[2], 3, 1, OLED_8X16);


}

uint8_t single_modeflag = 1;
uint8_t Single_Mode_Home(void)
{

	while(1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			single_modeflag--;
			if (single_modeflag <= 0)
				single_modeflag = 4;
		}
		else if (KeyNum == 2)
		{
			single_modeflag++;
			if (single_modeflag >= 5)
				single_modeflag = 1;
		}
		else if (KeyNum == 3)
		{
			if (single_modeflag == 1 || single_modeflag == 3)
			{
				OLED_Clear();
				show_data_flag = 0;
				return single_modeflag;
			}
			else if (single_modeflag == 2)
			{
			  SR_04_Start();
			  HAL_Delay(300);
//			  OLED_ShowNum(1, 1, distance, 5, OLED_8X16);
			  uint16_t Distance = distance;
			  Calculate_Result(1, Distance);
			  show_data_flag = 1;

//			  OLED_Update();
//			  HAL_Delay(1000);
			}
			else if (single_modeflag == 4)
			{
				show_data_flag = 0;
			}

		}
		switch(single_modeflag)
		{
			case 1:
				Show_Single_Mode_UI();
				OLED_ReverseArea(0, 0, 16, 16);
				OLED_Update();
				break;
			case 2:
				Show_Single_Mode_UI();
				OLED_ReverseArea(8, 40, 16, 16);
				OLED_Update();
				break;
			case 3:
				Show_Single_Mode_UI();
				OLED_ReverseArea(32, 40, 64, 16);
				OLED_Update();
				break;
			case 4:
				Show_Single_Mode_UI();
				OLED_ReverseArea(104, 40, 16, 16);
				OLED_Update();
				break;
		}
		if (shutdown_flag == 1)
		{
			Home_Flag = shutdown;
			return -1;
		}
	}
}

//多次模式
void Show_Multi_Mode_UI(void)
{
	OLED_ShowImage(0, 0, 16, 16, Return);
	OLED_ShowString(0, 16, "精确身高:", OLED_8X16);
	OLED_ShowImage(8, 40, 16, 16, Start);
	OLED_ShowString(32, 40, "查看历史", OLED_8X16);
	OLED_ShowImage(104, 40, 16, 16, Reset);
	if (show_data_flag == 1)
	{
		OLED_ShowFloatNum(72, 18, multi_height_data[multi_flag - 1], 3, 1, OLED_8X16);
	}
	else if (show_data_flag == 0)
	{
		OLED_ClearArea(72, 18, 56, 16);
	}
}

uint8_t multi_modeflag = 1;
uint8_t Multi_Mode_Home(void)
{
//	static uint8_t firstRun = 1;
//	if (firstRun) {
//	    SR_04_Start();
//	    HAL_Delay(200);
//	    firstRun = 0;
//	}
	while(1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			multi_modeflag--;
			if (multi_modeflag <= 0)
				multi_modeflag = 4;
		}
		else if (KeyNum == 2)
		{
			multi_modeflag++;
			if (multi_modeflag >= 5)
				multi_modeflag = 1;
		}
		else if (KeyNum == 3)
		{
			if (multi_modeflag == 1 || multi_modeflag == 3)
			{
				OLED_Clear();
				show_data_flag = 0;
				return multi_modeflag;
			}
			else if (multi_modeflag == 2)
			{
				for(uint8_t multi_count = 3; multi_count >= 1; multi_count--)
				{
				  SR_04_Start();
				  HAL_Delay(200);
	//			  OLED_ShowNum(1, 1, distance, 5, OLED_8X16);
				  OLED_ShowNum(64, 0, multi_count, 1, OLED_8X16);
				  OLED_Update();
				  Calculate_Result(2, distance);
				}
				  show_data_flag = 1;

//			  OLED_Update();
//			  HAL_Delay(1000);
			}
			else if (multi_modeflag == 4)
			{
				show_data_flag = 0;
			}
		}
		switch(multi_modeflag)
		{
			case 1:
				Show_Multi_Mode_UI();
				OLED_ReverseArea(0, 0, 16, 16);
				OLED_Update();
				break;
			case 2:
				Show_Multi_Mode_UI();
				OLED_ReverseArea(8, 40, 16, 16);
				OLED_Update();
				break;
			case 3:
				Show_Multi_Mode_UI();
				OLED_ReverseArea(32, 40, 64, 16);
				OLED_Update();
				break;
			case 4:
				Show_Multi_Mode_UI();
				OLED_ReverseArea(104, 40, 16, 16);
				OLED_Update();
				break;
		}
		if (shutdown_flag == 1)
		{
			Home_Flag = shutdown;
			return -1;
		}
	}
}

//单,多次模式历史数据
//1代表单次模式，0代表多次模式
void Show_Data_UI(uint8_t mode)
{
	OLED_ShowImage(0, 0, 16, 16, Return);
	OLED_ShowString(32, 0, "历史数据", OLED_8X16);
	OLED_ShowString(0, 16, "1.", OLED_8X16);
	OLED_ShowString(0, 32, "2.", OLED_8X16);
	OLED_ShowString(0, 48, "3.", OLED_8X16);
	if (mode == 1)
	{
		OLED_ShowFloatNum(16, 16, single_height_data[0], 3, 1, OLED_8X16);
		OLED_ShowFloatNum(16, 32, single_height_data[1], 3, 1, OLED_8X16);
		OLED_ShowFloatNum(16, 48, single_height_data[2], 3, 1, OLED_8X16);
	}
	else
	{
		OLED_ShowFloatNum(16, 16, multi_height_data[0], 3, 1, OLED_8X16);
		OLED_ShowFloatNum(16, 32, multi_height_data[1], 3, 1, OLED_8X16);
		OLED_ShowFloatNum(16, 48, multi_height_data[2], 3, 1, OLED_8X16);
	}

}

uint8_t Mode_Flag = 0;//用来让主函数判断显示多次(2)模式的数据还是单次(1)
uint8_t single_dataflag = 1;
uint8_t Single_Data_Home(uint8_t mode)
{
	while(1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			single_dataflag--;
			if (single_dataflag <= 0)
				single_dataflag = 1;
		}
		else if (KeyNum == 2)
		{
			single_dataflag++;
			if (single_dataflag >= 2)
				single_dataflag = 1;
		}
		else if (KeyNum == 3)
		{
			OLED_Clear();
			return single_dataflag;
		}
		switch(single_dataflag)
		{
			case 1:
				Show_Data_UI(Mode_Flag);
				OLED_ReverseArea(0, 0, 16, 16);
				OLED_Update();
				break;
		}
		if (shutdown_flag == 1)
		{
			Home_Flag = shutdown;
			return -1;
		}
	}
}


//显示数据模式选择
void Show_Select_Data_UI(void)
{
	OLED_ShowImage(0, 0, 16, 16, Return);
	OLED_ShowString(0, 16, "单次模式", OLED_8X16);
	OLED_ShowString(0, 32, "多次模式", OLED_8X16);
}

uint8_t select_dataflag = 1;
uint8_t Select_Data_Home(void)
{
	while(1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			select_dataflag--;
			if (select_dataflag <= 0)
				select_dataflag = 3;
		}
		else if (KeyNum == 2)
		{
			select_dataflag++;
			if (select_dataflag >= 4)
				select_dataflag = 1;
		}
		else if (KeyNum == 3)
		{
			OLED_Clear();
			return select_dataflag;
		}
		switch(select_dataflag)
		{
			case 1:
				Show_Select_Data_UI();
				OLED_ReverseArea(0, 0, 16, 16);
				OLED_Update();
				break;
			case 2:
				Show_Select_Data_UI();
				OLED_ReverseArea(0, 16, 64, 16);
				OLED_Update();
				break;
			case 3:
				Show_Select_Data_UI();
				OLED_ReverseArea(0, 32, 64, 16);
				OLED_Update();
				break;
		}
		if (shutdown_flag == 1)
		{
			Home_Flag = shutdown;
			return -1;
		}
	}
}


//计算结果，单次和多次
void Calculate_Result(uint8_t Mode, uint16_t Distance)
{

	if (Mode == 1)
	{
		uint16_t Height_mm = 2000 - Distance;
		OLED_ShowNum(64, 16, Height_mm, 5, OLED_8X16);
		float Height = Height_mm * 0.1;
//		OLED_ShowFloatNum(64, 32, Height,3, 1, OLED_8X16);
		if (single_flag <= 2)
		{
			single_height_data[single_flag] = Height;
			single_flag++;
		}
		else if (single_flag >=3)
		{
			single_height_data[0] = Height;
			single_flag = 1;
		}

	}
	else if (Mode == 2)
	{
		static multi_count_num = 3; //计次三次结果
		uint16_t Height_mm = 2000 - Distance;

		float Height = Height_mm * 0.1;

		multi_height_data_Buffer[multi_count_num - 1] = Height;
		multi_count_num--;
//		OLED_ShowFloatNum(0, 0, multi_height_data_Buffer[0], 3, 1, OLED_8X16);
//		OLED_ShowFloatNum(0, 16, multi_height_data_Buffer[1], 3, 1, OLED_8X16);
//		OLED_ShowFloatNum(0, 32, multi_height_data_Buffer[2], 3, 1, OLED_8X16);
//		OLED_Update();
		if (multi_count_num == 0)
		{
			if (multi_flag <= 2)
			{
				multi_height_data[multi_flag] = (multi_height_data_Buffer[2] + multi_height_data_Buffer[1] + multi_height_data_Buffer[0]) / 3.0;
				multi_flag++;
			}
			else if (multi_flag >= 3)
			{
				multi_height_data[0] = (multi_height_data_Buffer[2] + multi_height_data_Buffer[1] + multi_height_data_Buffer[0]) / 3.0;
				multi_flag = 1;
			}
			multi_count_num = 3;
		}


	}
}
