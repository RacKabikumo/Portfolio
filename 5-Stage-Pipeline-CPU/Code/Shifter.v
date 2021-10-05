`timescale 1ns/1ns
module Shifter( data, sel, dataOut, Signal ) ;
	input [31:0] data ;
	input [4:0] sel ;
	input [2:0] Signal ;
	output [31:0] dataOut ;
	wire [31:0] data, dataOut ;
	wire [31:0] t1, t2, t4, t8, t16 ;

	parameter SRL = 3'b101;

	
	Shifter_1bit S_1bit( .data(data), .sel( sel[0] ), .dataOut( t1 ) ) ;
	Shifter_2bit S_2bit( .data(t1), .sel( sel[1] ), .dataOut( t2 ) ) ;
	Shifter_4bit S_4bit( .data(t2), .sel( sel[2] ), .dataOut( t4 ) ) ;
	Shifter_8bit S_8bit( .data(t4), .sel( sel[3] ), .dataOut( t8 ) ) ;
	Shifter_16bit S_16bit( .data(t8), .sel( sel[4] ), .dataOut( t16 ) ) ;

	assign dataOut = ( Signal == SRL ) ? t16 : dataOut ; 


endmodule