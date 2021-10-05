`timescale 1ns/1ns
module IF( clk, rst, PCSrc, b_tgt, pc, pc_incr, jump_addr, Jump, jumpR_addr, JR, instr, pc_add4, Branch, Hi_Lo ) ;
	input clk, rst, PCSrc, Jump, JR, Branch, Hi_Lo ;
	input [31:0] b_tgt, jump_addr, jumpR_addr, pc_add4 ;
	output [31:0] instr, pc, pc_incr ;
	wire [31:0] branch_addr, jump_address ;
	wire [31:0] pc, pc_incr, pc_next, instr_temp, addr ;
	reg en_reg ;
	reg [2:0] count ;
	reg div ;
	reg32 PC( .clk(clk), .rst(rst), .en_reg(en_reg), .d_in(pc_next), .d_out(pc) );
	memory InstrMem( .clk(clk), .MemRead(1'b1), .MemWrite(1'b0), .wd(32'd0), .addr(pc), .rd(instr_temp) );
	add32 PCADD( .a(pc), .b(32'd4), .result(pc_incr) );
	mux2 #(32) BRMUX( .sel(Branch), .a(pc_incr), .b(pc_add4), .y(branch_addr) );
	mux2 #(32) PCMUX( .sel(PCSrc), .a(branch_addr), .b(b_tgt), .y(addr) );
	mux2 #(32) JORJRMUX( .sel(JR), .a(jump_addr), .b(jumpR_addr), .y(jump_address) );
	mux2 #(32) JMUX( .sel(Jump), .a(addr), .b(jump_address), .y(pc_next) );

	always@( instr_temp or pc or Branch or Hi_Lo ) begin
		if ( rst ) begin
			count <= 3'b0 ;
			en_reg <= 1'b1 ;
			div <= 1'b0 ;
			if ( ( instr_temp[31:26] == 6'b0 && instr_temp[5:0] == 6'd8 ) || instr_temp[31:26] == 6'd2 || instr_temp[31:26] == 6'd4 ) begin
				count <= 3'd1 ;
			end
			else if ( instr_temp[31:26] == 6'b0 && instr_temp[5:0] == 6'd27 ) begin // DIVU
				count <= 3'd5 ;
				div <= 1'b1 ;
				en_reg <= 1'b0 ;
			end
		end
		else if ( count == 3'd1 || count == 3'd2 || count == 3'd3 ) begin
			count <= count + 1 ;
			div <= 1'b0 ;
		end
		else if ( ( instr_temp[31:26] == 6'b0 && instr_temp[5:0] == 6'd8 ) || instr_temp[31:26] == 6'd2 || instr_temp[31:26] == 6'd4 ) begin
			count <= 3'd1 ;
			div <= 1'b0 ;
		end
		else if ( instr_temp[31:26] == 6'b0 && instr_temp[5:0] == 6'd27 && div == 1'b0 ) begin // DIVU
			count <= 3'd5 ;
			div <= 1'b1 ;
			en_reg <= 1'b0 ;
		end
		else if ( Hi_Lo == 1'b0 && div == 1'b1 ) begin
			count <= 3'd6 ;
			en_reg <= 1'b0 ;
		end
		else begin
			div <= 1'b0 ;
			count <= 3'd0 ;
			en_reg <= 1'b1 ;
		end
	end
	
	assign instr = ( count == 3'd0 || count == 3'd1 || count == 3'd5 ) ? instr_temp : 32'b0 ;
endmodule
		