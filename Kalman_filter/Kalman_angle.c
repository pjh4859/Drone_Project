/* Copyright (C) 2012 Kristian Lauszus, TKJ Electronics. All rights reserved.

 This software may be distributed and modified under the terms of the GNU
 General Public License version 2 (GPL2) as published by the Free Software
 Foundation and appearing in the file GPL2.TXT included in the packaging of
 this file. Please note that GPL2 Section 2[b] requires that all works based
 on this software must also be made publicly available under the terms of
 the GPL2 ("Copyleft").

 Contact information
 -------------------

 Kristian Lauszus, TKJ Electronics
 Web      :  http://www.tkjelectronics.com
 e-mail   :  kristianl@tkjelectronics.com
 */
#include<stdio.h>
#include "Kalman.h"

typedef struct Kalman
{
    /* We will set the variables like so, these can also be tuned by the user */
    float Q_angle;// = 0.001f;
    float Q_bias;// = 0.003f;
    float R_measure;// = 0.03f;

    float angle;// = 0.0f; // Reset the angle
    float bias;// = 0.0f; // Reset bias

    float P[2][2];// = 0.0f; // Since we assume that the bias is 0 and we know the starting angle (use setAngle), the error covariance matrix is set like so - see: http://en.wikipedia.org/wiki/Kalman_filter#Example_application.2C_technical
   //P[0][1] = 0.0f;
   //P[1][0] = 0.0f;
   //P[1][1] = 0.0f;
}KALMAN;

KALMAN Kalman = { 0.001f,0.003f,0.03f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
KALMAN* kal;

// The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
float getAngle(float newAngle, float newRate, float dt)
{
    // KasBot V2  -  Kalman filter module - http://www.x-firm.com/?page_id=145
    // Modified by Kristian Lauszus
    // See my blog post for more information: http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it

    // Discrete Kalman filter time update equations - Time Update ("Predict")
    // Update xhat - Project the state ahead
    /* Step 1 */
    //KALMAN* kal;
    float rate = newRate - kal->bias;
    angle += dt * rate;    

    // Update estimation error covariance - Project the error covariance ahead
   /* Step 2 */
    kal->P[0][0] += dt * (dt * kal->P[1][1] - kal->P[0][1] - kal->P[1][0] + kal->Q_angle);
    kal->P[0][1] -= dt * kal->P[1][1];
    kal->P[1][0] -= dt * kal->P[1][1];
    kal->P[1][1] += kal->Q_bias * dt;
    
    // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
    // Calculate Kalman gain - Compute the Kalman gain
    /* Step 4 */
    float S = kal->P[0][0] + kal->R_measure; // Estimate error
    /* Step 5 */
    float K[2]; // Kalman gain - This is a 2x1 vector
    K[0] = kal->P[0][0] / S;
    K[1] = kal->P[1][0] / S;

    // Calculate angle and bias - Update estimate with measurement zk (newAngle)
    /* Step 3 */
    float y = newAngle - kal->angle; // Angle difference
    /* Step 6 */
    kal->angle += K[0] * y;
    kal->bias += K[1] * y;

    // Calculate estimation error covariance - Update the error covariance
    /* Step 7 */
    float P00_temp = kal->P[0][0];
    float P01_temp = kal->P[0][1];

    kal->P[0][0] -= K[0] * P00_temp;
    kal->P[0][1] -= K[0] * P01_temp;
    kal->P[1][0] -= K[1] * P00_temp;
    kal->P[1][1] -= K[1] * P01_temp;

    return kal->angle;
}
void setAngle(float angle) { kal->angle = angle; } // Used to set angle, this should be set as the starting angle
float getRate() { return kal->rate; } // Return the unbiased rate

/* These are used to tune the Kalman filter */
void setQangle(float Q_angle) { kal->Q_angle = Q_angle; }
void setQbias(float Q_bias) { kal->Q_bias = Q_bias; }
void setRmeasure(float R_measure) { kal->R_measure = R_measure; }

float getQangle() { return kal->Q_angle; }
float getQbias() { return kal->Q_bias; }
float getRmeasure() { return kal->R_measure; }