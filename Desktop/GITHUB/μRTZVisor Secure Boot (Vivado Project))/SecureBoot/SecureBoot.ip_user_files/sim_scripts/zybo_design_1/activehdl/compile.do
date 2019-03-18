vlib work
vlib activehdl

vlib activehdl/xil_defaultlib
vlib activehdl/xpm
vlib activehdl/processing_system7_bfm_v2_0_5
vlib activehdl/axi_lite_ipif_v3_0_4
vlib activehdl/lib_cdc_v1_0_2
vlib activehdl/interrupt_control_v3_1_4
vlib activehdl/axi_gpio_v2_0_13
vlib activehdl/blk_mem_gen_v8_3_5
vlib activehdl/axi_bram_ctrl_v4_0_10
vlib activehdl/proc_sys_reset_v5_0_10
vlib activehdl/generic_baseblocks_v2_1_0
vlib activehdl/axi_infrastructure_v1_1_0
vlib activehdl/axi_register_slice_v2_1_11
vlib activehdl/fifo_generator_v13_1_3
vlib activehdl/axi_data_fifo_v2_1_10
vlib activehdl/axi_crossbar_v2_1_12
vlib activehdl/axi_protocol_converter_v2_1_11

vmap xil_defaultlib activehdl/xil_defaultlib
vmap xpm activehdl/xpm
vmap processing_system7_bfm_v2_0_5 activehdl/processing_system7_bfm_v2_0_5
vmap axi_lite_ipif_v3_0_4 activehdl/axi_lite_ipif_v3_0_4
vmap lib_cdc_v1_0_2 activehdl/lib_cdc_v1_0_2
vmap interrupt_control_v3_1_4 activehdl/interrupt_control_v3_1_4
vmap axi_gpio_v2_0_13 activehdl/axi_gpio_v2_0_13
vmap blk_mem_gen_v8_3_5 activehdl/blk_mem_gen_v8_3_5
vmap axi_bram_ctrl_v4_0_10 activehdl/axi_bram_ctrl_v4_0_10
vmap proc_sys_reset_v5_0_10 activehdl/proc_sys_reset_v5_0_10
vmap generic_baseblocks_v2_1_0 activehdl/generic_baseblocks_v2_1_0
vmap axi_infrastructure_v1_1_0 activehdl/axi_infrastructure_v1_1_0
vmap axi_register_slice_v2_1_11 activehdl/axi_register_slice_v2_1_11
vmap fifo_generator_v13_1_3 activehdl/fifo_generator_v13_1_3
vmap axi_data_fifo_v2_1_10 activehdl/axi_data_fifo_v2_1_10
vmap axi_crossbar_v2_1_12 activehdl/axi_crossbar_v2_1_12
vmap axi_protocol_converter_v2_1_11 activehdl/axi_protocol_converter_v2_1_11

vlog -work xil_defaultlib  -sv2k12 "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" \
"/home/jltm/Vivado/2016.4/data/ip/xpm/xpm_cdc/hdl/xpm_cdc.sv" \
"/home/jltm/Vivado/2016.4/data/ip/xpm/xpm_memory/hdl/xpm_memory.sv" \

vcom -work xpm -93 \
"/home/jltm/Vivado/2016.4/data/ip/xpm/xpm_VCOMP.vhd" \

vlog -work processing_system7_bfm_v2_0_5  -v2k5 "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl/processing_system7_bfm_v2_0_vl_rfs.v" \

vlog -work xil_defaultlib  -v2k5 "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" \
"../../../bd/zybo_design_1/ip/zybo_design_1_processing_system7_0_0/sim/zybo_design_1_processing_system7_0_0.v" \
"../../../bd/zybo_design_1/hdl/zybo_design_1.v" \

vcom -work axi_lite_ipif_v3_0_4 -93 \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/0ba0/hdl/axi_lite_ipif_v3_0_vh_rfs.vhd" \

vcom -work lib_cdc_v1_0_2 -93 \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/52cb/hdl/lib_cdc_v1_0_rfs.vhd" \

