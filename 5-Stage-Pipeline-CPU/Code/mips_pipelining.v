`timescale 1ns/1ns
module mips_pipelining( clk, rst ) ;
	input clk, rst ;
	wire[31:0] instr_if;
	
	// break out important fields from instruction
    wire [4:0] shamt_id, shamt_idtoex ;
    wire [31:0] extend_immed_id, extend_immed_idtoex;
	
	// datapath signals
    wire [4:0] rfile_wn_id, rfile_wn_idtoex, rfile_wn_extomem, rfile_wn_wb ;
    wire [31:0] rfile_rd1_id, rfile_rd1_idtoex, 
				rfile_rd2_id, rfile_rd2_idtoex,
				rfile_wd_wb, 
				alu_out_ex, alu_out_memtowb, addr_extomem, wd_extomem, b_tgt_ex, b_tgt_extomem,
                pc, pc_if, dmem_rdata_mem, dmem_rdata_memtowb,
				jump_addr_id, jump_addr_idtoex, jump_addr_extomem, jumpR_addr, jumpR_addr_extomem ;
	wire [31:0] pc_iftoid, pc_idtoex, pc_extomem, instr_iftoid ;

	// control signals
    wire RegWrite, RegWrite_idtoex, RegWrite_extomem, RegWrite_memtowb,
		Branch, Branch_idtoex, Branch_extomem, PCSrc_mem,
		MemtoReg, MemtoReg_idtoex, MemtoReg_extomem, MemtoReg_memtowb,
		MemRead, MemRead_idtoex, MemRead_extomem, DIV, Hi, Lo, Hi_Lo,
		MemWrite, MemWrite_idtoex, MemWrite_extomem, ALUSrc, ALUSrc_idtoex, 
		zero, zero_extomem, Jump, Jump_idtoex, Jump_extomem, JR, JR_extomem ;
    wire [1:0] ALUOp, ALUOp_idtoex;
	
	

	IF instr_fetch( .clk(clk), .rst(rst), .PCSrc(PCSrc_mem), .b_tgt(b_tgt_extomem), .instr(instr_if), .pc(pc), .pc_incr(pc_if), .pc_add4(pc_extomem), .Branch(Branch_extomem),
					.jump_addr(jump_addr_extomem), .Jump(Jump_extomem), .jumpR_addr(jumpR_addr_extomem), .JR(JR_extomem), .Hi_Lo(Hi_Lo) ) ;
	
	IF_ID iftoid( .clk(clk), .rst(rst), .pc(pc_if), .ins(instr_if), .pc_out(pc_iftoid), .ins_out(instr_iftoid) ) ;
	
	ID instr_decode( .clk(clk), .rst(rst), .pc(pc_iftoid), .rfile_wn(rfile_wn_wb), .instr(instr_iftoid), .rfile_wd(rfile_wd_wb), .RegWrite_out(RegWrite), .rfile_wn_out(rfile_wn_id),
			.rfile_rd1(rfile_rd1_id), .rfile_rd2(rfile_rd2_id), .extend_immed(extend_immed_id), .shamt(shamt_id), .jump_addr(jump_addr_id),
			.ALUSrc(ALUSrc), .MemtoReg(MemtoReg), .RegWrite(RegWrite_memtowb), .MemRead(MemRead), .MemWrite(MemWrite), .Branch(Branch), .Jump(Jump), .ALUOp(ALUOp) ) ;
			
	ID_EX idtoex( .clk(clk), .rst(rst), .RD1(rfile_rd1_id), .RD2(rfile_rd2_id), .immed(extend_immed_id), .jump_addr(jump_addr_id), .pc(pc_iftoid), 
				.shamt(shamt_id), .shamt_out(shamt_idtoex), .RD1_out(rfile_rd1_idtoex), .RD2_out(rfile_rd2_idtoex), .immed_out(extend_immed_idtoex), 
				.jump_addr_out(jump_addr_idtoex), .pc_out(pc_idtoex), .instr(instr_iftoid), .JR(JR), .DIV(DIV), .Hi(Hi), .Lo(Lo),
				.ALUSrc(ALUSrc), .MemtoReg(MemtoReg), .RegWrite(RegWrite), .MemRead(MemRead), .MemWrite(MemWrite), .Branch(Branch), .Jump(Jump), 
				.ALUOp(ALUOp), .ALUSrc_out(ALUSrc_idtoex), .MemtoReg_out(MemtoReg_idtoex), .RegWrite_out(RegWrite_idtoex), .rfile_wn(rfile_wn_id), .rfile_wn_out(rfile_wn_idtoex),
				.MemRead_out(MemRead_idtoex), .MemWrite_out(MemWrite_idtoex), .Branch_out(Branch_idtoex), .Jump_out(Jump_idtoex), .ALUOp_out(ALUOp_idtoex) ) ;
				
	EX execute( .clk(clk), .rst(rst), .ALUSrc(ALUSrc_idtoex), .Branch(Branch_idtoex), .MemtoReg(MemtoReg_idtoex), .Hi(Hi), .Lo(Lo),
				.Jump(Jump_idtoex), .ALUOp(ALUOp_idtoex), .RegWrite(RegWrite_idtoex), .DIV(DIV), .Hi_Lo(Hi_Lo),
				.shamt(shamt_idtoex), .rfile_rd1(rfile_rd1_idtoex), .extend_immed(extend_immed_idtoex), 
				.pc_incr(pc_idtoex), .rfile_rd2(rfile_rd2_idtoex), .jumpR_addr(jumpR_addr), 
				.b_tgt(b_tgt_ex), .alu_out(alu_out_ex), .zero(zero) ) ;
			
	EX_MEM extomem( .clk(clk), .rst(rst), .pc(b_tgt_ex), .ALUout(alu_out_ex), .RD2(rfile_rd2_idtoex), .wn(rfile_wn_idtoex), .zero(zero), .Branch(Branch_idtoex), 
					.MemtoReg(MemtoReg_idtoex), .MemRead(MemRead_idtoex), .MemWrite(MemWrite_idtoex), .Jump(Jump_idtoex), .RegWrite(RegWrite_idtoex),
					.zero_out(zero_extomem), .Branch_out(Branch_extomem), .MemtoReg_out(MemtoReg_extomem), .MemRead_out(MemRead_extomem), .pc_add4(pc_idtoex), .pc_add4_out(pc_extomem),
					.MemWrite_out(MemWrite_extomem), .Jump_out(Jump_extomem), .RegWrite_out(RegWrite_extomem), .pc_out(b_tgt_extomem), .addr(addr_extomem), 
					.wd(wd_extomem), .wn_out(rfile_wn_extomem), .jump_addr(jump_addr_idtoex), .jump_addr_out(jump_addr_extomem),
					.jumpR_addr(jumpR_addr), .jumpR_addr_out(jumpR_addr_extomem), .JR(JR), .JR_out(JR_extomem) ) ;
				
	MEM memory_access( .clk(clk), .rst(rst), .MemRead(MemRead_extomem), .MemWrite(MemWrite_extomem), .zero(zero_extomem), .Branch(Branch_extomem), 
					.rfile_rd2(wd_extomem), .ALUout(addr_extomem), .dmem_rdata(dmem_rdata_mem),
					.PCSrc(PCSrc_mem), .MemtoReg(MemtoReg_extomem), .wn(rfile_wn_extomem) ) ;
			
	MEM_WB memtowb( .clk(clk), .rst(rst), .ALUout(addr_extomem), .RD(dmem_rdata_mem), .wn(rfile_wn_extomem), .MemtoReg(MemtoReg_extomem), .RegWrite(RegWrite_extomem), 
					.MemtoReg_out(MemtoReg_memtowb), .RegWrite_out(RegWrite_memtowb), .wn_out(rfile_wn_wb), .ALUout_out(alu_out_memtowb), .RD_out(dmem_rdata_memtowb) ) ;
	
	WB write_back( .clk(clk), .rst(rst), .alu_out(alu_out_memtowb), .MemtoReg(MemtoReg_memtowb), .dmem_rdata(dmem_rdata_memtowb), .rfile_wd(rfile_wd_wb) ) ;
endmodule
	
	