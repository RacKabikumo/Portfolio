`timescale 1ns/1ns
module EX_MEM( clk, rst, pc, ALUout, RD2, wn, zero, Branch, MemtoReg, MemRead, MemWrite, Jump, RegWrite,
				zero_out, Branch_out, MemtoReg_out, MemRead_out, MemWrite_out, Jump_out, RegWrite_out, pc_add4, pc_add4_out,
				pc_out, addr, wd, wn_out, jump_addr, jump_addr_out, jumpR_addr, jumpR_addr_out, JR, JR_out ) ;
	input clk, rst, zero ;
	input Branch, MemtoReg, MemRead, MemWrite, Jump, RegWrite, JR ;
	input [31:0] pc, ALUout, RD2, jump_addr, jumpR_addr, pc_add4 ;
	input [4:0] wn ;
	
	output zero_out, Branch_out, MemtoReg_out, MemRead_out, MemWrite_out, Jump_out, RegWrite_out, JR_out, pc_add4_out ;
	output [31:0] pc_out, addr, wd, jump_addr_out, jumpR_addr_out ;
	output [4:0] wn_out ;
	
	reg [4:0] wn_out ;
	reg [5:0] count ;
	reg zero_out, Branch_out, MemtoReg_out, MemRead_out, MemWrite_out, Jump_out, RegWrite_out, JR_out ;
	reg [31:0] pc_out, addr, wd, jump_addr_out, jumpR_addr_out, pc_add4_out ;

	
	always@( posedge clk ) begin
		if ( rst ) begin
			count <= 6'b0 ;
			wn_out <= 5'b0 ;
			zero_out <= 1'b0 ;
			Branch_out <= 1'b0 ;
			MemtoReg_out <= 1'b0 ;
			MemRead_out <= 1'b0 ;
			MemWrite_out <= 1'b0 ;
			Jump_out <= 1'b0 ;
			RegWrite_out <= 1'b0 ;
			pc_out <= 32'b0 ;
			addr <= 32'b0 ;
			wd <= 32'b0 ;
			jump_addr_out <= 32'b0 ;
			jumpR_addr_out <= 32'b0 ;
			JR_out <= 1'b0 ;
			pc_add4_out <= 32'b0 ;
		end
		else begin
			wn_out <= wn ;
			zero_out <= zero ;
			Branch_out <= Branch ;
			MemtoReg_out <= MemtoReg ;
			MemRead_out <= MemRead ;
			MemWrite_out <= MemWrite ;
			Jump_out <= Jump ;
			RegWrite_out <= RegWrite ;
			pc_out <= pc ;
			addr <= ALUout ;
			wd <= RD2 ;
			jump_addr_out <= jump_addr ;
			jumpR_addr_out <= jumpR_addr ;
			JR_out <= JR ;
			pc_add4_out <= pc_add4 ;
		end
	end
	
endmodule
		