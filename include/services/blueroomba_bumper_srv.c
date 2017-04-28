#include "blueroomba_bumper_srv.h"


static BlueStack_Service_t br_srv;
static BlueStack_Characteristic_t br_chars[2] = { {CHAR_BUMPERLEFT}, {CHAR_BUMPERRIGHT} };
static uint32_t br_srv_id;

uint32_t BlueRoombaSrv_Init() {

    ble_uuid128_t uuid_base = {BLUEROOMBA_UUID_BASE};

    memset(&br_srv, 0, sizeof(br_srv));

    br_srv.service_uuid_base = uuid_base;
    br_srv.service_uuid      = BLUEROOMBA_UUID_SERVICE;
    br_srv.chars             = br_chars;
    br_srv.chars_count       = 2;
    br_srv.ble_write_handler = NULL;

    br_srv_id = BlueStack_ServiceAdd(&br_srv);
   
    if(br_srv_id != -1)
        return 1;
    else 
        return 0;
}