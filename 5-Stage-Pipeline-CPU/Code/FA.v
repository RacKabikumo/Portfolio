`timescale 1ns/1ns
module FA( dataA, dataB, Cin, Cout, Sum );
input Cin ;
input dataA ;
input dataB ;
output Cout ;
output Sum ;

wire Cout, Sum ;


	assign Sum = dataA ^ dataB ^ Cin ;
	assign Cout = ( ( dataA | dataB ) & Cin ) | ( dataA & dataB ) ;



endmodule