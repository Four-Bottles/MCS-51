
#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
uchar buf;

unsigned char num;
sbit dula=P2^6;		//段选信号的锁存器控制
sbit wela=P2^7;		//位选信号的锁存器控制
sbit beep=P2^3;
sbit lcden=P3^4;
sbit rs=P3^5;
sbit rw=P3^6;	

unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
						//0-F的码表
uchar table1[]="   Ning Kejun   ";
uchar table2[]="                ";
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

void main()
{
		uchar a;
	init();
	
	while(1)
	{
	
  
	SCON=0x50;           //设定串口工作方式
    PCON=0x00;           //波特率不倍增
			
    TMOD=0x20;           //定时器1工作于8位自动重载模式, 用于产生波特率
    EA=1;
    ES = 1;              //允许串口中断
    TL1=0xfd;
    TH1=0xfd;             //波特率9600
    TR1=1;
	
	write_com(0x80);	 
	delay(20);
	for(a=0;a<16;a++)
	{
	write_date(table1[a]);
	delay(20);		
	}

	write_com(0xc0);
	delay(50);
	for(a=0;a<16;a++)
	{
	write_date(table2[a]);
	delay(40);		

			
	
	}
  }
}

void  serial() interrupt 4 
{
   ES = 0;                //关闭串行中断
   RI = 0;                //清除串行接受标志位
   buf = SBUF;            //从串口缓冲区取得数据
	
	num = (buf - '0');
	P0=table[num];	   
			dula=1;
			dula=0;

			P0=0xfe;	   //选中第一个数码管
			wela=1;
			wela=0;
			delay(1000);
	
  switch(buf)
   {
      case 0x31:  P1=0xfe;beep=1;table2[1]=buf;break;  //接受到1，第一个LED亮         
      case 0x32:  P1=0xfd;beep=1;table2[1]=buf;break;  //接受到2，第二个LED亮        
      case 0x33:  P1=0xfb;beep=1;table2[1]=buf;break;  //接受到3，第三个LED亮        
      case 0x34:  P1=0xf7;beep=1;table2[1]=buf;break;  //接受到4，第四个LED亮       
      case 0x35:  P1=0xef;beep=1;table2[1]=buf;break;  //接受到5，第五个LED亮            
      case 0x36:  P1=0xdf;beep=1;table2[1]=buf;break;  //接受到6，第六个LED亮                   
      case 0x37:  P1=0xbf;beep=1;table2[1]=buf;break;  //接受到7，第七个LED亮
	  case 0x38:  P1=0x7f;beep=1;table2[1]=buf;break;  //接受到8，第八个LED亮
	  default:    beep=0;P1=0xff;break;  //接受到其它数据，蜂鸣器响         
   }
   ES = 1;    //允许串口中断
}
