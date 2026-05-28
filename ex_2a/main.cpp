#include "consumer.h"
#include "producer.h"
#include "fifo_1a.h"

int sc_main(int argc, char *argv[]) {
	// the following instruction generates a clock signal with clock named
	// "clock" with a period of 100 ns, a duty cycle of 50%, and a falling edge
	// after 50 ns
	sc_clock clk("clock", 100, SC_NS, 0.5, 50, SC_NS, false);

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// fill in the required commands to instantiate and connect producer, fifo,
	// and consumer

	// signals
	sc_signal<unsigned char> prod_fifo_data;
	sc_signal<unsigned char> fifo_cons_data;

	sc_signal<bool> wr_en_sig;
	sc_signal<bool> rd_en_sig;

	sc_signal<bool> full_sig;
	sc_signal<bool> empty_sig;

	// instantiate modules
	producer prod("producer");

	fifo_1a fifo("fifo");

	consumer cons("consumer");

	// connect producer
	prod.clk(clk);
	prod.d_out(prod_fifo_data);
	prod.wr_en(wr_en_sig);
	prod.full(full_sig);

	// connect fifo
	fifo.clk(clk);

	fifo.d_in(prod_fifo_data);
	fifo.wr_en(wr_en_sig);
	fifo.full(full_sig);

	fifo.d_out(fifo_cons_data);
	fifo.rd_en(rd_en_sig);
	fifo.empty(empty_sig);

	// connect consumer
	cons.clk(clk);
	cons.d_in(fifo_cons_data);
	cons.rd_en(rd_en_sig);
	cons.empty(empty_sig);
	// ####################### UP TO HERE ####################### //

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// fill in code to generate traces that can be used to observe the
	// functionality of the model with the waveform viewer gtkwave
	sc_trace_file *tf;

	tf = sc_create_vcd_trace_file("traces");

	// trace signals
	sc_trace(tf, clk, "clk");

	sc_trace(tf, prod_fifo_data, "prod_fifo_data");
	sc_trace(tf, fifo_cons_data, "fifo_cons_data");

	sc_trace(tf, wr_en_sig, "wr_en");
	sc_trace(tf, rd_en_sig, "rd_en");

	sc_trace(tf, full_sig, "full");
	sc_trace(tf, empty_sig, "empty");

	// trace internal signals
	sc_trace(tf, prod.send, "producer_send");

	sc_trace(tf, cons.fetch, "consumer_fetch");
	sc_trace(tf, cons.data_valid, "data_valid");

	// trace fifo internals
	sc_trace(tf, fifo.fill_level, "fill_level");
	// ####################### UP TO HERE ####################### //

	sc_time sim_dur = sc_time(5000, SC_NS);
	if(argc != 2) {
		cout << "Default simulation time = " << sim_dur << endl;
	}
	else {
		sim_dur = sc_time(atoi(argv[1]), SC_NS);
		cout << "Simulation time = " << sim_dur << endl;
	}

	// start simulation
	sc_start(sim_dur);

	sc_close_vcd_trace_file(tf);

	return 0;
}
