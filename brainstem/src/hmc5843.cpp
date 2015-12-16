#include <stdio.h>
#include <unistd.h>
#include <roboard.h>

void wait_ms (unsigned long msec) {
	while(msec--) usleep(1000);
}

int main(void){
	unsigned int d1,d2,d3,d4,d5,d6;
	unsigned char i2c_address = 0x1e;
	
	// if you use RB-110, modify the parameter "RB_100" to "RB_110"
	roboio_SetRBVer(RB_100); // use RB-100

    if (i2c_Initialize(I2CIRQ_DISABLE) == false)
	{
		printf("FALSE!!  %s\n", roboio_GetErrMsg());
		return -1;
	}

	i2c0_SetSpeed(I2CMODE_FAST, 400000L);
	
	i2c0master_StartN(i2c_address,I2C_WRITE,2);//write 2 byte
	i2c0master_WriteN(0x02); //mode register
	i2c0master_WriteN(0x00); //continue-measureture mode

	wait_ms(100);

	do
	{
		i2c0master_StartN(i2c_address, I2C_WRITE, 1);
		i2c0master_SetRestartN(I2C_READ, 6);
		i2c0master_WriteN(0x03); //Read from data register (Address : 0x03)

		d1 = i2c0master_ReadN();//X MSB
		d2 = i2c0master_ReadN();//X LSB
		d3 = i2c0master_ReadN();//Y MSB
		d4 = i2c0master_ReadN();//Y LSB
		d5 = i2c0master_ReadN();//Z MSB
		d6 = i2c0master_ReadN();//Z LSB

		printf("X axis :%5d\n", ((d1 & 0x80) != 0) ? (((~0)>>16)<<16) | ((d1<<8)+d2): (d1<<8)+d2);
	    printf("Y axis :%5d\n", ((d3 & 0x80) != 0) ? (((~0)>>16)<<16) | ((d3<<8)+d4): (d3<<8)+d4);
	    printf("Z axis :%5d\n", ((d5 & 0x80) != 0) ? (((~0)>>16)<<16) | ((d5<<8)+d6): (d5<<8)+d6);

		wait_ms(100);
	}while(getchar() != 27);

	i2c_Close();
    return 0;
}
