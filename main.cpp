#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include "matrix.h"
using std::cout;
int main(){
	
	try{
		const Matrix m({{1},{2},{3}});
		cout<<m.at(0,0);
		cout<<"\n cols: "<<m.cols()<<"\nrows: "<<m.rows();
		return EXIT_SUCCESS;
	}catch(const std::exception &e){
		cout<<"\nError: "<<e.what()<<" !!";
	}
	return EXIT_FAILURE;
}
