#include <iostream>
#include "matrix.h"


int main(){

	Matrix x({{1,1},{1,2},{1,3},{1,4}});
	Matrix y({{6},{8},{10},{12}});
	
	Matrix xt = x.transpose();
	Matrix theta = (xt*x).inverse()*xt*y;
	
	std::cout<<"Theta:\n"<<theta;
	
	return 0;
}
