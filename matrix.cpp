// SPDX-License-Identifier: MIT

#include <iostream>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>
#include <iomanip>
#include <sstream>
#include <cmath>
#include "matrix.h"


constexpr std::size_t BLOCK = 32; 
constexpr std::size_t FLUSH_SIZE = 128*1024;

Matrix::Matrix() : r(0), c(0),ptr(nullptr){}

Matrix::Matrix(const int row, const int col){
	if(row<=0 || col<=0){
		throw std::invalid_argument("Dimensions must be positive");
	}
	r = row;
	c = col;
	ptr= new double[r*c]();

}

Matrix::Matrix(const int row,const int col, const double val){
	if(row<=0 || col<=0){
		throw std::invalid_argument("Dimensions must be positive");
	}
	r=row;
	c= col;
	ptr= new double[r*c];
	std::fill(ptr, ptr+r*c,val);
	
}

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> ls){
	
	//checking for empty ls
	
	if(!ls.size()){throw std::invalid_argument("Empty list provided.");}
	r = ls.size();
	auto beg = ls.begin(), en = ls.end();

	//checking for empty column
	
	if(!beg->size()){throw std::invalid_argument("Dimensions must be positive");}
	c = beg->size();
	ptr = new double[c*r];
	int d = 0;
	while(beg!=en){

		//checks row sizes
		
		if(c!=beg->size()){
			
			//cleaning resources before throwing.
			
			delete[] ptr;
			ptr = nullptr;
			throw std::invalid_argument("All rows must have same size.");
		}

		for(double i : *beg){
			ptr[d] = i;
			++d;
		}
		++beg;
	}
}


Matrix::Matrix(const Matrix& m) :r(m.r), c(m.c), ptr(new double[r*c]){
	std::size_t sz = r*c;
	for(std::size_t i = 0;i< sz ;++i){
		ptr[i]=m.ptr[i];
	}
}

Matrix::Matrix(const int row ,const int col, std::initializer_list<double> ls){
	if(row<=0 || col<=0){
		throw std::invalid_argument("Dimensions must be positive");
	}else if((std::size_t)(row*col)!=ls.size()){
		throw std::invalid_argument("Not enough values.");
	}
	r = row;
	c = col;
	ptr = new double[r*c];
	std::size_t i = 0;
	for(double d : ls){
		ptr[i] = d;
		++i;
	}
}

//Destructor
Matrix::~Matrix(){
	if(ptr){
		delete[] ptr;
	}
}

//Move constructor
Matrix::Matrix(Matrix &&m)noexcept: r(m.r), c(m.c), ptr(m.ptr){m.ptr = nullptr; m.r=0;m.c=0;}

//copy and move assignment operator
Matrix &Matrix::operator=(Matrix rhs){
	this->swap(rhs);
	return *this;
}

void Matrix::swap(Matrix &m2)noexcept{
	using std::swap;
	swap(ptr, m2.ptr);
	swap(r, m2.r);
	swap(c,m2.c);
}

double &Matrix::at(const int i, const int j){
	check(i,j);
	return ptr[c*i+j];
}

const double &Matrix::at(const int i,const int j)const{
	check(i,j);
	return ptr[c*i+j];
}

void Matrix::check(const int i , const int j)const{
	if(r==0 && c==0){
		throw std::out_of_range("Empty Matrix");
	}else if(i<0 || j < 0){
		throw std::out_of_range("Index cant be negative");
	}else if(i>=(int)r || j>=(int)c){
		throw std::out_of_range("Out of range i or j");
	}
}

Matrix Matrix::hadamard(const Matrix &m)const{
	if(r!=m.r || c!=m.c){
		throw std::invalid_argument("Dimensions must match for hadamard product.");
	}
	Matrix result(r,c);
	std::size_t sz = size();
	for(std::size_t i = 0;i<sz; ++i){
		result.ptr[i] = m.ptr[i]*ptr[i];
	}
	return result;
}

