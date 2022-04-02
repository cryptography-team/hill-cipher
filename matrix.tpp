#include "matrix.h"

// Default Constructor
template <typename T> matrix<T>::matrix() : data(NULL) {}

// A constructor that creates the matrix
template <typename T>
matrix<T>::matrix(const int &rows_, const int &cols_)
    : rows(rows_), cols(cols_) {
  create(); // Create the matrix with the given dimensions
}

// A constructor that creates the matrix using the previous constructor, then
// fills it with the value val
template <typename T>
matrix<T>::matrix(const int &rows_, const int &cols_, const T &val)
    : matrix<T>(rows_, cols_) /*construct the matrix*/ {
  fill(val); // Fill the matrix
}

// Copy Constructor: construct the matrix first then copy
template <typename T>
matrix<T>::matrix(const matrix<T> &other)
    : matrix<T>() /*constructing the matrix*/ {
  *this = other; // It uses operator= instead (so that the code is not written
                 // twice)
}

// Destructor
template <typename T> matrix<T>::~matrix() {
  erase(); // It uses function erase instead (so that the code is not written
           // twice)
}

template <typename T> matrix<T> &matrix<T>::operator=(const matrix<T> &other) {
  erase(); // First, remove the previously stored matrix (if found)
  rows = other.rows;
  cols = other.cols;
  create(); // Construct the matrix

  // Copy elements:
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      data[i][j] = other(i, j);
  return *this;
}

// Create the matrix using operator new
template <typename T> void matrix<T>::create() {
  data = new T *[rows];
  for (int i = 0; i < rows; i++)
    data[i] = new T[cols];
}

// Erase the matrix using operator delete
template <typename T> void matrix<T>::erase() {
  if (!data)
    return; // If it is already erased, return

  for (int i = 0; i < rows; i++)
    delete[] data[i];
  delete[] data;
  data = NULL; // Mark that the matrix is erased for future calls to erase()
}

// Defining the behaviour of: {cout << matrix;} by overloading operator<<
template <typename TT>
ostream &operator<<(ostream &out, const matrix<TT> &cur) {
  for (int i = 0; i < cur.rows; i++) {
    out << '\t'; // This is just for indentation

    for (int j = 0; j < cur.cols; j++)
      out << cur(i, j)
          << " \n"[j + 1 ==
                   cur.cols]; // This line will print the element at the i-th
                              // row and j-th column. After that, it will print
                              // a space (if it is not the last element in the
                              // row), or an endl (if it is the last element in
                              // the row)
  }

  return out; // This return statement is for concatenating multiple elements to
              // print like this: cout << x << y << z << ...;
}

// A writable access operator (The element can now be modified outside the class
// declaration). For example:
// mat(0, 0) = 55;
template <typename T> T &matrix<T>::operator()(const int &row, const int &col) {
  return data[row][col];
}

// A read-only access operator (The matrix can now be pushed as a const
// parameter in any function while accessing its elements). For example:
// int func(const matrix<int> &mat){
//    return mat(0, 0) + mat(0, 1);
// }
template <typename T>
const T &matrix<T>::operator()(const int &row, const int &col) const {
  return data[row][col];
}

// From now on, the following two functions will represent a structure for
// defining operator@ and operator@=, such that @ is substituted with +, -, ...
template <typename T>
matrix<T> matrix<T>::operator+(const matrix<T> &other) const {
  matrix<T> res(*this); // Copy the phantom

  res += other; // Use the operator+= instead (so that the code is not written
                // twice)

  return res;
}

template <typename T> matrix<T> &matrix<T>::operator+=(const matrix<T> &other) {
  // First, make sure that the 2 matrices can be added together
  if (rows != other.rows || cols != other.cols) {
    cerr << "Error! Dimensions don't match\n"; // Using cerr instead of cout to
                                               // indicate an error
    exit(-1);                                  // Stop running the code
  }

  // Do the normal addition now
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      data[i][j] += other(i, j);

  return *this;
}

template <typename T>
matrix<T> matrix<T>::operator-(const matrix<T> &other) const {
  matrix<T> res(*this);
  res -= other;
  return res;
}

template <typename T> matrix<T> &matrix<T>::operator-=(const matrix<T> &other) {
  if (rows != other.rows || cols != other.cols) {
    cerr << "Error! Dimensions don't match\n";
    exit(-1);
  }
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      data[i][j] -= other(i, j);
  return *this;
}

template <typename T> bool matrix<T>::operator==(const matrix<T> &other) const {
  // Dimensions should match first
  if (rows != other.rows || cols != other.cols)
    return false;

  // After that, every element should match with its corresponding one
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      if (data[i][j] != other(i, j))
        return false;

  return true; // If no returns are evaluated, the two matrices for sure match
}

// Here, just use operator== and invert its answer
template <typename T> bool matrix<T>::operator!=(const matrix<T> &other) const {
  return !(*this == other);
}

template <typename T>
matrix<T> matrix<T>::operator*(const matrix<T> &other) const {
  // First, dimensions should be suitable. The dimensions are suitable if the
  // first matrix's number of columns is the same as the second matrix's number
  // of rows
  if (cols != other.rows) {
    cerr << "Error! Adjacent dimensions don't match\n";
    exit(-1);
  }

  // The resulting matrix should have a dimension of
  // (first_matrix's_row x second_matrix's_col). The matrix should be
  // initialized to zeros since we use addition as follows
  matrix<T> res(rows, other.cols, 0);

  // For every element at (i, j) in the resulting matrix, it will gain an amount
  // equal to the product of a number (from the first matrix) in i-th row with
  // its corresponding number (from the second matrix) in the j-th column
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < other.cols; j++)
      for (int k = 0; k < cols; k++)
        res(i, j) += data[i][k] * other(k, j);

  return res;
}

