/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/*
 * This file has been automatically generated using ChibiStudio board
 * generator plugin. Do not edit manually.
 */

#ifndef BOARD_H
#define BOARD_H

/*
 * Setup for STMicroelectronics STM32F4-Discovery board.
 */

/*
 * Board identifier.
 */
#define BOARD_STM32F407VET6_MINIMAL
#define BOARD_NAME                  "vcc-gnd.com STM32F407VET6 board"

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768U
#endif

#if !defined(STM32_HSECLK)
//TODO: change this for other board!
#define STM32_HSECLK                25000000U
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   330U

/*
 * MCU type as defined in the ST header.
 */
#define STM32F407xx

/*
 * IO pins assignments.
 */
#define GPIOA_PIN0                  0U
#define GPIOA_PIN1                  1U
#define GPIOA_PIN2                  2U
#define GPIOA_PIN3                  3U
#define GPIOA_PIN4                  4U
#define GPIOA_PIN5                  5U
#define GPIOA_PIN6                  6U
#define GPIOA_PIN7                  7U
#define GPIOA_SDDETECT              8U
#define GPIOA_PIN9                  9U
#define GPIOA_PIN10                 10U
#define GPIOA_OTG_FS_DM             11U
#define GPIOA_OTG_FS_DP             12U
#define GPIOA_SWDIO                 13U
#define GPIOA_SWCLK                 14U
#define GPIOA_PIN15                 15U

#define GPIOB_PIN0                  0U
#define GPIOB_PIN1                  1U
#define GPIOB_BOOT1                 2U
#define GPIOB_PIN3                  3U
#define GPIOB_PIN4                  4U
#define GPIOB_PIN5                  5U
#define GPIOB_SCL1                  6U
#define GPIOB_SDA1                  7U
#define GPIOB_PIN8                  8U
#define GPIOB_LED                   9U
#define GPIOB_PIN10                 10U
#define GPIOB_PIN11                 11U
#define GPIOB_PIN12                 12U
#define GPIOB_PIN13                 13U
#define GPIOB_PIN14                 14U
#define GPIOB_PIN15                 15U

#define GPIOC_PIN0                  0U
#define GPIOC_PIN1                  1U
#define GPIOC_PIN2                  2U
#define GPIOC_PIN3                  3U
#define GPIOC_PIN4                  4U
#define GPIOC_PIN5                  5U
#define GPIOC_PIN6                  6U
#define GPIOC_PIN7                  7U
#define GPIOC_SDD0                  8U
#define GPIOC_SDD1                  9U
#define GPIOC_SDD2                  10U
#define GPIOC_SDD3                  11U
#define GPIOC_SDCLK                 12U
#define GPIOC_PIN13                 13U
#define GPIOC_PIN14                 14U
#define GPIOC_PIN15                 15U

#define GPIOD_PIN0                  0U
#define GPIOD_PIN1                  1U
#define GPIOD_SDCMD                 2U
#define GPIOD_PIN3                  3U
#define GPIOD_PIN4                  4U
#define GPIOD_PIN5                  5U
#define GPIOD_PIN6                  6U
#define GPIOD_PIN7                  7U
#define GPIOD_PIN8                  8U
#define GPIOD_PIN9                  9U
#define GPIOD_PIN10                 10U
#define GPIOD_PIN11                 11U
#define GPIOD_PIN12                 12U
#define GPIOD_PIN13                 13U
#define GPIOD_PIN14                 14U
#define GPIOD_PIN15                 15U

#define GPIOE_PIN0                  0U
#define GPIOE_PIN1                  1U
#define GPIOE_PIN2                  2U
#define GPIOE_PIN3                  3U
#define GPIOE_PIN4                  4U
#define GPIOE_PIN5                  5U
#define GPIOE_PIN6                  6U
#define GPIOE_PIN7                  7U
#define GPIOE_PIN8                  8U
#define GPIOE_PIN9                  9U
#define GPIOE_PIN10                 10U
#define GPIOE_PIN11                 11U
#define GPIOE_PIN12                 12U
#define GPIOE_PIN13                 13U
#define GPIOE_PIN14                 14U
#define GPIOE_PIN15                 15U

/*
 * IO lines assignments.
 */
#define LINE_SDDETECT               PAL_LINE(GPIOA, 8U)
#define LINE_OTG_FS_DM              PAL_LINE(GPIOA, 11U)
#define LINE_OTG_FS_DP              PAL_LINE(GPIOA, 12U)
#define LINE_SWDIO                  PAL_LINE(GPIOA, 13U)
#define LINE_SWCLK                  PAL_LINE(GPIOA, 14U)

