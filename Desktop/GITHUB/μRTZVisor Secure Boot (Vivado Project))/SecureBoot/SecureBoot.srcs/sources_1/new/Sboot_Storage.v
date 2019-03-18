`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 23.05.2017 11:47:53
// Design Name: 
// Module Name: Sboot_Storage
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Sboot_Storage(
    input clk,
    input i_reset,
    input i_enable,
    input [2:0] i_address,
    output [255:0] o_data
    );
    
    integer i = 0;
    reg rom_address;
    
    blk_mem_gen_0 SBoot_ROM (
    .clka(clk),
    .ena(i_enable),
    .addra(i_address),
    .douta(o_data)
    );
    
    always @(posedge clk)
    begin
        if (i_reset) begin
            rom_address <= i_address;
            for (i = 0; i < rom_address; i=i+1) begin
                
            end
        end
    end
    
    
    
endmodule
