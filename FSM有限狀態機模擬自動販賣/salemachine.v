module SaleMachine( clk, reset, cancel, money, drink ) ;
input clk, reset, cancel ;
input [7:0] money, drink ;

reg keepmoney ;
reg [1:0] state, next_state ;
reg [7:0] sum, exchange ;

parameter S0 = 2'd0 ;
parameter S1 = 2'd1 ;
parameter S2 = 2'd2 ;
parameter S3 = 2'd3 ;

parameter tea = 8'd10 ;
parameter coke = 8'd15 ;
parameter coffee = 8'd20 ;
parameter milk = 8'd25 ;

always@( posedge clk ) begin
	if ( reset == 1'b1 ) begin
		state <= S0 ;
		keepmoney <= 1'b0 ; // S1的時候用來確認要不要繼續投錢
	end
	else 
		state <= next_state ;
end

always@( state ) begin
	case( state )
		S0 :
		begin
			if ( keepmoney == 1'b0 ) begin
				exchange = 8'b0 ;
				sum = 8'b0 ; // state變成0的時候就是沒錢!
			end
		end
		S1 : 			// 顯示可買的飲料唷
		begin
			if ( sum >= milk ) 
				$display( "drink: tea coke coffee milk\n" ) ;
			else if ( sum >= coffee )
				$display( "drink: tea coke coffee\n" ) ;
			else if ( sum >= coke )
				$display( "drink: tea coke\n" ) ;
			else if ( sum >= tea )
				$display( "drink: tea\n" ) ;
			else
				$display( "total: %d dollars, money is not enough!\n", money, sum ) ;
		end	
		S2 :
		begin
			if ( sum >= drink ) begin
				if ( drink == tea )
					$display( "tea out\n" ) ;
				else if ( drink == coke )
					$display( "coke out\n" ) ;
				else if ( drink == coffee ) 
					$display( "coffee out\n" ) ;
				else if ( drink == milk )
					$display( "milk out\n" ) ;
				else
					; // 沒選飲料就沒事!
			end
			else if ( sum < drink ) begin
				if ( drink == tea )
					$display( "choose tea, but money is not enough!" ) ;
				else if ( drink == coke )
					$display( "choose coke, but money is not enough!" ) ;
				else if ( drink == coffee ) 
					$display( "choose coffee, but money is not enough!" ) ;
				else if ( drink == milk )
					$display( "choose milk, but money is not enough!" ) ;
				else
					; // 沒選飲料就沒事!
			end	
		end
		S3 :
		begin
			exchange = sum - drink ;
			if ( sum < drink )
				$display( "exchange %d dollars\n", sum ) ;
			else
				$display( "exchange %d dollars\n", exchange ) ;
		end
	
	endcase
end

always@( state or money or drink or sum ) begin
	case(state)
		S0 : 
		begin
			sum = sum + money ;
			if ( cancel ) begin// 取消不要買了 要退錢-->S3
				$display( "Cancel! coin: %d, total: %d dollars\n", money, sum ) ;
				next_state = S3 ;
				keepmoney = 1'b0 ;
			end
			else if ( sum < tea ) begin // 買不起要再投錢
				if ( money != 8'd0 )
					$display( "coin: %d, total: %d dollars", money, sum ) ;
				next_state = S0 ;
			end
			else begin
				next_state = S1 ;
				if ( money != 8'd0 )
					$display( "coin: %d, total: %d dollars", money, sum ) ;
			end
			
		end
		S1 : // 看有沒有選飲料-->沒選(要再投錢),選了(給飲料)
		begin
			if ( drink == 8'd0 ) begin
				keepmoney = 1'b1 ;
				next_state = S0 ;
			end
			else begin
				next_state = S2 ;
				keepmoney = 1'b0 ; // 選飲料-->花錢了!
			end
		end
		S2 :
			next_state = S3 ;
		S3 :
			next_state = S0 ;
	endcase
end

endmodule