`timescale 1ns / 1ns

module  Decoder( x, y, z, d1, d2, d3, d4, d5, d6, d7 );
input x, y, z ;
output d1, d2, d3, d4, d5, d6, d7 ;
wire d0, x_not, y_not, z_not ;

assign x_not = ~x ;
assign y_not = ~y ;
assign z_not = ~z ;

assign d0 = x_not & y_not & z_not ;
assign d1 = x_not & y_not & z ;
assign d2 = x_not & y & z_not ;
assign d3 = x_not & y & z ;
assign d4 = x & y_not & z_not ;
assign d5 = x & y_not & z ;
assign d6 = x & y & z_not ;
assign d7 = x & y & z ;

endmodule


