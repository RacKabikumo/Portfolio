`timescale 1ns/1ns
module Divider( clk, dataA, dataB, Signal, dataOut, reset, stop );
input clk ;
input reset ;
input [31:0] dataA ;
input [31:0] dataB ;
input Signal ;
output stop ;
output [63:0] dataOut ;

//   Signal ( 6-bits)
//   DIVU  : 27
reg [63:0] temp ;
parameter DIVU = 6'b011011;
parameter OUT = 6'b111111;
/*
�w�q�U�ذT��
*/

reg stop ;
reg [5:0] count ;
reg	[63:0]	divr ; // ���� // �Q����/�l��
reg	[63:0]	remain ; // �l�� // �Q����/�l��
reg	[31:0]	quot ;	// ��

always@( posedge clk or reset or Signal )
begin
        if ( reset ) // ��l�Ƽƾ�
        begin
                temp = 64'b0 ;
				remain = {32'b0,dataA} ; // �Q���ƫe��0
				divr = {dataB, 32'b0} ; // ���ƫ��0	
				quot = 32'b0 ;
				count <= 6'b0 ;
				stop <= 1'b0 ;
        end
        else
        begin
			if ( Signal == 1'b1 && count == 6'b0 ) begin
				temp = 64'b0 ;
				remain = {32'b0,dataA} ; // �Q���ƫe��0
				divr = {dataB, 32'b0} ; // ���ƫ��0	
				quot = 32'b0 ;
				count <= 6'b1 ;
				stop <= 1'b0 ;
			end
			if ( Signal == 1'b1 && stop == 1'b0 && count != 6'd33 ) begin // �T���O���k
				if ( dataB == 32'b0 )
				begin
					remain = 64'bx ;
					quot = 32'bx ;
				end
				else
				begin
					count <= count + 1 ;
					remain = remain - divr ;
					if ( remain[63] == 0 )
					begin
						quot = quot << 1 ;
						quot[0] = 1'b1 ;		
					end	
			
					if ( remain [63] == 1 )
					begin

						remain = remain + divr ;
						quot = quot << 1 ;
						quot[0] = 1'b0 ;
					end
			
					divr = divr >> 1 ;
				end
			end
			else if ( count == 6'd33 )
			begin
				stop = 1'b1 ;
				count <= 6'b0 ;
				temp[63:32] = remain[31:0] ; // dataA % dataB ;
				temp[31:0] = quot ; // dataA / dataB ;
			end
		end

/*
���k�B��
OUT�������O�n��control���A���O�A�~����⵪�׿�X��HILO�Ȧs��
*/

end
assign dataOut = temp ;

endmodule