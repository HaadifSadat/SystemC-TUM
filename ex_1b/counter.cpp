#include "counter.h"

void counter::count() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //

	cnt_int = 0;

	while(1){
		if(rst_n.read() == 0){
			cnt_int = 0;
		}
		else{
			cnt_int++;

			if(cnt_int > 99){
				cnt_int = 0;
			}
		}
		
		cnt.write(cnt_int);
		wait();
	}

	

	// ####################### UP TO HERE ####################### //
}
