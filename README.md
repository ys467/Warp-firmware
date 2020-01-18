# Air Quality Monitor (CO)

**Yungyu Song, Churchill College**
**ys467**

## Project Summary
This is the firmware for the Warp hardware and its publicly available and unpublished derivatives. This firmware also runs on the Freescale/NXP FRDM KL03 evaluation board which we use for teaching at the University of Cambridge.
The firmware modification for this project is implemented to create an air quality monitor, specifically for carbon monoxide (CO). The project is designed to provide a multi-sensory alarm system for those at risk of CO intoxication, which can be fatal.
The system consists of an ADC input from CO sensor (MQ7), and both a visual and auditive output from SSD1331 OLED module and HPA17F buzzer.

**Prerequisites:** You need an arm cross-compiler such as `arm-none-eabi-gcc` installed as well as a working `cmake` (installed, e.g., via `apt-get` on Linux or via [MacPorts](https://www.macports.org) on macOS). You will also need an installed copy of the SEGGER [JLink commander](https://www.segger.com/downloads/jlink/), `JlinkExe`, which is available for Linux, macOS, and Windows (here are direct links for downloading it for [macOS](https://www.segger.com/downloads/jlink/JLink_MacOSX.pkg), and [Linux tgz 64-bit](https://www.segger.com/downloads/jlink/JLink_Linux_x86_64.tgz)).

## Repository Layout
The project is based on Warp-firmware framework, and the source code is provided [here](https://github.com/physical-computation/Warp-firmware).
All project files are located in the following directory.

```
Warp-firmware/src/boot/ksdk1.1.0
```

The main modifications to the source can be divided into 4 main sections, as described below.

### 1. MQ7 Gas Sensor
Driver files: devMQ7.c and devMQ7.h
The files allow ADC set up and initialisation to receive readings from MQ7 gas sensor without interrupt, and initiates piezo buzzer and OLED module if the ADC value exceeds a certain threshold, indicating significant presence of CO.

### 2. HPA17F Piezo buzzer
Driver files: devHPA17F.c and devHPA17F.h
The piezo buzzer is driven from a 5V pk-pk signal at 1 kHz with a duty cycle of 50%. A TPM module is used to create PWM signal which matches the specification of the buzzer.

### 3. SSD1331 OLED module
Driver files: devSSD1331.c and dev1331.h
Two OLED functions, devSSD1331test() and devSSD1331alert() are added to notify the users of the system that the board is in gas detecting mode, and that the system has detected significant presence of CO, respectively. This results in a change of colour displayed by the OLED.

### 4. warp-kl03-ksdk1.1-boot.c
This file is modified from the source to include additional driver files, and a new case 'w' is created to use the Freedom board as an air quality monitor. The updated warp menu should look as follows.

````
[ *				W	a	r	p	(rev. b)			* ]
[  				      Cambridge / Physcomplab   				  ]

	Supply=0mV,	Default Target Read Register=0x00
	I2C=200kb/s,	SPI=200kb/s,	UART=1kb/s,	I2C Pull-Up=32768

	SIM->SCGC6=0x20000001		RTC->SR=0x10		RTC->TSR=0x5687132B
	MCG_C1=0x42			MCG_C2=0x00		MCG_S=0x06
	MCG_SC=0x00			MCG_MC=0x00		OSC_CR=0x00
	SMC_PMPROT=0x22			SMC_PMCTRL=0x40		SCB->SCR=0x00
	PMC_REGSC=0x00			SIM_SCGC4=0xF0000030	RTC->TPR=0xEE9

	0s in RTC Handler to-date,	0 Pmgr Errors
Select:
- 'a': set default sensor.
- 'b': set I2C baud rate.
- 'c': set SPI baud rate.
- 'd': set UART baud rate.
- 'e': set default register address.
- 'f': write byte to sensor.
- 'g': set default SSSUPPLY.
- 'h': powerdown command to all sensors.
- 'i': set pull-up enable value.
- 'j': repeat read reg 0x00 on sensor #3.
- 'k': sleep until reset.
- 'l': send repeated byte on I2C.
- 'm': send repeated byte on SPI.
- 'n': enable SSSUPPLY.
- 'o': disable SSSUPPLY.
- 'p': switch to VLPR mode.
- 'r': switch to RUN mode.
- 's': power up all sensors.
- 't': dump processor state.
- 'u': set I2C address.
- 'x': disable SWD and spin for 10 secs.
- 'z': dump all sensors data.
- 'w': gas sensor mode
Enter selection>
````

Note that files such as CMakeLists.txt (in src folder) and build.sh (in build folder) must be modified adequately to include new driver files in the CMake and build process.

### Acknowledgements
This project is supported by Dr. Phillip Stanley-Marbell, University of Cambridge.
