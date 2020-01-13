#include "TERMINAL.h"

//#include "debug_frmwrk.h"


#include "debug_frmwrk.h"
#include "lpc17xx_gpio.h"

#include "PORTS.h"
#include "ADC.h"
#include "TERMINAL.h"
#include "DAC.h"

UART_CFG_Type UARTConfigStruct;
uint32_t MatchCount;

uint8_t ADCflag = 0;
unsigned long time = 0;

enum State {
	HELP, 
	PINX, 
	PINXexactlyY,
	ADC,
	ADCcount,
	TIME,
	DAC,
	STORE,
	PWMXexactlyY,
	PWMX
}; 

void SysTick_Handler (void) {
  SysTickCnt++;
	if(SysTickCnt>=1000){
		ADCflag = 1;
		SysTickCnt = 0;
		time++;
	}
}

uint8_t port;
uint8_t pin;
uint8_t data;

enum State ReadUart(){
	uint8_t t[20];
	uint8_t count = 0;
	char str_str[20];
	for(int i = 0; i< 20; i++){
		t[i] = _DG;
		if(t[i] == 59){
			count = i;
			i = 21;
		}
	}	
	char* str;
	str = (char*)t;	
	if(str[0] == 'h'&& str[1] == ';'||
	str[0]=='h' &&	 str[1]=='e' && str[2] == 'l' && str[3] == 'p' && str[5] == ';'){
		return HELP;
	}else if(str[0] == 'p' && str[1] =='i' && str[2] == 'n'
	&& count < 9 && count>=7){
				
		int pin0  = (int)str[6]-48;
		int pin2  = ((int)str[5]-48)*10;
		pin = (uint8_t)(pin2 + pin0);
		_DBG("\npin:");
		_DBD(pin);
		_DBG("\n");

		_DBG("port:");
		port =  (uint8_t)str[3]-48;
		_DBD(port);
		_DBG("\n");
			
		return PINX;
	}else if(str[0] == 'p' && str[1] == 'i' && str[2] == 'n' && count < 15 && count>=9){
		int pin0  = (int)str[6]-48;
		int pin2  = ((int)str[5]-48)*10;
		pin = (uint8_t)(pin2 + pin0);
		_DBG("\npin:");
		_DBD(pin);
		_DBG("\n");

		_DBG("port:");
		port =  (uint8_t)str[3]-48;
		_DBD(port);
		_DBG("\n");
		
		data  = (uint8_t)str[8] - 48;
		
		return PINXexactlyY;
	}else if(str[0] == 'A' && str[1] == 'D' && str[2] == 'C' && count>=2&&count<=4){
		return ADC;
	}else if(str[0] == 'A' && str[1] == 'D' && str[2] == 'C' && str[3] == 'c' && str[4] == 'o' && str[5] == 'u' && str[6] == 'n' && str[7] == 't'){
		return ADCcount;
	}else if(str[0] == 't' && str[1] == 'i' && str[2] == 'm' && str[3] == 'e' ){
		return TIME;
	}else if(str[0] == 'D' && str[1] == 'A' && str[2] == 'C'){
		return DAC;
	}else if(str[0] == 's' && str[1] == 't' && str[2] == 'o' && str[3] == 'r' && str[4] == 'e'){
		return STORE;
	}
	else if(str[0] == 'P' && str[1] == 'W' && str[2] == 'M' && str[4] == '.' && str[7] == '=' && str[9] == '.'){
		return PWMXexactlyY;
	}else if(str[0] == 'P' && str[1] == 'W' && str[2] == 'M' && str[4] == '.' && count<9){
		return PWMX;
	}
	else{
		_DBG("\n ---------------------- \n");
		return 0;
	}
	return 0;
}


char* DeleteSpace(char* str, uint8_t index){
	uint8_t count_space = 0;
	for(int i = 0; i<index;i++){
		if(str[i] == " "){
			count_space++;
		}
	}
}

