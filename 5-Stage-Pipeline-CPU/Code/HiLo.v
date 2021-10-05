`timescale 1ns/1ns
module HiLo( clk, DivAns, HiOut, LoOut, reset, Signal, Hi, Lo );
input clk ;
input reset ;
input Signal, Hi, Lo ;
input [63:0] DivAns ;
output [31:0] HiOut ;
output [31:0] LoOut ;

reg [63:0] HiLo ;

always@( posedge clk or reset )
begin
  if ( reset )
  begin
    HiLo = 64'b0 ;
  end
/*
reset�T�� �p�G�Oreset�N���k0
*/
  else
  begin
	if ( Signal == 1'b1 ) begin // Open HiLo reg for Div
		HiLo = DivAns ;
	end
  end
/*
��ǤJ�����k���צs�_��
*/
end

assign HiOut = ( Hi ) ? HiLo[63:32] : 32'b0 ;
assign LoOut = ( Lo ) ? HiLo[31:0] : 32'b0 ;

endmodule