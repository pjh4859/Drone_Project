#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define Device_Address 0x68	/*Device Address/Identifier for MPU9250*/
#define MAG_ADDRESS		0x0C

#define SUM(a,b)  (a)+(b)

#define GYRO_FULL_SCALE_250_DPS    0x00 
#define GYRO_FULL_SCALE_500_DPS    0x08
#define GYRO_FULL_SCALE_1000_DPS   0x10
#define GYRO_FULL_SCALE_2000_DPS   0x18
#define	GYRO_FCHOICE_B			0x03

#define ACC_FULL_SCALE_2_G        0x00 
#define ACC_FULL_SCALE_4_G        0x08
#define ACC_FULL_SCALE_8_G        0x10
#define ACC_FULL_SCALE_16_G       0x18

#define	I2C_MASTER_CTR  0x24

#define PWR_MGMT_1   0x6B
#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG1 0x1C
#define ACCEL_CONFIG2 0x1D
#define INT_ENABLE   0x38
#define	INTERRUPT_ENABLE 0x59
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define GYRO_XOUT_H  0x43
#define GYRO_YOUT_H  0x45
#define GYRO_ZOUT_H  0x47
#define TEMP_OUT_H	0x41
#define TEMP_OUT_L	0x42

#define MAG_CNTR1	0x0A
#define MAG_CNTR2	0x0B
#define MAG_XOUT_L	0x03
#define MAG_XOUT_H	0x04
#define MAG_YOUT_L	0x05
#define MAG_YOUT_H	0x06
#define MAG_ZOUT_L	0x07
#define MAG_ZOUT_H	0x08

int MPU9250;

void MPU9250_Init() {
	wiringPiI2CWriteReg8(MPU9250, SMPLRT_DIV, 0x07);//SAMPLE_RATE=internal_Sample_Rate /(1+0x07)
	wiringPiI2CWriteReg8(MPU9250, PWR_MGMT_1, 0x01);
	wiringPiI2CWriteReg8(MPU9250, CONFIG, 0x00);
	wiringPiI2CWriteReg8(MPU9250, GYRO_CONFIG, GYRO_FULL_SCALE_2000_DPS + GYRO_FCHOICE_B);
	wiringPiI2CWriteReg8(MPU9250, ACCEL_CONFIG1, ACC_FULL_SCALE_2_G);
	wiringPiI2CWriteReg8(MPU9250, ACCEL_CONFIG2, 0x09);//LPF ACCL_FCHOICE:1, DLPFCFG:1
	wiringPiI2CWriteReg8(MPU9250, INT_ENABLE, 0x01);

	wiringPiI2CWriteReg8(MPU9250, MAG_CNTR1, 0x06);
	wiringPiI2CWriteReg8(MPU9250, MAG_CNTR2, 0x01);
}

short read_raw_data(int addr,int a) {
	//short high_byte, low_byte, value;
	short value;
	switch(a){
		case 0: {
			value = wiringPiI2CReadReg8(MPU9250, addr);
			value = (value << 8) | wiringPiI2CReadReg8(MPU9250, addr + 0x01);
			break;
		}
		case 1: {
			value = wiringPiI2CReadReg8(MPU9250, addr);
			value = (value << 8) | wiringPiI2CReadReg8(MPU9250, addr - 0x01);
			break;
		}
	}	
	return value;
}

int main() {
	float Acc_x, Acc_y, Acc_z;
	float Gyro_x, Gyro_y, Gyro_z;
	float Mag_x, Mag_y, Mag_z;
	float Ax = 0, Ay = 0, Az = 0;
	float Gx = 0, Gy = 0, Gz = 0;
	float Mx = 0, My = 0, Mz = 0;

	MPU9250 = wiringPiI2CSetup(Device_Address);   /*Initializes I2C with device Address*/
	MPU9250_Init();		                 /* Initializes MPU9250 */

	while (1)
	{
		/*Read raw value of Accelerometer and gyroscope from MPU9250*/
		Acc_x = read_raw_data(ACCEL_XOUT_H,0);
		Acc_y = read_raw_data(ACCEL_YOUT_H,0);
		Acc_z = read_raw_data(ACCEL_ZOUT_H,0);
		Gyro_x = read_raw_data(GYRO_XOUT_H,0);
		Gyro_y = read_raw_data(GYRO_YOUT_H,0);
		Gyro_z = read_raw_data(GYRO_ZOUT_H,0);

		Mag_x = read_raw_data(MAG_XOUT_H,1);
		Mag_y = read_raw_data(MAG_YOUT_H,1);
		Mag_z = read_raw_data(MAG_ZOUT_H,1);


		/* Divide raw value by sensitivity scale factor */
		Ax = Acc_x / 16384.0;
		Ay = Acc_y / 16384.0;
		Az = Acc_z / 16384.0;
		Gx = Gyro_x / 131;
		Gy = Gyro_y / 131;
		Gz = Gyro_z / 131;

		Mx = Mag_x;
		My = Mag_y;
		Mz = Mag_z;

		printf("\n Gx=%.3f ¡Æ/s\tGy=%.3f ¡Æ/s\tGz=%.3f ¡Æ/s\tAx=%.3f g\tAy=%.3f g\tAz=%.3f g\n", Gx, Gy, Gz, Ax, Ay, Az);
		printf("Gx=%6.3f \tGy=%6.3f \tGz=%6.3f \tAx=%6.3f \tAy=%6.3f \tAz=%6.3f \n", Gyro_x, Gyro_y, Gyro_z, Acc_x, Acc_y, Acc_z);
		printf("Mx=%6.3f \tMy=%6.3f \tMz=%6.3f\n", Mx,My,Mz);
		delay(100);
	}
	return 0;
}