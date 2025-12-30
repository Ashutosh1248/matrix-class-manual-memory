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
		Matrix(int row , int col, std::initializer_list<double> ls);
		Matrix(const Matrix& m);
		~Matrix();
		Matrix(Matrix &&);

		inline std::size_t rows()const;
		inline std::size_t cols()const;
		inline std::size_t size()const;
	private:
		std::size_t r;
		std::size_t c;
		double *ptr;
};



inline std::size_t Matrix::rows()const{return r;}
inline std::size_t Matrix::cols()const{return c;}
inline std::size_t Matrix::size()const{return r*c;}
inline void getinfo(const Matrix &m){std::cout<<"\n"<<m.ptr<<"\n";}
#endif
