#include "LIB/STD_TYPES.h"
#include "LIB/BIT_MATH.h"

#include "MCAL/DIO/DIO_interface.h"
#include "MCAL/TIMERS/TIMERS_Interface.h"
#include "MCAL/GIE/GIE_interface.h"

#include "HAL/LCD/LCD_interface.h"
#include "HAL/KPD/KPD_Interface.h"

#include "avr/delay.h"

void APP(void);
volatile static s8 num_of_miniute;
volatile static s8 num_of_second;
void main (void)
{
	u8 Local_u8Key=KPD_NO_PRESSED_KEY;
	u8 	previous_key=0 ;
	HLCD_VoidInit();

	DIO_u8SetPinDirection(DIO_u8_PORTA,DIO_u8_PIN0,DIO_u8_PIN_OUTPUT);
	HLCD_VoidGotoXY(0,0);
	HLCD_VoidSendString((u8*)"Enetr Minutes");
	HLCD_VoidGotoXY(1,0);
	while(Local_u8Key != '=')
	{
		do
		{
			if(Local_u8Key!= KPD_NO_PRESSED_KEY )
			{
				HLCD_VoidSendNumber(Local_u8Key);
				previous_key =previous_key*10+Local_u8Key;
			}
			Local_u8Key=KPD_u8GetPressedKey();
		}while(Local_u8Key==KPD_NO_PRESSED_KEY );
	}
	num_of_miniute = previous_key ;
	previous_key =0;
	_delay_ms(500);
	HLCD_VoidSendCommand(DISPLAYCLEAR);
	Local_u8Key= KPD_NO_PRESSED_KEY ;
	HLCD_VoidSendString((u8*)"Enter Seconds");
	HLCD_VoidGotoXY(1,0);
	while(Local_u8Key != '=')
	{
		do
		{
			if(Local_u8Key!= KPD_NO_PRESSED_KEY)
			{
				HLCD_VoidSendNumber(Local_u8Key);
				previous_key =previous_key*10+Local_u8Key;

			}
			Local_u8Key=KPD_u8GetPressedKey();
		}while(Local_u8Key==KPD_NO_PRESSED_KEY );
	}
	num_of_second = previous_key;
	_delay_ms(500);
	HLCD_VoidSendCommand(DISPLAYCLEAR);

	HLCD_VoidSendString("Minutes  Seconds");
	HLCD_VoidGotoXY(1,1);
	HLCD_VoidSendNumber(num_of_miniute);
	HLCD_VoidGotoXY(1,11);
	HLCD_VoidSendNumber(num_of_second);


	TIMER0_CTC_SetOCR0(250);
	TIMER0_Init(TIMER0_CTC_MODE, TIMER0_SCALER_8);
	TIMER0_OC_InterruptEnable();
	Timer0_CTC_SetCallBack(&APP);
	GIE_voidEnable();


	while(1)
	{


	}
}

void APP (void)
{
	volatile static u16 counter = 0;
	counter++ ;
	if(counter == 4000)
	{
		if(num_of_miniute !=0 || num_of_second !=0)
		{
			if(num_of_miniute < 9)
			{
				HLCD_VoidGotoXY(1,0);
				HLCD_VoidSendNumber(0);
			}
			HLCD_VoidGotoXY(1,11);

			if(num_of_second <= 10 )
			{
				HLCD_VoidSendNumber(0);
			}
			num_of_second--;
			HLCD_VoidSendNumber(num_of_second);

			if(num_of_miniute >0 && num_of_second ==0)
			{
				HLCD_VoidGotoXY(1,1);
				num_of_miniute--;
				HLCD_VoidSendNumber(num_of_miniute);
				HLCD_VoidGotoXY(1,11);
				HLCD_VoidSendNumber(59);
				num_of_second = 59;
			}
		}
		counter =0;


	}


}
