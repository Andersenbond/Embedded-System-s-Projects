// (c) Copyright 1995-2017 Xilinx, Inc. All rights reserved.
// 
// This file contains confidential and proprietary information
// of Xilinx, Inc. and is protected under U.S. and
// international copyright and other intellectual property
// laws.
// 
// DISCLAIMER
// This disclaimer is not a license and does not grant any
// rights to the materials distributed herewith. Except as
// otherwise provided in a valid license issued to you by
// Xilinx, and to the maximum extent permitted by applicable
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
// WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
// (2) Xilinx shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of
// liability) for any loss or damage of any kind or nature
// related to, arising under or in connection with these
// materials, including for any direct, or any indirect,
// special, incidental, or consequential loss or damage
// (including loss of data, profits, goodwill, or any type of
// loss or damage suffered as a result of any action brought
// by a third party) even if such damage or loss was
// reasonably foreseeable or Xilinx had been advised of the
// possibility of the same.
// 
// CRITICAL APPLICATIONS
// Xilinx products are not designed or intended to be fail-
// safe, or for use in any application requiring fail-safe
// performance, such as life-support or safety devices or
// systems, Class III medical devices, nuclear facilities,
// applications related to the deployment of airbags, or any
// other applications that could lead to death, personal
// injury, or severe property or environmental damage
// (individually and collectively, "Critical
// Applications"). Customer assumes the sole risk and
// liability of any use of Xilinx products in Critical
// Applications, subject only to applicable laws and
// regulations governing limitations on product liability.
// 
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES.
// 
// DO NOT MODIFY THIS FILE.


// IP VLNV: user.org:user:Sboot_ROM:1.0
// IP Revision: 2

(* X_CORE_INFO = "Sboot_ROM_v1_0,Vivado 2016.4" *)
(* CHECK_LICENSE_TYPE = "zybo_design_1_Sboot_ROM_0_0,Sboot_ROM_v1_0,{}" *)
(* DowngradeIPIdentifiedWarnings = "yes" *)
module zybo_design_1_Sboot_ROM_0_0 (
  sboot_axi_awid,
  sboot_axi_awaddr,
  sboot_axi_awlen,
  sboot_axi_awsize,
  sboot_axi_awburst,
  sboot_axi_awlock,
  sboot_axi_awcache,
  sboot_axi_awprot,
  sboot_axi_awregion,
  sboot_axi_awqos,
  sboot_axi_awvalid,
  sboot_axi_awready,
  sboot_axi_wdata,
  sboot_axi_wstrb,
  sboot_axi_wlast,
  sboot_axi_wvalid,
  sboot_axi_wready,
  sboot_axi_bid,
  sboot_axi_bresp,
  sboot_axi_bvalid,
  sboot_axi_bready,
  sboot_axi_arid,
  sboot_axi_araddr,
  sboot_axi_arlen,
  sboot_axi_arsize,
  sboot_axi_arburst,
  sboot_axi_arlock,
  sboot_axi_arcache,
  sboot_axi_arprot,
  sboot_axi_arregion,
  sboot_axi_arqos,
  sboot_axi_arvalid,
  sboot_axi_arready,
  sboot_axi_rid,
  sboot_axi_rdata,
  sboot_axi_rresp,
  sboot_axi_rlast,
  sboot_axi_rvalid,
  sboot_axi_rready,
  sboot_axi_aclk,
  sboot_axi_aresetn
);

