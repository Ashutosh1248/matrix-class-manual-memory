#include <iostream>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>
#include <iomanip>
#include <sstream>
#include <cmath>
#include "matrix.h"

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
	auto sz = r*c;
	for(int i = 0;i<sz;++i){
		ptr[i]=m.ptr[i];
	}
}

Matrix::Matrix(const int row ,const int col, std::initializer_list<double> ls){
	if(row<=0 || col<=0){
		throw std::invalid_argument("Dimensions must be positive");
	}else if(row*col!=ls.size()){
		throw std::invalid_argument("Not enough values.");
	}
	r = row;
	c = col;
	ptr = new double[r*c];
	int i = 0;
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
Matrix::Matrix(Matrix &&m)noexcept: r(m.r), c(m.c), ptr(m.ptr){m.ptr = nullptr;}

//copy assignment operator
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
	}else if(i>=r || j>=c){
		throw std::out_of_range("Out of range i or j");
	}
}

Matrix Matrix::hadamard(const Matrix &m)const{
	if(r!=m.r || c!=m.c){
		throw std::invalid_argument("Dimensions must match for hadamard product.");
	}
	Matrix result(r,c);
	auto sz = size();
	for(int i = 0;i<sz; ++i){
		result.ptr[i] = m.ptr[i]*ptr[i];
	}
	return result;
}

std::ostream &operator<<(std::ostream& out, const Matrix &m){
	std::ostringstream ou("");
	ou<<std::fixed<<std::right<<std::setprecision(2);
	ou<<"\nMatrix("<<m.r<<","<<m.c<<"): \n";
	if(m.size()==0){out<<"[empty]";return out;}
	for(int i = 0 ; i<m.r;++i){
		ou<<"[";
		for(int j = 0; j<m.c;++j){
			ou<<std::setw(7)<<m.ptr[i*m.c + j];
			if(j!= m.c-1){ou<<", ";}
		}
		ou<<"]\n";
	}
	ou<<std::defaultfloat;
	ou<<"\n";
	out<<ou.str();
	return out;
}

void Matrix::resize(const int i , const int j){
	if(i <= 0 || j<=0){
		throw std::invalid_argument("Dimensions must be positive");
	}
	auto nptr = new double[i*j];
	for(int row = 0; row<i;++row){
		for(int col = 0; col<j;++col){
			if(row<r && col<c){
				nptr[row*j+col] = ptr[row*c+col];
			}else{
				nptr[row*j+col] = 0;
			}
		}
	}
	delete[] ptr;
	ptr = nptr;
	r = i;
	c= j;
}

Matrix Matrix::submatrix(const int row_start, const int row_end, const int col_start, const int col_end)const{
	if(row_start>=r || col_start>=c || row_start>=row_end || col_start>=col_end)
		throw std::invalid_argument("Invalid row/column range");
	else if(row_end > r || col_end > c)
		throw std::invalid_argument("Submatrix range out of bounds");
	Matrix m(row_end - row_start, col_end -col_start);
	auto sz = m.size();
	for(int i = row_start; i<row_end;++i){
		for(int j = col_start; j<col_end;++j){
			m.ptr[(i-row_start)*m.c+j-col_start] = ptr[i*c+j];
		}
	}
	return m;

}

void Matrix::apply(func f){
	auto lambda = [f](double &d){f(d);};
	std::for_each(ptr, ptr+r*c, lambda);
}

Matrix Matrix::map(func f){
	Matrix m = *this;
	m.apply(f);
	return m;
}

bool operator==(const Matrix &lhs,const Matrix &rhs){
	if(rhs.rows() != lhs.rows() || lhs.cols() != lhs.cols())
		return false;
	auto sz = lhs.size();
	for(int i = 0;i<sz;++i){
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
	check(1,j);
	Matrix m = submatrix(0,r,j,j+1);
	return m;
}

Matrix Matrix::get_row(const int i)const{
	check(i,1);
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
	if(c!=r || c>3)
		throw std::invalid_argument("Determinant only implemented for 2 cross 2 and 3 cross 3 matrices");
	else if(this->empty())
		throw std::invalid_argument("Cannot compute determinant of empty matrix");
	if(r==2){
		//determinant formula of 2*2 matrix
		return ptr[0]*ptr[3] - ptr[2]*ptr[1];
	}
	//Determinant formula of 3*3 matrix.

	return ptr[0]*(ptr[4]*ptr[8]-ptr[5]*ptr[7])-ptr[1]*(ptr[3]*ptr[8] - ptr[5]*ptr[6])+ptr[2]*(ptr[3]*ptr[7]-ptr[4]*ptr[6]);
}
