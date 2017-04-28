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
 * @file BlueStack.h
 * 
 * @author Alfredo Orozco de la Paz
 * @date 25 de Febrero del 2017
 * @brief Archivo con definiciones de las funciones de la API BlueStack.
 *
 * Configuración de la aplicación BLE, basado en la plantilla BLE de Nordic.
 * Este  archivo  define los parámetros  para la configuración de la aplicación 
 * BLE de usuario.
 *     
 * Se puede  modificar  el nombre  del dispositivo  a  mostrar,  el nombre  de 
 * Manufactura, el intervalo  de  advertising   el  timeout   del  advertisin, 
 * parámetros de conexión, de seguridad, configura parámetros de los timers de 
 * aplicación, y las configuraciónes del usuario.
 */


#ifndef BLUESTACK_H_
#define BLUESTACK_H_

// Macro para checar errores sin bloqueo.
#define delay_ms(ms) nrf_delay_ms(ms)

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_sdm.h"
#include "nrf_delay.h"
#include "app_timer.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "softdevice_handler.h"
#include "app_error.h"
#include "peer_manager.h"

#include "bluestack_events.h"
#include "bluestack_service.h"

#define CHECK_ERROR(ERR_CODE)  if (ERR_CODE != NRF_SUCCESS) return ERR_CODE

/**
 * @struct BlueStack_cfg_t
 * @brief BlueStack configuration struct.
 * 
 * This struct contains the parametes for the configutarion
 * of the Bluetooh module by the Nordic SDK.
 */
typedef struct {
	char* device_name;									///< The bluetooth device name.
	uint8_t device_appearance;							///< The appearance type of the bluetooth (optional)
	ble_uuid_t adv_uuid;								///< The UUID to show in the advertising.
	uint32_t adv_interval;								///< The advertising interval in orden on 0.625ms
	uint32_t adv_timeout;								///< The advertising timeout in seconds
	char* adv_data;										///< The data to send in the advertising
	uint32_t adv_data_len;								///< The lenght of the data to send in the advertising.
	uint8_t erase_bonds;								///< Indicate if the bonded device will be erased.
	BlueStack_BLE_Dispatcher ble_event_dispatcher;		///< Pointer to the Bluettoth Events dispatcher function.
	BlueStack_SYS_Dispatcher sys_event_dispatcher;		///< Pointer to the System Events dispatcher function.
	BlueStack_BLE_Handler ble_event_handler;			///< Pointer to the Bluetooth events handler.
	BlueStack_SYS_Handler sys_event_handler;			///< Pointer to the System events handler.
	BlueStack_CONN_Handler conn_event_handler;			///< Pointer to the Connection event handler.
	BlueStack_ERRCONN_Handler errconn_event_handler;	///< Pointer to the Error Connection event handler
	BlueStack_ADV_Handler adv_event_handler;			///< Pointer to the Advertising events handler.
	BlueStack_PEER_Handler peer_event_handler;			///< Pointer to the Pairing events handler.
}BlueStack_cfg_t;

/**
 * @brief      Initialize the Bluetooth Stack
 *
 * @param      bs_cfg  The BlueStacl configuration struct.
 *
 * @return     Return the error code, NRF_SUCCESS if no errors ocurred.
 */
uint32_t BlueStack_Init(BlueStack_cfg_t* bs_cfg);

/**
 * @brief      Power manager for the BlueStack events.
 *             This fuction musb be call it continuosly to
 *             reduce the power consumption.
 *             
 * @return     The error of the Power Manager.
 */
uint32_t BlueStack_PowerManage();

/**
 * @brief      Starts the Bluetooth Advertsing.
 *
 * @return     Return the error code, NRF_SUCCESS if no errors ocurred.
 */
uint32_t BlueStack_AdvertisingStart();

/**
 * @brief      Función para actualizar los datos de publicidad.
 *
 * @details     Actualiza los datos a enviar en el modo advertising. La actualización
 * se mostrará inmediatamente si se ha iniciado el Advertising.
 * 
 * @param[in]  adv_uuid     UUID del servicio a enviar por advertising (16 bits)
 * @param      p_data  		Apuntador al buffer de datos a enviar
 * @param[in]  size    		Tamaño del buffer de datos.
 *
 * @return     Devuelve NRF_SUCCESS si no hubo errores, de lo contrario devuelve el código de error.
 */
