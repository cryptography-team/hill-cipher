#include "hillCipher.h"
#include "matrix.h"
#include <fstream>
#include <iostream>

using namespace std;

void getText(const string &type, string &text) {
  cout << "\tThe last " << type << " text was \"" << text
       << "\".. Continue with this " << type << " text? (Y/n): " << flush;
  char ch;
  cin >> ch;
  if (ch == 'n') {
    cout << "\tSet your " << type << " text from a file? (y/N): " << flush;
    cin >> ch;
    if (ch == 'y') {
      cout << "\tEnter file name: " << flush;
      string tmp;
      cin >> tmp;
      ifstream in(tmp.c_str());
      text.clear();
      while (getline(in, tmp))
        text += tmp;
    } else {
      cout << "\tSet your " << type << " text (in one line):\n\t" << flush;
      cin.ignore();
      getline(cin, text);
    }
  }
}

void printText(const string &type, const string &text) {
  cout << "\tPrint to a file? (y/N): " << flush;
  char ch;
  cin >> ch;
  if (ch == 'y') {
    cout << "\tEnter file name: " << flush;
    string tmp;
    cin >> tmp;
    ofstream out(tmp.c_str());
    out << text << endl;
  } else
    cout << "\tThis is the corresponding " << type << " text:\n\t" << text
         << endl;
}

int main() {
  hillCipher tool;
  string plain = "Muns And Yasser";
  string cipher;
  matrix<int> key(3, 3);
  int size;

  matrix<int> reversekey(3, 3);
  cout
      << "  ================================================================\n";
  cout << "\tHill Cipher tool was initiated with a random key.\n";
  cout << "\tHope it will be a good interface. Use it as you wish :)\n";
  while (true) {
    cout
        << "  "
           "================================================================\n";
    cout << "\tChoose one of the following options:\n";
    cout << "\t1- Encrypt a plain text using the current key\n";
    cout << "\t2- Decrypt a cipher text using the current key\n";
    cout << "\t3- Get the current key\n";
    cout << "\t4- Get the current reverse key\n";
    cout << "\t5- Set a custom key\n";
    cout << "\t6- Set a random key\n";
    cout << "\t7- Exit the program\n";
    cout << "\n\tYour choice is [1-7]: " << flush;
    int choice;
    cin >> choice;
    if (choice == 7)
      break;
    switch (choice) {
    case 1:
      getText("plain", plain);
      cout << "\tSet your dummy letter that will be used in case the plain "
           << "text was not long enough: " << flush;
      char dummyLetter;
      cin >> dummyLetter;
      cipher = tool.encrypt(plain, dummyLetter);
      printText("cipher", cipher);
      break;
    case 2:
      getText("cipher", cipher);
      plain = tool.decrypt(cipher);
      if (plain.empty() && !cipher.empty())
        cout << "\tNot a valid cipher.. Cipher's size is not divisible by "
             << "matrix size (add some dummy letters)\n";
      else
        printText("plain", plain);
      break;
    case 3:
      cout << tool.getKey() << endl;
      break;
    case 4:
      cout << tool.getReverseKey() << endl;
      break;
    case 5:
      int size;
      cout << "\tSet your key size: " << flush;
      cin >> size;
      key = matrix<int>(size, size);
      reversekey = matrix<int>(size, size);
      cout << "\tEnter your " << size << 'x' << size
           << " key (one row on each line):" << endl;
      for (int i = 0; i < size; i++) {
        cout << '\t' << flush;
        for (int j = 0; j < size; j++)
          cin >> key(i, j);
      }
      if (!tool.setKey(key))
        cout << "\tSad.. Not a valid key.. This key will not be set.";
      else
        cout << "\tPerfecto.. The key has been set.";
      cout << endl;
      break;
    case 6:
      cout << "\tSet your key size: " << flush;
      cin >> size;
      tool.generateRandomKey(size);
      cout << "\tThis random key has been set:\n" << tool.getKey() << endl;
      break;
    default:
      cout << "\tPlease enter a valid option..." << endl;
    }
#ifdef __unix__
    system("read -p \"Press return key to continue...\"");
    system("clear");
#else
    system("pause");
    system("cls");
#endif
  }
  cout
      << "  ================================================================\n";
  cout << "\tSad to see you go :(\n";
  return 0;
}
