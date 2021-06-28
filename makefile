#
#             LUFA Library
#     Copyright (C) Dean Camera, 2014.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU          = atmega32u4
ARCH         = AVR8
BOARD        = LEONARDO
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = SilverLinkClone
SRC          = $(TARGET).c Descriptors.c TiLink.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS) $(LUFA_SRC_PLATFORM)
LUFA_PATH    = LUFA-src
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig -DNDEBUG
LD_FLAGS     =
#OBJDIR       = OBJ
AVRDUDE_PROGRAMMER = usbtiny

#REMOVE IF ON LINUX
SHELL=C:/Windows/System32/cmd.exe
# END REMOVE


# Default target
all:

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk