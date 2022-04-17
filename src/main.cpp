#include "hillCipher.h"
#include "matrix.h"
#include <fstream>
#include <iostream>

using namespace std;

void getText(const string &type, string &text) {
  // flush: it makes sure that the text sent to cout is sent out from the buffer
  // to the screen, so that the user can see what is the question he needs to
  // answer
  cout << "\tThe last " << type << " text was \"" << text << "\"..\n\t"
       << "Continue with this " << type << " text? (Y/n): " << flush;
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
      // The input cursor is at the last place where input was taken using cin,
      // so when we use cin.ignore(), it moves down to the current location. Not
      // using this function yields in invalid input (the line read using
      // getline will be an empty line for example).
      cin.ignore();

      // getline -by default- takes input from the user until he presses the
      // return key
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

  cout
      << "  ================================================================\n";
  cout << "\tHill Cipher tool was initiated with a 3x3 random key.\n";
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
    if (choice == 1) {
      getText("plain", plain);
      cout << "\tSet your dummy letter that will be used in case the plain "
           << "text was not long enough: " << flush;
      char dummyLetter;
      cin >> dummyLetter;
      cipher = tool.encrypt(plain, dummyLetter);
      printText("cipher", cipher);
    } else if (choice == 2) {
      getText("cipher", cipher);
      plain = tool.decrypt(cipher);
      if (plain.empty() && !cipher.empty())
        cout << "\tNot a valid cipher.. Cipher's size is not divisible by "
             << "matrix size (add some dummy letters)\n";
      else
        printText("plain", plain);
    } else if (choice == 3) {
      cout << tool.getKey();
    } else if (choice == 4) {
      cout << tool.getReverseKey();
    } else if (choice == 5) {
      cout << "\tSet your key size: " << flush;
      int size;
      cin >> size;
      matrix<int> key(size, size);
      cout << "\tEnter your " << size << 'x' << size
           << " key (one row on each line):" << endl;
      for (int i = 0; i < size; i++) {
        cout << '\t' << flush;
        for (int j = 0; j < size; j++)
          cin >> key(i, j);
      }
      cout << endl;
      if (!tool.setKey(key))
        cout << "\tSad.. Not a valid key.. This key will not be set.";
      else
        cout << "\tPerfecto.. The key has been set.";
      cout << endl;
    } else if (choice == 6) {
      cout << "\tSet your key size: " << flush;
      int size;
      cin >> size;
      tool.generateRandomKey(size);
      cout << "\tThis random key has been set:\n" << tool.getKey();
    } else if (choice == 7) {
      break;
    } else {
      cout << "\tPlease enter a valid option..." << endl;
    }
    cout << endl;
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
