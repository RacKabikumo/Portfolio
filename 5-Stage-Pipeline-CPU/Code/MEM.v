`timescale 1ns/1ns
module MEM( clk, rst, MemRead, MemWrite, zero, Branch, rfile_rd2, ALUout, dmem_rdata, 
			PCSrc, MemtoReg, wn ) ;
	input clk, rst ;
	input MemRead, MemWrite, zero, Branch ;
	input MemtoReg ;
	input [4:0] wn ;
	input [31:0] rfile_rd2, ALUout ;

	output [31:0] dmem_rdata ;
	output PCSrc ;

	assign PCSrc = Branch & zero ;

	memory DatMem( .clk(clk), .MemRead(MemRead), .MemWrite(MemWrite), .wd(rfile_rd2), 
				   .addr(ALUout), .rd(dmem_rdata) );	
	
	
	
endmodule
		