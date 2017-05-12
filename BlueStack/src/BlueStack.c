#include "BlueStack.h"
#include "bluestack_events.h"


BlueStack_BLE_Handler  user_ble_handler;


uint32_t BlueStack_Init(BlueStack_cfg_t* bs_cfg) {


    // Initialize timer module.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
    // Inicializa el softdevice de nordic con los manejadores
	ble_stack_init(bs_cfg->ble_event_dispatcher, bs_cfg->sys_event_dispatcher);
    peer_manager_init(bs_cfg->erase_bonds, bs_cfg->peer_event_handler);
	gap_params_init(bs_cfg->device_name, bs_cfg->device_appearance);
    bluestack_adv_init(&(bs_cfg->adv_uuid), bs_cfg->adv_interval, bs_cfg->adv_timeout, bs_cfg->adv_event_handler);
    //services_init();
    conn_params_init(bs_cfg->conn_event_handler, bs_cfg->errconn_event_handler);

    user_ble_handler = bs_cfg->ble_event_handler;
	return 0;
}

uint32_t BlueStack_AdvertisingStop() {
    return sd_ble_gap_adv_stop();   
}
uint32_t BlueStack_AdvertisingStart() {
	return ble_advertising_start(BLE_ADV_MODE_FAST);	
}

uint32_t BlueStack_PowerManage(){
	return sd_app_evt_wait();
}

uint32_t BlueStack_AdvertisingUpdate(uint16_t UUID, uint8_t* p_data, uint8_t size)
{
    uint32_t      err_code;
    ble_advdata_t advdata;
    //ble_advdata_manuf_data_t manuf_data; // Variable para guardar los datos de manufactura
    uint8_array_t data;                      // Arreglo de datos a enviar
    ble_advdata_service_data_t adv_data;     // Arreglo de datos de advertising

    data.p_data     = p_data;       // Llena el primer arreglo datos a enviar.
    data.size       = size;         // Tamaño de arreglo de datos.

    adv_data.service_uuid = UUID;         // UUID del servicio para el primer arreglo de datos
    adv_data.data=data;                   // Datos

    // Máximo 31 bytes entre el nombre del dispositivo y los datos
    // configurados en esta función.
    // 
    // Inicializa los datos de manofactura
    /*
    uint8_t man_data[] = MANUFACTURER_NAME; // Datos de manufactura.
    manuf_data.company_identifier   = MANUFACTURER_ID;                       // Nordics company ID
    manuf_data.data.p_data          = man_data;     
    manuf_data.data.size            = sizeof(man_data); 
    */

    // Inicia la estructura de datos de Advertising que se pasará a @ref ble_advertising_init.
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type               = BLE_ADVDATA_FULL_NAME;        // Indica como muestra el nombre del dispositivo.
    //advdata.short_name_len          = 6;                            // Tamaño para el nombre corto del advertising.
    advdata.include_appearance      = false;                        // Muestra la apariencia (2 bytes ḿas al paquete)                         // Indica si incluye la apariencia
    advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    //advdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    //advdata.uuids_complete.p_uuids  = m_adv_uuids;
    //advdata.include_ble_device_addr = true;
    //advdata.p_manuf_specific_data   = &manuf_data;                  // Datos de manufactura.
    advdata.p_service_data_array    = &adv_data;
    advdata.service_data_count      = 1;                            // Numero de estructuras en el arreglo de datos. 

    err_code = ble_advdata_set(&advdata, NULL);
    return err_code;
}

/**@brief Function for initializing the Advertising functionality.
 */
uint32_t bluestack_adv_init(ble_uuid_t* adv_uuid, uint32_t adv_interval, uint32_t adv_timeout, BlueStack_ADV_Handler adv_evt)
{
    uint32_t               err_code;
    ble_advdata_t          advdata;
    ble_adv_modes_config_t options;

    // Build advertising data struct to pass into @ref ble_advertising_init.
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance      = true;
    advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    advdata.uuids_complete.uuid_cnt = 1; //sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    advdata.uuids_complete.p_uuids  = adv_uuid;

    memset(&options, 0, sizeof(options));
    options.ble_adv_fast_enabled  = true;
    options.ble_adv_fast_interval = adv_interval;
    options.ble_adv_fast_timeout  = adv_timeout;

    err_code = ble_advertising_init(&advdata, NULL, &options, adv_evt, NULL);
    return err_code;
}


/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
uint32_t ble_stack_init(BlueStack_BLE_Dispatcher ble_evt_dispatch, BlueStack_SYS_Dispatcher sys_evt_dispatch)
{
    uint32_t err_code;

    nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);

    ble_enable_params_t ble_enable_params;
    err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                    PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);
    
    CHECK_ERROR(err_code);

    // Check the ram settings against the used number of links
    CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT, PERIPHERAL_LINK_COUNT);

    // Enable BLE stack.
#if (NRF_SD_BLE_API_VERSION == 3)
    ble_enable_params.gatt_enable_params.att_mtu = NRF_BLE_MAX_MTU_SIZE;
#endif
    err_code = softdevice_enable(&ble_enable_params);
    CHECK_ERROR(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    CHECK_ERROR(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    return err_code;
}


/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
uint32_t gap_params_init(char* device_name, uint8_t ble_appearance)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)device_name,
                                          strlen(device_name));
    
    CHECK_ERROR(err_code);

    if(ble_appearance)
    {
        err_code = sd_ble_gap_appearance_set(ble_appearance);
        CHECK_ERROR(err_code);
    }

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    return err_code;
}

/**@brief Function for initializing the Connection Parameters module.
 */
uint32_t conn_params_init(BlueStack_CONN_Handler conn_evt_handler, BlueStack_ERRCONN_Handler err_conn_handler)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = conn_evt_handler;
    cp_init.error_handler                  = err_conn_handler;

    err_code = ble_conn_params_init(&cp_init);
    return err_code;
}

/**@brief Function for the Peer Manager initialization.
 *
 * @param[in] erase_bonds  Indicates whether bonding information should be cleared from
 *                         persistent storage during initialization of the Peer Manager.
 */
uint32_t peer_manager_init(bool erase_bonds, BlueStack_PEER_Handler pm_evt_handler )
{
    ble_gap_sec_params_t sec_param;
    ret_code_t           err_code;

    err_code = pm_init();
    CHECK_ERROR(err_code);

    if (erase_bonds)
    {
        err_code = pm_peers_delete();
        CHECK_ERROR(err_code);
    }

    memset(&sec_param, 0, sizeof(ble_gap_sec_params_t));

    // Security parameters to be used for all security procedures.
    sec_param.bond           = SEC_PARAM_BOND;
    sec_param.mitm           = SEC_PARAM_MITM;
    sec_param.lesc           = SEC_PARAM_LESC;
    sec_param.keypress       = SEC_PARAM_KEYPRESS;
    sec_param.io_caps        = SEC_PARAM_IO_CAPABILITIES;
    sec_param.oob            = SEC_PARAM_OOB;
    sec_param.min_key_size   = SEC_PARAM_MIN_KEY_SIZE;
    sec_param.max_key_size   = SEC_PARAM_MAX_KEY_SIZE;
    sec_param.kdist_own.enc  = 1;
    sec_param.kdist_own.id   = 1;
    sec_param.kdist_peer.enc = 1;
    sec_param.kdist_peer.id  = 1;

    err_code = pm_sec_params_set(&sec_param);
    CHECK_ERROR(err_code);

    err_code = pm_register(pm_evt_handler);
    return err_code;
}

