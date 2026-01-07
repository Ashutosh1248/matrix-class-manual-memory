# Matrix Library

Minimal c++ matrix library supporting:

- Constructors, copy/move
- Element access(`at()` and `operator()`)
- Arithmetic:`+`, `-`, `*`, scalar operations
- Matrix operations: transpose, inverse(2(U+2715)2), determinant, submatrix
- Element-wise(Hadamard) multiplication
- Utilities: identity, map/apply

## Usage 

```cpp
#include "matrix.h"
Matrix A(3,3,1.0);
Matrix B({{1,2,3},{4,5,6},{7,8,9}});
Matrix C = A*B;
Matrix D = C.transpose();
std::cout<<D;
