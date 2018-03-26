
#include <reg52.h>
#define jingzhen     11059200UL			 /*ʹ��22.1184M����*/	 //
#define botelv   9600UL		     /*�����ʶ���Ϊ9600*/
unsigned char zifu1='1';			//����ʾ�ַ���
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
		EA=0; //��ʱ�ر��ж�
 TMOD&=0x0F;  //��ʱ��1ģʽ�����ڸ�4λ
 TMOD|=0x20;    //��ʱ��1������ģʽ2���Զ���װģʽ
 SCON=0x50;     //���ڹ�����ģʽ1
 TH1=256-jingzhen/(botelv*12*16);  //���㶨ʱ����װֵ
 TL1=256-jingzhen/(botelv*12*16);
 PCON|=0x80;    //���ڲ����ʼӱ�
 ES=1;         //�����ж�����
 TR1=1;        //������ʱ��1
 REN=1;        //������� 
 EA=1;         //�����ж�
	
        rw=0;
	dula=0;
	wela=0;
	write_com(0x38);   //��ʾģʽ���ã�16��2��ʾ��5��7����8λ���ݽӿ�
	delay(20);
	write_com(0x0f);   //��ʾģʽ����
	delay(20);
	write_com(0x06);   //��ʾģʽ���ã�������ƣ��ַ�����
	delay(20);
	write_com(0x01);   //����Ļָ�����ǰ����ʾ�������
	delay(20);	
}
void send(unsigned char d)		  //����һ���ֽڵ����ݣ��β�d��Ϊ���������ݡ�
{
 
 SBUF=d; //������д�뵽���ڻ���
 sending=1;	 //���÷��ͱ�־
 while(sending); //�ȴ��������
}

void main()
{
	uchar a;
	init();

	write_com(0x80+17);	 //����һ���ַ�д������ƫ��17���ַ�����Ϊ�����������������׼����
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
		write_com(0x18); //����
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
void uart(void) interrupt 4		 //���ڷ����ж�
{
 if(RI)    //�յ�����
 {
  RI=0;   //���ж�����
 }
 else      //������һ�ֽ�����
 {
  TI=0;
  sending=0;  //�����ڷ��ͱ�־
 }
}
