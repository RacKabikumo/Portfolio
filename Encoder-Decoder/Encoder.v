`timescale 1ns / 1ns

module  Encoder( d1, d2 , d3, d4, d5, d6, d7, x, y, z );
input d1, d2, d3, d4, d5, d6, d7 ;

output x, y, z ;

assign x = d4 | d5 | d6 | d7 ;
assign y = d2 | d3 | d6 | d7 ;
assign z = d1 | d3 | d5 | d7 ;
	
endmodule


