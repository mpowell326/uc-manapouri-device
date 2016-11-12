/*  -------------------------------------------------------------
*   bbb_config.h
*
*   Purpose:            contains all pins used etc. on the beagelebone black
*
*   Created:            xx/yy/2016  ...    
  -------------------------------------------------------------*/



#ifndef BBB_CONFIG_H
#define BBB_CONFIG_H


/* Motor PWM pins:  */
#define MOTOR_F_H       P9_21       //  front horizontal (Thruster closet to front of device)
#define MOTOR_F_V       P9_22       //              |
#define MOTOR_R_V       P9_14       //              |
#define MOTOR_R_H       P9_16       //              |--> (Thruster closet to back of device)

/* I2C pins (using second I2C bus) */
#define I2C2_SCL        P9_19
#define I2C2_SDA        P9_20

/* Serial communication with Power Distribution Board */
// #define SERIAL_RX       PX_YY
// #define SERIAL_TX       PX_YY

/* IR sensor ADC pins */
#define IRup_PIN        AIN2
#define IRdown_PIN      AIN3
#define IRleft_PIN      AIN1       //in direction of travel
#define IRright_PIN     AIN0

/* Pressure sensor adc pin */
#define PRESSURE_SENSOR_PIN    AIN4



#endif /* BBB_CONFIG_H */
/**************************************************************************************************
*   End of header file bbb_config.h
**************************************************************************************************/