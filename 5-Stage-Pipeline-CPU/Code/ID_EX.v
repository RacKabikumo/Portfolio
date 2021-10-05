`timescale 1ns/1ns
module ID_EX( clk, rst, RD1, RD2, immed, jump_addr, pc, shamt, shamt_out, RD1_out, RD2_out, immed_out, jump_addr_out, pc_out, DIV,
				ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, Jump, ALUOp, instr, JR, rfile_wn, rfile_wn_out, Hi, Lo,
				ALUSrc_out, MemtoReg_out, RegWrite_out, MemRead_out, MemWrite_out, Branch_out, Jump_out, ALUOp_out ) ;
				
	input clk, rst ;
	input [31:0] pc, immed, jump_addr, RD1, RD2, instr ;
	input [4:0] shamt, rfile_wn ;
	input ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, Jump;
    input[1:0] ALUOp;
	output [31:0] pc_out, immed_out, jump_addr_out, RD1_out, RD2_out ;
	output [4:0] shamt_out, rfile_wn_out ;
	output ALUSrc_out, MemtoReg_out, RegWrite_out, MemRead_out, MemWrite_out, Branch_out, Jump_out, JR, DIV, Hi, Lo ;
	output [1:0] ALUOp_out ;
	
    reg ALUSrc_out, MemtoReg_out, RegWrite_out, MemRead_out, MemWrite_out, Branch_out, Jump_out, JR, DIV, Hi, Lo ;
    reg[1:0] ALUOp_out ;
	reg [4:0] shamt_out, rfile_wn_out ;
	reg [31:0] pc_out, immed_out, jump_addr_out, RD1_out, RD2_out ;
	
	always@( posedge clk ) begin
		if ( rst ) begin
			pc_out <= 32'b0 ;
			RD1_out <= 32'b0 ;
			RD2_out <= 32'b0 ;
			shamt_out <= 5'b0 ;
			immed_out <= 32'b0 ;
			jump_addr_out <= 32'b0 ;
			ALUSrc_out <= 1'b0 ;
			MemtoReg_out <= 1'b0 ;
			RegWrite_out <= 1'b0 ;
			MemRead_out <= 1'b0 ;
			MemWrite_out <= 1'b0 ;
			Branch_out <= 1'b0 ;
			Jump_out <= 1'b0 ;
			ALUOp_out <= 2'b0 ;
			JR <= 1'b0 ;
			rfile_wn_out <= 5'b0 ;
			DIV <= 1'b0 ;
			Hi <= 1'b0 ;
			Lo <= 1'b0 ;
		end 
		else begin
			if ( instr == 32'b0 ) begin // NOP 什麼都不做
				ALUSrc_out <= 1'b0 ;
				MemtoReg_out <= 1'bx ;
				RegWrite_out <= 1'b0 ;
				MemRead_out <= 1'b0 ;
				MemWrite_out <= 1'b0 ;
				Branch_out <= 1'b0 ;
				Jump_out <= 1'b0 ;
				ALUOp_out <= 2'bxx ;
				JR <= 1'b0 ;
				DIV <= 1'b0 ;
				Hi <= 1'b0 ;
				Lo <= 1'b0 ;
			end
			else if ( instr[31:26] == 6'b0 && instr[5:0] == 6'd8 ) begin // JR指令 不需要寫回 只要跳
				ALUSrc_out <= 1'b0 ;
				MemtoReg_out <= 1'bx ;
				RegWrite_out <= 1'b0 ;
				MemRead_out <= 1'b0 ;
				MemWrite_out <= 1'b0 ;
				Branch_out <= 1'b0 ;
				Jump_out <= 1'b0 ;
				ALUOp_out <= 2'b01 ;
				JR <= 1'b1 ;
				DIV <= 1'b0 ;
				Hi <= 1'b0 ;
				Lo <= 1'b0 ;
			end
			else if ( instr[31:26] == 6'b0 && instr[5:0] == 6'd27 ) begin // DIVU指令 
				ALUSrc_out <= 1'b0 ;
				MemtoReg_out <= 1'bx ;
				RegWrite_out <= 1'b0 ;
				MemRead_out <= 1'b0 ;
				MemWrite_out <= 1'b0 ;
				Branch_out <= 1'b0 ;
				Jump_out <= 1'b0 ;
				ALUOp_out <= 2'bxx ;
				JR <= 1'b0 ;
				DIV <= 1'b1 ;
				Hi <= 1'b0 ;
				Lo <= 1'b0 ;
			end
			else if ( instr[31:26] == 6'b0 && instr[5:0] == 6'd16 ) begin // mfhi指令 
				ALUSrc_out <= 1'b0 ;
				MemtoReg_out <= 1'b0 ;
				RegWrite_out <= 1'b1 ;
				MemRead_out <= 1'b0 ;
				MemWrite_out <= 1'b0 ;
				Branch_out <= 1'b0 ;
				Jump_out <= 1'b0 ;
				ALUOp_out <= 2'bxx ;
				JR <= 1'b0 ;
				DIV <= 1'b0 ;
				Hi <= 1'b1 ;
				Lo <= 1'b0 ;
			end
			else if ( instr[31:26] == 6'b0 && instr[5:0] == 6'd18 ) begin // mflo指令 
				ALUSrc_out <= 1'b0 ;
				MemtoReg_out <= 1'b0 ;
				RegWrite_out <= 1'b1 ;
				MemRead_out <= 1'b0 ;
				MemWrite_out <= 1'b0 ;
				Branch_out <= 1'b0 ;
				Jump_out <= 1'b0 ;
				ALUOp_out <= 2'bxx ;
				JR <= 1'b0 ;
				DIV <= 1'b0 ;
				Hi <= 1'b0 ;
				Lo <= 1'b1 ;
			end
			else begin
				ALUSrc_out <= ALUSrc ;
				MemtoReg_out <= MemtoReg ;
				RegWrite_out <= RegWrite ;
				MemRead_out <= MemRead ;
				MemWrite_out <= MemWrite ;
				Branch_out <= Branch ;
				Jump_out <= Jump ;
				ALUOp_out <= ALUOp ;
				JR <= 1'b0 ;
				DIV <= 1'b0 ;
				Hi <= 1'b0 ;
				Lo <= 1'b0 ;
			end
			rfile_wn_out <= rfile_wn ;
			pc_out <= pc ;
			RD1_out <= RD1 ;
			RD2_out <= RD2 ;
			shamt_out <= shamt ;
			immed_out <= immed ;
			jump_addr_out <= jump_addr ;		
		end
	end
	
endmodule
		