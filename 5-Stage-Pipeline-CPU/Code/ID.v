`timescale 1ns/1ns
module ID( clk, rst, pc, rfile_wn, instr, rfile_wd, rfile_rd1, rfile_rd2, extend_immed, shamt, jump_addr,
			ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, Jump, ALUOp, RegWrite_out, rfile_wn_out ) ;
	input clk, rst ;
	input RegWrite ;
	input [4:0] rfile_wn ;
	input [31:0] rfile_wd, instr ;
	output [31:0] extend_immed, rfile_rd1, rfile_rd2, jump_addr, pc ; 
	output [4:0] shamt, rfile_wn_out ;
	output Branch, MemtoReg, MemRead, MemWrite, ALUSrc, Jump, RegWrite_out;
	output [1:0] ALUOp ;
	wire [5:0] opcode, funct;
    wire [4:0] rs, rt, rd, shamt;
    wire [15:0] immed;
    wire [31:0] extend_immed ;
    wire [25:0] jumpoffset;
	wire RegDst ;
	
    assign opcode = instr[31:26];
    assign rs = instr[25:21];
    assign rt = instr[20:16];
    assign rd = instr[15:11];
    assign shamt = instr[10:6];
    assign funct = instr[5:0];
    assign immed = instr[15:0];
    assign jumpoffset = instr[25:0];
	assign jump_addr = { pc[31:28], jumpoffset <<2 } ;

	mux2 #(5) RFMUX( .sel(RegDst), .a(rt), .b(rd), .y(rfile_wn_out) );
	sign_extend SignExt( .immed_in(immed), .ext_immed_out(extend_immed) );
	
	reg_file RegFile( .clk(clk), .RegWrite(RegWrite), .RN1(rs), .RN2(rt), .WN(rfile_wn), 
					  .WD(rfile_wd), .RD1(rfile_rd1), .RD2(rfile_rd2) );
	
	
	control_single CTL(.opcode(opcode), .RegDst(RegDst), .ALUSrc(ALUSrc), .MemtoReg(MemtoReg), 
                       .RegWrite(RegWrite_out), .MemRead(MemRead), .MemWrite(MemWrite), .Branch(Branch), 
                       .Jump(Jump), .ALUOp(ALUOp));
	
	
endmodule
		