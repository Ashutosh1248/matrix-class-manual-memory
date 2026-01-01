#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <chrono>
#include "matrix.h"
using std::cout;
int main(){
	
	try{
		auto start = std::chrono::high_resolution_clock::now();
		Matrix m1(3,3,{1,2,3,4,5.8989,6,7,8,9});
		Matrix m2(3,3,{4,2,1,10,2,1.55550,2,9,2});

		auto m3 = m1;
		cout<<"m3: \n"<<m3;
		m3 = m2;
		cout<<"m3: \n"<<m3;
		cout<<"m1: \n"<<m1;
		m3.swap(m1);
		cout<<"m1: \n"<<m1;
		cout<<"m3: \n"<<m3;
		auto end =std::chrono::high_resolution_clock::now(); 
		std::chrono::duration<double, std::milli> duration = end-start;
		cout<<"Time taken: "<<duration.count()<<" miilliseconds.";

		return EXIT_SUCCESS;
	}catch(const std::exception &e){
		cout<<"\nError: "<<e.what()<<" !!";
	}
	return EXIT_FAILURE;
}
