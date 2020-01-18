#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "fsl_misc_utilities.h"
#include "fsl_device_registers.h"
#include "fsl_i2c_master_driver.h"
#include "fsl_spi_master_driver.h"
#include "fsl_tpm_driver.h"
#include "fsl_tpm_hal.h"
#include "fsl_rtc_driver.h"
#include "fsl_clock_manager.h"
#include "fsl_power_manager.h"
#include "fsl_mcglite_hal.h"
#include "fsl_port_hal.h"
#include "fsl_smc_hal.h"
#include "fsl_pmc_hal.h"

#include "SEGGER_RTT.h"
#include "gpio_pins.h"
#include "warp.h"

#include "devHPA17F.h"

#define TPM_BASEADDR    (0U)
#define TPM_CHANNEL     (0U)

enum
{
	kHPA17FPinPWM		= GPIO_MAKE_PIN(HW_GPIOB, 11),
};

int devHPA17Finit(void)
{
  PORT_HAL_SetMuxMode(PORTB_BASE, 11u, kPortMuxAlt2);

  tpm_pwm_param_t tpmParam;
  tpm_general_config_t tpmConfig;

	//PWM configuration
  tpmParam.mode = kTpmEdgeAlignedPWM;
  tpmParam.edgeMode = kTpmHighTrue;
  tpmParam.uFrequencyHZ = 1000; 		//Frequency of PWM
  tpmParam.uDutyCyclePercent = 50;	//Duty cycle of PWM

  tpmConfig.isDBGMode = false;
  tpmConfig.isGlobalTimeBase = false;
  tpmConfig.isTriggerMode = false;
  tpmConfig.isStopCountOnOveflow = false;
  tpmConfig.isCountReloadOnTrig = false;
	tpmConfig.triggerSource = 0;

  //SEGGER_RTT_WriteString(0, "Starting PWM ...\r\n");

	//PWM initialisation
  TPM_DRV_Init(TPM_BASEADDR, &tpmConfig);
	TPM_DRV_SetClock(TPM_BASEADDR, kTpmClockSourceModuleMCGIRCLK, kTpmDividedBy1);
  if(!TPM_DRV_PwmStart(TPM_BASEADDR, &tpmParam, TPM_CHANNEL))
	{
		SEGGER_RTT_printf(0, "PWM FAILED\n");
	}

	return 0;
}