std::ostream &operator<<(std::ostream& out, const Matrix &m){
	std::ostringstream buf("");

	//setting up the format
	buf.setf(std::ios::fixed);
	buf.precision(2);

	buf<<"\nMatrix("<<m.r<<","<<m.c<<"): \n";
	if(m.size()==0){out<<"[empty]";return out;}
	for(std::size_t i = 0 ; i<m.r;++i){
		buf<<"[";
		for(std::size_t j = 0; j<m.c;++j){
			buf<<std::setw(7)<<m.ptr[i*m.c + j];
			if(j!= m.c-1){buf<<", ";}
		}
		buf<<"]\n";
		if((std::size_t)buf.tellp()>= FLUSH_SIZE){
			out<<buf.str();
			buf.str("");
			buf.clear();
		}
	}
	buf<<"\n";
	out<<buf.str();
	return out;
}

void Matrix::resize(const int i , const int j){
	if(i <= 0 || j<=0){
		throw std::invalid_argument("Dimensions must be positive");
	}

	auto nptr = new double[i*j];
	for(std::size_t row = 0; row<(std::size_t)i;++row){
		for(std::size_t col = 0; col<(std::size_t)j;++col){
			if(row<r && col<c){
				nptr[row*j+col] = ptr[row*c+col];
			}else{
				nptr[row*j+col] = 0;
			}
		}
	}

	//freeing previous state
	
	delete[] ptr;
	ptr = nptr;
	r = i;
	c= j;
}

Matrix Matrix::submatrix(const int row_start, const int row_end, const int col_start, const int col_end)const{
	if(row_start>=(int)r || col_start>=(int)c || row_start>=row_end || col_start>=col_end)
		throw std::invalid_argument("Invalid row/column range");
	else if(row_end > (int)r || col_end > (int)c)
		throw std::invalid_argument("Submatrix range out of bounds");
	else if(row_start<0 || col_start<0)
		throw std::invalid_argument("Invalid row/column range");

	Matrix m(row_end - row_start, col_end -col_start);
	for(int i = row_start; i<row_end;++i){
		for(int j = col_start; j<col_end;++j){
			m.ptr[(i-row_start)*m.c+j-col_start] = ptr[i*c+j];
		}
	}

	return m;

}

void Matrix::apply(func f){
	std::size_t sz  = size();
	for(std::size_t i = 0; i< sz; ++i){
		ptr[i] = f(ptr[i]);
	}
}

Matrix Matrix::map(func f){
	Matrix m = *this;
	m.apply(f);
	return m;
}

bool operator==(const Matrix &lhs,const Matrix &rhs){
	if(rhs.rows() != lhs.rows() || lhs.cols() != rhs.cols())
		return false;
	std::size_t sz = lhs.size();
	for(std::size_t i = 0;i<sz;++i){
		if(std::abs(rhs.ptr[i]-lhs.ptr[i])>=epsilon){
			return false;
		}
	}
	return true;

}

bool operator!=(const Matrix &lhs, const Matrix &rhs){
	return !(lhs==rhs);
}
Matrix Matrix::get_col(const int j)const{
	Matrix m = submatrix(0,r,j,j+1);
	return m;
}

Matrix Matrix::get_row(const int i)const{
	Matrix m = submatrix(i,i+1,0,c);
	return m;
}

Matrix Matrix::Identity(const int dim){
	if(dim<=0)
		throw std::invalid_argument("Identity matrix size must be positive");
	Matrix m(dim, dim);
	for(int i = 0; i<dim; ++i){
		m.ptr[i*m.c+i]=1;
	}
	return m;
}


double Matrix::Determinant()const{

	if(empty())
		throw std::invalid_argument("Cannot compute determinant of empty matrix.");
	if(c!=r)
		throw std::invalid_argument("Determinant requires square matrix");
	if(r>3)
		throw std::invalid_argument("Determinant only implemented for 2x2 and 3x3 matrices");
	if(r==1)
		return ptr[0];
	if(r==2){

		//determinant formula of 2x2 matrix
		
		return ptr[0]*ptr[3] - ptr[2]*ptr[1];
	}

	//Determinant formula of 3x3 matrix.

	return ptr[0]*(ptr[4]*ptr[8]-ptr[5]*ptr[7])-ptr[1]*(ptr[3]*ptr[8] - ptr[5]*ptr[6])+ptr[2]*(ptr[3]*ptr[7]-ptr[4]*ptr[6]);
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs){
	Matrix m = lhs;
	m+=rhs;
	return m;
}

