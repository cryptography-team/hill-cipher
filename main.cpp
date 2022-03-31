#include "hillCipher.h"
#include "matrix.h"
#include <iostream>

using namespace std;

int main() {
  hillCipher tool;
  string plain ="muns and yasser";
  string cipher ;
  matrix<int> key(3,3);
  matrix<int>reversekey(3,3);
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
    cout << "\t5- Set a key\n";
    cout << "\t6- Set a random key\n";
    cout << "\t7- Exit the program\n";
    cout << "\n\tYour choice is [1-7]: " << flush;
    int choice;
    cin >> choice;
    if (choice == 7)
      break;
    switch (choice) {
    case 1:
        char ch ;
        cout<<"Existing plain text is : <" <<plain<<"> continue with this plain? press y if you or n if u don't : ";
        cin>>ch;

        if(ch == 'n')
        {
            cout<<"Set your plain : ";
            plain = "";
            cin>>plain;
        }
        cipher = tool.encrypt(plain);
        cout<<"\n"<<cipher;

      break;
    case 2:
        cout<<tool.decrypt(cipher);

      break;
    case 3:
        cout<<tool.getKey();
        break;
    case 4:
        cout<<tool.getReverseKey();
        break;
    case 5:
        int siz;
        cout<<"Set your key size : ";
        cin >> siz;
        key = matrix<int>(siz,siz);
        reversekey = matrix<int>(siz,siz);
        cout<<"Enter your key row by row with space between every two columns and break line between every two rows... \n";
        for(int i = 0 ;i<siz;i++){
            for(int j = 0;j<siz;j++){
                cin>>key(i,j);
                cout<<" ";
            }
            cout<<"\n";
        }
        if(!tool.isValidKey(key))
            cout<<"Not valid key \n";
        break;
    case 6:
        tool.generateRandomKey();
        break;
    case 7:
        exit(-1);
        break;
    // Other cases here...
    default:
      cout << "\tPlease enter a valid number..." << endl;
    }
    cout << "\n\tPress any key to continue..." << endl;
#ifdef __unix__
    system("read");
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
      break;
    // Other cases here...
    default:
      cout << "\tPlease enter a valid number..." << endl;
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
