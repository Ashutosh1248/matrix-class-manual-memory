#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <initializer_list>
#include <functional>

inline constexpr double epsilon{1e-9};
//const std::vector<int> index = {0, 4, 8, 5, 7, 1, 3, 8, 5, 6, 2, 3, 7, 4, 6};
using func=std::function<double(double)>;
class Matrix{
	friend inline void getinfo(const Matrix &);

	friend bool operator==(const Matrix &,const Matrix &);
	friend bool operator!=(const Matrix &,const Matrix &);
	friend Matrix operator+(const Matrix&, const Matrix&);
	friend Matrix operator-(const Matrix&, const Matrix&);
	friend Matrix operator-(const Matrix& rhs);
	friend Matrix operator*(const Matrix&, const Matrix&);
	friend Matrix operator*(const Matrix&, double);
	friend Matrix operator*(double, const Matrix&);
	friend Matrix operator/(Matrix &, double);

	friend std::ostream &operator<<(std::ostream&, const Matrix &);
	public:
		Matrix();
		Matrix(const int,const int);
		Matrix(const int, const int, const double);
		Matrix(std::initializer_list<std::initializer_list<double>> ls);
		Matrix(const int, const int, std::initializer_list<double>);
		Matrix(const Matrix&);
		~Matrix();
		Matrix(Matrix &&)noexcept;
		Matrix &operator=(Matrix);
		void swap(Matrix &)noexcept;
		double &at(const int ,const int );
		const double &at(const int, const int)const;
		Matrix hadamard(const Matrix &)const;
		void resize(const int , const int);
		Matrix submatrix(const int, const int, const int, const int)const;
		void apply(func);
		Matrix map(func);
		Matrix get_row(const int)const;
		Matrix get_col(const int)const;
		static Matrix Identity(const int);
		static void multiply(const Matrix &lhs, const Matrix &rhs,Matrix &c);
		static void blocked_kernel(const Matrix &lhs, const Matrix &rhs,Matrix &c);
		double Determinant()const;
		Matrix transpose()const;
		Matrix inverse()const;
		Matrix& operator+=(const Matrix&);
		Matrix& operator-=(const Matrix&);
		Matrix& operator*=(const Matrix& rhs);
		Matrix &operator/=(double d);
		double operator()(int, int);

		inline void fill(const double);
		inline void clear();
		inline std::size_t rows()const;
		inline std::size_t cols()const;
		inline std::size_t size()const;
		inline bool empty()const;
	private:
		std::size_t r;
		std::size_t c;
		double *ptr;
		void check(const int, const int)const;
};


inline void Matrix::fill(const double val){std::fill(ptr, ptr+r*c,val);}
inline void Matrix::clear(){delete[] ptr; r=c=0;ptr=nullptr;}
inline std::size_t Matrix::rows()const{return r;}
inline std::size_t Matrix::cols()const{return c;}
inline std::size_t Matrix::size()const{return r*c;}
inline bool Matrix::empty()const{return r==0;}
inline void getinfo(const Matrix &m){std::cout<<"\n"<<m.ptr<<"\n";}
#endif
