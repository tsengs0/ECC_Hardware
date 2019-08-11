SIMPRG   = vcs -full64 -v2005
SIM_FILE = hamming_tb
SOURCES  = src/hamming_encoder_8bit.v src/hamming_decoder_8bit.v ../ecc_define.v src/hamming_tb.v

all:
	$(SIMPRG) $(SOURCES) -o $(SIM_FILE)

run:
	./${SIM_FILE}

run_vis:
	./${SIM_FILE}
	gtkwave ./${SIM_FILE}.vcd

clean:
	rm -rf $(SIM_FILE) $(SIM_FILE).daidir csrc ucli.key $(SIM_FILE).vcd
