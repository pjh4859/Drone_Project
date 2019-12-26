#include<stdio.h>
#include"C_Kalman.h"

int main()
{
	void Init(struct Kalman* klm);
	double getAngle(struct Kalman* klm, double newAngle, double newRate, double dt);
	void setAngle(struct Kalman* klm, double newAngle);
	double getRate(struct Kalman* klm);
	void setQangle(struct Kalman* klm, double newQ_angle);

	void setQbias(struct Kalman* klm, double newQ_bias);
	void setRmeasure(struct Kalman* klm, double newR_measure);
	double getQangle(struct Kalman* klm);
	double getQbias(struct Kalman* klm);
	double getRmeasure(struct Kalman* klm);

	return 0;
}