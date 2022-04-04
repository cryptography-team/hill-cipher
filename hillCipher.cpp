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
// 25]. This is done by applying the modulo whenever possible in the function.
// This is correct since the modulous operation is distributive.
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
        else if (res(i, j) < 0)
          res(i, j) += ALPHABETS;
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
  for (int i = 0, j = 1; i < 12; i++, j += 2) {
    if (j == 13)
      j += 2;
    coprimeTo26[i] = j;
  }

  // Using modular arithmetic techniques, we retracted the determinant of the
  // key matrix from 1 to 25. The mul-inverse for our unique determinant [0:25]
  // not include the even nums or 13 is then easily found. Non-coprimes with 26
  // numbers get a -1.
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

// We build a diagonal matrix key from our coprimes numbers, as described below.
void hillCipher::generateRandomKey(const int &size) {
  // To ensure that our key has a determinant with GCD of 1 with 26,
  // we generate a diagonal matrix with all numbers being relatively prime
  // to 26. We have 12 coprimes to 26 (all except even numbers and 13). This
  // method is always correct since the determinant of a diagonal matrix is the
  // product of the elements in its diagonal.
  key = matrix<int>(size, size, 0);
  for (int i = 0; i < size; i++) {
    // uniform_int_distribution<int>(0, 11)(rng) return a number between 0 and
    // 11 at random
    key(i, i) = coprimeTo26[uniform_int_distribution<int>(0, 11)(rng)];
  }
  int randomRowAdditions =
      uniform_int_distribution<int>(0, size * ALPHABETS)(rng);

  // Following the generation of a random diagonal key, we do a random number of
  // row operations on a random number of row indexes. Row additions doesn't
  // change the modulous of the matrix, so the matrix will still be valid.
  for (int i = 0; i < randomRowAdditions; i++) {
    int firstIndex = uniform_int_distribution<int>(0, size - 1)(rng);
    int secondIndex =
        (firstIndex + uniform_int_distribution<int>(1, size - 1)(rng)) % size;

    // Random row is multiplied by random number, and the result is added to
    // random row
    rowAddition(firstIndex, secondIndex,
                uniform_int_distribution<int>(0, ALPHABETS - 1)(rng));
  }

  damageReverseKey(); // After we get a new key, the reversekey is no longer
                      // valid and should be destroyed
}

// If you wish to use your key, make sure it's valid first.
bool hillCipher::isValidKey(const matrix<int> &key) const {
  int det = determinantWithMod(key);
  return inverse[det] != -1;
}

// You can use your own key if you wish to
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

  // To begin, count how many valid characters there are in the plain string.
  for (const char &c : plainText)
    numOfChars += ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');

  if (!numOfChars)
    return "";
  int j = 0, k = 0, len = plainText.size(), matSize = key.getRows();

  // Fit all characters into a single matrix, with column sizes equaling key row
  // sizes. In case (numOfChars % 26 != 0), the amount of rows is ceiled using
  // the equation to fit all characters (plus dummy characters if needed).
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

  // If there are any empty elements in the text mat after fitting all
  // characters, the dummy char should be used to fill them.
  if (k != 0) {
    dummyLetter -= 'a';
    while (k < matSize) {
      textMat(j, k) = dummyLetter;
      ++k;
      ++numOfChars;
    }
  }

  // The cipher text is returned as a result.
  textMat = mulWithMod(textMat, key);

  string cipherText(numOfChars, 'A');
  j = 0;
  k = 0;
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

  // Check to see if we have a valid reversekey, and if not, fix it.
  fixReverseKey();

  // For the next step, we will store the cipher text in a matrix called plain.
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
