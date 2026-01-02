#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <chrono>
#include "matrix.h"
using std::cout;
void cube(double &d){ d=d*d*d;}
int main(){
	
	try{
		auto start = std::chrono::high_resolution_clock::now();
		Matrix m3(3,3);

		Matrix m4(2,2,{3,0,0,3});
		cout<<m4;
		Matrix m5 = Matrix::Identity(3);
		Matrix m6 = m5;
		cout<<m3.Determinant();
		auto end =std::chrono::high_resolution_clock::now(); 
		std::chrono::duration<double, std::milli> duration = end-start;
		cout<<"\nTime taken: "<<duration.count()<<" miilliseconds.";

		return EXIT_SUCCESS;
	}catch(const std::exception &e){
		cout<<"\nError: "<<e.what()<<" !!";
	}
	return EXIT_FAILURE;
}
