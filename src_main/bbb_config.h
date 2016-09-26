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
#define MOTOR1_PIN      P9_14
#define MOTOR2_PIN      P9_16
#define MOTOR3_PIN      P9_21
#define MOTOR4_PIN      P9_22

/* I2C pins (using second I2C bus) */
#define I2C2_SCL        P9_19
#define I2C2_SDA        P9_20

/* Serial communication with Power Distribution Board */
// #define SERIAL_RX       PX_YY
// #define SERIAL_TX       PX_YY

/* IR sensor ADC pins */
#define IRup_PIN        AIN0
#define IRdown_PIN      AIN1
#define IRleft_PIN      AIN2
#define IRright_PIN     AIN3

/* Pressure sensor adc pin */
#define PRESSURE_SENSOR_PIN    AIN4



#endif /* BBB_CONFIG_H */
/**************************************************************************************************
*   End of header file bbb_config.h
**************************************************************************************************/