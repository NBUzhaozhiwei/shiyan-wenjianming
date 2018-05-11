#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit DQ=P1^4;
uchar one,two,three,four,five,six,seven,eight;
uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff}; //-  ????  
uchar code dot[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
/*******************************************??????**************************************************/
#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();}
//??????
sbit SDA = P2^1;  /* ??? */
sbit SCL = P2^0;  /* ??? */
void IIC_Start(void); 
void IIC_Stop(void);  
void IIC_Ack(unsigned char ackbit); 
void IIC_SendByte(unsigned char byt); 
bit IIC_WaitAck(void);  
unsigned char IIC_RecByte(void);
uchar IIC_read(uchar add);
/*****************************************************??????*****************************************/
void delayms(uchar xms);
void closebuzz();
void display1(uchar one,uchar two);
void display2(uchar three,uchar four);
void display3(uchar five,uchar six);
void display4(uchar seven,uchar eight);
/***********************************************????????*********************************************/
sbit s7=P3^0;
sbit s6=P3^1;
uchar flag=0;
uint money=0;
 uchar num1=0;
uint counter=0;
void ADtrans();
void keyscan();
void Timer0Init();
/******************************************************???***********************************************/
void main()
{	   closebuzz();
   	Timer0Init();
     one=11;two=0;three=5;four=0;five=0;six=0;seven=0;eight=0;
	  while(1)
	  {	  ADtrans();
			  keyscan();
			five=counter/10000;six=counter%10000/1000;seven=counter%1000/100;eight=counter%100/10;
	      
	   
	     display1(one,two);
		   display2(three,four);
		   display3(five,six);
		   display4(seven,eight); 
		
			     
	  }    
}
/*************************************************??????*****************************************/
void T0time() interrupt 1
{  
	   TH0=(65536-10000)/256;
     TL0=(65536-10000)%256; 
	   num1++;
	   if(num1==1)
		 {  counter++;
			  num1=0; 
		 }	
}
void Timer0Init(void)		
{
	//AUXR |= 0x80;		
	TMOD = 0x01;		
	TH0=(65536-1000)/256;
  TL0=(65536-1000)%256;
	TF0 = 0;	
  ET0=1;	
	EA=1;	
}
/************************************************??????*********************************************/
void keyscan()
{  
	   if(s7==0)
		 { delayms(10);
			  if(s7==0)
				{ if(flag==1)
	  			 { counter=0;
						  flag=0;
					 }	
					 P2=0xa0;P0=0x10;	
           TR0=1;         					 
				}
				while(!s7); 
		 }
		 if(s6==0)
		 {  delayms(10);
			  if(s6==0)
				{ flag=1;
					P2=0xa0;P0=0x00;
          TR0=0;	
					counter=counter*5/10;   					
				}	
       while(!s6);				
		 }	 
} 
/******************************************AD????************************************************/
void ADtrans()
{   uchar num;
	  num=IIC_read(0x01);
	 if(num<25)
	   {
		 P2=0x80;P0=0xfe;
	   }
	 else
	  {
		 P2=0x80;P0=0xff;		 
	  }
}
/***********************************************??????********************************************/
uchar IIC_read(uchar add)
{  uchar temp;
	   IIC_Start();
	  IIC_SendByte(0x90);
	   IIC_WaitAck();
	   IIC_SendByte(add);
	    IIC_WaitAck();
	    IIC_Stop();
	
	   IIC_Start();
	   IIC_SendByte(0x91);
	   IIC_WaitAck();
	   temp=IIC_RecByte();
	   IIC_Ack(1);
	   IIC_Stop();
	   temp=0.39*temp;
	   return temp;
}

//??????
void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	somenop;
	SDA = 0;
	somenop;
	SCL = 0;	
}

//??????
void IIC_Stop(void)
{
	SDA = 0;
	SCL = 1;
	somenop;
	SDA = 1;
}

//?????  
void IIC_Ack(unsigned char ackbit)
{
	if(ackbit) 
	{	
		SDA = 0;
	}
	else 
	{
		SDA = 1;
	}
	somenop;
	SCL = 1;
	somenop;
	SCL = 0;
	SDA = 1; 
	somenop;
}

//????  
bit IIC_WaitAck(void)	  //ack() 
{
	SDA = 1;
	somenop;
	SCL = 1;
	somenop;
	if(SDA)    
	{   
		SCL = 0;
		IIC_Stop();
		return 0;
	}
	else  
	{ 
		SCL = 0;
		return 1;
	}
}

//??I2C??????	  
void IIC_SendByte(unsigned char byt)	 // writebyte() 
{
	unsigned char i;
	for(i=0;i<8;i++)
	{   
		if(byt&0x80) 
		{	
			SDA = 1;
		}
		else 
		{
			SDA = 0;
		}
		somenop;
		SCL = 1;
		byt <<= 1;
		somenop;
		SCL = 0;
	}
}

//?I2C???????      
unsigned char IIC_RecByte(void)		 //readbyte() 
{
	unsigned char da;
	unsigned char i;
	
	for(i=0;i<8;i++)
	{   
		SCL = 1;
		somenop;
		da <<= 1;
		if(SDA) 
		da |= 0x01;
		SCL = 0;
		somenop;
	}
	return da;
}
/*************************************************??????***********************************************/

/*************************************************??????******************************************/
void  delayms(uchar xms)
{	uint i,j;
  for(i=xms;i>0;i--)
    for(j=110;j>0;j--);  
}
void closebuzz()   //???? 
{  P2=0xa0;
   P0=0x00;  
   P2=0x80;
   P0=0xff;	
}
void display1(uchar one,uchar two)
{	P2=0xc0;
    P0=0x01;
	P2=0xe0;
	P0=table[one];
	delayms(5);

	P2=0xc0;
	P0=0x02;
	P2=0xe0;
	P0=dot[two]; 
	delayms(5);      
}

void display2(uchar three,uchar four)
{	P2=0xc0;
    P0=0x04;
	P2=0xe0;
	P0=table[three];
	delayms(5);

	P2=0xc0;
	P0=0x08;
	P2=0xe0;
	P0=table[four];  
	delayms(5);     
}
void display3(uchar five,uchar six)
{	P2=0xc0;
    P0=0x10;
	P2=0xe0;
	P0=table[five];
	delayms(5);

	P2=0xc0;
	P0=0x20;
	P2=0xe0;
	P0=dot[six];  
	delayms(5);     
}
void display4(uchar seven,uchar eight)
{	P2=0xc0;
    P0=0x40;
	P2=0xe0;
	P0=table[seven]; 
	delayms(5);

	P2=0xc0;
	P0=0x80;
	P2=0xe0;
	P0=table[eight];  
	delayms(5);     
}
