#include <stdio.h>
#include <unistd.h>
#include <roboard.h>

void wait_ms (unsigned long msec) {
	while(msec--) usleep(1000);
}

int main(void){
	unsigned int d1,d2,d3,d4,d5,d6;
	unsigned char i2c_address = 0x53;
	
	// if you use RB-110, modify the parameter "RB_100" to "RB_110"
	roboio_SetRBVer(RB_100); // use RB-100

    if (i2c_Initialize(I2CIRQ_DISABLE) == false)
    {
		printf("FALSE!!  %s\n", roboio_GetErrMsg());
		return -1;
	}
	
	i2c0_SetSpeed(I2CMODE_FAST, 400000L);

	i2c0master_StartN(i2c_address,I2C_WRITE,2);//write 2 byte
	i2c0master_WriteN(0x2d); //Pwoer_Control register
	i2c0master_WriteN(0x28); //link and measure mode

	wait_ms(100);

	i2c0master_StartN(i2c_address,I2C_WRITE,2);//write 2 byte
	i2c0master_WriteN(0x31); //Data_Format register
	i2c0master_WriteN(0x08); //Full_Resolution

	wait_ms(100);
	
	i2c0master_StartN(i2c_address,I2C_WRITE,2);//write 2 byte
	i2c0master_WriteN(0x38); //FIFO_Control register
	i2c0master_WriteN(0x00); //bypass mode

	wait_ms(100);

	do
	{
		i2c0master_StartN(i2c_address, I2C_WRITE, 1);
		i2c0master_SetRestartN(I2C_READ, 6);
		i2c0master_WriteN(0x32); //Read from X register (Address : 0x32)
		d1 = i2c0master_ReadN();//X LSB
		d2 = i2c0master_ReadN();//X MSB
		d3 = i2c0master_ReadN();//Y LSB
		d4 = i2c0master_ReadN();//Y MSB
		d5 = i2c0master_ReadN();//Z LSB
		d6 = i2c0master_ReadN();//Z MSB
		
		printf("Acc of X-axis :%5d\n", ((d2 & 0x80) != 0) ? (((~0)>>16)<<16) | ((d2<<8)+d1): (d2<<8)+d1);
	    printf("Acc of Y-axis :%5d\n", ((d4 & 0x80) != 0) ? (((~0)>>16)<<16) | ((d4<<8)+d3): (d4<<8)+d3);
	    printf("Acc of Z-axis :%5d\n", ((d6 & 0x80) != 0) ? (((~0)>>16)<<16) | ((d6<<8)+d5): (d6<<8)+d5);

		wait_ms(100);
	}while(getchar() != 27);

    i2c_Close();
    return 0;
}
