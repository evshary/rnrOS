ARCH = CM4
MCPU = cortex-m4
PLATFORM = STM32F4X
STARTUP_CODE = libraries/$(PLATFORM)/system/src/startup/startup_stm32f429_439xx.s

PLAT_DEFINE = -DSTM32F429_439xx -DUSE_STDPERIPH_DRIVER

FILTER_OUT_SRC = libraries/$(PLATFORM)/driver/src/stm32f4xx_fsmc.c
