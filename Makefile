PROJECT_NAME     = Blueroomba
DEVICE			 = NRF51822
MODEL			 = XXAC
VERSION			 = 1.0

BLUESTACK_PATH	 = ./BlueStack
NORDIC_SDK_PATH  = /opt/nRF5_SDK_12
PROJECT_PATH	 = .


SOURCE_FILES += $(PROJECT_PATH)/main.c
SOURCE_FILES += $(PROJECT_PATH)/include/myevents.c

INCLUDE_FOLDERS += $(PROJECT_PATH)/include
INCLUDE_FOLDERS += $(PROJECT_PATH)/include/services

include $(BLUESTACK_PATH)/Makefile.bs
