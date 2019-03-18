
`timescale 1 ns / 1 ps

	module Sboot_ROM_v1_0 #
	(
		// Users to add parameters here

		// User parameters ends
		// Do not modify the parameters beyond this line


		// Parameters of Axi Slave Bus Interface SBoot_AXI
		parameter integer C_SBoot_AXI_ID_WIDTH	= 1,
		parameter integer C_SBoot_AXI_DATA_WIDTH	= 32,
		parameter integer C_SBoot_AXI_ADDR_WIDTH	= 8,
		parameter integer C_SBoot_AXI_AWUSER_WIDTH	= 0,
		parameter integer C_SBoot_AXI_ARUSER_WIDTH	= 0,
		parameter integer C_SBoot_AXI_WUSER_WIDTH	= 0,
		parameter integer C_SBoot_AXI_RUSER_WIDTH	= 0,
		parameter integer C_SBoot_AXI_BUSER_WIDTH	= 0
	)
	(
		// Users to add ports here

		// User ports ends
		// Do not modify the ports beyond this line


		// Ports of Axi Slave Bus Interface SBoot_AXI
		input wire  sboot_axi_aclk,
		input wire  sboot_axi_aresetn,
		input wire [C_SBoot_AXI_ID_WIDTH-1 : 0] sboot_axi_awid,
		input wire [C_SBoot_AXI_ADDR_WIDTH-1 : 0] sboot_axi_awaddr,
		input wire [7 : 0] sboot_axi_awlen,
		input wire [2 : 0] sboot_axi_awsize,
		input wire [1 : 0] sboot_axi_awburst,
		input wire  sboot_axi_awlock,
		input wire [3 : 0] sboot_axi_awcache,
		input wire [2 : 0] sboot_axi_awprot,
		input wire [3 : 0] sboot_axi_awqos,
		input wire [3 : 0] sboot_axi_awregion,
		input wire [C_SBoot_AXI_AWUSER_WIDTH-1 : 0] sboot_axi_awuser,
		input wire  sboot_axi_awvalid,
		output wire  sboot_axi_awready,
		input wire [C_SBoot_AXI_DATA_WIDTH-1 : 0] sboot_axi_wdata,
		input wire [(C_SBoot_AXI_DATA_WIDTH/8)-1 : 0] sboot_axi_wstrb,
		input wire  sboot_axi_wlast,
		input wire [C_SBoot_AXI_WUSER_WIDTH-1 : 0] sboot_axi_wuser,
		input wire  sboot_axi_wvalid,
		output wire  sboot_axi_wready,
		output wire [C_SBoot_AXI_ID_WIDTH-1 : 0] sboot_axi_bid,
		output wire [1 : 0] sboot_axi_bresp,
		output wire [C_SBoot_AXI_BUSER_WIDTH-1 : 0] sboot_axi_buser,
		output wire  sboot_axi_bvalid,
		input wire  sboot_axi_bready,
		input wire [C_SBoot_AXI_ID_WIDTH-1 : 0] sboot_axi_arid,
		input wire [C_SBoot_AXI_ADDR_WIDTH-1 : 0] sboot_axi_araddr,
		input wire [7 : 0] sboot_axi_arlen,
		input wire [2 : 0] sboot_axi_arsize,
		input wire [1 : 0] sboot_axi_arburst,
		input wire  sboot_axi_arlock,
		input wire [3 : 0] sboot_axi_arcache,
		input wire [2 : 0] sboot_axi_arprot,
		input wire [3 : 0] sboot_axi_arqos,
		input wire [3 : 0] sboot_axi_arregion,
		input wire [C_SBoot_AXI_ARUSER_WIDTH-1 : 0] sboot_axi_aruser,
		input wire  sboot_axi_arvalid,
		output wire  sboot_axi_arready,
		output wire [C_SBoot_AXI_ID_WIDTH-1 : 0] sboot_axi_rid,
		output wire [C_SBoot_AXI_DATA_WIDTH-1 : 0] sboot_axi_rdata,
		output wire [1 : 0] sboot_axi_rresp,
		output wire  sboot_axi_rlast,
		output wire [C_SBoot_AXI_RUSER_WIDTH-1 : 0] sboot_axi_ruser,
		output wire  sboot_axi_rvalid,
		input wire  sboot_axi_rready
	);
// Instantiation of Axi Bus Interface SBoot_AXI
	Sboot_ROM_v1_0_SBoot_AXI # ( 
		.C_S_AXI_ID_WIDTH(C_SBoot_AXI_ID_WIDTH),
		.C_S_AXI_DATA_WIDTH(C_SBoot_AXI_DATA_WIDTH),
		.C_S_AXI_ADDR_WIDTH(C_SBoot_AXI_ADDR_WIDTH),
		.C_S_AXI_AWUSER_WIDTH(C_SBoot_AXI_AWUSER_WIDTH),
		.C_S_AXI_ARUSER_WIDTH(C_SBoot_AXI_ARUSER_WIDTH),
		.C_S_AXI_WUSER_WIDTH(C_SBoot_AXI_WUSER_WIDTH),
		.C_S_AXI_RUSER_WIDTH(C_SBoot_AXI_RUSER_WIDTH),
		.C_S_AXI_BUSER_WIDTH(C_SBoot_AXI_BUSER_WIDTH)
	) Sboot_ROM_v1_0_SBoot_AXI_inst (
		.S_AXI_ACLK(sboot_axi_aclk),
		.S_AXI_ARESETN(sboot_axi_aresetn),
		.S_AXI_AWID(sboot_axi_awid),
		.S_AXI_AWADDR(sboot_axi_awaddr),
		.S_AXI_AWLEN(sboot_axi_awlen),
		.S_AXI_AWSIZE(sboot_axi_awsize),
		.S_AXI_AWBURST(sboot_axi_awburst),
		.S_AXI_AWLOCK(sboot_axi_awlock),
		.S_AXI_AWCACHE(sboot_axi_awcache),
		.S_AXI_AWPROT(sboot_axi_awprot),
		.S_AXI_AWQOS(sboot_axi_awqos),
		.S_AXI_AWREGION(sboot_axi_awregion),
		.S_AXI_AWUSER(sboot_axi_awuser),
		.S_AXI_AWVALID(sboot_axi_awvalid),
		.S_AXI_AWREADY(sboot_axi_awready),
		.S_AXI_WDATA(sboot_axi_wdata),
		.S_AXI_WSTRB(sboot_axi_wstrb),
		.S_AXI_WLAST(sboot_axi_wlast),
		.S_AXI_WUSER(sboot_axi_wuser),
		.S_AXI_WVALID(sboot_axi_wvalid),
		.S_AXI_WREADY(sboot_axi_wready),
		.S_AXI_BID(sboot_axi_bid),
		.S_AXI_BRESP(sboot_axi_bresp),
		.S_AXI_BUSER(sboot_axi_buser),
		.S_AXI_BVALID(sboot_axi_bvalid),
		.S_AXI_BREADY(sboot_axi_bready),
		.S_AXI_ARID(sboot_axi_arid),
		.S_AXI_ARADDR(sboot_axi_araddr),
		.S_AXI_ARLEN(sboot_axi_arlen),
		.S_AXI_ARSIZE(sboot_axi_arsize),
		.S_AXI_ARBURST(sboot_axi_arburst),
		.S_AXI_ARLOCK(sboot_axi_arlock),
		.S_AXI_ARCACHE(sboot_axi_arcache),
		.S_AXI_ARPROT(sboot_axi_arprot),
		.S_AXI_ARQOS(sboot_axi_arqos),
		.S_AXI_ARREGION(sboot_axi_arregion),
		.S_AXI_ARUSER(sboot_axi_aruser),
		.S_AXI_ARVALID(sboot_axi_arvalid),
		.S_AXI_ARREADY(sboot_axi_arready),
		.S_AXI_RID(sboot_axi_rid),
		.S_AXI_RDATA(sboot_axi_rdata),
		.S_AXI_RRESP(sboot_axi_rresp),
		.S_AXI_RLAST(sboot_axi_rlast),
		.S_AXI_RUSER(sboot_axi_ruser),
		.S_AXI_RVALID(sboot_axi_rvalid),
		.S_AXI_RREADY(sboot_axi_rready)
	);

	// Add user logic here

	// User logic ends

	endmodule
