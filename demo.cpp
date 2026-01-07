#include <iostream>
#include "matrix.h"

int main(){
	
	Matrix A(3,3,1.0);
	Matrix B({{1,2,3},{4,5,6},{7,8,9}});

	Matrix C=A*B;

	Matrix D = C.transpose();

	std::cout<<"Matrix A:\n"<<A;
	std::cout<<"Matrix B:\n"<<B;
	std::cout<<"A * B = C:\n"<<C;
	std::cout<<"Transpose(C) = D:\n"<<D;

	std::cout<<"C(0,2) = "<<C(0,2)<<"\n";

	Matrix E = C.hadamard(D);
	std::cout<<"Hadamard(C,D): \n"<<E;

	return 0;
}
