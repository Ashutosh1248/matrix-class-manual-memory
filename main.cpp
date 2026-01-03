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
		Matrix m3 = Matrix::Identity(10);
		Matrix m5(10,10, 800);
		Matrix m6 = m3*m5;
		cout<<m3;
		cout<<m6;
		auto end =std::chrono::high_resolution_clock::now(); 
		std::chrono::duration<double, std::milli> duration = end-start;
		cout<<"\nTime taken: "<<duration.count()<<" miilliseconds.";

		return EXIT_SUCCESS;
	}catch(const std::exception &e){
		cout<<"\nError: "<<e.what()<<" !!";
	}
	return EXIT_FAILURE;
}
