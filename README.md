# I6.12 RTOS - Token Ring base project

The base code for the Token Ring project was developed as part of the HEI RTOS course.

The provided project is a real-time chat application based on a custom Token Ring protocol implementation using the RTX5 RTOS and the CMSIS-RTOS API version 2.

This is a Keil uVision project that is ready to use. It was tested with Keil ARMCC version 5. The project compiles without error but is not functional since the MAC layer has not been implemented.

The project runs on an ARM Cortex-M7 STM32F746 SoC at 216 MHz. uGFX (https://ugfx.io/) is used as a graphical library. The provided project has the stdout/ITM enabled (Debug printf Viewer) and the Event Recorder by default. TraceAnlyzer can be used to debug the real-time application. All configuration settings are available in the `main.h` header file.
