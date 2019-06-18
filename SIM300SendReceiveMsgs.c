/******************************************************************************



*******************************************************************************/


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "lcd.h"
#include "sim300.h"
#include "usart.h"
#include <string.h>
#include <avr/eeprom.h>
#include <util/delay_basic.h>
#include <avr/interrupt.h>

unsigned char data;


void Halt();


int main(void)
{
	//Initialize LCD Module
	

	DDRD|=(1<<PD6);
	DDRD|=(1<<PD5);


	PORTD&=~(1<<PD5);
	PORTD&=~(1<<PD6);

	DDRA=0x00;
	PORTA=0xff;



	LCDInit(LS_BLINK);

	LCDClear();
	//Initialize SIM300 module
	LCDWriteString("Initializing ...");
	int8_t r= SIM300Init();
	
	_delay_ms(1000);
	
	//Check the status of initialization
	switch(r)
	{
		case SIM300_OK:
			LCDWriteStringXY(0,1,"OK !");
			break; 
		case SIM300_TIMEOUT:
			LCDWriteStringXY(0,1,"No response");
			Halt();
		case SIM300_INVALID_RESPONSE:
			LCDWriteStringXY(0,1,"Inv response");
			Halt();
		case SIM300_FAIL:
			LCDWriteStringXY(0,1,"Fail");
			Halt();
		default:
			LCDWriteStringXY(0,1,"Unknown Error");
			Halt();	
	}
	
	_delay_ms(1000);


  UWriteString("AT+CMGD=6");


              data=13;
              UWriteData(data);
              data=10;
                  UWriteData(data);

                  _delay_ms(1000);
                                                                                                 _delay_ms(1000);

  LCDClear();

  char model[48];

  r=SIM300GetModel(model);

  if(r==SIM300_TIMEOUT)
  {
    LCDWriteString("Comm Error !");
    Halt();
  }

  LCDWriteString("Model:");
  LCDWriteStringXY(0,1,model);

  _delay_ms(1000);



  UWriteString("AT+CNMI=2,1,0,0,0");


              data=13;
              UWriteData(data);
              data=10;
                  UWriteData(data);

                  _delay_ms(1000);
	
	//IMEI No display
	LCDClear();
	
	char pname[32];
	r=SIM300GetProviderName(pname);
	
	if(r==0)
	{
		LCDWriteString("Comm Error !");
		Halt();
	}
	
	LCDWriteString(pname);
		
	_delay_ms(1000);

  UWriteString("AT+CNMI=2,1,0,0,0");


              data=13;
              UWriteData(data);
              data=10;
                  UWriteData(data);

                  _delay_ms(1000);


      UWriteString("AT+CMGF=1");

      data=13;
      UWriteData(data);
      data=10;
          UWriteData(data);

          _delay_ms(1000);


          UWriteString("AT+CSCS=\"GSM\"");


          data=13;
          UWriteData(data);
          data=10;
              UWriteData(data);

              _delay_ms(1000);







char sp[2];

char leval=0,m_status=0;

	while(1)
	{
	    uint8_t id;

	    UFlushBuffer();
	    LCDClear();
	
		LCDWriteStringXY(0,0,"Waiting for msg");
	  LCDWriteStringXY(0,1,"Lvl");
		LCDWriteIntXY(4,1,leval,1);
		LCDWriteStringXY(7,1,"M_stat");
		LCDWriteIntXY(14,1,m_status,1);

	

		while(SIM300WaitForMsg(&id)!=SIM300_OK)
		{
		    if((~PINA)&(1<<PA0))
		      {
		        leval=0;
		       LCDWriteIntXY(4,1,leval,2);
		       if((~PINA)&(1<<PA1))
		                     {
		                     leval=1;
		                    LCDWriteIntXY(4,1,leval,2);
		                    if((~PINA)&(1<<PA2))
		                                 {
		                                 leval=2;
		                                LCDWriteIntXY(4,1,leval,2);
		                                 }
		                    if((~PINA)&(1<<PA3))
		                                 {
		                                 leval=3;
		                                LCDWriteIntXY(4,1,leval,2);

		                                m_status=0;
		                                LCDWriteIntXY(14,1,m_status,2);
		                                PORTD&=~(1<<PD5);
		                                 }
		                     }
		      }





		}

		LCDClear();
		LCDWriteStringXY(0,0,"MSG Received    ");

		char msg[20];
		char oa[20];
		
		r=SIM300ReadMsg(id,msg);
		
		if(r==SIM300_OK)
		{
			
			LCDWriteStringXY(0,1,msg);



			if(strncmp(msg,"MOTOR ON",8)==0)
			{
				PORTD|=(1<<PD5);
				m_status=1;
				LCDWriteIntXY(14,1,m_status,1);
				_delay_ms(2000);

			}
	
			else if(strncmp(msg,"MOTOR OFF",9)==0)
		{
			PORTD&=~(1<<PD5);
			m_status=0;
			LCDWriteIntXY(14,1,m_status,1);
			_delay_ms(2000);

		}
			
		}	
		else
		{
			LCDWriteStringXY(0,0,"Err Reading Msg !");
			
			_delay_ms(3000);
			
		}
		
		//Finally delete the msg
		if (SIM300DeleteMsg(id)!=SIM300_OK)
		{
		    LCDClear();
			LCDWriteStringXY(0,0,"Deleting Msg !");
			
			_delay_ms(2000);
		}
		
	}
		
	
	Halt();
}






void Halt()
{
  while(1);
}
