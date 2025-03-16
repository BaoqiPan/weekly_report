#include "ds1302.h"
#include "intrins.h"

u8 gWRITE_RTC_ADDR[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};//其中rtc指的是实时时钟，real-time-clock
u8 gREAD_RTC_ADDR[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};            
u8 gDS1302_TIME[7]={0x21,0x35,0x17,0x10,0x03,0x01,0x25};
	
void ds1302_write_byte(u8 addr, u8 dat)
{
	u8 i=0;
	
	DS1302_RST=0;
	_nop_();
	DS1302_CLK=0;
	_nop_();
	DS1302_RST=1;
	_nop_();
	
	for(i=0;i<8;i++)
	{
		DS1302_IO=addr&0x01;//由于取出来的位直接就可以传给io，就不用再新建变量了
		addr>>=1;
		DS1302_CLK=1;
		_nop_();
		DS1302_CLK=0;
		_nop_();
	}
	for(i=0;i<8;i++)
	{
		DS1302_IO=dat&0x01;
		dat>>=1;
		DS1302_CLK=1;
		_nop_();
		DS1302_CLK=0;
		_nop_();
	}
	DS1302_RST=0;
	_nop_();
}

u8 ds1302_read_byte(u8 addr)
{
	u8 i=0;
	u8 value=0;
	u8 temp=0;
	
	DS1302_RST=0;
	_nop_();
	DS1302_CLK=0;
	_nop_();
	DS1302_RST=1;
	_nop_();		//这里可以延时也可以不延时
	//	ds1302_write_byte(addr);		////////////有时间想一下这里可不可以直接调用之前的写函数
	
	
	for(i=0;i<8;i++)
	{
		DS1302_IO=addr&0x01;
		addr>>=1;
		DS1302_CLK=1;
		_nop_();
		DS1302_CLK=0;
		_nop_();
	}
	
	for(i=0;i<8;i++)
	{				//由于上面是下降沿，刚好可以直接读
		temp=DS1302_IO;
		value=(temp<<7)|(value>>1);
		_nop_();
		DS1302_CLK=1;
		_nop_();
		DS1302_CLK=0;
		_nop_();
	}
	DS1302_RST=0;		//注意这里要让时钟线为低电平
	_nop_();
	DS1302_CLK=1;//对于实物中， P3.4 口没有外接上拉电阻的， 此处代码需要添加， 使数据口有一个上升沿脉冲。
	_nop_();
	DS1302_IO = 0;
	_nop_();
	DS1302_IO = 1;
	_nop_();		//////////这一块代码完全不理解

	return value;
}

void ds1302_init(void)
{
	u8 i=0;
	
	ds1302_write_byte(0x8e,0x00);//先关闭写保护，写完再打开
	for(i=0;i<7;i++)
	{
		ds1302_write_byte(gWRITE_RTC_ADDR[i],gDS1302_TIME[i]);
	}
	ds1302_write_byte(0x8e,0x80);
}

void ds1302_read_time(void)
{
	u8 i=0;
	for(i=0;i<7;i++)
	{
		gDS1302_TIME[i]=ds1302_read_byte(gREAD_RTC_ADDR[i]);
	}
}

