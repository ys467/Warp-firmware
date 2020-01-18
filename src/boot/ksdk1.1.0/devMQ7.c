/*
Library for CO (Carbon Monoxide Gas) ppm value
by using MQ7 gas sensor.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "fsl_misc_utilities.h"
#include "fsl_device_registers.h"
#include "fsl_i2c_master_driver.h"
#include "fsl_spi_master_driver.h"
#include "fsl_tpm_driver.h"
#include "fsl_rtc_driver.h"
#include "fsl_clock_manager.h"
#include "fsl_power_manager.h"
#include "fsl_mcglite_hal.h"
#include "fsl_port_hal.h"
#include "fsl_adc16_hal.h"
#include "fsl_adc16_driver.h"
#include "fsl_smc_hal.h"
#include "fsl_pmc_hal.h"

#include "SEGGER_RTT.h"
#include "gpio_pins.h"
#include "warp.h"

#include "devMQ7.h"
#include "devSSD1331.h" //OLED header file included
#include "devHPA17F.h" //buzzer header file included

//Gas sensor connection made at PTA8, which is ADC0_SE3
#define ADC_0                   (0U)
#define CHANNEL_0               (0U)
#define CHANNEL_G               (3U)

enum
{
	kMQ7PinADC		= GPIO_MAKE_PIN(HW_GPIOA, 8),
};

/*
Initialisation of ADC
*/

int32_t init_adc(uint32_t instance)
{
/*  adc16_calibration_param_t auto_params;

  ADC16_DRV_GetAutoCalibrationParam(instance, &auto_params);*/

#if FSL_FEATURE_ADC16_HAS_CALIBRATION
  adc16_calibration_param_t adcCalibrationParam;
#endif
  adc16_user_config_t adcUserConfig;
  adc16_chn_config_t adcChnConfig;

#if FSL_FEATURE_ADC16_HAS_CALIBRATION
  ADC16_DRV_GetAutoCalibrationParam(instance, &adcCalibrationParam);
  ADC16_DRV_SetCalibrationParam(instance, &adcCalibrationParam);
#endif

	// Configure ADC structure
  ADC16_DRV_StructInitUserConfigDefault(&adcUserConfig);
  adcUserConfig.intEnable = true;
  adcUserConfig.lowPowerEnable = false;
  adcUserConfig.asyncClkEnable = true;
  adcUserConfig.resolutionMode = kAdcResolutionBitOf12or13;
  adcUserConfig.refVoltSrcMode = kAdcRefVoltSrcOfVref;
  adcUserConfig.hwTriggerEnable = false;
  adcUserConfig.continuousConvEnable = true;
  adcUserConfig.clkSrcMode = kAdcClkSrcOfAsynClk;
  ADC16_DRV_Init(instance, &adcUserConfig);

  // Configure channel
  adcChnConfig.chnNum = CHANNEL_G;
  adcChnConfig.intEnable = false;
  adcChnConfig.chnMux = kAdcChnMuxOfA;

  ADC16_DRV_ConfigConvChn(instance, CHANNEL_0, &adcChnConfig);

  return 0;
}

/*
Function is used to return the ppm value of CO gas concentration
by using the parameter found using the function f(x) = a * ((Rs/R0) ^ b)

This will return the ppm value of Carbon Monoxide concentration

The graph in the datasheet is represented with the function
f(x) = a * (x ^ b).
where
	f(x) = ppm
	x = Rs/R0
The values were mapped with this function to determine the coefficients a and b.

The coefficients are estimated from the sensitivity characteristics graph
of the MQ7 sensor for CO (Carbon Monoxide) gas by using Correlation function.
The graph is provided in the sensor datasheet.

float getPPM(void)
{

  float ppm;
  float v_out;
  v_out = (float)adcValue * (5.0 / 4096.0);
  float vratio;
  vratio = (5.0 - v_out) / v_out;
  return (float)(19.32 * pow(vratio, -0.64));

  return ppm;
}
*/

/*
Reads value from gas sensor from input analogue pin
*/

int devMQ7init(void)
{
	PORT_HAL_SetMuxMode(PORTA_BASE, 8u, kPortMuxAsGpio);

  SEGGER_RTT_WriteString(0, "Receiving ADC value ...\r\n");
  //Initialize ADC
  init_adc(ADC_0);

  while (1)
  {
			if (ADC16_DRV_GetConvValueRAW(ADC_0, CHANNEL_0) > 50){ //ADC value threshold set to be 50 for the purpose of demonstration
				SEGGER_RTT_printf(0, "ADC Value: %d\r\n", ADC16_DRV_GetConvValueRAW(ADC_0, CHANNEL_0));
				devHPA17Finit(); //Piezo alarm triggered when significant amount of CO is detected
				devSSD1331alert();
				OSA_TimeDelay(1000);
			}
			else{
				SEGGER_RTT_printf(0, "ADC Value: %d\r\n", ADC16_DRV_GetConvValueRAW(ADC_0, CHANNEL_0));
				OSA_TimeDelay(1000);
			}
  }

  return 0;
}
