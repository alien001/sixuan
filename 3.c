#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char//宏定义
#define uint unsigned int//宏定义 
uchar  hour,min,sec;		  //显示时间
uchar  hour1=0,min1=0,sec1=0;	  //设定闹钟时间
uchar  hour2=0,min2=0,sec2=0;	  //中间比较时间
uchar  shi,fen,miao;
uchar nian,yue,ri;
uchar year,month,day;
uchar n,j;
uchar c = 1;  //清零一次
uchar key = 0;
uchar noiscum = 0;
uchar data_7seg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90}; //时钟数码管显示定义0~9
 uchar n=0;//S1按键功能选择常数
sbit sck=P1^1;//定义控制DS1302的时钟线
sbit io=P1^0;//定义控制DS1302的串行数据
sbit rst=P1^2;// 复位线
sbit P3_5=P3^5;    //按键S1
sbit P3_6=P3^6;     //按键S2
sbit P3_7=P3^7;     //按键S3
sbit P3_1=P3^1;     //按键S3
sbit led=P1^7;
sbit nois = P1^6;
////函数声明
//void write_ds1302_byte(uchar dat); //写一个字节
//void write_ds1302(uchar add,uchar dat);	   //写多个字节
//uchar read_ds_1302(uchar add);			  //读取
//void read_rtc();					  //读取时间
//void set_rtc();						  //调整时间
//void display();						  //显示时间
//void delay(int n);					 //延时函数
////ms延时函数
void delayms(uint n)
{
   uchar i;
   while(n--)
   for(i=0;i<113;i++);
}
/*DS1302单字节写入；*/
void write_ds1302_byte(uchar dat)
{
     uchar i;
	 for(i=0;i<8;i++)
	 {
	    sck=0;       
		io=dat&0x01;      //每次从低字节开始传输
		dat=dat>>1;
		sck=1;
	}
}
/*DS1302 多字节写入；*/
void write_ds1302(uchar add,uchar dat)
{
     rst=0;_nop_();              //res引脚为低，数据传输中止
	 sck=0;	_nop_();              //清零时钟总线
	 rst=1;_nop_();             //res引脚为高，逻辑控制有效
	 write_ds1302_byte(add);    
	 write_ds1302_byte(dat);    
	 rst=0;_nop_();
	 io=1;
	 sck=1;
}
/*DS1302读取；*/
uchar read_ds1302(uchar add)
{
    uchar i,value;
	rst=0;_nop_();
	sck=0;_nop_();
	rst=1;_nop_();
	write_ds1302_byte(add);  //发送地址
	for(i=0;i<8;i++)
	{
	    value=value>>1;
		sck=0;
		if(io)value=value|0x80;   //从低到高
		sck=1;
	}
	 rst=0;_nop_();
	 sck=0;_nop_();
	 sck=1;
	 io=1;
	 return value;
}
/*按键控制时钟/闹钟模式*/
void set_rtc()
{
if(P3_1 == 0)
{
   delayms(30);
	    if(P3_1==0)
	     {
			while(!P3_1);
		    key = 1;	   //根据按次数控制模式
	     }
}
  	if(P3_5==0)	 //按下是0
   	{
		delayms(30);
	    if(P3_5==0)
	     {
			while(!P3_5);
		    n++;	   //根据按次数控制模式
	     }
		if(n>=3)
		n=0;
    }
  if(P3_6==0)	 
    {
 	 delayms(100);
     if(P3_6==0)
    	  {
			while(!P3_6);
	   		 j++;	        //根据按键次数控制模式
	   	  }
	if(j>=3)
	j=0;
     }
  if(n==0)	  //显示时间
   {   
    if(j==0) //时
	{
	    shi=P3_7;
       if(shi==0)
	   {
	   delayms(300);
	    if(shi==0)
		{ 
		    hour=(hour>>4)*10+(hour&0x0f);
			hour++;
			if(hour==24) hour=0;
			hour=((hour/10)<<4)+(hour%10);
			write_ds1302(0x84,hour);
		 }
	     }
		}
	  if(j==1)		//分
	   {
	     fen=P3_7;
	    if(fen==0)
		{ delayms(300);
			if(fen==0)
			{
		    min=(min>>4)*10+(min&0x0f);
			min++;
			if(min==60) min=0;
			min=((min/10)<<4)+(min%10);
			write_ds1302(0x82,min);
		   }
	    }
	  }
	  if(j==2)		  //秒
	  {
	    miao=P3_7;
	     if(miao==0)
	     {
	     delayms(300);
	      if(miao==0)
		   { 
		    sec=(sec>>4)*10+(sec&0x0f);
			sec++;
			if(sec==60) sec=0;
			sec=((sec/10)<<4)+(sec%10);
			write_ds1302(0x80,sec);
		   }
	     }
	  }
   }

  if(n==1)	   //显示日历
   {  	
   if(j==3)j=0;
     { 
    	if(j==0)
	   {	 
	    nian=P3_7;
	     if(nian==0)
	     {	   
	     delayms(300);
	      if(nian==0)
		   { 
		    year=(year>>4)*10+(year&0x0f);
			year++;
			if(year==100) year=0;
			year=((year/10)<<4)+(year%10);
			write_ds1302(0x8c,year);
		   }
	     }
	  }
		if(j==1)
	  {
	    yue=P3_7;
	     if(yue==0)
	     {
	     delayms(300);
	      if(yue==0)
		   { 
		    month=(month>>4)*10+(month&0x0f);
			month++;		  
			if(month==13) month=0;
			month=((month/10)<<4)+(month%10);
			write_ds1302(0x88,month);
		   }
	     }
	  }
	  
	 if(j==2)
	 {ri=P3_7;
	  if(ri==0)
	  {
	  delayms(300);
	    if(ri==0)
		{ 
		    day=(day>>4)*10+(day&0x0f);
			day++;
			if(day==31) day=0;
			day=((day/10)<<4)+(day%10);
			write_ds1302(0x86,day);
		 }
	  } 
	    
    } 
  }
 }
 if(n==2)		 //设定闹钟时间
 {	 while(c)
	 {
     P2=0x01;
	 P0=data_7seg[0/16];
	 delayms(4);
	 P2=0x02;
	 P0=data_7seg[0%16];
	 delayms(4);
	 P2=0x04;
	 P0=~0x40;
	 delayms(4);
	 P2=0x08;
	 P0=data_7seg[0/16];
	 delayms(4);
	 P2=0x10;
	 P0=data_7seg[0%16];
	 delayms(4);
	 P2=0x20;
	 P0=~0x40;
	 delayms(4);
	 P2=0x40;
	 P0=data_7seg[0/16];
	 delayms(4);
	 P2=0x80;
	 P0=data_7seg[0%16];
	 delayms(4);	  //清显示
	 c=c-1;
	 }
	if(j==0) //时
	{
	    shi=P3_7;
       if(shi==0)
	   {
	   delayms(300);
	    if(shi==0)
		{
	        //hour=(hour>>4)*10+(hour&0x0f);
			hour1++;
			if(hour1==24) hour1=0;
			//hour1=((hour1/10)<<4)+(hour1%10);
			//write_ds1302(0x84,hour);
			//hour1 = hour;
		 }
	     }
		}
	  if(j==1)		//分
	   {
	     fen=P3_7;
	    if(fen==0)
		{ delayms(300);
			if(fen==0)
			{
		    //min=(min>>4)*10+(min&0x0f);
			min1++;
			if(min1==60) min1=0;
			//min1=((min1/10)<<4)+(min1%10);
			//write_ds1302(0x82,min);
			//min1 = min;
		   }
	    }
	  }
	  if(j==2)		  //秒
	  {
	    miao=P3_7;
	     if(miao==0)
	     {
	     delayms(300);
	      if(miao==0)
		   { 
		    //sec=(sec>>4)*10+(sec&0x0f);
			sec1++;
			if(sec1==60) sec1=0;
			//sec1=((sec1/10)<<4)+(sec%10);
			//write_ds1302(0x80,sec);
			 // sec1 = sec;
		   }
	     }
	  }
  }
 }


