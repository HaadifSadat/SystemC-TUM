#include <iomanip>
#include "fifo_1b.h"

fifo_1b::fifo_1b(sc_module_name name, unsigned int fifo_size) : fifo_size(fifo_size) {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// register process

	SC_METHOD(read_write_fifo);
	sensitive << clk.pos();
	// ####################### UP TO HERE ####################### //

	// initialization of member variables/ports
	fifo_data = new unsigned char[fifo_size];
	rd_ptr.write(0);
	wr_ptr.write(0);
	fill_level = 0;

	// ############# COMPLETE THE FOLLOWING SECTION ############# //

	d_out.initialize(0);
	full.initialize(0);
	valid.initialize(0);

	// ####################### UP TO HERE ####################### //
}

void fifo_1b::read_write_fifo() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //

	bool write;
	bool read;

	write = wr_en_read() && !full_read();
	read = rd_en_read() && !valid_read();

	if(read == 0 && write == 1){
		if(filllevel > 0){
			fifo_data[wr_ptr.read()] = d_in.read();
			wr_ptr.write((wr_ptr.read() + 1) % fifo_size);
			fill_level++;
		}
		else{
			d_out.write(rd_ptr.read());
			valid.write(1);
		}
	}

	else if(read == 1 && write == 0){
		if(fill_level > 0){
			d_out.write(fifo_data[rd_ptr.read()]);
			rd_ptr.write((rd_ptr.read()+1)%fifo_size);
			fill_level--;
		}
		else{
			valid.write(0);
		}
	}

	else if(read == 1 && write == 1){
		if(fill_level > 0){
		d_out.write(fifo_data[rd_ptr.read()]);
		rd_ptr.read((rd_ptr.read()+1)%fifo_size);

		fifo_data[wr_ptr.read()] = d_in.read();
		wr_ptr.write((wr_ptr.read() + 1) % fifo_size);
		}
		else{
		d_out.write(rd_ptr.read());
		valid_write(1);
		}
	}
	// ####################### UP TO HERE ####################### //
}
