

proc generate {drv_handle} {
	xdefine_include_file $drv_handle "xparameters.h" "Sboot_ROM" "NUM_INSTANCES" "DEVICE_ID"  "C_SBoot_AXI_BASEADDR" "C_SBoot_AXI_HIGHADDR"
}
