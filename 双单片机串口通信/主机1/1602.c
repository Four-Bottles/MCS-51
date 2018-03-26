
#include <reg52.h>
#define jingzhen     11059200UL			 /*使用22.1184M晶体*/	 //
#define botelv   9600UL		     /*波特率定义为9600*/
unsigned char zifu1='1';			//待显示字符。
unsigned char zifu2='2';	
unsigned char zifu3='3';	
unsigned char zifu4='4';	
volatile unsigned char sending;
sbit S2=P3^0;
sbit S3=P3^1;
sbit S4=P3^2;
sbit S5=P3^3;

#define uchar unsigned char
#define uint unsigned int
sbit lcden=P3^4;
sbit rs=P3^5;
sbit rw=P3^6;	
sbit dula=P2^6;
sbit wela=P2^7;
uchar table1[]="I'm Ning Kejun";
uchar table2[]="  2015211862  ";
void delay(uint x)
{
	uint a,b;
	for(a=x;a>0;a--)
		for(b=10;b>0;b--); 
}

void delay1(uint x)
{
	uint a,b;
	for(a=x;a>0;a--)
		for(b=100;b>0;b--); 
}

void write_com(uchar com)
{
	P0=com;
	rs=0;
	lcden=0;
	delay(10);
	lcden=1;
	delay(10);
	lcden=0;
	
}

void write_date(uchar date)
{
	P0=date;
	rs=1;
	lcden=0;
	delay(10);
	lcden=1;
	delay(10);
	lcden=0;
	
}

void init()
{
		EA=0; //暂时关闭中断
 TMOD&=0x0F;  //定时器1模式控制在高4位
 TMOD|=0x20;    //定时器1工作在模式2，自动重装模式
 SCON=0x50;     //串口工作在模式1
 TH1=256-jingzhen/(botelv*12*16);  //计算定时器重装值
 TL1=256-jingzhen/(botelv*12*16);
 PCON|=0x80;    //串口波特率加倍
 ES=1;         //串行中断允许
 TR1=1;        //启动定时器1
 REN=1;        //允许接收 
 EA=1;         //允许中断
	
        rw=0;
	dula=0;
	wela=0;
	write_com(0x38);   //显示模式设置：16×2显示，5×7点阵，8位数据接口
	delay(20);
	write_com(0x0f);   //显示模式设置
	delay(20);
	write_com(0x06);   //显示模式设置：光标右移，字符不移
	delay(20);
	write_com(0x01);   //清屏幕指令，将以前的显示内容清除
	delay(20);	
}
void send(unsigned char d)		  //发送一个字节的数据，形参d即为待发送数据。
{
 
 SBUF=d; //将数据写入到串口缓冲
 sending=1;	 //设置发送标志
 while(sending); //等待发送完毕
}

void main()
{
	uchar a;
	init();

	write_com(0x80+17);	 //将第一个字符写在向右偏移17个字符处，为后面的由右向左划入做准备。
	delay(20);
	for(a=0;a<14;a++)
	{
	write_date(table1[a]);
	delay(20);		
	}

	write_com(0xc0+17);
	delay(50);
	for(a=0;a<13;a++)
	{
	write_date(table2[a]);
	delay(40);		
	}

	for(a=0;a<16;a++)
	{
		write_com(0x18); //左移
		delay1(600);
	}
	while(1)
	{
		if(S2==0)
		{
			delay(20);
			if(!S2)
			{
				while(!S2);		   
				send(zifu1);
			}
		}
		if(S3==0)
		{
			delay(20);
			if(!S3)
			{
				while(!S3);		   
				send(zifu2);
			}
		}

		if(S4==0)
		{
			delay(20);
			if(!S4)
			{
				while(!S4);		   
				send(zifu3);
			}
		}
		
		if(S5==0)
		{
			delay(20);
			if(!S5)
			{
				while(!S5);		   
				send(zifu4);
			}
		}
		
	}
}
void uart(void) interrupt 4		 //串口发送中断
{
 if(RI)    //收到数据
 {
  RI=0;   //清中断请求
 }
 else      //发送完一字节数据
 {
  TI=0;
  sending=0;  //清正在发送标志
 }
}
