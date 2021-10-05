`timescale 1ns/1ns
module MEM_WB( clk, rst, ALUout, RD, wn, MemtoReg, RegWrite, MemtoReg_out, RegWrite_out, wn_out, ALUout_out, RD_out ) ;
	input clk, rst ;
	input MemtoReg, RegWrite ;
	input [31:0] RD, ALUout ;
	input [4:0] wn ;
	
	output MemtoReg_out, RegWrite_out ;
	output [31:0] RD_out, ALUout_out ;
	output [4:0] wn_out ;
	
	reg [4:0] wn_out ;
	reg MemtoReg_out, RegWrite_out ;
	reg [31:0] RD_out, ALUout_out ;

	
	always@( posedge clk ) begin
		if ( rst ) begin
			wn_out <= 5'b0 ;
			MemtoReg_out <= 1'b0 ;
			RegWrite_out <= 1'b0 ;
			ALUout_out <= 32'b0 ;
			RD_out <= 32'b0 ;
		end
		else begin
			wn_out <= wn ;
			MemtoReg_out <= MemtoReg ;
			RegWrite_out <= RegWrite ;
			ALUout_out <= ALUout ;
			RD_out <= RD ;
		end
	end
	
endmodule
		