#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <initializer_list>
class Matrix{
	friend inline void getinfo(const Matrix &m);
	public:
		Matrix();
		Matrix(int row, int col);
		Matrix(int r, int c, double val);
		Matrix(std::initializer_list<std::initializer_list<double>> ls);
		Matrix(const Matrix& m);
		inline std::size_t rows();
		inline std::size_t cols();
		inline std::size_t size();
	private:
		std::size_t r;
		std::size_t c;
		double *ptr;
};



inline std::size_t Matrix::rows(){return r;}
inline std::size_t Matrix::cols(){return c;}
inline std::size_t Matrix::size(){return r*c;}
inline void getinfo(const Matrix &m){std::cout<<"\n"<<m.ptr<<"\n";}
#endif
