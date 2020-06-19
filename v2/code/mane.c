#include <reg51.h>
#include <stdio.h>

#define lcdport P1
#define scanport P2
sbit s0 = scanport^2;
sbit s1 = scanport^3;
sbit rs = lcdport^3;
sbit en  = lcdport^2;
sbit sublot0 = P3^6;
sbit sublot1 = P3^7;
sbit sublot2 = P1^1;
sbit sublot3 = P1^0;


unsigned int i,j,k,ang,tim;
unsigned char z;
unsigned char count=0;
unsigned char temp=0;
sbit LDR= P3^0;                                  //input from LDR
sbit Led=P3^1;									 //Output for LED
sbit big_entry = P3^4; 
sbit big_exit = P3^5;                                  
sfr ldata = 0x90;																//configuring port 1 for LCD
unsigned char emp1[]= "EMPTY LOT ";
unsigned char fu[]= "FULL LOT ";
unsigned char noc[]= "No. Of Cars: ";
int value[2] = {0};
//integer -> seperate  numerical characters
int* div_num(unsigned int t)
{
	if(t<10)
	{
		value[0] = t;
		value[1] = 0;
	}
	if(t>=10)
	{
		value[1] = t/10;
		value[0] = t%10;
	}
	return value;
}


void dela(int a)//delay for lcd
{
   int i;
   for(i=0;i<a;i++);   //null statement
}


void Delay(unsigned int x)                          
 // Generic function for 10ms Delay
{  
	for(i=0;i<x;i++)
	for (j=0;j<10000;j++);
}
void Delay123(unsigned int x)  		   
// Generic function for 1ms Delay(for motor )
{  
	for(i=0;i<x;i++)
	for (j=0;j<1000;j++);
}

void scan_sublots()
{
	scanport = 0xf0;
	temp=0;
	//scan sublot-lot 0
	s0 = 0;
	s1 = 0;
	temp = scanport;//scanned values on higher bits
	temp = temp&0xf0;//Mask lower bits
	if(temp)
	sublot0 = 1;
	if(!temp)
	sublot0 = 0;
	//scan sublot-lot 1
	s0 = 1;
	s1 = 0;
	temp = scanport;//scanned values on higher bits
	temp = temp&0xf0;//Mask lower bits
	if(temp)
	sublot1 = 1;
	if(!temp)
	sublot1 = 0;
	//scan sublot-lot 2
	s0 = 0;
	s1 = 1;
	temp = scanport;//scanned values on higher bits
	temp = temp&0xf0;//Mask lower bits
	if(temp)
	sublot2 = 1;
	if(!temp)
	sublot2 = 0;
	//scan sublot-lot 3
	s0 = 1;
	s1 = 1;
	temp = scanport;//scanned values on higher bits
	temp = temp&0xf0;//Mask lower bits
	if(temp)
	sublot3 = 1;
	if(!temp)
	sublot3 = 0;
	temp=0;
	
}

void gate_operate_1(unsigned int ang,unsigned int tim)
{
	for(z=0;z<(ang*2);z++)//Change multiplier according to motor
	{
	P0=0x03;         //00000011
	Delay123(3);             
	P0=0x06;        //00000110
	Delay123(3);
	P0=0x0C;	   //00001100
	Delay123(3);
	P0=0x09;	  //00001001
    Delay123(3);
	}
	Delay(tim);                        //Barrier opens for seconds(Choose)
  	for(z=0;z<(ang*2);z++)//Change multiplier according to motor
	{
	P0=0x09;	  //00001001
    Delay123(3);
	P0=0x0C;	   //00001100
	Delay123(3);
	P0=0x06;        //00000110
	Delay123(3);
	P0=0x03;         //00000011
	Delay123(3);
	}             //Motor in opposite direction to close the barrier
	Delay123(3);
	P0=0x06;
	Delay123(3);
    P0=0x00;
}

void gate_operate_2(unsigned int ang, unsigned int tim)
{
	for(z=0;z<(ang*2);z++)//Change multiplier according to motor
	{
	P0=0x30;         //00110000
    Delay123(3);
	P0=0x60;        //01100000
	Delay123(3);
	P0=0xC0;	   //11000000
	Delay123(3);
	P0=0x90;	  //10010000
    Delay123(3);
	}
	Delay(tim);                        //Barrier opens for seconds(Choose)
  	for(z=0;z<(ang*2);z++)//Change multiplier according to motor
	{
	P0=0x90;	  //10010000
    Delay123(3);
	P0=0xC0;	   //11000000
	Delay123(3);
	P0=0x60;        //01100000
	Delay123(3);
	P0=0x30;         //00110000
	Delay123(3);
	}
	Delay123(3);
	P0=0x60;
	Delay123(3);
	P0=0x00;
}


void lcdcommand(unsigned char a)
{
   rs = 0;             // This is command

   lcdport &= 0x0F;		   // Make P2.4 to P2.7 zero
   lcdport |= (a&0xF0);     // Write Upper nibble of data

   en  = 1;             // => en = 1
   dela(150);
   en  = 0;             // => en = 0
   dela(150);

   lcdport &= 0x0F;		   // Make P2.4 to P2.7 zero
   lcdport |= ((a<<4)&0xF0);// Write Lower nibble of data

   en  = 1;             // => en = 1
   dela(150);
   en  = 0;             // => en = 0
   dela(150);
}

