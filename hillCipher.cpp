#include "hillCipher.h"

// This function checks if the reverseKey is damaged, and if it is, it will
// generate it using the key. reverseKey is generated by calculating the inverse
// of key and taking the modulo.
void hillCipher::fixReverseKey() {
  // If it is not damaged, return
  if (reverseKey != NULL)
    return;

  // Otherwise, generate a new matrix
  reverseKey = new matrix<int>;
  *reverseKey = adjugateWithMod(key);

  // You should multiply the adjugate (after applying modulo) by the
  // multiplicative inverse of the key's determinant (after applying modulo).
  // inverse[] will give this multiplicative inverse..
  *reverseKey *= inverse[determinantWithMod(key)];

  // Finally, take the modulo again to make sure that all elements are in the
  // range of [0, 25]
  *reverseKey %= ALPHABETS;
}

// This function makes sure that the reverseKey is damaged. This function is
// useful to indicate that the reverseKey that was calculated before is no
// longer correct.
void hillCipher::damageReverseKey() {
  // If it is already damaged, return
  if (!reverseKey)
    return;

  // Damage the reverseKey otherwise
  delete reverseKey;
  reverseKey = NULL;
}

// This function is the same as operator* in matrix.tpp file. However, it makes
// sure that the generated matrix will have elements only in the range of [0,
// 25]. The function assumes that the given matrices are This is done by
// applying the modulo whenever possible in the function. This is correct since
// the modulous operation is distributive.
matrix<int> hillCipher::mulWithMod(const matrix<int> &mat1,
                                   const matrix<int> &mat2) const {
  int size1 = mat1.getRows(), size2 = mat1.getCols(), size3 = mat2.getCols();
  matrix<int> res(size1, size3, 0);
  for (int i = 0; i < size1; i++)
    for (int j = 0; j < size3; j++)
      for (int k = 0; k < size2; k++) {
        // Apply the modulo immediately after the multiplication
        res(i, j) += mat1(i, k) * mat2(k, j) % ALPHABETS;

        // res(i, j) will range from 0 to 2 * 25, so we should subtract 26 if
        // needed. This is correct because we are adding 2 integers and each one
        // is [0, 25]. This is equivalent to taking the modulous (but a little
        // faster).
        if (res(i, j) >= ALPHABETS)
          res(i, j) -= ALPHABETS;
      }
  return res;
}

// This function is the same as determinant() in matrix.tpp file. The purpose of
// the function is the same as mulWithMod().
int hillCipher::determinantWithMod(const matrix<int> &mat) const {
  int size = mat.getRows();
  if (size == 1)
    return mat(0, 0);
  matrix<int> tmp(size - 1, size - 1);
  for (int i = 1; i < size; ++i)
    for (int j = 1; j < size; ++j)
      tmp(i - 1, j - 1) = mat(i, j);
  int det = 0;
  for (int i = 0, sign = 1; i < size; i++, sign *= -1) {
    // Take the modulo whenever possible, and using determinantWithMod() instead
    // of determinant()
    det += sign * mat(0, i) * determinantWithMod(tmp) % ALPHABETS;

    // If det became negative after this operation, you only need to add 26,
    // since it won't be less than -25.
    if (det < 0)
      det += ALPHABETS;
    else if (det >= ALPHABETS)
      det -= ALPHABETS;

    if (i != size - 1) {
      for (int j = 1; j < size; j++)
        tmp(j - 1, i) = mat(j, i);
    }
  }
  return det;
}

// This function is the same as adjugate() in matrix.tpp file. The purpose of
// the function is the same as determinantWithMod().
matrix<int> hillCipher::adjugateWithMod(const matrix<int> &mat) const {
  int size = mat.getRows();
  if (size == 1)
    return matrix<int>(1, 1, 1);
  matrix<int> adj(size, size);
  for (int i = 0; i < size; ++i) {
    matrix<int> tmp(size - 1, size - 1);
    for (int j = 0, jj = 0; j < size; ++j)
      if (j != i) {
        for (int k = 1; k < size; ++k)
          tmp(jj, k - 1) = mat(j, k);
        ++jj;
      }
    for (int j = 0; j < size; ++j) {
      adj(i, j) = (i + j) & 1 ? -1 : 1;
      adj(i, j) *= determinantWithMod(tmp);
      if (adj(i, j) < 0)
        adj(i, j) += ALPHABETS;
      if (j != size - 1) {
        for (int k = 0, kk = 0; k < size; ++k)
          if (k != i) {
            tmp(kk, j) = mat(k, j);
            ++kk;
          }
      }
    }
  }
  return adj.transpose();
}

// This function makes the following elementary operation:
// Rx -> Rx + m * Ry
// Rx refers to additionRow, m refers to mulVal, Ry refers to mulRow
void hillCipher::rowAddition(const int &mulRow, const int &additionRow,
                             const int &mulVal) {
  // If mulVal is 0, we don't need to continue.
  if (mulVal == 0)
    return;

  int len = key.getRows();
  for (int i = 0; i < len; i++) {
    // Modulous operation should be applied
    key(additionRow, i) += (key(mulRow, i) * mulVal) % ALPHABETS;
    if (key(additionRow, i) >= ALPHABETS)
      key(additionRow, i) -= ALPHABETS;
  }
}

