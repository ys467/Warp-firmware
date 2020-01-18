/*
Library for CO (Carbon Monoxide Gas) ppm value
by using MQ7 gas sensor.

Datasheet : https://www.sparkfun.com/datasheets/Sensors/Biometric/MQ-7.pdf
*/

#ifndef WARP_BUILD_ENABLE_DEVMQ7
#define WARP_BUILD_ENABLE_DEVMQ7

#endif

int32_t init_adc(uint32_t instance);
int devMQ7init(void);
