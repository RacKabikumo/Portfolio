module stimulus ;

reg clk, reset, cancel ;
reg [7:0] money, drink ;


parameter tea = 8'd10 ;
parameter coke = 8'd15 ;
parameter coffee = 8'd20 ;
parameter milk = 8'd25 ;

SaleMachine salemachine( clk, reset, cancel, money, drink ) ;

initial clk = 1'b0;
always #5 clk = ~clk;

initial 
begin
$display( "B1 START\n" ) ;
reset = 1'b1 ;
drink = 8'b0 ;
cancel = 1'b0 ;
money = 8'd0 ;
#10	reset = 1'b0 ;
#5	money = 8'd10 ;
#20 money = 8'd1 ;
#20 money = 8'd10 ;
#10 drink = coffee ;
	money = 8'd0 ;
#30 drink = 8'd0 ;
#10	$display( "B1 END\n" ) ;
	$display( "B2 START\n" ) ;
#10	drink = 8'd0 ;
	reset = 1'b1 ;
	cancel = 1'b0 ;
	money = 8'd0 ;
#10	reset = 1'b0 ;
#5	money = 8'd5 ;
#10	money = 8'd10 ;
#10	cancel = 1'b1 ;
	money = 8'd0 ;
#10	cancel = 1'b0 ;
#20	money = 8'd10 ;
#20	money = 8'd10 ;
#10	money = 8'd1 ;
#20	money = 8'd1 ;
#20	money = 8'd1 ;
#20	money = 8'd1 ;
#20	money = 8'd1 ;
#10 drink = milk ;
	money = 8'd0 ;
#30 drink = 8'd0 ;
	$display( "B2 END\n" ) ;
	$display( "B3 START\n" ) ;
#10	drink = 8'd0 ;
	reset = 1'b1 ;
	cancel = 1'b0 ;
	money = 8'd0 ;
#10	reset = 1'b0 ;
#5	money = 8'd5 ;
#10	money = 8'd10 ;
#20	cancel = 1'b1 ;
	money = 8'd0 ;
#10	cancel = 1'b0 ;
#20	money = 8'd10 ;
#20	money = 8'd10 ;
#10 drink = coke ;
	money = 8'd0 ;
#30 drink = 8'd0 ;
#10	money = 8'd10 ;
#20 money = 8'd10 ;
#10 drink = milk ;
	money = 8'd0 ;
#30	drink = 8'd0 ;
#10 money = 8'd10 ;
#10 drink = tea ;
	money = 8'd0 ;
#30	drink = 8'd0 ;
#10 money = 8'd50 ;
#10 drink = coffee ;
	money = 8'd0 ;
#30	drink = 8'd0 ;
	$display( "B3 END\n" ) ;
end
endmodule