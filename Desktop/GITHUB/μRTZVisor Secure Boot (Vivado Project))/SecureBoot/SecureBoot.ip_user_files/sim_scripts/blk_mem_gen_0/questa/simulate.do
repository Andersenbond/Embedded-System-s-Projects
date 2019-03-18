onbreak {quit -f}
onerror {quit -f}

vsim -t 1ps -pli "/home/jltm/Vivado/2016.4/lib/lnx64.o/libxil_vsim.so" -lib xil_defaultlib blk_mem_gen_0_opt

do {wave.do}

view wave
view structure
view signals

do {blk_mem_gen_0.udo}

run -all

quit -force
