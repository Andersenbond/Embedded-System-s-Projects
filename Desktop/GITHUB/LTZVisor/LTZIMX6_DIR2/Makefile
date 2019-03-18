HYPERVISOR_PRODUCT = LTZVisor
HYPERVISOR_MAJOR_VERSION = 0
HYPERVISOR_MINOR_VERSION = 2
HYPERVISOR_REVISION_VERSION = 1

# Suported S_GUEST
#		- BARE_METAL
#		- FREERTOS
#
# Suported NS_GUEST
#		- BARE_METAL
#		- FREERTOS
#	  - LINUX



export BOARD:= IMX6QSD
export S_GUEST:= BARE_METAL
export NS_GUEST:= LINUX
export CROSS_COMPILE:= arm-none-eabi-
export APP_CORE:= SingleCore
#export APP_CORE:= Multicore

export SDK_PATH = $(CURDIR)

all:
	echo 'LTZVisor builds start'
	$(MAKE) -C $(SDK_PATH)/src

clean:
	$(MAKE) -C $(SDK_PATH)/src clean

	sudo rm -f bin/*.bin bin/*.elf
	sudo rm -f $(VERSION_FILE)