template <typename T> matrix<T> &matrix<T>::operator*=(const matrix<T> &other) {
  return *this = *this * other;
}

template <typename T> matrix<T> matrix<T>::transpose() const {
  matrix<T> res(cols, rows);
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      res(j, i) = data[i][j];
  return res;
}

template <typename T> int matrix<T>::getRows() const { return rows; }

template <typename T> int matrix<T>::getCols() const { return cols; }

template <typename T> bool matrix<T>::isSquare() const { return rows == cols; }

template <typename T> void matrix<T>::fill(const T &val) {
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      data[i][j] = val;
}

// The following 2 functions multiply each element in the matrix by val
template <typename T> matrix<T> matrix<T>::operator*(const T &val) const {
  matrix<T> res(*this);
  res *= val;
  return res;
}

template <typename T> matrix<T> &matrix<T>::operator*=(const T &val) {
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      data[i][j] *= val;
  return *this;
}

// The following 2 functions divide each element in the matrix by val
template <typename T> matrix<T> matrix<T>::operator/(const T &val) const {
  matrix<T> res(*this);
  res /= val;
  return res;
}

template <typename T> matrix<T> &matrix<T>::operator/=(const T &val) {
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      data[i][j] /= val;
  return *this;
}

template <typename T> T matrix<T>::determinant() const {
  // The matrix should be square first
  if (!isSquare()) {
    cerr << "matrix has no determinant\n";
    exit(-1);
  }

  // If it is a 1x1 matrix, the determinant is the value of (0, 0)
  if (rows == 1)
    return data[0][0];

  // Otherwise, construct a matrix with a smaller dimension.
  // This matrix will hold the elements of which we want their determinant to
  // calculate the determinant of the current matrix.
  // It will first hold the elements are not in the first row or the first
  // column..
  matrix<T> tmp(rows - 1, cols - 1);
  for (int i = 1; i < rows; i++)
    for (int j = 1; j < cols; j++)
      tmp(i - 1, j - 1) = data[i][j];

  T det = 0; // For accumulating the determinant
  for (int i = 0, sign = 1; i < cols; i++, sign *= -1) {

    // To calculate the determinant, we should multiply each number in the first
    // row and the i-th column by the determinant of the matrix that has
    // elements from all rows other than the first row and the i-th column
    det += sign * data[0][i] * tmp.determinant();

    // After that we should get the matrix that has elements from all the matrix
    // other than the first row and the (i + 1)-th column. This is done by
    // copying the i-th column from the main matrix to the i-th column in the
    // tmp matrix. Note that this operation shouldn't be done at the last column
    // of the main matrix.
    if (i != cols - 1) {
      for (int j = 1; j < rows; j++)
        tmp(j - 1, i) = data[j][i];
    }
  }
  return det;
}

// This is just the definition of the matrix inverse operation
template <typename T> matrix<T> matrix<T>::inverse() const {
  return adjugate() / determinant();
}

// The adjugate is calculated by evaluating the coefficient matrix first, then
// transposing it.
template <typename T> matrix<T> matrix<T>::adjugate() const {
  // The matrix should first be a square matrix
  if (!isSquare()) {
    cerr << "matrix has no adjugate\n";
    exit(-1);
  }

  // We never should continue if the matrix is a 1x1 matrix. The adjugate of
  // such a matrix is always [1]..
  if (rows == 1)
    return matrix<T>(1, 1, 1);

  // To calculate the coefficent matrix, we should do the following operation
  // for each element in the matrix:
  // 1- Get the matrix that has elements from all the matrix other than the i-th
  // row and the j-th column.
  // 2- Multiply its determinant by a negative sign if (i + j) is odd
  matrix<T> adj(rows, cols);
  for (int i = 0; i < rows; i++) {

    // First, get the smaller matrix as in determinant()
    matrix<T> tmp(rows - 1, cols - 1);
    for (int j = 0, jj = 0; j < rows; j++)
      if (j != i) {
        for (int k = 1; k < cols; k++)
          tmp(jj, k - 1) = data[j][k];
        jj++;
      }

    // Second, evaluate the determinant for each element in that row
    for (int j = 0; j < cols; j++) {
      adj(i, j) =
          (i + j) & 1 ? -1 : 1; // It should have -1 iff (i + j) is odd.. If the
                                // number is odd it will have the first bit set
                                // so ANDing it with 1 returns 1

      adj(i, j) *= tmp.determinant();

      // Shift the columns as in determinant()
      if (j != cols - 1) {
        for (int k = 0, kk = 0; k < rows; k++)
          if (k != i) { // We should only move the elements that are not in the
                        // i-th row

            tmp(kk, j) = data[k][j];

            kk++; // This variable is used to get the current row in tmp matrix
          }
      }
    }
  }

  // Finally, transpose the coefficient matrix to get the adjugate
  return adj.transpose();
}

template <typename T>
matrix<T> matrix<T>::operator/(const matrix<T> &other) const {
  matrix<T> res(*this);
  res /= other;
  return res;
}

// Dividing by a matrix is equivalent to multiplying by the inverse of the
// matrix
template <typename T> matrix<T> &matrix<T>::operator/=(const matrix<T> &other) {
  return *this *= other.inverse();
}

// The following 2 functions takes the modulo of every element in the matrix
// using val
template <typename T> matrix<T> matrix<T>::operator%(const int &val) const {
  matrix<T> res(*this);
  res %= val;
  return res;
}

template <typename T> matrix<T> &matrix<T>::operator%=(const int &val) {
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      data[i][j] %= val;

      // If the number is negative it will also be negative after taking the
      // modulous, so we need to add only one val to it so that it becomes
      // positive
      if (data[i][j] < 0)
        data[i][j] += val;
    }
  return *this;
}
