#include "bcd_decoder.h"

void bcd_decoder::decode() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //

	unsigned short int value;

	value = val.read();

	hi.write(value/10);
	lo.write(value%10);

	// ####################### UP TO HERE ####################### //
}
