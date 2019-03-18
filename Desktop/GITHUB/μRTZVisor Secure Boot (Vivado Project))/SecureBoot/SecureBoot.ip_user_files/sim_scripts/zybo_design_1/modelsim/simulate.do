onbreak {quit -f}
onerror {quit -f}

vsim -voptargs="+acc" -t 1ps -pli "/home/jltm/Vivado/2016.4/lib/lnx64.o/libxil_vsim.so" -L xil_defaultlib -L xpm -L processing_system7_bfm_v2_0_5 -L axi_lite_ipif_v3_0_4 -L lib_cdc_v1_0_2 -L interrupt_control_v3_1_4 -L axi_gpio_v2_0_13 -L blk_mem_gen_v8_3_5 -L axi_bram_ctrl_v4_0_10 -L proc_sys_reset_v5_0_10 -L generic_baseblocks_v2_1_0 -L axi_infrastructure_v1_1_0 -L axi_register_slice_v2_1_11 -L fifo_generator_v13_1_3 -L axi_data_fifo_v2_1_10 -L axi_crossbar_v2_1_12 -L axi_protocol_converter_v2_1_11 -L unisims_ver -L unimacro_ver -L secureip -lib xil_defaultlib xil_defaultlib.zybo_design_1 xil_defaultlib.glbl

do {wave.do}

view wave
view structure
view signals

do {zybo_design_1.udo}

run -all

quit -force
