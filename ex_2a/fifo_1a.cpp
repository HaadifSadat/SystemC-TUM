#include <iomanip>
#include "fifo_1a.h"

fifo_1a::fifo_1a(sc_module_name name, unsigned int fifo_size) : fifo_size(fifo_size) {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// register processes
	SC_METHOD(write_fifo);
	sensitive << clk.pos();

	SC_METHOD(read_fifo);
	sensitive << clk.pos();
	// ####################### UP TO HERE ####################### //

	SC_METHOD(set_flags);
	sensitive << rd_ptr << wr_ptr;

	// initialization of member variables/ports
	fifo_data = new unsigned char[fifo_size];
	rd_ptr.write(0);
	wr_ptr.write(0);
	fill_level = 0;

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// initialize output ports
	full.initialize(0);
	empty.initialize(1);
	d_out.initialize(0);
	// ####################### UP TO HERE ####################### //
}

void fifo_1a::write_fifo() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	if(wr_en.read() == true && full.read() == false) {

		fifo_data[wr_ptr.read()] = d_in.read();

		cout << std::setw(9) << sc_time_stamp()
			 << ": '" << name()
			 << "'\twriting "
			 << (int)d_in.read()
			 << " to FIFO at position "
			 << wr_ptr.read()
			 << endl;

		wr_ptr.write((wr_ptr.read() + 1) % fifo_size);

		fill_level++;
	}
	// ####################### UP TO HERE ####################### //
}

void fifo_1a::read_fifo() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	if(rd_en.read() == true && empty.read() == false) {

		unsigned char value;

		value = fifo_data[rd_ptr.read()];

		d_out.write(value);

		cout << std::setw(9) << sc_time_stamp()
			 << ": '" << name()
			 << "'\treading "
			 << (int)value
			 << " from FIFO at position "
			 << rd_ptr.read()
			 << endl;

		rd_ptr.write((rd_ptr.read() + 1) % fifo_size);

		fill_level--;
	}
	// ####################### UP TO HERE ####################### //
}

void fifo_1a::set_flags() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// set 'full' & 'empty' flags according to fill level
	full.write(fill_level == fifo_size);

	empty.write(fill_level == 0);
	// ####################### UP TO HERE ####################### //
}
