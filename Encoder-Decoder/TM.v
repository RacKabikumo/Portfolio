`timescale 1ns / 1ns

module  TM;

	reg x, y, z ;
	wire d1, d2, d3, d4, d5, d6, d7 ;
	wire x_out, y_out, z_out ;
	Decoder	D( .x(x), .y(y), .z(z), .d1(d1), .d2(d2), .d3(d3), .d4(d4), .d5(d5), .d6(d6), .d7(d7) ) ;
	Encoder E( .d1(d1), .d2(d2), .d3(d3), .d4(d4), .d5(d5), .d6(d6), .d7(d7), .x(x_out), .y(y_out), .z(z_out) ) ;
	parameter	t = 20;

	initial
	begin
		x = 1'b0 ;
		y = 1'b1 ;
		z = 1'b0 ;
		#t
		x = 1'b1 ;
		y = 1'b0 ;
		z = 1'b1 ;
		#t
		x = 1'b1 ;
		y = 1'b1 ;
		z = 1'b1 ;
		#t	$stop;
	end

endmodule


