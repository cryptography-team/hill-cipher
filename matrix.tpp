#include "matrix.h"

template <typename T> matrix<T>::matrix() : data(NULL) {}

template <typename T>
matrix<T>::matrix(const int &rows_, const int &cols_)
    : rows(rows_), cols(cols_) {
  create();
}

template <typename T>
matrix<T>::matrix(const int &rows_, const int &cols_, const T &val)
    : matrix<T>(rows_, cols_) {
  fill(val);
}

template <typename T> matrix<T>::matrix(const matrix<T> &other) : matrix<T>() {
  *this = other;
}

template <typename T> matrix<T>::~matrix() { erase(); }

template <typename T> matrix<T> &matrix<T>::operator=(const matrix<T> &other) {
  erase();
  rows = other.rows;
  cols = other.cols;
  create();
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      data[i][j] = other(i, j);
  return *this;
}

template <typename T> void matrix<T>::create() {
  data = new T *[rows];
  for (int i = 0; i < rows; i++)
    data[i] = new T[cols];
}

template <typename T> void matrix<T>::erase() {
  if (!data)
    return;
  for (int i = 0; i < rows; i++)
    delete[] data[i];
  delete[] data;
  data = NULL;
}

template <typename TT>
ostream &operator<<(ostream &out, const matrix<TT> &cur) {
  for (int i = 0; i < cur.rows; i++) {
    out << '\t';
    for (int j = 0; j < cur.cols; j++)
      out << cur(i, j) << " \n"[j == cur.cols - 1];
  }
  return out;
}

template <typename T> T &matrix<T>::operator()(const int &row, const int &col) {
  return data[row][col];
}

template <typename T>
const T &matrix<T>::operator()(const int &row, const int &col) const {
  return data[row][col];
}

template <typename T>
matrix<T> matrix<T>::operator+(const matrix<T> &other) const {
  matrix<T> res(*this);
  res += other;
  return res;
}

template <typename T> matrix<T> &matrix<T>::operator+=(const matrix<T> &other) {
  if (rows != other.rows || cols != other.cols) {
    cerr << "Error! Dimensions don't match\n";
    exit(-1);
  }
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
  if (rows != other.rows || cols != other.cols)
    return false;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      if (data[i][j] != other(i, j))
        return false;
  return true;
}

template <typename T> bool matrix<T>::operator!=(const matrix<T> &other) const {
  return !(*this == other);
}

template <typename T>
matrix<T> matrix<T>::operator*(const matrix<T> &other) const {
  if (cols != other.rows) {
    cerr << "Error! Adjacent dimensions don't match\n";
    exit(-1);
  }
  matrix<T> res(rows, other.cols, 0);
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
  if (!isSquare()) {
    cerr << "matrix has no determinant\n";
    exit(-1);
  }
  if (rows == 1)
    return data[0][0];
  matrix<T> tmp(rows - 1, cols - 1);
  for (int i = 1; i < rows; i++)
    for (int j = 1; j < cols; j++)
      tmp(i - 1, j - 1) = data[i][j];
  T det = 0;
  for (int i = 0, sign = 1; i < cols; i++, sign *= -1) {
    det += sign * data[0][i] * tmp.determinant();
    if (i != cols - 1) {
      for (int j = 1; j < rows; j++)
        tmp(j - 1, i) = data[j][i];
    }
  }
  return det;
}

template <typename T> matrix<T> matrix<T>::inverse() const {
  return adjugate() / determinant();
}

template <typename T> matrix<T> matrix<T>::adjugate() const {
  if (!isSquare()) {
    cerr << "matrix has no adjugate\n";
    exit(-1);
  }
  matrix<T> adj(rows, cols);
  for (int i = 0; i < rows; i++) {
    matrix<T> tmp(rows - 1, cols - 1);
    for (int j = 0, jj = 0; j < rows; j++)
      if (j != i) {
        for (int k = 1; k < cols; k++)
          tmp(jj, k - 1) = data[j][k];
        jj++;
      }
    for (int j = 0; j < cols; j++) {
      adj(i, j) = (i + j) & 1 ? -1 : 1;
      adj(i, j) *= tmp.determinant();
      if (j != cols - 1) {
        for (int k = 0, kk = 0; k < rows; k++)
          if (k != i) {
            tmp(kk, j) = data[k][j];
            kk++;
          }
      }
    }
  }
  return adj.transpose();
}

template <typename T>
matrix<T> matrix<T>::operator/(const matrix<T> &other) const {
  matrix<T> res(*this);
  res /= other;
  return res;
}

template <typename T> matrix<T> &matrix<T>::operator/=(const matrix<T> &other) {
  return *this *= other.inverse();
}

template <typename T> matrix<T> matrix<T>::operator%(const int &val) const {
  matrix<T> res(*this);
  res %= val;
  return res;
}

template <typename T> matrix<T> &matrix<T>::operator%=(const int &val) {
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      data[i][j] %= val;
      if (data[i][j] < 0)
        data[i][j] += val;
    }
  return *this;
}
