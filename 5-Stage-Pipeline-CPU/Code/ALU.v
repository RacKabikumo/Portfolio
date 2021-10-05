`timescale 1ns/1ns
module ALU( dataA, dataB, Signal, dataOut, zero, shamt );
input [31:0] dataA ;
input [31:0] dataB ;
input [2:0] Signal ;
input [4:0] shamt ;
output [31:0] dataOut ;
output zero ;
wire [31:0] shifterOut, ALUout ;
wire [31:0] Cin ;
wire Cout ;
wire Binvert ;
wire less ;
wire [30:0] temp ;
//   Signal ( 6-bits)
//   AND  : 36
//   OR   : 37
//   ADD  : 32
//   SUB  : 34
//   SLT  : 42

	parameter ADD = 3'b010;
    parameter SUB = 3'b110;
    parameter AND = 3'b000;
    parameter OR  = 3'b001;
    parameter SLT = 3'b111;
	parameter SRL = 3'b101;
/*
定義各種訊號
*/
	assign Binvert = ( Signal == SUB || Signal == SLT ) ? 1'b1 : 1'b0 ;
	assign Cin[0] = Binvert ;

	Shifter Shifter( .data(dataB), .sel(shamt), .Signal(Signal), .dataOut(shifterOut) );

	ALU_1bit alu0( .dataA( dataA[0] ), .dataB( dataB[0] ), .Cin( Cin[0] ), .Cout( Cin[1] ), .dataOut( ALUout[0] ), .sel( Signal ), .less(less), .Binvert( Binvert ), .Set(temp[0]) ) ;
	ALU_1bit alu1( .dataA( dataA[1] ), .dataB( dataB[1] ), .Cin( Cin[1] ), .Cout( Cin[2] ), .dataOut( ALUout[1] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[1]) ) ;
	ALU_1bit alu2( .dataA( dataA[2] ), .dataB( dataB[2] ), .Cin( Cin[2] ), .Cout( Cin[3] ), .dataOut( ALUout[2] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[2]) ) ;
	ALU_1bit alu3( .dataA( dataA[3] ), .dataB( dataB[3] ), .Cin( Cin[3] ), .Cout( Cin[4] ), .dataOut( ALUout[3] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[3]) ) ;
	ALU_1bit alu4( .dataA( dataA[4] ), .dataB( dataB[4] ), .Cin( Cin[4] ), .Cout( Cin[5] ), .dataOut( ALUout[4] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[4]) ) ;
	ALU_1bit alu5( .dataA( dataA[5] ), .dataB( dataB[5] ), .Cin( Cin[5] ), .Cout( Cin[6] ), .dataOut( ALUout[5] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[5]) ) ;
	ALU_1bit alu6( .dataA( dataA[6] ), .dataB( dataB[6] ), .Cin( Cin[6] ), .Cout( Cin[7] ), .dataOut( ALUout[6] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[6]) ) ;
	ALU_1bit alu7( .dataA( dataA[7] ), .dataB( dataB[7] ), .Cin( Cin[7] ), .Cout( Cin[8] ), .dataOut( ALUout[7] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[7]) ) ;
	ALU_1bit alu8( .dataA( dataA[8] ), .dataB( dataB[8] ), .Cin( Cin[8] ), .Cout( Cin[9] ), .dataOut( ALUout[8] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[8]) ) ;
	ALU_1bit alu9( .dataA( dataA[9] ), .dataB( dataB[9] ), .Cin( Cin[9] ), .Cout( Cin[10] ), .dataOut( ALUout[9] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[9]) ) ;
	ALU_1bit alu10( .dataA( dataA[10] ), .dataB( dataB[10] ), .Cin( Cin[10] ), .Cout( Cin[11] ), .dataOut( ALUout[10] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[10]) ) ;
	ALU_1bit alu11( .dataA( dataA[11] ), .dataB( dataB[11] ), .Cin( Cin[11] ), .Cout( Cin[12] ), .dataOut( ALUout[11] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[11]) ) ;
	ALU_1bit alu12( .dataA( dataA[12] ), .dataB( dataB[12] ), .Cin( Cin[12] ), .Cout( Cin[13] ), .dataOut( ALUout[12] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[12]) ) ;
	ALU_1bit alu13( .dataA( dataA[13] ), .dataB( dataB[13] ), .Cin( Cin[13] ), .Cout( Cin[14] ), .dataOut( ALUout[13] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[13]) ) ;
	ALU_1bit alu14( .dataA( dataA[14] ), .dataB( dataB[14] ), .Cin( Cin[14] ), .Cout( Cin[15] ), .dataOut( ALUout[14] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[14]) ) ;
	ALU_1bit alu15( .dataA( dataA[15] ), .dataB( dataB[15] ), .Cin( Cin[15] ), .Cout( Cin[16] ), .dataOut( ALUout[15] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[15]) ) ;
	ALU_1bit alu16( .dataA( dataA[16] ), .dataB( dataB[16] ), .Cin( Cin[16] ), .Cout( Cin[17] ), .dataOut( ALUout[16] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[16]) ) ;
	ALU_1bit alu17( .dataA( dataA[17] ), .dataB( dataB[17] ), .Cin( Cin[17] ), .Cout( Cin[18] ), .dataOut( ALUout[17] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[17]) ) ;
	ALU_1bit alu18( .dataA( dataA[18] ), .dataB( dataB[18] ), .Cin( Cin[18] ), .Cout( Cin[19] ), .dataOut( ALUout[18] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[18]) ) ;
	ALU_1bit alu19( .dataA( dataA[19] ), .dataB( dataB[19] ), .Cin( Cin[19] ), .Cout( Cin[20] ), .dataOut( ALUout[19] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[19]) ) ;
	ALU_1bit alu20( .dataA( dataA[20] ), .dataB( dataB[20] ), .Cin( Cin[20] ), .Cout( Cin[21] ), .dataOut( ALUout[20] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[20]) ) ;
	ALU_1bit alu21( .dataA( dataA[21] ), .dataB( dataB[21] ), .Cin( Cin[21] ), .Cout( Cin[22] ), .dataOut( ALUout[21] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[21]) ) ;
	ALU_1bit alu22( .dataA( dataA[22] ), .dataB( dataB[22] ), .Cin( Cin[22] ), .Cout( Cin[23] ), .dataOut( ALUout[22] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[22]) ) ;
	ALU_1bit alu23( .dataA( dataA[23] ), .dataB( dataB[23] ), .Cin( Cin[23] ), .Cout( Cin[24] ), .dataOut( ALUout[23] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[23]) ) ;
	ALU_1bit alu24( .dataA( dataA[24] ), .dataB( dataB[24] ), .Cin( Cin[24] ), .Cout( Cin[25] ), .dataOut( ALUout[24] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[24]) ) ;
	ALU_1bit alu25( .dataA( dataA[25] ), .dataB( dataB[25] ), .Cin( Cin[25] ), .Cout( Cin[26] ), .dataOut( ALUout[25] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[25]) ) ;
	ALU_1bit alu26( .dataA( dataA[26] ), .dataB( dataB[26] ), .Cin( Cin[26] ), .Cout( Cin[27] ), .dataOut( ALUout[26] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[26]) ) ;
	ALU_1bit alu27( .dataA( dataA[27] ), .dataB( dataB[27] ), .Cin( Cin[27] ), .Cout( Cin[28] ), .dataOut( ALUout[27] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[27]) ) ;
	ALU_1bit alu28( .dataA( dataA[28] ), .dataB( dataB[28] ), .Cin( Cin[28] ), .Cout( Cin[29] ), .dataOut( ALUout[28] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[28]) ) ;
	ALU_1bit alu29( .dataA( dataA[29] ), .dataB( dataB[29] ), .Cin( Cin[29] ), .Cout( Cin[30] ), .dataOut( ALUout[29] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[29]) ) ;
	ALU_1bit alu30( .dataA( dataA[30] ), .dataB( dataB[30] ), .Cin( Cin[30] ), .Cout( Cin[31] ), .dataOut( ALUout[30] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set(temp[30]) ) ;
	ALU_1bit alu31( .dataA( dataA[31] ), .dataB( dataB[31] ), .Cin( Cin[31] ), .Cout( Cout ), .dataOut( ALUout[31] ), .sel( Signal ), .less(1'b0), .Binvert( Binvert ), .Set( less ) ) ;
	
	
	assign dataOut = ( Signal == SRL ) ? shifterOut : ALUout ;
	assign zero = ( dataOut == 32'b0 ) ? 1'b1 : 1'b0 ;
endmodule