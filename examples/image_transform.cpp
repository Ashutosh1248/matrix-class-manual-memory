#include <iostream>
#include "matrix.h"

int main(){
	Matrix image({
		{1,2,3},
		{4,5,6},
		{7,8,9}
	});

	Matrix kernel({
		{0,-1,0},
		{-1,5,-1},
		{0,-1,0}		
	});

	Matrix result = image.hadamard(kernel);
	std::cout<<result;

	return 0;
}
