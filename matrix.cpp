#include <iostream>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>
#include <iomanip>
#include "matrix.h"

Matrix::Matrix() : r(0), c(0),ptr(nullptr){}

Matrix::Matrix(int row, int col){
	if(row<=0 || col<=0){
		throw std::invalid_argument("Dimensions must be positive");
	}
	r = row;
	c = col;
	ptr= new double[r*c]();

}

Matrix::Matrix(int row, int col, double val){
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

Matrix::Matrix(int row , int col, std::initializer_list<double> ls){
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
Matrix::Matrix(Matrix &&m) : r(m.r), c(m.c), ptr(m.ptr){m.ptr = nullptr;}

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

double &Matrix::at(int i, int j){
	check(i,j);
	return ptr[c*i+j];
}

const double &Matrix::at(int i, int j)const{
	check(i,j);
	return ptr[c*i+j];
}

void Matrix::check(int i , int j)const{
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
	out<<std::fixed<<std::right<<std::setprecision(2);
	out<<"\nMatrix("<<m.r<<","<<m.c<<"): \n";
	if(m.size()==0){out<<"[empty]";return out;}
	for(int i = 0 ; i<m.r;++i){
		out<<"[";
		for(int j = 0; j<m.c;++j){
			out<<std::setw(7)<<m.ptr[i*m.c + j];
			if(j!= m.c-1){out<<", ";}
		}
		out<<"]\n";
	}
	out<<std::defaultfloat;
	out<<"\n";
	return out;
}

void Matrix::resize(int i , int j){
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