Matrix& Matrix::operator+=(const Matrix& rhs){
	if(r!=rhs.rows() || c!=rhs.cols())
		throw std::invalid_argument("Matrix dimensions must match for addition");
	std::size_t sz = r*c;
	for(std::size_t i = 0; i<sz; ++i){
		ptr[i]+=rhs.ptr[i];
	}
	return *this;
}
Matrix operator-(const Matrix& lhs, const Matrix& rhs){
	Matrix m = lhs;
	m-=rhs;
	return m;
}
Matrix operator-(const Matrix& rhs){
	Matrix m(rhs);
	std::size_t sz = rhs.size();
	for(std::size_t i = 0; i<sz; ++i){
		m.ptr[i]=-m.ptr[i];
	}
	return m;
}


Matrix& Matrix::operator-=(const Matrix& rhs){
	if(r!=rhs.rows() || c!=rhs.cols())
		throw std::invalid_argument("Matrix dimensions must match for addition");
	std::size_t sz = r*c;
	for(std::size_t i = 0; i<sz; ++i){
		ptr[i]-=rhs.ptr[i];
	}
	return *this;
}
Matrix operator*(const Matrix& lhs, const Matrix& rhs){
	if(lhs.cols()!=rhs.rows())
		throw std::invalid_argument("Matrix dimensions must match for addition");
	
	Matrix result(lhs.rows(), rhs.cols());
	Matrix::multiply(lhs,rhs,result);
	return result;
}
void Matrix::multiply(const Matrix &lhs, const Matrix &rhs,Matrix &c){
	Matrix rhsT = rhs.transpose();
	blocked_kernel(lhs, rhsT,c);
}
void Matrix::blocked_kernel(const Matrix &lhs, const Matrix &rhs,Matrix &c){


	for(std::size_t ii=0;ii<lhs.r;ii+=BLOCK){
		for(std::size_t jj = 0;jj<rhs.r;jj+=BLOCK){
			for(std::size_t kk=0;kk<lhs.c;kk+=BLOCK){
				for(std::size_t i = ii; i<std::min(ii+BLOCK,lhs.r);++i){
					for(std::size_t j = jj; j<std::min(jj+BLOCK, rhs.r);++j){
						double sum = c.at(i,j);
						const double *a = &lhs.at(i,kk);
						const double *b=&rhs.at(j,kk);
						for(std::size_t k = kk;k<std::min(BLOCK,lhs.c - kk);++k){
							sum+=a[k]*b[k];
						}
						c.at(i,j)=sum;
					}
				}
			}
		}
	}


	
}

Matrix& Matrix::operator*=(const Matrix& rhs){

	Matrix result(r,rhs.c);
	Matrix::multiply(*this, rhs, result);
	this->swap(result);
	return *this;
	
}

Matrix operator*(const Matrix& m, double num){
	Matrix result(m.rows(),m.cols());
	std::size_t sz = m.size();
	for(std::size_t i = 0; i < sz; ++i){
		result.ptr[i] = m.ptr[i]*num;
	}
	return result;
}
Matrix operator*(double i, const Matrix& m){
	return operator*(m,i);
}

Matrix operator/(Matrix &m, double d){
	if(abs(d)<=epsilon)
		throw std::runtime_error("Cannot divide by zero");
	Matrix result = m;
	result/=d;
	return result;
}

Matrix &Matrix::operator/=(double d){
	std::size_t sz = size();
	for(std::size_t i = 0; i<sz;++i){
		ptr[i]=ptr[i]/d;
	}
	return *this;
}

double Matrix::operator()(int i, int j){
	check(i,j);
	return ptr[i*c+j];
}

double Matrix::operator()(int i, int j)const{
	check(i,j);
	return ptr[i*c+j];
}


Matrix Matrix::transpose()const{
	if(empty()){return *this;}
	Matrix m(c,r);
	for(std::size_t i = 0; i<c;++i){
		for(std::size_t j = 0; j<r;++j){
			m.ptr[i*r+j]=ptr[j*c+i];
		}
	}
	return m;
}

Matrix Matrix::inverse()const{
	if(r!=c || c!=2){
		throw std::invalid_argument("Inverse only implement for 2x2 matrices");
	}
	double d = Determinant();
	if( abs(d)< 1e-10 ){
		throw std::runtime_error("Matrix is singular cannot invert.");
	}

	//Inverse of 2*2 matrix
	Matrix m(r,c, {ptr[3],-1*ptr[1],-1*ptr[2],ptr[0]});
	m = m/d;
	return m;

}
