/*
 *  ======== ti_drivers_config.h ========
 *  Configured TI-Drivers module declarations
 *
 *  DO NOT EDIT - This file is generated for the CC3220SF_LAUNCHXL
 *  by the SysConfig tool.
 */
#ifndef ti_drivers_config_h
#define ti_drivers_config_h

#define CONFIG_SYSCONFIG_PREVIEW

#define CONFIG_CC3220SF_LAUNCHXL

#ifndef DeviceFamily_CC3220
#define DeviceFamily_CC3220
#endif

#include <ti/devices/DeviceFamily.h>

#include <stdint.h>

/* support C++ sources */
#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ======== Crypto ========
 */

#define CONFIG_Crypto_0             0

/*
 *  ======== GPIO ========
 */

/* P04, LaunchPad User Button SW2 (left) */
#define CONFIG_GPIO_BUTTON_0        0
/* P15, LaunchPad User Button SW3 (right) */
#define CONFIG_GPIO_BUTTON_1        1
/* P64, LaunchPad LED D10 (Red) */
#define CONFIG_GPIO_LED_0           2
/* P01, LaunchPad LED D9 (Yellow) */
#define CONFIG_GPIO_LED_1           3
/* P02, LaunchPad LED D8 (Green) */
#define CONFIG_GPIO_LED_2           4
/* P18 */
#define CONFIG_GPIO_0               5
/* P61 */
#define CONFIG_GPIO_1               6
/* P03 */
#define CONFIG_GPIO_2               7

/* LEDs are active high */
#define CONFIG_GPIO_LED_ON  (1)
#define CONFIG_GPIO_LED_OFF (0)

#define CONFIG_LED_ON  (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)


/*
 *  ======== PWM ========
 */

/* P21 */
#define CONFIG_PWM_0                0


/*
 *  ======== SPI ========
 */

#define CONFIG_NWP_SPI               0



/*
 *  ======== Timer ========
 */

#define CONFIG_TIMER_0              0

/*
 *  ======== UART ========
 */

/*
 *  TX: P07
 *  RX: P08
 */
#define CONFIG_UART_1               0
/*
 *  TX: P55
 *  RX: P57
 *  XDS110 UART
 */
#define CONFIG_UART_0               1


/*
 *  ======== Watchdog ========
 */

#define CONFIG_WATCHDOG_0           0


/*
 *  ======== Board_init ========
 *  Perform all required TI-Drivers initialization
 *
 *  This function should be called once at a point before any use of
 *  TI-Drivers.
 */
extern void Board_init(void);

/*
 *  ======== Board_initGeneral ========
 *  (deprecated)
 *
 *  Board_initGeneral() is defined purely for backward compatibility.
 *
 *  All new code should use Board_init() to do any required TI-Drivers
 *  initialization _and_ use <Driver>_init() for only where specific drivers
 *  are explicitly referenced by the application.  <Driver>_init() functions
 *  are idempotent.
 */
#define Board_initGeneral Board_init

#ifdef __cplusplus
}
#endif

#endif /* include guard */