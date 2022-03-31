#ifndef MATRIX_HEADER
#define MATRIX_HEADER

#include <iomanip>
#include <iostream>
#include <string>

using std::cerr;
using std::max;
using std::min;
using std::ostream;
using std::string;

template <typename T> class matrix {
private:
  int rows, cols;
  T **data;
  void create();
  void erase();

public:
  matrix();
  matrix(const int &rows_, const int &cols_);
  matrix(const int &rows_, const int &cols_, const T &val);
  matrix(const matrix<T> &other);
  ~matrix();
  matrix<T> &operator=(const matrix<T> &other);
  template <typename TT>
  friend ostream &operator<<(ostream &out, const matrix<TT> &cur);
  T &operator()(const int &row, const int &col);
  const T &operator()(const int &row, const int &col) const;
  matrix<T> operator+(const matrix<T> &other) const;
  matrix<T> &operator+=(const matrix<T> &other);
  matrix<T> operator-(const matrix<T> &other) const;
  matrix<T> &operator-=(const matrix<T> &other);
  bool operator==(const matrix<T> &other) const;
  bool operator!=(const matrix<T> &other) const;
  matrix<T> operator*(const matrix<T> &other) const;
  matrix<T> &operator*=(const matrix<T> &other);
  matrix<T> transpose() const;
  int getRows() const;
  int getCols() const;
  bool isSquare() const;
  void fill(const T &val);
  matrix<T> operator*(const T &val) const;
  matrix<T> &operator*=(const T &val);
  matrix<T> operator/(const T &val) const;
  matrix<T> &operator/=(const T &val);
  T determinant() const;
  matrix<T> inverse() const;
  matrix<T> adjugate() const;
  matrix<T> operator/(const matrix<T> &other) const;
  matrix<T> &operator/=(const matrix<T> &other);
  matrix<T> operator%(const int &val) const;
  matrix<T> &operator%=(const int &val);
};

#include "matrix.tpp"

#endif
