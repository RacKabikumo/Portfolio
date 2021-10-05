`timescale 1ns/1ns
module EX( clk, rst,ALUSrc, Branch, MemtoReg, Jump, ALUOp, RegWrite, Hi_Lo,
			shamt, rfile_rd1, extend_immed, pc_incr, jumpR_addr, DIV, Hi, Lo,
			rfile_rd2, b_tgt, alu_out, zero ) ;
	input clk, rst ;
	input ALUSrc, DIV ;
	input Branch, MemtoReg, Jump, RegWrite, Hi, Lo ;
	input [1:0] ALUOp ;
	input [4:0] shamt ;
	input [31:0] rfile_rd1, extend_immed, pc_incr ;
	
	input [31:0] rfile_rd2 ;
	
	output [31:0] b_tgt, alu_out, jumpR_addr ;
	output zero, Hi_Lo ;
	wire [2:0] Operation ;
	wire [31:0] alu_b, b_offset, Hiout, Loout, ALUout ;
	wire [63:0] DIVUout ;
	wire [5:0] funct ;
	wire Hi_Lo ;

	

	assign funct = extend_immed[5:0] ;
	assign b_offset = extend_immed << 2;
	assign jumpR_addr = rfile_rd1 ;
	add32 BRADD( .a(pc_incr), .b(b_offset), .result(b_tgt) );
	alu_ctl ALUCTL( .ALUOp(ALUOp), .Funct(funct), .ALUOperation(Operation) );
	ALU ALU( .Signal(Operation), .dataA(rfile_rd1), .dataB(alu_b), .dataOut(ALUout), .zero(zero), .shamt(shamt) );
	Divider Divid( .clk(clk), .dataA(rfile_rd1), .dataB(alu_b), .Signal(DIV), .dataOut(DIVUout), .reset(rst), .stop(Hi_Lo) );
	HiLo HiloReg( .clk(clk), .DivAns(DIVUout), .HiOut(Hiout), .LoOut(Loout), .reset(rst), .Signal(Hi_Lo), .Hi(Hi), .Lo(Lo) );
	assign alu_out = ( Hi ) ? Hiout : ( Lo ) ? Loout : ALUout ;
	mux2 #(32) ALUMUX( .sel(ALUSrc), .a(rfile_rd2), .b(extend_immed), .y(alu_b) );
	
	
	
endmodule
		