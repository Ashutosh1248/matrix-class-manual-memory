#include <iostream>
#include <stdexcept>
#include "matrix.h"
using std::cout;
int main(){
	try{
		Matrix m({{1,2,3}, {4,5,6},{7,8,9}});
		cout<<"size: "<<m.size()<<" rows: "<<m.rows()<<" cols: "<<m.cols();
		Matrix m1(m);
		getinfo(m1);
		getinfo(m);
	}catch(const std::exception &e){
		cout<<"\nError: "<<e.what()<<" !!.";
	}



	return 0;
}