/*读取时间；*/
void read_rtc()
{
    hour=read_ds1302(0x85);	
//	hour2=((hour/10)<<4)+(hour%10);
	min=read_ds1302(0x83);
//	min2=((min/10)<<4)+(min%10);
	sec=read_ds1302(0x81);
//	sec2=((sec/10)<<4)+(sec%10);
	year=read_ds1302(0x8d);
	month=read_ds1302(0x89);
	day=read_ds1302(0x87);
}
/*显示时间；*/
void display()
{
	 
	 P2=0x01;
	 P0=data_7seg[hour/16];
	 delayms(4);
	 P2=0x02;
	 P0=data_7seg[hour%16];
	 delayms(4);
	 P2=0x04;
	 P0=~0x40;
	 delayms(4);
	 P2=0x08;
	 P0=data_7seg[min/16];
	 delayms(4);
	 P2=0x10;
	 P0=data_7seg[min%16];
	 delayms(4);
	 P2=0x20;
	 P0=~0x40;
	 delayms(4);
	 P2=0x40;
	 P0=data_7seg[sec/16];
	 delayms(4);
	 P2=0x80;
	 P0=data_7seg[sec%16];
	 delayms(4);
}
/*显示日历*/
void display1()
{
     P2=0x01;
	 P0=data_7seg[2];
	 delayms(4);
	 P2=0x02;
	 P0=data_7seg[0];
	 delayms(4);
	 P2=0x04;
	 P0=data_7seg[year/16];
	 delayms(4);
	 P2=0x08;
	 P0=data_7seg[year%16]&0x7f;
	 delayms(4);
	 P2=0x10;
	 P0=data_7seg[month/16];
	 delayms(4);
	 P2=0x20;
	 P0=data_7seg[month%16]&0x7f;
	 delayms(4);
	 P2=0x40;
	 P0=data_7seg[day/16];
	 delayms(4);
	 P2=0x80;
	 P0=data_7seg[day%16];
	 delayms(4);
}

