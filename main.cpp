#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include "matrix.h"
using std::cout;
int main(){
	
	try{
		Matrix m1(2,3);
		cout<<"\nm1: ";
		getinfo(m1);
		Matrix m2(std::move(m1));
		cout<<"\nm2: ";
		getinfo(m2);
		cout<<"\nm1: ";
		getinfo(m1);
		cout<<m1.size()<<" "<<m2.size();
		return EXIT_SUCCESS;
	}catch(const std::exception &e){
		cout<<"\nError: "<<e.what()<<" !!.";
	}
	

	return 1;
}
