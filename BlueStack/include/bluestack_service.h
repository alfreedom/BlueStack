#ifndef _BLUESTACK_SERVICE_H_
#define _BLUESTACK_SERVICE_H_

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "bluestack_events.h"

#define CHECK_ERROR(ERR_CODE)  if (ERR_CODE != NRF_SUCCESS) return ERR_CODE

#define MAX_SERVICES      16
#define MAX_SERVICE_CHARS 16

#define BLUESTACK_CHAR_TYPE_RD      0x01
#define BLUESTACK_CHAR_TYPE_WR      0x02
#define BLUESTACK_CHAR_TYPE_NTFY    0x04

    
#define BLUESTACK_SERVICE_DISPATCHER_LOOP for(uint8_t i = 0; i < _services_count; ++i) BlueStack_OnServiceBleEvent(&_services[i], p_ble_evt)

typedef struct bluestack_srv_s BlueStack_Service_t;
typedef struct bluestack_chr_s BlueStack_Characteristic_t;

typedef void (*BlueStack_Service_Write_Handler_t) (BlueStack_Characteristic_t * p_char, uint8_t* data, uint8_t data_len);

typedef struct bluestack_chr_s{
    ble_gatts_char_handles_t    handle;
    uint8_t                     type;
    uint16_t                    uuid;
    uint8_t                     data_size;   
}BlueStack_Characteristic_t;

typedef struct bluestack_srv_s{
    uint16_t                            service_handle;
    uint16_t                            conn_handle;
    uint8_t                             service_uuid_type;              // Tipo del UUID del servicio
    ble_uuid128_t                       service_uuid_base;
    uint16_t                            service_uuid;
    BlueStack_Service_Write_Handler_t   ble_write_handler;
    BlueStack_Characteristic_t          *chars;
    uint8_t                             chars_count;
}BlueStack_Service_t;


void BlueStack_OnServiceBleEvent(BlueStack_Service_t* service, ble_evt_t * p_ble_evt);
uint32_t BlueStack_ServiceAdd(BlueStack_Service_t* service);
uint32_t BlueStack_WriteCharAdd(BlueStack_Characteristic_t* c, uint8_t service_id);
uint32_t BlueStack_ReadCharAdd(BlueStack_Characteristic_t* c, uint8_t service_id);
uint32_t BlueStack_CharAdd(BlueStack_Characteristic_t* c, uint8_t service_id);

uint32_t BlueStack_ServiceRead(uint8_t service_id, BlueStack_Characteristic_t* p_char, uint8_t* data, uint8_t data_len);
uint32_t BlueStack_ServiceNotify(uint8_t service_id, BlueStack_Characteristic_t* p_char, uint8_t* data, uint8_t data_len);

#endif // _BLUESTACK_SERVICE_H_