void display2()		   //显示闹钟
{
     P2=0x01;
	 P0=data_7seg[hour1/10];
	 delayms(4);
	 P2=0x02;
	 P0=data_7seg[hour1%10];
	 delayms(4);
	 P2=0x04;
	 P0=~0x40;
	 delayms(4);
	 P2=0x08;
	 P0=data_7seg[min1/10];
	 delayms(4);
	 P2=0x10;
	 P0=data_7seg[min1%10];
	 delayms(4);
	 P2=0x20;
	 P0=~0x40;
	 delayms(4);
	 P2=0x40;
	 P0=data_7seg[sec1/10];
	 delayms(4);
	 P2=0x80;
	 P0=data_7seg[sec1%10];
	 delayms(4);
}
///*显示学号；*/
//void show()
//{
//    uchar m;
//	for(m=0;m<50;m++)
//	{	  
//		P2=0x01;P0=data_7seg[7];delayms(4);
//		P2=0x02;P0=data_7seg[5];delayms(4);
//		P2=0x04;P0=data_7seg[0];delayms(4);
//		P2=0x08;P0=data_7seg[4];delayms(4);
//		P2=0x10;P0=data_7seg[4];delayms(4);
//		P2=0x20;P0=data_7seg[2];delayms(4);
//		P2=0x40;P0=data_7seg[0];delayms(4);	
//		P2=0x80;P0=data_7seg[4];delayms(4);		
//	}
//}
void  ss()
{
    //uchar i;
//   for(i=0;i<100;i++)
//   {
   	   led=!led;
	   delayms(2);
//   }
  }

void main()
{
//     show();	 
	 write_ds1302(0x84,0x12);	  //小时
	 write_ds1302(0x82,0x00);	 //分
	 write_ds1302(0x80,0x00);	//秒
	 write_ds1302(0x8c,0x18);		  //年
	 write_ds1302(0x88,0x06);		 //月
	 write_ds1302(0x86,0x11);		//天
	 nois = 0;
	 while(1)
	 {	 
	    set_rtc();
        read_rtc();	 //读取时间
		hour2 = ((hour1/10)<<4)+(hour1%10);		//二进制的BCD码转化为十进制数
		min2 = ((min1/10)<<4)+(min1%10);
		sec2 = ((sec1/10)<<4)+(sec1%10);
		if(hour2 == hour&&min2 == min&& sec2 == sec)//读取回时间和设定时间对比
		{	
			nois = 1;	
		}
//		if(key == 0)			//key外部停止闹钟报警key == 1停止
//				{
//					nois = 1;//做报警
//				}
		if(key == 1)
			{ 
				nois = 0;//清除报警
				noiscum++;
				if(noiscum == 5)
				{
					  noiscum =0;
					  key = 0;
				}

			}
		if(n==0)	   //时间
		{
			display();
		}		
		if(n==1)		 //日期
		{		
			display1();
		}
		if(n==2)		 //闹钟设定时间
		{		
			display2();
		}
		if(min==0&sec<5)   //五秒闪烁
		{
			ss();
		}		    
	   	else 
		{
		led=0x01;
		}
      }
}

