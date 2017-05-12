/****************************************************************************
 * Copyright (C) 2017 by Alfredo Orozco, alfredoopa@gmail.com               *
 *                                                                          *
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
 * @file main.c
 * 
 * @author Alfredo Orozco de la Paz
 * @date 2 de Marzo del 2017
 * @brief Archivo principal que contiene la función main e inicialización
 *        del software con el SoC NRF51822
 *
 * Se utiliza el framework BlueStack para incializar el stack bluetooth
 * provisto por Nordic Semiconductos©. Para mas detalles acerca de
 * las fuciones implementadas en la biblioteca ver la documentación de
 * BlueStack.
 */

#include "BlueStack.h"
#include "ble400.h"


//////////////////////////////////////////////////////////////////////////////////////
// UUID Base del servicio.
#define SRV_UUID_BASE          {0x23, 0xD4, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}

// UUID del servicio
#define SRV_UUID_SERVICE       0xAA01

// UUIDE de las características que incluye el servicio.
#define SRV_UUID_WRITE_CHAR    0xAA02
#define SRV_UUID_READ_CHAR     0xAA03


BlueStack_Characteristic_t chars[2];

uint32_t my_service_id;
uint32_t err_code;

APP_TIMER_DEF(advertising_led_timer_id);
#define ADVERTISING_LED_INTERVAL APP_TIMER_TICKS(1500, APP_TIMER_PRESCALER)

void My_ADV_Event(ble_adv_evt_t ble_adv_evt)
{
    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
            app_timer_start(advertising_led_timer_id, ADVERTISING_LED_INTERVAL, NULL);
            break;

        case BLE_ADV_EVT_IDLE:
            app_timer_stop(advertising_led_timer_id);
            break;

        default:
            break;
    }
}

static void AdvertisingLedUpdate(){
    nrf_gpio_pin_set(LED3);
    nrf_delay_ms(20);
    nrf_gpio_pin_clear(LED3);
}

void OnBlueRoombaWrite(BlueStack_Characteristic_t * p_char, uint8_t* data, uint8_t data_len){

    if(p_char->uuid == SRV_UUID_WRITE_CHAR && data_len == 1){
        
        if(data[0])
            nrf_gpio_pin_set(LED4);
        else
            nrf_gpio_pin_clear(LED4);

        BlueStack_ServiceRead(my_service_id, &chars[0], data, 1);
    }

}

/**@brief Function for the Application Timer initialization.
 *
 * @details Initializes the timer module. This creates and starts application timers.
 */
static void timers_init(void)
{
    // Create timers.
    err_code = app_timer_create(&advertising_led_timer_id, APP_TIMER_MODE_REPEATED, AdvertisingLedUpdate);
    APP_ERROR_CHECK(err_code);

}

/**@brief Function for starting the Application timers.
 *
 * @details Starts the application timers.
 */
static void application_timers_start(void)
{
    /* YOUR_JOB: Start your timers. below is an example of how to start a timer.
       uint32_t err_code;
       err_code = app_timer_start(m_app_timer_id, TIMER_INTERVAL, NULL);
       APP_ERROR_CHECK(err_code); */


}

/**@brief Function for initializing the board buttons and leds.
 *
 * @details Configure the leds an buttons on the main board. This configuration
 *          depends of the circuit conections.
 */
static void buttons_leds_init()
{
    nrf_gpio_cfg_output(LED4);
    nrf_gpio_cfg_output(LED3);
}

