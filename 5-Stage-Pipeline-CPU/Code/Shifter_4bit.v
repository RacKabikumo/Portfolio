`timescale 1ns/1ns
module Shifter_4bit( data, sel, dataOut );
	input [31:0] data ;
	input sel ;
	output [31:0] dataOut ;
	wire [31:0] temp ;
	assign temp[0] = ( sel == 1'b1 ) ? data[4] : data[0] ;
	assign temp[1] = ( sel == 1'b1 ) ? data[5] : data[1] ;
	assign temp[2] = ( sel == 1'b1 ) ? data[6] : data[2] ;
	assign temp[3] = ( sel == 1'b1 ) ? data[7] : data[3] ;
	assign temp[4] = ( sel == 1'b1 ) ? data[8] : data[4] ;
	assign temp[5] = ( sel == 1'b1 ) ? data[9] : data[5] ;
	assign temp[6] = ( sel == 1'b1 ) ? data[10] : data[6] ;
	assign temp[7] = ( sel == 1'b1 ) ? data[11] : data[7] ;
	assign temp[8] = ( sel == 1'b1 ) ? data[12] : data[8] ;
	assign temp[9] = ( sel == 1'b1 ) ? data[13] : data[9] ;
	assign temp[10] = ( sel == 1'b1 ) ? data[14] : data[10] ;
	assign temp[11] = ( sel == 1'b1 ) ? data[15] : data[11] ;
	assign temp[12] = ( sel == 1'b1 ) ? data[16] : data[12] ;
	assign temp[13] = ( sel == 1'b1 ) ? data[17] : data[13] ;
	assign temp[14] = ( sel == 1'b1 ) ? data[18] : data[14] ;
	assign temp[15] = ( sel == 1'b1 ) ? data[19] : data[15] ;
	assign temp[16] = ( sel == 1'b1 ) ? data[20] : data[16] ;
	assign temp[17] = ( sel == 1'b1 ) ? data[21] : data[17] ;
	assign temp[18] = ( sel == 1'b1 ) ? data[22] : data[18] ;
	assign temp[19] = ( sel == 1'b1 ) ? data[23] : data[19] ;
	assign temp[20] = ( sel == 1'b1 ) ? data[24] : data[20] ;
	assign temp[21] = ( sel == 1'b1 ) ? data[25] : data[21] ;
	assign temp[22] = ( sel == 1'b1 ) ? data[26] : data[22] ;
	assign temp[23] = ( sel == 1'b1 ) ? data[27] : data[23] ;
	assign temp[24] = ( sel == 1'b1 ) ? data[28] : data[24] ;
	assign temp[25] = ( sel == 1'b1 ) ? data[29] : data[25] ;
	assign temp[26] = ( sel == 1'b1 ) ? data[30] : data[26] ;
	assign temp[27] = ( sel == 1'b1 ) ? data[31] : data[27] ;
	assign temp[28] = ( sel == 1'b1 ) ? 1'b0 : data[28] ;
	assign temp[29] = ( sel == 1'b1 ) ? 1'b0 : data[29] ;
	assign temp[30] = ( sel == 1'b1 ) ? 1'b0 : data[30] ;
	assign temp[31] = ( sel == 1'b1 ) ? 1'b0 : data[31] ;

	assign dataOut = temp ;

endmodule