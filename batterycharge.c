

#include <libndls.h>
#include <nspireio2.h>




int main()
{
	volatile unsigned int *rawinterrupt = (volatile unsigned int*) 0xC4000004; // https://hackspire.org/index.php?title=Memory-mapped_I/O_ports_on_CX#C4000000_-_Analog-to-Digital_Converter_.28ADC.29
	volatile unsigned int *enableinterrupts = (volatile unsigned int*) 0xC4000008;
	
	volatile unsigned int *startc1 = (volatile unsigned int*) 0xC4000120;
	volatile unsigned int *readc1 = (volatile unsigned int*) 0xC4000130;
	
	volatile unsigned int *startc2 = (volatile unsigned int*) 0xC4000140;
	volatile unsigned int *readc2 = (volatile unsigned int*) 0xC4000150;
	
	
	volatile unsigned int *startc4 = (volatile unsigned int*) 0xC4000180;
	volatile unsigned int *readc4 = (volatile unsigned int*) 0xC4000190;
	
	volatile unsigned int *peripherals = (volatile unsigned int*) 0x900B0018; // https://hackspire.org/index.php?title=Memory-mapped_I/O_ports_on_CX#900B0000_-_Power_management
	volatile unsigned int *peripherals2 = (volatile unsigned int*) 0x900B0020;
	*peripherals = 0;
	*peripherals2 = 0;
	
	char buff[200];
	
	double vbatt = 0,vsys = 0,b12 = 0;
	
	unsigned int intmask = *enableinterrupts;
	
	*enableinterrupts = 0x0;
	*rawinterrupt = 0xFFFFFFFF;
	
	unsigned int prevstart1 = *startc1;
	unsigned int prevstart2 = *startc2;
	unsigned int prevstart4 = *startc4;
	
	msleep(500);
	
	
	*peripherals = 0;
	*peripherals2 = 0;
	*rawinterrupt = 0xFFFFFFFF;
	*startc1 = 0b1;
	
	msleep(500);
	
	*rawinterrupt = 0xFFFFFFFF;
	unsigned int tmp = *readc1;
	
	
	vbatt = (double) tmp / (double) 155;
	
	
	*peripherals = 0;
	*peripherals2 = 0;
	
	*startc2 = 0b1;
	
	msleep(500);
	
	vsys = (double) (*readc2) / (double) 155;
	
	
	*startc4 = 0b1;
	
	msleep(500);
	
	b12 = (double) (*readc4) / (double) 310;
	
	*enableinterrupts = intmask;
	
	*startc1 = prevstart1;
	*startc2 = prevstart2;
	*startc4 = prevstart4;
	
	
	
	memset(buff,'\0',195);
	sprintf(buff,"VBATT: %4f\nVSYS: %4f\nB12: %4f",vbatt,vsys,b12);
	
	
	show_msgbox("Battery Report",buff);
	
	
	
	
	return 0;
}










