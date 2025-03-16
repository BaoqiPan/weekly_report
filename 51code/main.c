#include "public.h"
#include "smg.h"
#include "ds1302.h"


/*系统运行时， 数码管上显示电子时钟时分秒， 格式为
“XX-XX-XX” */
//u8 gDS1302_TIME[7]={0x21,0x35,0x17,0x10,0x03,0x01,0x25};
//u8 gsmg_code[18]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
void main()
{
	u8 time_buf[8];
	ds1302_init();		//如果不初始化会显示乱码，有时间研究一下
	while(1)
	{
		ds1302_read_time();
		time_buf[0]=gsmg_code[gDS1302_TIME[0]/16];
		time_buf[1]=gsmg_code[gDS1302_TIME[0]%16];
		time_buf[2]=0x40;
		time_buf[3]=gsmg_code[gDS1302_TIME[1]/16];
		time_buf[4]=gsmg_code[gDS1302_TIME[1]%16];
		time_buf[5]=0x40;
		time_buf[6]=gsmg_code[gDS1302_TIME[2]/16];
		time_buf[7]=gsmg_code[gDS1302_TIME[2]%16];
		smg_display(time_buf,1);
	}

}