void services_init(){

    BlueStack_Service_t my_service;

    ble_uuid128_t uuid_base = {SRV_UUID_BASE};

    memset(&chars[0], 0, sizeof(BlueStack_Characteristic_t));
    chars[0].uuid = SRV_UUID_READ_CHAR;
    chars[0].type = BLUESTACK_CHAR_TYPE_NTFY;
    chars[0].data_size = 1;

    memset(&chars[1], 0, sizeof(BlueStack_Characteristic_t));
    chars[1].uuid = SRV_UUID_WRITE_CHAR;
    chars[1].type = BLUESTACK_CHAR_TYPE_WR;
    chars[1].data_size = 1;



    memset(&my_service, 0, sizeof(my_service));
    my_service.service_uuid_base = uuid_base;
    my_service.service_uuid      = SRV_UUID_SERVICE;
    my_service.chars             = chars;
    my_service.chars_count       = 2;
    my_service.ble_write_handler = OnBlueRoombaWrite;

    my_service_id = BlueStack_ServiceAdd(&my_service);


}
/**@brief Function for initializing Bluetooth Stack module.
 *
 * @details Call the BlueStack API functions whith the bluetooth params
 *          to initializing the BLE comunnication.
 */
void ble_init() {

    BlueStack_cfg_t bs_cfg;
    // Configuración e inicialización del Bluetooth
    bs_cfg.device_name           = "BlueStack-BLE";                              // Nombre a mostrar
    bs_cfg.adv_uuid.uuid         = 0;                                        // UUID a mostar en el Advertising
    bs_cfg.adv_uuid.type         = BLE_UUID_TYPE_BLE;                        // Tipo del dispositivo
    bs_cfg.device_appearance     = 0;                                    // Apariencia, puede ser raton, teclado, audio, etc..
    bs_cfg.erase_bonds           = 0;                                        // Indica si se eliminan los dispositivos emparejados
    bs_cfg.adv_interval          = 100;                                      // Intervalo del Advertising en unidades de 0.625 ms
    bs_cfg.adv_timeout           = 180;                                      // Tiempo de espera del Advertising. (tiempo de visibilidad)
    bs_cfg.adv_data              = NULL;                                     // Datos a enviar por el advertising
    bs_cfg.adv_data_len          = 0;                                        // Tamaño de los datos a enviar por Advertising
    // Configura los eventos del Stack Bluetooth. Ver archivo myevents.c
    bs_cfg.ble_event_dispatcher  = BlueStack_BLE_EventDispatcher;    // Función despachadora de los eventos deL BLE.
    bs_cfg.sys_event_dispatcher  = BlueStack_SYS_EventDispatcher;    // Función despachadora de los eventos del SISTEMA.
    bs_cfg.ble_event_handler     = BlueStack_BLE_Event;                  // Manejador de eventos del BLE.
    bs_cfg.sys_event_handler     = BlueStack_SYS_Event;                  // Manejador de eventos del SISTEMA.
    bs_cfg.conn_event_handler    = BlueStack_CONN_Event;                 // Manejador de eventos de conexión.
    bs_cfg.errconn_event_handler = BlueStack_ERRCONN_Event;          // Manejador de eventos de errores de conexión.
    bs_cfg.adv_event_handler     = My_ADV_Event;                  // Manejador de eventos del advertising.
    bs_cfg.peer_event_handler    = BlueStack_PEER_Event;                 // Manejador de eventos de emparejamiento de dispositivos.

    /* Inicializa el Stack BLE */
    err_code = BlueStack_Init(&bs_cfg);
    APP_ERROR_CHECK(err_code);

}

/**@brief Function for initializing the system.
 *
 * @details Call all the necesary functions to initialize all the
 *          system modules.
 */
void Init_System() 
{

    // Inicializa botones y leds
    buttons_leds_init();

    // Inicialización del módulo Bluetooth
    ble_init();

    //Inicializa los servicios BLE
    services_init();

    // Inicializa los timers de aplicación
    timers_init();

    /* Inicia los timers de aplicación */
    application_timers_start();


    
}

/**@brief Function for application main entry.
 */
int main(void)
{
    uint8_t  bps;
    
    // Inicialización del sistema
    Init_System();

    /* Inicia visibilidad (publicidad) */
    err_code = BlueStack_AdvertisingStart();
    APP_ERROR_CHECK(err_code);

    // Enter main loop.
    while(1)
    {
            BlueStack_AdvertisingUpdate(0xbaba, &bps, 1);
            BlueStack_PowerManage();
    }
}