Bool comparite(char* str1,char* str2){
	if(sizeof(str1)/sizeof(char) == sizeof(str1)/sizeof(char)){
		for(int i = 0; i<sizeof(str1)/sizeof(char);i++){
			
		}
	}else{
		return FALSE;
	}
	
	return TRUE;
}

void help(){
	_DBG("\n*************** HELP ********************\n");
	_DBG("\nhelp; or h; - this function show list all  functions\n");
	_DBG("\n");
	_DBG("\npinX;       - this function show status pin\n");
	_DBG("\n              exemple: pin1.03;\n");
	_DBG("\n");
	_DBG("\npinX=Y;     - this function writes 0 or 1 to pinY. 0-off 1-on \n");
	_DBG("\n              exemple: pin1.03=0;\n");
	_DBG("\n");
	_DBG("\nADC;        - this function shows result from ADC\n");
	_DBG("\n              exemple: ADC;\n");
	_DBG("\n");
	_DBG("\nADCcount;   - this function shows result from ADC in forever loop\n");
	_DBG("\n              exemple: ADCcount;\n");
	_DBG("\n");
	_DBG("\ntime;       - this function show? time(in second) from start work app\n");
	_DBG("\n              exemple: time;\n");
	_DBG("\n");
	_DBG("\nDACX;       - this function create? voltage in pin DAC\n");
	_DBG("\n              exemple: DAC;\n");
	_DBG("\n");
	_DBG("\nstore'XXX'; - writes string typed in by the user (0 to 128)\n");
	_DBG("\n              exemple: store'xxx';\n");
	_DBG("\n");
	_DBG("\nPWMX=Y;     - writes duty cycle Y  in to PWMX\n");
	_DBG("\n               exemple:PWM1.02=90;\n");
	_DBG("\n");	
	_DBG("\nPWMX;       - reads out in PWMX\n");
	_DBG("\n              exemple:PWM1.02;\n");
	_DBG("\n");	
}

int legenda(){
	SysTick_Config(SystemCoreClock/1000 - 1); 
	
	debug_frmwrk_init();
	enum State st = DAC;
	
	GPIO_SetDir(1, 0xB0000000, 1);         
  GPIO_SetDir(2, 0x0000007C, 1);           

  GPIO_ClearValue(1, 0xB0000000);
  GPIO_ClearValue(2, 0x0000007C);
	
	
	
	
	for(;;){
		_DBG("start");
		st = ReadUart();
		switch (st){
			case HELP:
				_DBG("\n---------------HELP----------------\n");
				help();
				break;
			case PINX:
				_DBG("\n---------------PINX----------------\n");
				ReadPin(port, pin);
				break;
			case PINXexactlyY:
				WritePin_OnOff(port, pin, data);
				_DBG("\n---------------PINX=Y----------------\n");
				break;
			case ADC:
				_DBG("\n---------------ADC----------------\n");
				ReadVoltage();
				break;
			case ADCcount:
				_DBG("\n---------------ADCcount----------------\n");
				while(1){
				if(ADCflag == 1)
					{
						ReadVoltage();
						ADCflag = 0;
					}
				}
				break;
			case TIME:
				_DBG("\n---------------TIME----------------\n");
				getTime();
				break;
			case DAC:
				_DBG("\n---------------DAC----------------\n");
				OutputVoltage();
				while(1);
				break;
			case STORE:
				_DBG("\n---------------STORE----------------\n");
				break;
			case PWMXexactlyY:
				_DBG("\n---------------PWMXexactlyY----------------\n");
				break;
			case PWMX:
				_DBG("\n---------------PWMX----------------\n");
				break;
			default:
				//_DBG("\n-------------------------------\n");
				break;
			
		}

	
		
	}
	
return 0;

}



void getTime(){
		char str[20];
		sprintf(str,"%lu second",time);
		_DBG("time from start work: ");

		_DBG(str);
		_DBG_("");
}