#ifndef HILL_CIPHER_HEADER
#define HILL_CIPHER_HEADER

#include "matrix.h"
#include <chrono>
#include <random>
#include <string>

using std::string;

class hillCipher {
private:
  enum { ALPHABETS = 26 };
  matrix<int> key, *reverseKey;
  int coprimeTo26[12], inverse[ALPHABETS];
  std::mt19937 rng;

  void fixReverseKeyIfDamaged(); // Mansour
  void damageReverseKey();       // Mansour

public:
  hillCipher(
      const int &size =
          3); // Mansour: initialize key, reverseKey, coprimeTo26,
              // inverse + generateRandomKey
              // initialize rng as stated below:
              // rng(std::chrono::steady_clock::now().time_since_epoch().count())
  ~hillCipher(); // Mansour

  void generateRandomKey(); // Mansour
  bool isValidKey(const matrix<int> &key) const;
  bool setKey(const matrix<int> &key);

  const matrix<int> &getKey() const;
  const matrix<int> &getReverseKey();

  string encrypt(const string &plainText, char dummyLetter = 'a') const;
  string decrypt(const string &cipherText); // Mansour
};

#endif
