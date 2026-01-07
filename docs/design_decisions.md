
# Design Decisions

This document explains the architectural and performance-related choices made in the Matrix library


## Memory Layout

 The matrix stores its elements in a single contiguous heap-allocated array.

**why:**
- Maximized cache locality
- Enables fast row-major traversal
- Allows pointer swapping for O(1) move operators


## Bounds Checking 

Two element access methods are provided:

- `at()` - performs bounds checking and throws on invalid indices 
- `operator()(i,j)` - performs bounds checking and throws on invalid indices 

## Move Semantics

The move constructor transfers ownership of the internal data pointer 
instead of copying memory.

**why:**
- Copying a large matrix is O(N^2)
- Moving is O(1)
- Enables fast return0by-value and temp objects

## Multiplication Strategy 

Matrix multiplication uses:

- Blocked / tiles multiplication
- Transposed rhs matrix for cache=friendly access

**why:**
- Prevents cache thrashing from column-wise access
- Maximizes linear memory traversal
- Greatly reduces cache misses for large matrices 


## Output Streming Optimization

`operator<<` uses `std::stringsteam` to buffer output before writing to the output stream.

**why:**
- Direct `std::ostream` calls inside nested loops are extremely expensive
- Formatting into memory first avoids thousands of I/O calls
- Write to output stream is performed periodically.

This improves printing perfromance by  **orders of magnitude** for large
matrices.


## Tranpose Strategy

Transpose produces a new matrix with row-major traversal.

**why:**
- Enables linear memory access in later multiplications
- Improves cache locality dramatically


## Inverse Support

Matrix inverse is implemented for sequare matrix of dim 2 only.

**why:**
- Keeps implementation simple and deterministic


## Error Handling

- Invalid indices throw `std::invalid_argument`
- Invalid submatrix ranges throw `std::invalid_argument`
- Dimension mismatch operations throw descriptive exceptions

**why:**
- Fail fast
- Prevent silent corruption or UB
