`timescale 1ns/1ns
module IF_ID( clk, rst, pc, ins, pc_out, ins_out ) ;
	input clk, rst ;
	input [31:0] pc, ins ;
	output [31:0] pc_out, ins_out ;
	
	reg [31:0] pc_out, ins_out ;

	
	always@( posedge clk ) begin
		if ( rst ) begin
			pc_out <= 32'b0 ;
			ins_out <= 32'b0 ;
		end
		else begin
			pc_out <= pc ;
			ins_out <= ins ;
		end
	end
	
endmodule
		