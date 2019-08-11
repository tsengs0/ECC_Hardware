`include "../ecc_define.v"

module hamming_decoder_8bit (
	output reg [`HAMMING_IN_8-1:0] data_out,
	output error_flag, // if there is 1-bit error, signaling '1' otherwise '0'
	output reg correct_flag, // Singaling '1' whilst error correction is done 
	input [`HAMMING_OUT_8-1:0] data_in,
	input sys_clk,
	input rstn
);

	reg [`HAMMING_PAR_8-1:0] error_detection;
	wire [`HAMMING_IN_8-1:0] mask_byte;
	// Detect the error bit at rising edge of clock
	always @(posedge sys_clk, negedge rstn)
	begin
		if(rstn == 1'b0) 
			error_detection[`HAMMING_PAR_8-1:0] <= `HAMMING_PAR_8'd0;
		else begin
			error_detection[0] <= data_in[0] ^ data_in[0+`HAMMING_PAR_8] ^ data_in[1+`HAMMING_PAR_8] ^ data_in[3+`HAMMING_PAR_8] ^ data_in[4+`HAMMING_PAR_8] ^ data_in[6+`HAMMING_PAR_8]; 
			error_detection[1] <= data_in[1] ^ data_in[0+`HAMMING_PAR_8] ^ data_in[2+`HAMMING_PAR_8] ^ data_in[3+`HAMMING_PAR_8] ^ data_in[5+`HAMMING_PAR_8] ^ data_in[6+`HAMMING_PAR_8]; 
			error_detection[2] <= data_in[2] ^ data_in[1+`HAMMING_PAR_8] ^ data_in[2+`HAMMING_PAR_8] ^ data_in[3+`HAMMING_PAR_8];
			error_detection[3] <= data_in[3] ^ data_in[4+`HAMMING_PAR_8] ^ data_in[5+`HAMMING_PAR_8] ^ data_in[6+`HAMMING_PAR_8]; 
		end			
	end

	// Correct the error bit at falling edge of clock
	always @(negedge sys_clk, negedge rstn)
	begin
		if(rstn == 1'b0) begin 
			data_out[`HAMMING_IN_8-1:0] <= {`HAMMING_IN_8{1'bz}};
			correct_flag <= 1'b0;
		end
		else begin
			data_out[`HAMMING_IN_8-1:0] <= 	data_in[`HAMMING_OUT_8-1:`HAMMING_PAR_8] ^ mask_byte[`HAMMING_IN_8-1:0];
			correct_flag <= 1'b1;
		end
	end
	
	assign mask_byte[`HAMMING_IN_8-1:0] =  (error_detection == 'd3 ) ? 'd1   : 
					       (error_detection == 'd5 ) ? 'd2   :
					       (error_detection == 'd6 ) ? 'd4   :
					       (error_detection == 'd7 ) ? 'd8   :
					       (error_detection == 'd9 ) ? 'd16  :
					       (error_detection == 'd10) ? 'd32  :
					       (error_detection == 'd11) ? 'd64  :
					       (error_detection == 'd12) ? 'd128 : 0;

	assign error_flag = (error_detection[`HAMMING_PAR_8-1:0] == 'd0) ? 1'b0 : 1'b1;
endmodule
