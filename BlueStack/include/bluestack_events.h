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
 * @file bluestack_events.h
 * 
 * @author Alfredo Orozco de la Paz
 * @date 1 de Marzo del 2017
 * @brief Archivo con definiciones de las funciones manejadoras de eventos.
 * 
 * Provee definiciones a las funciones manejadoras de eventos de la aplicación
 * usando el SDK de Nordic. Atiende eventos del protocolo bluetooth y del sistema. 
 * Implementamanejadores por default, pero cada usuario puede defnir sus propios
 * manejadores y despachadores de eventos.
 *
 */

#ifndef _BLUESTACK_EVENTS_H_
#define _BLUESTACK_EVENTS_H_

#include "app_error.h"
#include "ble_hci.h"
#include "ble_advertising.h"
#include "peer_manager.h"
#include "ble_conn_params.h"
#include "ble_conn_state.h"
#include "fstorage.h"
#include "fds.h"
#include "bluestack_conf.h"

/**
 * @brief Ponter definition of function for the System Event dispatcher.
 */
typedef void (*BlueStack_SYS_Dispatcher)(uint32_t sys_evt);
/**
 * @brief Ponter definition of function for the Softdevice-BLE Event dispatcher.
 */
typedef void (*BlueStack_BLE_Dispatcher)(ble_evt_t* ble_evt);
/**
 * @brief Ponter definition of function for the Softdevice-BLE Events handler.
 */
typedef void (*BlueStack_BLE_Handler)(ble_evt_t* ble_evt);
/**
 * @brief Ponter definition of function for the System Events handler.
 */
typedef void (*BlueStack_SYS_Handler)(uint32_t sys_evt);
/**
 * @brief Ponter definition of function for the Connection Events handler.
 */
typedef void (*BlueStack_CONN_Handler)(ble_conn_params_evt_t * p_evt);
/**
 * @brief Ponter definition of function for the Error Connection Events handler.
 */
typedef void (*BlueStack_ERRCONN_Handler)(uint32_t error);
/**
 * @brief Ponter definition of function for the BLE-Advertising Events handler.
 */
typedef void (*BlueStack_ADV_Handler)(ble_adv_evt_t ble_adv_evt);
/**
 * @brief Ponter definition of function for the Peer Manager Events handler.
 */
typedef void (*BlueStack_PEER_Handler)(const pm_evt_t* p_evt);


/**@brief Default function for dispatching a BLE stack event to all modules with a BLE stack event handler.
 *
 * @details This function is called from the BLE Stack event interrupt handler after a BLE stack
 *          event has been received.
 *
 * @param[in] p_ble_evt  Bluetooth stack event.
 */
void BlueStack_BLE_EventDispatcher(ble_evt_t * p_ble_evt);

/**@brief Default function for dispatching a system event to interested modules.
 *
 * @details This function is called from the System event interrupt handler after a system
 *          event has been received.
 *
 * @param[in] sys_evt  System stack event.
 */
void BlueStack_SYS_EventDispatcher(uint32_t sys_evt);

/**@brief Default function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
void BlueStack_ADV_Event(ble_adv_evt_t ble_adv_evt);

/**@brief Default function for handling the Application's BLE Stack events.
 *
 * @param[in] p_ble_evt  Bluetooth stack event.
 */
void BlueStack_BLE_Event(ble_evt_t * p_ble_evt);

/**@brief Default unction for handling the  System events.
 *
 * @param[in] sys_evt  system.
 */
void BlueStack_SYS_Event(uint32_t sys_evt);

/**@brief Default function for handling a Connection Parameters error.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
void BlueStack_ERRCONN_Event(uint32_t nrf_error);

/**@brief Default function for handling the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module which
 *          are passed to the application.
 *          @note All this function does is to disconnect. This could have been done by simply
 *                setting the disconnect_on_fail config parameter, but instead we use the event
 *                handler mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
void BlueStack_CONN_Event(ble_conn_params_evt_t * p_evt);

/**
 * @brief      Default function for handling the Peer Manager module events.
 * 
 * @details    This function is called from the Perr Manager event interrupt handler after a Peer
 *             event has been received.
 *
 * @param[in]  p_evt  The p event pointer.
 */
void BlueStack_PEER_Event(const pm_evt_t* p_evt);


#endif //_BLUESTACK_EVENTS_H_