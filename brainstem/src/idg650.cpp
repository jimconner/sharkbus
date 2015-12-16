#include <stdio.h>
#include <unistd.h>
#include <roboard.h>

void wait_ms (unsigned long msec) {
	while(msec--) usleep(1000);
}

char *out_title[] ={"X-OUT","X45-OUT","Y-OUT","Y45-OUT","Z-OUT","Z45-OUT","IDG_Temp","ISZ_Temp"};

int main(void) {
	unsigned int i,d1,d2;
	unsigned char i2c_address,high,low;

	i2c_address = 0x21;

	// if you use RB-110, modify the parameter "RB_100" to "RB_110"
	roboio_SetRBVer(RB_100); // use RB-100

    if (i2c_Initialize(I2CIRQ_DISABLE) == false)
    {
		printf("FALSE!!  %s\n", roboio_GetErrMsg());
		return -1;
	}

	i2c0_SetSpeed(I2CMODE_FAST, 400000L);

	i2c0master_StartN(i2c_address,I2C_WRITE,2); //AS pin is high
	i2c0master_WriteN(0x03); //cycle time register
	i2c0master_WriteN(0x01); //convert time

	do
	{
		printf("read 3-axis values of Gyro and chip temperature\n\n");
		for(i = 0;i < 8;i++)
		{
			high = (0xf0 & (0x01 << i)) >> 4;//CH5 ~ CH8
			low = (0x0f & (0x01 << i)) << 4;//CH1 ~ CH4

			i2c0master_StartN(i2c_address,I2C_WRITE,3);//write 3 bytes
			i2c0master_WriteN(0x02);//configuration register
			i2c0master_WriteN(high);
			i2c0master_WriteN(low + 0x0c);//0x0c : FLTR = 1,ALERT/EN = 1
			wait_ms(10);

			i2c0master_StartN(i2c_address,I2C_WRITE,1);
			i2c0master_SetRestartN(I2C_READ,2);
			i2c0master_WriteN(0x00);//Read data form Conversion Result Register
			//Data : 12bits
			d1 = i2c0master_ReadN();
			d2 = i2c0master_ReadN();
			printf("%s : %d\n",out_title[((d1 & 0x70) >> 4)],(d1 & 0x0f)*256+d2);
		}
		printf("\n");
	}while(getchar() != 27);

    i2c_Close();
    return 0;
}
