#include <chrono>
#include <iostream>
#include <vector>
#include "matrix.h"

using namespace std;
using namespace std::chrono;

// Measure time of a function 
double measure_time_void(void (*func)(), int repeats = 1) {
    auto start = high_resolution_clock::now();
    for(int i = 0; i < repeats; ++i) func();
    auto end = high_resolution_clock::now();
    return duration_cast<nanoseconds>(end-start).count() / 1e9;
}

// Multiply two matrices
Matrix multiply_temp1, multiply_temp2, multiply_result;
int multiply_N;
void multiply_function() {
    multiply_result = multiply_temp1 * multiply_temp2;
}

// Copy constructor
Matrix copy_big;
Matrix copy_temp;
int copy_N;
void copy_function() {
    Matrix copy = copy_big;
}

// Move constructor
Matrix move_big;
Matrix move_temp;
Matrix move_result;
int move_N;
void move_function() {
    move_temp = move_big;
    move_result = std::move(move_temp);
}

// Transpose
Matrix transpose_temp;
Matrix transpose_result;
int transpose_N;
void transpose_function() {
    transpose_result = transpose_temp.transpose();
}

// Element access at()
Matrix access_temp;
int access_N;
volatile double sink;
void access_at_function() {
    for(int i=0;i<access_N;i++)
        for(int j=0;j<access_N;j++)
            sink += access_temp.at(i,j);
}

// Element access operator()
void access_op_function() {
    for(int i=0;i<access_N;i++)
        for(int j=0;j<access_N;j++)
            sink += access_temp(i,j);
}

int main() {
	cout << "===== MATRIX BENCHMARK SUITE =====\n";

	vector<int> sizes = {100, 500, 1000};


	for(int N : sizes) {
		multiply_N = N;
		multiply_temp1 = Matrix(N,N,1.0);
		multiply_temp2 = Matrix(N,N,2.0);
		multiply_result = Matrix(N,N);

	// Warmup
	for(int i=0;i<5;i++) multiply_result = multiply_temp1 * multiply_temp2;

	double t = measure_time_void(multiply_function,1);
	cout << "Matrix Multiplication (" << N << "x" << N << "): " << t << " s\n";
	}

	copy_N = 1000;
	copy_big = Matrix(copy_N, copy_N, 1.0);
	double t_copy = measure_time_void(copy_function, 10) / 10;
	cout << "Copy Constructor (" << copy_N << "x" << copy_N << "): " << t_copy << " s\n";

	move_N = 1000;
	move_big = Matrix(move_N, move_N, 1.0);
	double t_move = measure_time_void(move_function, 10) / 10;
	cout << "Move Constructor (" << move_N << "x" << move_N << "): " << t_move << " s\n";


	for(int N : sizes) {
		transpose_N = N;
		transpose_temp = Matrix(N,N,1.0);
		double t = measure_time_void(transpose_function,1);
		cout << "Transpose (" << N << "x" << N << "): " << t << " s\n";
	}


	for(int N : sizes) {
		access_N = N;
		access_temp = Matrix(N,N,1.0);
		sink = 0;

		double t_at = measure_time_void(access_at_function,1);
		double t_op = measure_time_void(access_op_function,1);
		cout << "Element access (" << N << "x" << N << ") at(): " << t_at
	<< " s, operator(): " << t_op << " s\n";
	}

	cout << "===== BENCHMARK COMPLETE =====\n";
	return 0;
}
