`include "../ecc_define.v"
module hamming_tb;

reg [`HAMMING_IN_8-1:0] data_in;
wire [`HAMMING_OUT_8-1:0] encoded_data;

wire [`HAMMING_IN_8-1:0] decoded_data;
wire error_flag; // if there is 1-bit error, signaling '1' otherwise '0'
wire correct_flag; // Singaling '1' whilst error correction is done 
reg sys_clk;
reg rstn;
reg [`HAMMING_IN_8-1:0] received_data;

initial begin
	$dumpfile("hamming_tb.vcd");
	$dumpvars;
	$monitor("%t: 0x%h => 0x%h | %b => %b", $time, data_in, encoded_data, data_in, encoded_data);
end

hamming_encoder_8bit u0 (
	.data_in (data_in),
	.code_out(encoded_data)
);

hamming_decoder_8bit u1 (
	.data_out     (decoded_data),
	.error_flag   (error_flag), 
	.correct_flag (correct_flag), 
	.data_in      ({received_data[`HAMMING_IN_8-1:0], encoded_data[`HAMMING_PAR_8-1:0]}),
	.sys_clk      (sys_clk),
	.rstn         (rstn)
);

initial begin
	#0 rstn <= 1'b1;
	
	repeat (5) begin
		#1 rstn <= 1'b0;
		#1 rstn <= 1'b1;
		#8;
	end
end

initial begin
	#0 sys_clk <= 1'b0;
	forever #5 sys_clk <= ~sys_clk;
end

// Simulating the 1-bit error occurance
integer bit_id; // randomly deciding one bit to be flipped
initial begin
	$srandom(`HAMMING_IN_8); // set the seed
	#0 received_data <= 'd0;
	
	repeat (5) begin
		#2 received_data <= encoded_data[`HAMMING_OUT_8-1:`HAMMING_PAR_8]; 
		//(SystemVerilog) #1 bit_id <= $urandom_range(`HAMMING_IN_8-1:0);
		#1 bit_id <= {$random} % `HAMMING_IN_8;
		#1 received_data[bit_id] <= received_data[bit_id] ^ 1'b1; // error occur
		#6; 
	end
end

initial begin
	#0  data_in <= 'h00;
	#10 data_in <= 'h0A;
	#10 data_in <= 'hFF;
	#10 data_in <= 'hA0;
	#10 data_in <= 'h1B;
	#10; $finish;
end
endmodule
