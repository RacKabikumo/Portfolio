/*
	Title: MIPS Single Cycle CPU Testbench
	Author: Selene (Computer System and Architecture Lab, ICE, CYCU) 
*/
`timescale 1ns/1ns
module tb_pipelining();
	reg clk, rst;
	
	// ���ͮɯߡA�g���G10ns
	initial begin
		clk = 1;
		forever #5 clk = ~clk;
	end

	initial begin
		rst = 1'b1;
		/*
			���O��ưO����A�ɦW"instr_mem.txt, data_mem.txt"�i�ۦ�ק�
			�C�@�欰1 Byte��ơA�H��ӤQ���i��Ʀr���
			�B��Little Endian�s�X
		*/
		$readmemh("instr_mem.txt", CPU.instr_fetch.InstrMem.mem_array );
		$readmemh("data_mem.txt", CPU.memory_access.DatMem.mem_array );
		// �]�w�Ȧs����l�ȡA�C�@�欰�@���Ȧs�����
		$readmemh("reg.txt", CPU.instr_decode.RegFile.file_array );
		#10;
		rst = 1'b0;
	end
	
	always @( posedge clk ) begin
		$display( "%d, PC:", $time/10-1, CPU.pc );
		if ( CPU.instr_decode.instr == 32'b0 ) $display( "%d, NOP\n", $time/10-1 );
		else if ( CPU.instr_decode.opcode == 6'd0 ) begin
			$display( "%d, wd: %d", $time/10-1, CPU.rfile_wd_wb );
			if ( CPU.instr_decode.funct == 6'd32 ) $display( "%d, ADD\n", $time/10-1 );
			else if ( CPU.instr_decode.funct == 6'd34 ) $display( "%d, SUB\n", $time/10-1 );
			else if ( CPU.instr_decode.funct == 6'd36 ) $display( "%d, AND\n", $time/10-1 );
			else if ( CPU.instr_decode.funct == 6'd37 ) $display( "%d, OR\n", $time/10-1 );
			else if ( CPU.instr_decode.funct == 6'd42 ) $display( "%d, SLT\n", $time/10-1 );
			else if ( CPU.instr_decode.funct == 6'd2 ) $display( "%d, SRL\n", $time/10-1 );
			else if ( CPU.instr_decode.funct == 6'd8 ) $display( "%d, JR\n", $time/10-1 );
			else if ( CPU.instr_decode.funct == 6'd27 ) $display( "%d, DIVU\n", $time/10-1 );
			else if ( CPU.instr_decode.funct == 6'd16 ) $display( "%d, MFHI\n", $time/10-1 );
			else if ( CPU.instr_decode.funct == 6'd18 ) $display( "%d, MFLO\n", $time/10-1 );
		end
		else if ( CPU.instr_decode.opcode == 6'd35 ) $display( "%d, LW\n", $time/10-1 );
		else if ( CPU.instr_decode.opcode == 6'd13 ) $display( "%d, ORI\n", $time/10-1 );
		else if ( CPU.instr_decode.opcode == 6'd43 ) $display( "%d, SW\n", $time/10-1 );
		else if ( CPU.instr_decode.opcode == 6'd4 ) $display( "%d, BEQ\n", $time/10-1 );
		else if ( CPU.instr_decode.opcode == 6'd2 ) $display( "%d, J\n", $time/10-1 );

	end
	
	mips_pipelining CPU( clk, rst );
	
endmodule