(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI AWID" *)
input wire [11 : 0] sboot_axi_awid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI AWADDR" *)
input wire [7 : 0] sboot_axi_awaddr;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI AWLEN" *)
input wire [7 : 0] sboot_axi_awlen;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI AWSIZE" *)
input wire [2 : 0] sboot_axi_awsize;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI AWBURST" *)
input wire [1 : 0] sboot_axi_awburst;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI AWLOCK" *)
input wire sboot_axi_awlock;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI AWCACHE" *)
input wire [3 : 0] sboot_axi_awcache;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI AWPROT" *)
input wire [2 : 0] sboot_axi_awprot;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI AWREGION" *)
input wire [3 : 0] sboot_axi_awregion;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI AWQOS" *)
input wire [3 : 0] sboot_axi_awqos;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI AWVALID" *)
input wire sboot_axi_awvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI AWREADY" *)
output wire sboot_axi_awready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI WDATA" *)
input wire [31 : 0] sboot_axi_wdata;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI WSTRB" *)
input wire [3 : 0] sboot_axi_wstrb;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI WLAST" *)
input wire sboot_axi_wlast;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI WVALID" *)
input wire sboot_axi_wvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI WREADY" *)
output wire sboot_axi_wready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI BID" *)
output wire [11 : 0] sboot_axi_bid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI BRESP" *)
output wire [1 : 0] sboot_axi_bresp;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI BVALID" *)
output wire sboot_axi_bvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI BREADY" *)
input wire sboot_axi_bready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI ARID" *)
input wire [11 : 0] sboot_axi_arid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI ARADDR" *)
input wire [7 : 0] sboot_axi_araddr;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI ARLEN" *)
input wire [7 : 0] sboot_axi_arlen;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI ARSIZE" *)
input wire [2 : 0] sboot_axi_arsize;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI ARBURST" *)
input wire [1 : 0] sboot_axi_arburst;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI ARLOCK" *)
input wire sboot_axi_arlock;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI ARCACHE" *)
input wire [3 : 0] sboot_axi_arcache;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI ARPROT" *)
input wire [2 : 0] sboot_axi_arprot;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI ARREGION" *)
input wire [3 : 0] sboot_axi_arregion;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI ARQOS" *)
input wire [3 : 0] sboot_axi_arqos;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI ARVALID" *)
input wire sboot_axi_arvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI ARREADY" *)
output wire sboot_axi_arready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI RID" *)
output wire [11 : 0] sboot_axi_rid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI RDATA" *)
output wire [31 : 0] sboot_axi_rdata;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI RRESP" *)
output wire [1 : 0] sboot_axi_rresp;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI RLAST" *)
output wire sboot_axi_rlast;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI RVALID" *)
output wire sboot_axi_rvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 SBoot_AXI RREADY" *)
input wire sboot_axi_rready;
(* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 SBoot_AXI_CLK CLK" *)
input wire sboot_axi_aclk;
(* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 SBoot_AXI_RST RST" *)
input wire sboot_axi_aresetn;

  Sboot_ROM_v1_0 #(
    .C_SBoot_AXI_ID_WIDTH(12),  // Width of ID for for write address, write data, read address and read data
    .C_SBoot_AXI_DATA_WIDTH(32),  // Width of S_AXI data bus
    .C_SBoot_AXI_ADDR_WIDTH(8),  // Width of S_AXI address bus
    .C_SBoot_AXI_AWUSER_WIDTH(0),  // Width of optional user defined signal in write address channel
    .C_SBoot_AXI_ARUSER_WIDTH(0),  // Width of optional user defined signal in read address channel
    .C_SBoot_AXI_WUSER_WIDTH(0),  // Width of optional user defined signal in write data channel
    .C_SBoot_AXI_RUSER_WIDTH(0),  // Width of optional user defined signal in read data channel
    .C_SBoot_AXI_BUSER_WIDTH(0)  // Width of optional user defined signal in write response channel
  ) inst (
    .sboot_axi_awid(sboot_axi_awid),
    .sboot_axi_awaddr(sboot_axi_awaddr),
    .sboot_axi_awlen(sboot_axi_awlen),
    .sboot_axi_awsize(sboot_axi_awsize),
    .sboot_axi_awburst(sboot_axi_awburst),
    .sboot_axi_awlock(sboot_axi_awlock),
    .sboot_axi_awcache(sboot_axi_awcache),
    .sboot_axi_awprot(sboot_axi_awprot),
    .sboot_axi_awregion(sboot_axi_awregion),
    .sboot_axi_awqos(sboot_axi_awqos),
    .sboot_axi_awuser(1'B0),
    .sboot_axi_awvalid(sboot_axi_awvalid),
    .sboot_axi_awready(sboot_axi_awready),
    .sboot_axi_wdata(sboot_axi_wdata),
    .sboot_axi_wstrb(sboot_axi_wstrb),
    .sboot_axi_wlast(sboot_axi_wlast),
    .sboot_axi_wuser(1'B0),
    .sboot_axi_wvalid(sboot_axi_wvalid),
    .sboot_axi_wready(sboot_axi_wready),
    .sboot_axi_bid(sboot_axi_bid),
    .sboot_axi_bresp(sboot_axi_bresp),
    .sboot_axi_buser(),
    .sboot_axi_bvalid(sboot_axi_bvalid),
    .sboot_axi_bready(sboot_axi_bready),
    .sboot_axi_arid(sboot_axi_arid),
    .sboot_axi_araddr(sboot_axi_araddr),
    .sboot_axi_arlen(sboot_axi_arlen),
    .sboot_axi_arsize(sboot_axi_arsize),
    .sboot_axi_arburst(sboot_axi_arburst),
    .sboot_axi_arlock(sboot_axi_arlock),
    .sboot_axi_arcache(sboot_axi_arcache),
    .sboot_axi_arprot(sboot_axi_arprot),
    .sboot_axi_arregion(sboot_axi_arregion),
    .sboot_axi_arqos(sboot_axi_arqos),
    .sboot_axi_aruser(1'B0),
    .sboot_axi_arvalid(sboot_axi_arvalid),
    .sboot_axi_arready(sboot_axi_arready),
    .sboot_axi_rid(sboot_axi_rid),
    .sboot_axi_rdata(sboot_axi_rdata),
    .sboot_axi_rresp(sboot_axi_rresp),
    .sboot_axi_rlast(sboot_axi_rlast),
    .sboot_axi_ruser(),
    .sboot_axi_rvalid(sboot_axi_rvalid),
    .sboot_axi_rready(sboot_axi_rready),
    .sboot_axi_aclk(sboot_axi_aclk),
    .sboot_axi_aresetn(sboot_axi_aresetn)
  );
endmodule
