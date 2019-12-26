#include<stdio.h>
#include<unistd.h>
#include<wiringPi.h>
#include<wiringPiI2C.h>

int main()
{
    int MPU_9250;
    int ACCEL_X;
    int ACCEL_Y;
    int ACCEL_Z;
    int TEMP_SENSOR;
    int GYRO_X;
    int GYRO_Y;
    int GYRO_Z;
    int MAG_X;
    int MAG_Y;
    int MAG_Z;
    MPU_9250 = wiringPiI2CSetup(0x68); //센서의 address
    wiringPiI2CWriteReg8(MPU_9250, 0x6B, 0);//PWE,MGMT_1 설정

    while (1) {
        ACCEL_X = wiringPiI2CReadReg8(MPU_9250, 0x3B);
        ACCEL_X = ACCEL_X << 8 | wiringPiI2CReadReg8(MPU_9250, 0x3C);

        ACCEL_Y = wiringPiI2CReadReg8(MPU_9250, 0x3D);
        ACCEL_Y = ACCEL_Y << 8 | wiringPiI2CReadReg8(MPU_9250, 0x3E);

        ACCEL_Z = wiringPiI2CReadReg8(MPU_9250, 0x3F);
        ACCEL_Z = ACCEL_Z << 8 | wiringPiI2CReadReg8(MPU_9250, 0x40);

        GYRO_X = wiringPiI2CReadReg8(MPU_9250, 0x43);
        GYRO_X = GYRO_X << 8 | wiringPiI2CReadReg8(MPU_9250, 0x44);

        GYRO_Y = wiringPiI2CReadReg8(MPU_9250, 0x45);
        GYRO_Y = GYRO_Y << 8 | wiringPiI2CReadReg8(MPU_9250, 0x46);

        GYRO_Z = wiringPiI2CReadReg8(MPU_9250, 0x47);
        GYRO_Z = GYRO_Z << 8 | wiringPiI2CReadReg8(MPU_9250, 0x48);

        printf("|%6d|%6d|%6d|\n|%6d|%6d|%6d|\n", ACCEL_X, ACCEL_Y, ACCEL_Z, GYRO_X, GYRO_Y, GYRO_Z);

	sleep(2);
    }

    return 0;
}
