
#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
uchar buf;

unsigned char num;
sbit dula=P2^6;		//��ѡ�źŵ�����������
sbit wela=P2^7;		//λѡ�źŵ�����������
sbit beep=P2^3;
sbit lcden=P3^4;
sbit rs=P3^5;
sbit rw=P3^6;	

unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
						//0-F�����
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
	write_com(0x38);   //��ʾģʽ���ã�16��2��ʾ��5��7����8λ���ݽӿ�
	delay(20);
	write_com(0x0f);   //��ʾģʽ����
	delay(20);
	write_com(0x06);   //��ʾģʽ���ã�������ƣ��ַ�����
	delay(20);
	write_com(0x01);   //����Ļָ�����ǰ����ʾ�������
	delay(20);	
}

void main()
{
		uchar a;
	init();
	
	while(1)
	{
	
  
	SCON=0x50;           //�趨���ڹ�����ʽ
    PCON=0x00;           //�����ʲ�����
			
    TMOD=0x20;           //��ʱ��1������8λ�Զ�����ģʽ, ���ڲ���������
    EA=1;
    ES = 1;              //�������ж�
    TL1=0xfd;
    TH1=0xfd;             //������9600
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
   ES = 0;                //�رմ����ж�
   RI = 0;                //������н��ܱ�־λ
   buf = SBUF;            //�Ӵ��ڻ�����ȡ������
	
	num = (buf - '0');
	P0=table[num];	   
			dula=1;
			dula=0;

			P0=0xfe;	   //ѡ�е�һ�������
			wela=1;
			wela=0;
			delay(1000);
	
  switch(buf)
   {
      case 0x31:  P1=0xfe;beep=1;table2[1]=buf;break;  //���ܵ�1����һ��LED��         
      case 0x32:  P1=0xfd;beep=1;table2[1]=buf;break;  //���ܵ�2���ڶ���LED��        
      case 0x33:  P1=0xfb;beep=1;table2[1]=buf;break;  //���ܵ�3��������LED��        
      case 0x34:  P1=0xf7;beep=1;table2[1]=buf;break;  //���ܵ�4�����ĸ�LED��       
      case 0x35:  P1=0xef;beep=1;table2[1]=buf;break;  //���ܵ�5�������LED��            
      case 0x36:  P1=0xdf;beep=1;table2[1]=buf;break;  //���ܵ�6��������LED��                   
      case 0x37:  P1=0xbf;beep=1;table2[1]=buf;break;  //���ܵ�7�����߸�LED��
	  case 0x38:  P1=0x7f;beep=1;table2[1]=buf;break;  //���ܵ�8���ڰ˸�LED��
	  default:    beep=0;P1=0xff;break;  //���ܵ��������ݣ���������         
   }
   ES = 1;    //�������ж�
}
