/****************************************************************************
 * Copyright (C) 2017 by Alfredo Orozco, alfredoopa@gmail.com               *
 *                                                                          *
 * This file is part of file of BlueRoomba Firmware.                        *
 *                                                                          *
 *   BlueRoomba isfree software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU Lesser General Public License as published  *
 *   by the Free Software Foundation, either version 3 of the License, or   *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   BlueRoomba is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU Lesser General Public License for more details.                    *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with Box.  If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

/**
 * @file ble400.h
 * 
 * @author Alfredo Orozco de la Paz
 * @date 1 de Marzo del 2017
 * @brief Archivo con definiciones para la tarjeta de desarrollo BLE400.
 * 
 * Define los componentes conectados a la tarjeta BLE400, es decir, a cuales 
 * pines del chip NRF51822 están conectados lo leds, push buttons, y los 
 * perifericos externos.
 * 
 * Todas las definiciones de estos pines se pueden cambiar acorde a la placa, es decir
 * se puede re-definir este archivo y agregar más conexiones según el diseño de la placa.
 * 
 * El chip NRF51822 puede redirecciónas las salidas de sus periféricos (UART,SPI,TWI.. etc)
 * a cualquier pin GPIO, esto da versatilidad y facilidad a la hora de realizar un diseño
 * con este chip.
 */

#ifndef BLE400_H_
#define BLE400_H_


#define LEDS_NUMBER	7	///< Definición de pines de los leds de la tarjeta BLE400

#define LED_TX     11	///< Led TX, conectado al pin GPIO RX de la UART (Activo en bajo)
#define LED_RX      9	///< Led RX, conectado al pin GPIO TX de la UART (Activo en bajo)
#define LED0       18	///< Led 0 de la tarjeta, pin GPIO del LED 0 (Activo en alto)
#define LED1       19	///< Led 1 de la tarjeta, pin GPIO del LED 1 (Activo en alto)
#define LED2       20	///< Led 2 de la tarjeta, pin GPIO del LED 2 (Activo en alto)
#define LED3       21	///< Led 3 de la tarjeta, pin GPIO del LED 3 (Activo en alto)
#define LED4       22	///< Led 4 de la tarjeta, pin GPIO del LED 4 (Activo en alto)

///< Lista de leds. Sirve para definir un arreglo de leds.
#define LEDS_LIST { LED0, LED1, LED2, LED3, LED4 }

///< Máscara de bits de cada led. Sirve para hacer operaciónes a bits
///< con los leds en el registro GPIO
#define LED0_MASK (1<<LED0)
#define LED1_MASK (1<<LED1)
#define LED2_MASK (1<<LED2)
#define LED3_MASK (1<<LED3)
#define LED4_MASK (1<<LED4)

///< Máscara de bits de todos los leds.
#define LEDS_MASK      (LED0_MASK | LED1_MASK | LED2_MASK | LED3_MASK | LED4_MASK)

///< Definición de pines los botones de la tarjeta BLE400
#define BUTTONS_NUMBER 2

// Los push buttons de la placa están conectados a tierra en un extremo y a un pin del NRF51822 en el otro,
// por lo que se debe configurar resistencias PULL-UP para los botones
#define BUTTON_START    16
#define BUTTON0         16                      ///< Push button 1 (En la placa KEY1)    
#define BUTTON1         17                      ///< Push button 2 (En la placa KEY2)
#define BUTTON_STOP     17                      ///< Push button 2 (En la placa KEY2)
#define BUTTON_PULL     NRF_GPIO_PIN_PULLUP     ///< Configuración de la resistencia para los botones (PULL-UP o PULL-DOWN), 

///< Lista de push buttons. Sirve para definir un arreglo de botones.
#define BUTTONS_LIST { BUTTON0, BUTTON1}

///< Máscara de bits de cada push button. Sirve para hacer operaciónes a bits
///< con los push buttons en el registro GPIO
#define BUTTON0_MASK (1<<BSP_BUTTON0)
#define BUTTON1_MASK (1<<BSP_BUTTON1)

///< Máscara de bits de todos los push buttons.
#define BUTTONS_MASK   (BUTTON0_MASK | BUTTON1_MASK)

// Definicion de pines para conexión del módulo UART con el conversor USB-Serie incluido en la tarjeta.
// En la tarjeta BLE400, estos pines están conectados al conversor USB-Serie
// por lo que los datos de la UART serán enviados y recibidos por medio de
// este conversor a la PC.
#define RX_PIN_NUMBER       11          ///< Pin del GPIO conectado a la señal RX del módulo UART con el conversor USB-Serie  (Entrada)
#define TX_PIN_NUMBER       9           ///< Pin del GPIO conectado a la señal RX del módulo UART con el conversor USB-Serie  (Salida)
#define CTS_PIN_NUMBER      10          ///< Pin del GPIO conectado a la señal CTS del módulo UART con el conversor USB-Serie (Entrada)
#define RTS_PIN_NUMBER      8           ///< Pin del GPIO conectado a la señal RTS del módulo UART con el conversor USB-Serie (Salida)
#define HWFC_ENABLED        false       ///< Habilita o desahabilita el control de flujo por Hardwarede la UART.

// Definicion de pines para conexión del módulo UART con conector externo de la tarjeta.
// En la tarjeta BLE400, estos pines se conectan al conector externo UART, lo que permite
// dirigir los datos de la UART a otro dispositivo externo.
#define RX_EXTERN_PIN_NUMBER    5       ///< Pin del GPIO conectado a la señal RX del módulo UART con el conector externo  (Entrada)
#define TX_EXTERN_PIN_NUMBER    6       ///< Pin del GPIO conectado a la señal TX del módulo UART con el conector externo  (Salida)
#define CTS_EXTERN_PIN_NUMBER   7       ///< Pin del GPIO conectado a la señal CTS del módulo UART con el conector externo (Entrada)
#define RTS_EXTERN_PIN_NUMBER   12      ///< Pin del GPIO conectado a la señal RTS del módulo UART con el conector externo (Salida)
#define HWFC_EXTERN             false   ///< Habilita o desahabilita el control de flujo por Hardware de la UART.

// Definicion de pines para conexión del módulo SPI-MASTER-0 con conector externo de la tarjeta.
// En la tarjeta BLE400, estos pines se conectan al conector externo SPI, lo que permite
// dirigir los datos del módulo SPI-MASTER-0 a otro dispositivo externo.
#define SPI0_SCK_PIN       25     ///< Pin del GPIO conectado a la señal CLK del módulo SPI con el conector externo
#define SPI0_MOSI_PIN      24     ///< Pin del GPIO conectado a la señal MOSI del módulo SPI con el conector externo
#define SPI0_MISO_PIN      23     ///< Pin del GPIO conectado a la señal MISO del módulo SPI con el conector externo
#define SPI0_SS_PIN        30     ///< Pin del GPIO conectado a la señal SS del módulo SPI (Activo en Bajo)

// Definicion de pines para conexión del módulo TWI-0 (I2C) con conector externo de la tarjeta.
// En la tarjeta BLE400, estos pines se conectan al conector externo I2C, lo que permite
// dirigir los datos del módulo TWI-0 a otro dispositivo externo.
#define TWI_MASTER_CONFIG_CLOCK_PIN_NUMBER 0     ///< Pin del GPIO conectado a la señal CLK del módulo TWI con el conector externo  (SCL)
#define TWI_MASTER_CONFIG_DATA_PIN_NUMBER  1     ///< Pin del GPIO conectado a la señal DATA del módulo TWI con el conector externo (SDA)

#endif // BLE400_H_