hillCipher::hillCipher(const int &size)
    : rng(std::chrono::steady_clock::now().time_since_epoch().count()),
      key(size, size), reverseKey(NULL) {
  // To make sure our key will has a determinate with gcd to 26 = 1
  // We generate a diagonal matrix with all numbers are relatively prime
  // Under 26 we have a 12 coprimes to 26 all numbers expect even numbers and 13
  for (int i = 0, j = 1; i < 12; i++, j += 2) {
    if (j == 13)
      j += 2;
    coprimeTo26[i] = j;
  }
  // we recede the determinate of the key matrix from 1 to 25 using modular
  // arithmetic operations then we can easily find the mul-inverse for our
  // specific determinate [0:25] not included the even nums or 13 With -1 for
  // non coprimes to 26 nums
  for (int k = 0; k < ALPHABETS; k++) {
    if (!(k % 2) || k == 13) {
      inverse[k] = -1;
      continue;
    }
    for (int j = 0; j < ALPHABETS; j++) {
      if (k * j % ALPHABETS == 1) {
        inverse[k] = j;
        break;
      }
    }
  }
  generateRandomKey(size); // initialize key matrix and generate it randomly
}

hillCipher::~hillCipher() { damageReverseKey(); }
// As mentioned before we generate a diagonal matrix key from our coprimes nums
void hillCipher::generateRandomKey(const int &size) {
  key = matrix<int>(size, size, 0);
  for (int i = 0; i < size; i++) {
    // uniform_int_distribution<int>(0, 11)(rng) return a random num from 0 to
    // 11
    key(i, i) = coprimeTo26[uniform_int_distribution<int>(0, 11)(rng)];
  }
  int randomRowAdditions =
      uniform_int_distribution<int>(0, size)(rng) * ALPHABETS;
  // After generate a random diagonal key we make a random number of row
  // operations on a random indexs of rows
  for (int i = 0; i < randomRowAdditions; i++) {
    int firstIndex = uniform_int_distribution<int>(0, size - 1)(rng);
    int secondIndex =
        (firstIndex + uniform_int_distribution<int>(0, size - 1)(rng)) % size;
    // multiply random row with random number and sum the result to random row
    rowAddition(firstIndex, secondIndex,
                uniform_int_distribution<int>(0, ALPHABETS - 1)(rng));
  }
  damageReverseKey(); // After we have a new key the reversekey not valid to use
                      // and it should be damaged
}
// In case u want to use your key it should be valid before set it
bool hillCipher::isValidKey(const matrix<int> &key) const {
  int det = determinantWithMod(key);
  return inverse[det] != -1;
}
// If you want to use your own key
bool hillCipher::setKey(const matrix<int> &key) {
  if (!isValidKey(key))
    return false;
  this->key = key % ALPHABETS;
  damageReverseKey();
  return true;
}
// Return the current key
const matrix<int> &hillCipher::getKey() const { return key; }
// Return the current reversekey
const matrix<int> &hillCipher::getReverseKey() {
  fixReverseKey();
  return *reverseKey;
}

string hillCipher::encrypt(const string &plainText, char dummyLetter) const {
  int numOfChars = 0;
  // first count the number of valid characters in the plain strig
  for (const char &c : plainText)
    numOfChars += ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
  if (!numOfChars)
    return "";
  int j = 0, k = 0, len = plainText.size(), matSize = key.getRows();
  // Fit all characters in one matrix with size of columns = size of key rows
  //ِAnd size of rows =(( numOfChars + matSize - 1) / matSize) to fit all
  // characters in case (numofchars % 26 != 0)
  matrix<int> textMat((numOfChars + matSize - 1) / matSize, matSize);
  for (int i = 0; i < len; ++i) {
    char c = plainText[i];
    if ('A' <= c && c <= 'Z')
      c -= 'A';
    else if ('a' <= c && c <= 'z')
      c -= 'a';
    else
      continue;
    textMat(j, k) = c;
    ++k;
    if (k == matSize) {
      ++j;
      k = 0;
    }
  }
  // After fit all characters if k!=0 there is an empty elements in the text mat
  // should be filled by the dummy char
  if (k != 0) {
    dummyLetter -= 'a';
    while (k < matSize) {
      textMat(j, k) = dummyLetter;
      ++k;
      ++numOfChars;
    }
  }
  // multiply the text matrix with the key the return result is the cipher text
  textMat = mulWithMod(textMat, key);
  string cipherText(numOfChars, 'A');
  j = 0;
  k = 0;
  // Store the cipher for future uses
  for (int i = 0; i < numOfChars; ++i) {
    cipherText[i] += textMat(j, k);
    ++k;
    if (k == matSize) {
      ++j;
      k = 0;
    }
  }
  return cipherText;
}

string hillCipher::decrypt(const string &cipherText) {
  if (cipherText.empty())
    return "";
  int size = cipherText.size();
  int len = key.getRows();
  if (size % len)
    return "";
  fixReverseKey();
  matrix<int> plain(size / len, len);
  for (int i = 0; i < size; i += len) {
    for (int j = 0; j < len; j++)
      plain(i / len, j) = cipherText[i + j] - 'A';
  }
  plain = mulWithMod(plain, *reverseKey);
  const int &rows = plain.getRows(), &cols = len;
  string res;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      res += plain(i, j) + 'a';
  return res;
}
