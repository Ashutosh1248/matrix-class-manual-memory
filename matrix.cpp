#include <iostream>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>
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
	auto d = ptr;
	while(beg!=en){

		//checks row sizes
		
		if(c!=beg->size()){
			
			//cleaning resources before throwing.
			
			delete[] ptr;
			ptr = d = nullptr;
			throw std::invalid_argument("All rows must have same size.");
		}

		for(double i : *beg){
			*d = i;
			++d;
		}
		++beg;
	}
}


Matrix::Matrix(const Matrix& m) :r(m.r), c(m.c), ptr(new double[r*c]){
	auto d = ptr;
	auto md=m.ptr;
	auto sz = r*c;
	for(int i = 0;i<sz;++i){
		*d=*md;
		++md;
	}
	std::cout<<"\ncopied\n";
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
	auto d = ptr;
	for(double i : ls){
		*d = i;
		++d;
	}
}

//Destructor
Matrix::~Matrix(){
	std::cout<<"\n in destru\n";
	if(ptr){
		delete[] ptr;
	}
}

//Move constructor
Matrix::Matrix(Matrix &&m) : r(m.r), c(m.c), ptr(m.ptr){m.ptr = nullptr;}

//copy assignment operator
Matrix &Matrix::operator=(Matrix rhs){
	swap(*this, rhs);
	std::cout<<"\nIn copy assig\n";
	return *this;
}

void Matrix::swap(Matrix& m1, Matrix &m2){
	using std::swap;
	swap(m1.ptr, m2.ptr);
	swap(m1.r, m2.r);
	swap(m1.c,m2.c);
}

double &Matrix::at(int i, int j){
	check(i,j);
	return ptr[r*i+j];
}

const double &Matrix::at(int i, int j)const{
	check(i,j);
	return ptr[r*i+j];
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
