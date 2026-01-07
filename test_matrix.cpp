#include <iostream>
#include <cassert>
#include <cmath>
#include <stdexcept>
#include "Matrix.h"

#define EPS 1e-9
#define EXPECT_THROW(stmt, ex)                 \
    try { stmt; assert(false); } catch(const ex&) {}

#define EXPECT_NO_THROW(stmt) stmt

bool nearly(double a, double b) {
    return std::fabs(a-b) < EPS;
}

void test_constructors() {
    Matrix a;
    assert(a.empty());

    Matrix b(3,4);
    assert(b.rows()==3 && b.cols()==4);

    Matrix c(2,2,5.0);
    assert(nearly(c.at(0,0),5));

    Matrix d{{1,2},{3,4}};
    assert(nearly(d.at(1,0),3));

    Matrix e(2,2,{1,2,3,4});
    assert(nearly(e.at(1,1),4));

    Matrix f = d;
    assert(f==d);

    Matrix g = std::move(f);
    assert(g.rows()==2);
    assert(f.empty());
}

void test_assignment() {
    Matrix a(2,2,3);
    Matrix b;
    b=a;
    assert(a==b);

    a = a;
    assert(nearly(a.at(0,0),3));

    Matrix c = std::move(a);
    assert(c.rows()==2);
    assert(a.empty());

    Matrix m1(2,3);
    Matrix m2(2,4);
    assert(m1!=m2);


}

void test_access() {
    Matrix a(3,3);
    EXPECT_THROW(a.at(-1,0), std::out_of_range);
    EXPECT_THROW(a.at(0,-1), std::out_of_range);
    EXPECT_THROW(a.at(3,0), std::out_of_range);
    EXPECT_THROW(a.at(0,3), std::out_of_range);
    EXPECT_THROW(a(0,3), std::out_of_range);
    EXPECT_THROW(a(-1,1), std::out_of_range);
}

void test_arithmetic() {
    Matrix a{{1,2},{3,4}};
    Matrix b{{5,6},{7,8}};
    Matrix c = a+b;
    assert(nearly(c.at(1,1),12));

    Matrix d = b-a;
    assert(nearly(d.at(0,0),4));

    Matrix e = a*2;
    assert(nearly(e.at(1,1),8));

    Matrix f = a*b;
    assert(nearly(f.at(0,0),19));

    EXPECT_THROW(a+Matrix(3,3), std::invalid_argument);
}

void test_matrix_ops() {
    Matrix a{{1,2},{3,4}};
    Matrix t = a.transpose();
    assert(nearly(t.at(1,0),2));

    Matrix h = a.hadamard(a);
    assert(nearly(h.at(1,1),16));
    Matrix sub = a.submatrix(0,1,0,1);
    assert(nearly(sub.at(0,0),1));
}

void test_advanced() {
    Matrix a{{4,7},{2,6}};
    Matrix inv = a.inverse();
    assert(nearly(inv.at(0,0),0.6));

    Matrix s{{1,2},{2,4}};
    EXPECT_THROW(s.inverse(), std::runtime_error);

    Matrix I = Matrix::Identity(3);
    assert(nearly(I.at(1,1),1));
}

void test_utilities() {
    Matrix a(2,2);
    a.fill(5);
    a.clear();

    assert(a.empty());
    assert(a.rows()==0 && a.cols()==0);
    EXPECT_THROW(a.at(0,0), std::out_of_range);
}

void test_edge_cases() {
    Matrix e;
    EXPECT_THROW(e.inverse(), std::invalid_argument);

    EXPECT_THROW(Matrix(1,1,2).inverse(), std::invalid_argument);
    EXPECT_THROW(Matrix(3,3).inverse(), std::invalid_argument);
    EXPECT_THROW(Matrix().inverse(), std::invalid_argument);

    Matrix a({{4,7},{2,6}});
    Matrix inv = a.inverse();
    assert(nearly(inv.at(0,0),0.6));
    Matrix s({{1,2},{2,4}});
    EXPECT_THROW(s.inverse(), std::runtime_error);

    Matrix big(1000,1000,1);
    assert(big.at(999,999)==1);


    Matrix b{{1,2},{3,4}};
    EXPECT_THROW(b/0, std::runtime_error);
}

int main(){
    test_constructors();
    test_assignment();
    test_access();
    test_arithmetic();
    test_matrix_ops();
    test_advanced();
    test_utilities();
    test_edge_cases();
    std::cout << "\nALL TESTS PASSED\n";
    return 0;
}
