`timescale 1ns/1ns
module ALU_1bit( dataA, dataB, sel, Cin, Cout, dataOut, less, Binvert, Set );
	input Cin, Binvert, dataA, dataB, less ;
	input [2:0] sel ;
	output Cout, dataOut, Set ;
	wire [3:0] S ;
	wire dataOut, Cout ;
	wire temp ;

	parameter ADD = 3'b010;
    parameter SUB = 3'b110;
    parameter AND = 3'b000;
    parameter OR  = 3'b001;
    parameter SLT = 3'b111;
	
	assign temp = dataB ^ Binvert ;
	
	AND U_AND( .dataA( dataA ), .dataB( dataB ), .out( S[0] ) ) ;
	OR U_OR( .dataA( dataA ), .dataB( dataB ), .out( S[1] ) ) ;
	FA U_FA( .dataA( dataA ), .dataB( temp ), .Cin( Cin ), .Cout( Cout ), .Sum( S[2] ) ) ;
	Slt U_Slt( .in( less ), .out( S[3] ) ) ;
	
	

	assign dataOut = ( sel == AND ) ? S[0] : ( ( sel == OR ) ? S[1] : ( ( sel == ADD || sel == SUB ) ? S[2] : ( ( sel == SLT ) ? S[3] : 1'bz ) ) ) ;
	assign Set = ( sel == SLT ) ? S[2] : 1'b0 ;

endmodule