#include <iostream>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;

int main() {
  ifstream f("test.txt", ifstream::binary);
  char t[1000];
  f.read(t, 800);
  cout << t << endl;
  cout << strlen(t) << endl;
  cout << f.gcount() << endl;
  for (int i = 0; i < strlen(t); ++i) {
    cout << i << ":" << (int)t[i] << endl;
  }
  char* a = new char[20];
  strcpy(a, "abcdefg;hij");
  string b(a);
  cout << &a << endl;
  cout << sizeof a << endl;
  cout << a << endl;
  cout << &b << endl;
  cout << sizeof b << endl;
  cout << b << endl;
  strcpy(a, "dsfd");
  cout << &a << endl;
  cout << sizeof a << endl;
  cout << a << endl;
  cout << &b << endl;
  cout << sizeof b << endl;
  cout << b << endl;
  b = "dfsdfsdf";
  cout << &b << endl;
  cout << b << endl;
  return 0;
}