vcom -work interrupt_control_v3_1_4 -93 \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/e956/hdl/interrupt_control_v3_1_vh_rfs.vhd" \

vcom -work axi_gpio_v2_0_13 -93 \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/4f16/hdl/axi_gpio_v2_0_vh_rfs.vhd" \

vcom -work xil_defaultlib -93 \
"../../../bd/zybo_design_1/ip/zybo_design_1_axi_gpio_0_0/sim/zybo_design_1_axi_gpio_0_0.vhd" \

vlog -work blk_mem_gen_v8_3_5  -v2k5 "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/6273/simulation/blk_mem_gen_v8_3.v" \

vcom -work axi_bram_ctrl_v4_0_10 -93 \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/91f5/hdl/axi_bram_ctrl_v4_0_rfs.vhd" \

vcom -work xil_defaultlib -93 \
"../../../bd/zybo_design_1/ip/zybo_design_1_axi_bram_ctrl_0_1/sim/zybo_design_1_axi_bram_ctrl_0_1.vhd" \

vlog -work xil_defaultlib  -v2k5 "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" \
"../../../bd/zybo_design_1/ip/zybo_design_1_blk_mem_gen_0_0/sim/zybo_design_1_blk_mem_gen_0_0.v" \

vcom -work proc_sys_reset_v5_0_10 -93 \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/04b4/hdl/proc_sys_reset_v5_0_vh_rfs.vhd" \

vcom -work xil_defaultlib -93 \
"../../../bd/zybo_design_1/ip/zybo_design_1_rst_ps7_0_100M_0/sim/zybo_design_1_rst_ps7_0_100M_0.vhd" \

vlog -work generic_baseblocks_v2_1_0  -v2k5 "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7ee0/hdl/generic_baseblocks_v2_1_vl_rfs.v" \

vlog -work axi_infrastructure_v1_1_0  -v2k5 "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl/axi_infrastructure_v1_1_vl_rfs.v" \

vlog -work axi_register_slice_v2_1_11  -v2k5 "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/0b6b/hdl/axi_register_slice_v2_1_vl_rfs.v" \

vlog -work fifo_generator_v13_1_3  -v2k5 "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/564d/simulation/fifo_generator_vlog_beh.v" \

vcom -work fifo_generator_v13_1_3 -93 \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/564d/hdl/fifo_generator_v13_1_rfs.vhd" \

vlog -work fifo_generator_v13_1_3  -v2k5 "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/564d/hdl/fifo_generator_v13_1_rfs.v" \

vlog -work axi_data_fifo_v2_1_10  -v2k5 "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/39ba/hdl/axi_data_fifo_v2_1_vl_rfs.v" \

vlog -work axi_crossbar_v2_1_12  -v2k5 "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/896d/hdl/axi_crossbar_v2_1_vl_rfs.v" \

vlog -work xil_defaultlib  -v2k5 "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" \
"../../../bd/zybo_design_1/ip/zybo_design_1_xbar_0/sim/zybo_design_1_xbar_0.v" \
"../../../bd/zybo_design_1/ipshared/d0c2/hdl/Sboot_ROM_v1_0_SBoot_AXI.v" \
"../../../bd/zybo_design_1/ipshared/d0c2/hdl/Sboot_ROM_v1_0.v" \
"../../../bd/zybo_design_1/ip/zybo_design_1_Sboot_ROM_0_0/sim/zybo_design_1_Sboot_ROM_0_0.v" \

vlog -work axi_protocol_converter_v2_1_11  -v2k5 "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" \
"../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/df1b/hdl/axi_protocol_converter_v2_1_vl_rfs.v" \

vlog -work xil_defaultlib  -v2k5 "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7dd0/hdl" "+incdir+../../../../SecureBoot.srcs/sources_1/bd/zybo_design_1/ipshared/7e3a/hdl" \
"../../../bd/zybo_design_1/ip/zybo_design_1_auto_pc_0/sim/zybo_design_1_auto_pc_0.v" \
"../../../bd/zybo_design_1/ip/zybo_design_1_auto_pc_1/sim/zybo_design_1_auto_pc_1.v" \

vlog -work xil_defaultlib "glbl.v"

