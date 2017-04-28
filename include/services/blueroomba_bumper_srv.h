#ifndef _BLUEROOMBA_BUMPER_SRV_
#define _BLUEROOMBA_BUMPER_SRV_

#include "BlueStack.h"


//////////////////////////////////////////////////////////////////////////////////////
// UUID Base del servicio.
#define BLUEROOMBA_UUID_BASE          {0x23, 0xD4, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}

// UUID del servicio
#define BLUEROOMBA_UUID_SERVICE       0x000A

// UUIDE de las características que incluye el servicio.
#define BLUEROOMBA_UUID_BUMPLEFT_CHAR    0x0010
#define BLUEROOMBA_UUID_BUMPRIGHT_CHAR     0x0011

#define CHAR_BUMPERLEFT {0, BLUESTACK_CHAR_TYPE_RD, BLUEROOMBA_UUID_BUMPLEFT_CHAR,1}
#define CHAR_BUMPERRIGHT  {0, BLUESTACK_CHAR_TYPE_RD, BLUEROOMBA_UUID_BUMPRIGHT_CHAR, 1}



uint32_t BlueRoombaSrv_Init();

#endif // _BLUEROOMBA_BUMPER_SRV_


