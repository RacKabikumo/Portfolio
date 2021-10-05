`timescale 1ns/1ns
module WB( clk, rst, alu_out, MemtoReg, dmem_rdata, rfile_wd ) ;
	input clk, rst ;
	input [31:0] alu_out, dmem_rdata ;
	input MemtoReg ;
	output [31:0] rfile_wd ;
	
	wire [31:0] rfile_wd_temp ;

	mux2 #(32) WRMUX( .sel(MemtoReg), .a(alu_out), .b(dmem_rdata), .y(rfile_wd) );
 
	 
	
endmodule
		