void data1(unsigned char b)
{
   rs = 1;             // This is data

   lcdport &= 0x0F;		   // Make P2.4 to P2.7 zero
   lcdport |= (b&0xF0);     // Write Upper nibble of data

   en  = 1;             // => en = 1
   dela(150);
   en  = 0;             // => en = 0
   dela(150);

   lcdport &= 0x0F;		   // Make P2.4 to P2.7 zero
   lcdport |= ((b<<4)&0xF0);// Write Lower nibble of data

   en  = 1;             // => en = 1
   dela(150);
   en  = 0;             // => en = 0
   dela(150);
}
void init(void)
{
  ///////////// Reset process from datasheet /////////
     dela(15000);

	 lcdport &= 0x0F;		   // Make P2.4 to P2.7 zero
	 lcdport |= (0x30&0xF0);    // Write 0x3
	
	 en  = 1;               // => en = 1
	 dela(150);
	 en  = 0;               // => en = 0
	 dela(150);

     dela(4500);

	 lcdport &= 0x0F;		   // Make P2.4 to P2.7 zero
	 lcdport |= (0x30&0xF0);    // Write 0x3
	
	 en  = 1;               // => en = 1
	 dela(150);
	 en  = 0;               // => en = 0
	 dela(150);

     dela(300);

	 lcdport &= 0x0F;		   // Make P2.4 to P2.7 zero
	 lcdport |= (0x30&0xF0);    // Write 0x3
	
	 en  = 1;               // => en = 1
	 dela(150);
	 en  = 0;               // => en = 0
	 dela(150);

     dela(650);

	 lcdport &= 0x0F;		   // Make P2.4 to P2.7 zero
	 lcdport |= (0x20&0xF0);    // Write 0x2
	
	 en  = 1;               // => en = 1
	 dela(150);
	 en  = 0;               // => en = 0
	 dela(150);

	 dela(650);

  /////////////////////////////////////////////////////
   lcdcommand(0x28);    //function set
   lcdcommand(0x0c);    //display on,cursor off,blink off
   lcdcommand(0x01);    //clear display
   lcdcommand(0x06);    //entry mode, set increment
}

void emptylot(void)
{
//Empty lot message
	lcdcommand(0x01);
    for(z=0;z<9;z++)
    { 
			data1(emp1[z]);
    }
	lcdcommand(0x0c);
}

void fulllot(void)
{
   //unsigned char ful[]= "    FULL LOT";
    lcdcommand(0x01);
    for(z=0;z<8;z++)
    { 
			data1(fu[z]);
    }
	lcdcommand(0x0c);
}

void display_no(void)
{
//Display no of cars
		lcdcommand(0x01);
		for(z=0;z<13;z++)
		{ 
			data1(noc[z]);
		}					
		div_num(count);
		for(z= 0 ; z < 2 ; z++ )
    		data1(value[1-z]+48);
		lcdcommand(0x0c);
}

//**********************Interrupt for enter******************//
void Gate_Enter() interrupt 0                                   //Interrupt INT0 p3.2, address=0003H
{  
	unsigned char temp=count;
	ang = 10;
	if(big_entry) 
	ang = 20;
	gate_operate_1(ang,50);

  count++;
	temp=count+48;
  switch (temp)
  {
    case (48):
    {
   	emptylot();
   	break;
    }  
    case (64):
    {
    fulllot();
    break;
    }
    default:
    {  
	display_no();
	break;
    }   
   }
  }
 //******************Interrupt for exit******************//
void Gate_Exit() interrupt 2                                   //Interrupt INT1 p3.3 , address=0013H
{  
unsigned char temp=count;
	ang = 10;
	if(big_exit) 
	ang = 20; 
	gate_operate_2(ang,50);
  count--;
	temp=count+48;
  switch (temp)
  {
    case (48):
    {
   	emptylot();
   	break;
    }  
    case (64):
    {
    fulllot();
    break;
    }
    default:
    {  
	display_no();
	break;
    }   
   }
}
	//*************************main********************************//
void main()
{
	unsigned char wel[]= "    WELCOME ";
  P0=0x00;       //output configure for motor
  P2=0x00;
  P1=0x00;                      // P3 and P2 as output ports
  P3=0x00;
  init();	
for(z=0;z<12;z++)
 data1(wel[z]);
lcdcommand(0x0c);
Delay(6);

emptylot();
  TCON=0x05;                              //Edge sensitive
  IE=0x85;      			//enabling interrupts                                                    
  LDR=1;      //Configure input
  while (1)
  {
  scan_sublots();	
  if (count==0)
		{	
			Led=0;
			P2=0x00;
		}
		else
		{
      if(LDR==1 && count > 0)           //Check for LDR Input, 1=high R =Night
			{	Led=1;
				P2=0xFF;
			}
			else if(LDR==0)
			{	Led=0;
				P2=0x00;
			}
		}
  }
}