#define LINE_BOOT1                  PAL_LINE(GPIOB, 2U)
#define LINE_SCL1                   PAL_LINE(GPIOB, 6U)
#define LINE_SDA1                   PAL_LINE(GPIOB, 7U)
#define LINE_LED                    PAL_LINE(GPIOB, 9U)

#define LINE_SDD0                   PAL_LINE(GPIOC, 8U)
#define LINE_SDD1                   PAL_LINE(GPIOC, 9U)
#define LINE_SDD2                   PAL_LINE(GPIOC, 10U)
#define LINE_SDD3                   PAL_LINE(GPIOC, 11U)
#define LINE_SDCLK                  PAL_LINE(GPIOC, 12U)

#define LINE_SDCMD                  PAL_LINE(GPIOD, 2U)

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

/* GPIO initial config */

#define VAL_GPIOA_MODER             (PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DM) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DP) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO) |      \
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DM) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DP) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDIO) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWCLK))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_HIGH(GPIOA_OTG_FS_DM) |     \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DP) |     \
                                     PIN_OSPEED_HIGH(GPIOA_SWDIO) |         \
                                     PIN_OSPEED_HIGH(GPIOA_SWCLK))
#define VAL_GPIOA_PUPDR             0
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_OTG_FS_DM) |        \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DP))
#define VAL_GPIOA_AFRL              0
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_OTG_FS_DM, 10U) |    \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DP, 10U) |    \
                                     PIN_AFIO_AF(GPIOA_SWDIO, 0U) |         \
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0U))

#define VAL_GPIOB_MODER             (PIN_MODE_ALTERNATE(GPIOB_SCL1) |       \
                                     PIN_MODE_ALTERNATE(GPIOB_SDA1) |       \
                                     PIN_MODE_OUTPUT(GPIOB_LED))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_OPENDRAIN(GPIOB_SDA1) |      \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_SDA1) |      \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_LED))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_HIGH(GPIOB_SCL1) |          \
                                     PIN_OSPEED_HIGH(GPIOB_SDA1) |          \
                                     PIN_OSPEED_HIGH(GPIOB_LED))
#define VAL_GPIOB_PUPDR             0
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_LED))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_SCL1, 4U) |          \
                                     PIN_AFIO_AF(GPIOB_SDA1, 4U))
#define VAL_GPIOB_AFRH              0

#define VAL_GPIOC_MODER             (PIN_MODE_ALTERNATE(GPIOC_SDD0) |           \
                                     PIN_MODE_ALTERNATE(GPIOC_SDD1) |           \
                                     PIN_MODE_ALTERNATE(GPIOC_SDD2) |           \
                                     PIN_MODE_ALTERNATE(GPIOC_SDD3) |           \
                                     PIN_MODE_ALTERNATE(GPIOC_SDCLK))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_SDD0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SDD1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SDD2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SDD3) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SDCLK))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_HIGH(GPIOC_SDD0) |          \
                                     PIN_OSPEED_HIGH(GPIOC_SDD1) |          \
                                     PIN_OSPEED_HIGH(GPIOC_SDD2) |          \
                                     PIN_OSPEED_HIGH(GPIOC_SDD3) |         \
                                     PIN_OSPEED_HIGH(GPIOC_SDCLK))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_FLOATING(GPIOC_SDD0) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_SDD1) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_SDD2) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_SDD3) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_SDCLK))
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(GPIOC_SDD0) |             \
                                     PIN_ODR_HIGH(GPIOC_SDD1) |             \
                                     PIN_ODR_HIGH(GPIOC_SDD2) |             \
                                     PIN_ODR_HIGH(GPIOC_SDD3) |            \
                                     PIN_ODR_HIGH(GPIOC_SDCLK))
#define VAL_GPIOC_AFRL              0
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_SDD0, 12U) |          \
                                     PIN_AFIO_AF(GPIOC_SDD1, 12U) |          \
                                     PIN_AFIO_AF(GPIOC_SDD2, 12U) |          \
                                     PIN_AFIO_AF(GPIOC_SDD3, 12U) |         \
                                     PIN_AFIO_AF(GPIOC_SDCLK,12U))

#define VAL_GPIOD_MODER             (PIN_MODE_ALTERNATE(GPIOD_SDCMD))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_SDCMD))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_HIGH(GPIOD_SDCMD))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_FLOATING(GPIOD_SDCMD))
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(GPIOD_SDCMD))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_SDCMD, 12U))
#define VAL_GPIOD_AFRH              0

#define VAL_GPIOE_MODER             0
#define VAL_GPIOE_OTYPER            0
#define VAL_GPIOE_OSPEEDR           0
#define VAL_GPIOE_PUPDR             0
#define VAL_GPIOE_ODR               0
#define VAL_GPIOE_AFRL              0
#define VAL_GPIOE_AFRH              0


#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* BOARD_H */
