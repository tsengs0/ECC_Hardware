`include "../ecc_define.v"

module hamming_encoder_8bit (
	input [`HAMMING_IN_8-1:0] data_in,
	output wire [`HAMMING_OUT_8-1:0] code_out
);

	wire [`HAMMING_PAR_8-1:0] parity;
	
	assign parity[0] = data_in[0] ^ data_in[1] ^ data_in[3] ^ data_in[4] ^ data_in[6];
	assign parity[1] = data_in[0] ^ data_in[2] ^ data_in[3] ^ data_in[5] ^ data_in[6];
	assign parity[2] = data_in[1] ^ data_in[2] ^ data_in[3];
	assign parity[3] = data_in[4] ^ data_in[5] ^ data_in[6]; 
	assign code_out[`HAMMING_OUT_8-1:0] = {data_in[`HAMMING_IN_8-1:0], parity[`HAMMING_PAR_8-1:0]};
endmodule
