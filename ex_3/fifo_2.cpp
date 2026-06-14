#include <iostream>
#include <iomanip>
#include "fifo_2.h"

fifo_2::fifo_2(sc_module_name name, unsigned int fifo_size) : fifo_size(fifo_size) {
	// initialization of member variables/ports
	fifo_data = new unsigned char[fifo_size];
	wr_ptr = 0;
	rd_ptr = 0;
	fill_level = 0;
}

bool fifo_2::write_fifo(unsigned char *data, unsigned int &count) {

	bool result = false;
	unsigned int len;
	unsigned char *ptr = data;

	if(fill_level + count > fifo_size) {
		len = fifo_size - fill_level;
	}
	else {
		len = count;
		result = true;
	}

	// Copy data into circular buffer
	if(wr_ptr + len <= fifo_size) {
		memcpy(fifo_data + wr_ptr, ptr, len);
	}
	else {
		unsigned int first = fifo_size - wr_ptr;
		unsigned int second = len - first;

		memcpy(fifo_data + wr_ptr, ptr, first);
		memcpy(fifo_data, ptr + first, second);
	}

	// Transaction delay
	wait(len * 100, SC_NS);

	// Debug output
	cout << std::setw(9) << sc_time_stamp()
		 << ": '" << name()
		 << "'\twrote " << len
		 << " byte(s) at address "
		 << wr_ptr << endl;

	// Update FIFO state
	wr_ptr = (wr_ptr + len) % fifo_size;
	fill_level += len;
	count = len;

	if(fifo_size <= 50)
		output_fifo_status();

	return result;
}

bool fifo_2::read_fifo(unsigned char *data, unsigned int &count) {
	bool result = false;

	unsigned int len;
	unsigned char *ptr = data;

	if(fill_level < count) {
		len = fill_level;
	}
	else {
		len = count;
		result = true;
	}

	// Copy data from circular buffer
	if(rd_ptr + len <= fifo_size) {
		memcpy(ptr, fifo_data + rd_ptr, len);
	}
	else {
		unsigned int first = fifo_size - rd_ptr;
		unsigned int second = len - first;

		memcpy(ptr, fifo_data + rd_ptr, first);
		memcpy(ptr + first, fifo_data, second);
	}

	// Transaction delay
	wait(len * 100, SC_NS);

	// Debug output
	cout << std::setw(9) << sc_time_stamp()
		 << ": '" << name()
		 << "'\tread " << len
		 << " byte(s) from address "
		 << rd_ptr << endl;

	// Update FIFO state
	rd_ptr = (rd_ptr + len) % fifo_size;
	fill_level -= len;
	count = len;

	if(fifo_size <= 50)
		output_fifo_status();

	return result;
}

// helper function to output content of FIFO
void fifo_2::output_fifo_status() {
	cout << "\tCurrent status of '" << name() << "': write address: "
			<< wr_ptr << ", read address: " << rd_ptr
			<< ", fill level: " << fill_level << endl;
	cout << "\t";
	cout << hex; // switch to hexadecimal mode;
	if(fill_level == 0) {
		for(unsigned int i = 0; i < fifo_size; i++)
			cout << "-- ";
	}
	else if(fill_level == fifo_size) {
		for(unsigned int i = 0; i < fifo_size; i++)
			cout << std::setw(2) << std::setfill('0') << (int)*(fifo_data + i)
					<< " ";
	}
	else if(wr_ptr > rd_ptr) {
		for(unsigned int i = 0; i < fifo_size; i++) {
			if((i >= rd_ptr) && (i < wr_ptr)) {
				cout << std::setw(2) << std::setfill('0')
						<< (int)*(fifo_data + i) << " ";
			}
			else
				cout << "-- ";
		}
	}
	else if(wr_ptr < rd_ptr) {
		for(unsigned int i = 0; i < fifo_size; i++) {
			if((i >= rd_ptr) || (i < wr_ptr)) {
				cout << std::setw(2) << std::setfill('0')
						<< (int)*(fifo_data + i) << " ";
			}
			else
				cout << "-- ";
		}
	}
	cout << dec << std::setfill(' ') << endl << endl;
}
