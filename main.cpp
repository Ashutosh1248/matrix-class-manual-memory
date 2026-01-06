#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <chrono>
#include "matrix.h"
using std::cout;
using std::vector;
using Clock = std::chrono::steady_clock;
double cube(double d){ return d*d*d;}
#ifdef _WIN32
#include <windows.h>
#endif
int main(){
	
#ifdef _WIN32
	SetThreadAffinityMask(GetCurrentThread(),1);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
#endif
	constexpr int  WARMUP= 50;
	constexpr int RUNS= 1000;
	constexpr int ROUNDS = 7;
	auto sink1 = Matrix(100,100,12.7676);
	auto sink2 = Matrix(100,100,4.2423);
	Matrix sink3;
	for(int i  = 0; i< WARMUP;++i){
		auto sink3 = sink1*sink2;
	}

	std::vector<long long> samples;
	for(int r = 0; r<ROUNDS; ++r){
		auto t1 = Clock::now();
		for(int i = 0; i<RUNS; ++i){
		auto sink3 = sink1*sink2;
		}
		auto t2 = Clock::now();
		auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count(); 
		samples.push_back(ns);
	}

	std::sort(samples.begin(), samples.end());
	long long median = samples[samples.size()/2];
	cout<<"Median total time: "<<median<<" ns\n";
	cout<<"per call: "<<(double)median/RUNS<<" ns\n";
	
		/*
		auto t1 = Clock::now();
	Matrix m(2,4,{3,2,1,5,9,1,3,0});
	Matrix m1(4,3,{2,9,0,1,3,5,2,4,7,8,1,5});
	cout<<m*m1;
		auto t2 = Clock::now();
		auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count(); 
	cout<<"per call: "<<(double)ns/1000000<<" ns\n";
	*/
		//return EXIT_FAILURE;
}