uint32_t BlueStack_AdvertisingUpdate(uint16_t UUID, uint8_t* p_data, uint8_t size);

/**
 * @brief Función para inicializar el Advertising (Publicidad)
 *
 * @details Esta fución inicializa los parámetros de advertising, enviando
 * los datos como Nombre del dispositivo, Nombre de Manufactura
 * entre otros datos.
 *  
 * Se pueden enviar tamién datos de advertising con la opción
 * Scanner Request en la función ble_advertising_init del mismo 
 * modo que en los parámetros de nombre.
 *  
 * Sólo se pueden enviar 31 bytes por advertising y 31 datos por
 * Scanner Request, teniendo un total de 62 bytes para enviar mediante
 * Advertising.
 *  
 * @param[in]  adv_uuid      UUID del servicio (16 bits)
 * @param[in]  adv_interval  Intervalo de advertising (en unidades de 0.625 ms).
 * @param[in]  adv_timeout   Tiempo de timeout para terminar el advertising en segundos
 * @param[in]  adv_evt       Función para manejar los eventos del advertising.
 *
 * @return     Devuelve NRF_SUCCESS si no hubo errores, de lo contrario devuelve el código de error.
 */
uint32_t bluestack_adv_init(ble_uuid_t* adv_uuid, uint32_t adv_interval, uint32_t adv_timeout, BlueStack_ADV_Handler adv_evt);


/**
 * @brief      Función para incializar el Stack Ble y el softdevice
 *
 * @details    Inicializa el stack ble llamando a las funciones del Softdevice
 * provistas por Nordic, ercibiendo las funciones que se encargan de despachar los
 * eventos del systema y del softdevice.
 * 
 * @param[in]  ble_evt_dispatch  Función que despacha los eventos BLE.
 * @param[in]  sys_evt_dispatch  Función que despacha los eventos del Sistema.
 *
 * @return     Devuelve NRF_SUCCESS si no hubo errores, de lo contrario devuelve el código de error.
 */
uint32_t ble_stack_init(BlueStack_BLE_Dispatcher ble_evt_dispatch, BlueStack_SYS_Dispatcher sys_evt_dispatch);

/**
 * @brief      Función para inicializar el GAP (Generic Access Profile)
 *
 * @details    Inicializa todos los parámetros GAP (Generic Access Profile) 
 * del dispositivo incluyendo el nombre, la apariencia, y el parámetro de conexión preferida.
 * 
 * @param      device_name     Nombre del dispositvo.
 * @param[in]  ble_appearance  Apariencia del dispositvo (Raton, taclado, beacon, etc.).
 *
 * @return     Devuelve NRF_SUCCESS si no hubo errores, de lo contrario devuelve el código de error.
 */
uint32_t gap_params_init(char* device_name, uint8_t ble_appearance);

/**
 * @brief      Función para incializar los parámetros de conexión.
 * 
 * @details    Inicializa el módulo de conexión Bluetooth con llamadas a las funciones
 * del Softdevice de Nordic. Recibe las funciones manejadores de enventos de conexión.
 *
 * @param[in]  conn_evt_handler  Función para manejar los eventos de conexión.
 * @param[in]  err_conn_handler  Función para manejar los enventos de errores de conexión.
 *
 * @return     Devuelve NRF_SUCCESS si no hubo errores, de lo contrario devuelve el código de error.
 */
uint32_t conn_params_init(BlueStack_CONN_Handler conn_evt_handler, BlueStack_ERRCONN_Handler err_conn_handler);

/**
 * @brief      Función para incializar el modulo de emparejamiento.
 * 
 * @details    Inicializa el modulo de emparejamiento. Sirve para guardar y recordar los
 * dispositvos que se han emparejado con el periférico.
 *
 * @param[in]  erase_bonds     Indica si se eliminan todos los dispositivos emparejados.
 * @param[in]  pm_evt_handler  Funcion para manejar los eventos de emparejamiento.
 *
 * @return     { description_of_the_return_value }
 */
uint32_t peer_manager_init(bool erase_bonds, BlueStack_PEER_Handler pm_evt_handler);

#endif // BLUESTACK_H_
