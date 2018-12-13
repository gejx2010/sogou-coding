#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

typedef long long ll;
typedef vector<string> vts;
typedef vector<char*> vtc;

#define PR(x) cerr << #x << ": " << x << endl;
#define COMPILE true
#define TESTTIME true
#define pb push_back
#define COM 129

// global variable
clock_t st = clock();
vtc bfs;
char* bf;
ll sz;

//void split_to_string() {
//  string cs;
//  for (ll i = 0; i < sz; ++i) {
//    cs += bf[i];
//    if (bf[i] == '\n') { bfs.pb(cs); cs = ""; }
//  }
//  if (!cs.empty()) bfs.pb(cs);
//}

//char* write_string_to_chs() {
//  string ans;
//  for (auto& it: bfs) {
//    ans += it;
//  }
//  return const_cast<char*>(ans.c_str());
//}

void write_string_to_chs() {
  ll bias = 0;
  bf = new char[sz];
  for (auto& it: bfs) {
    sprintf(bf + bias, "%s", it);
    bias += strlen(it);
  }
}

string join_path(string l, string r) {
  if (l[l.length() - 1] == '/') return l + r;
  else return l + '/' + r;
}

//void get_strings_from_file(string ifs) {
//  ifstream iff(ifs, ifstream::binary);
//  iff.seekg(0, iff.end);
//  sz = iff.tellg();
//  PR(sz);
//  iff.seekg(0);
//  bf = new char[sz];
//  if (TESTTIME) fprintf(stderr, "Befor read, %f seconds pass in total.\n", (float)(clock() - st) / CLOCKS_PER_SEC);
//  iff.read(bf, sz);
//  if (TESTTIME) fprintf(stderr, "Read data %f seconds pass in total.\n", (float)(clock() - st) / CLOCKS_PER_SEC);
//  iff.close();
//  // split to string
//  split_to_string();
//  if (TESTTIME) fprintf(stderr, "Split to string %f seconds pass in total.\n", (float)(clock() - st) / CLOCKS_PER_SEC);
//}

void get_strings_from_file(string ifs) {
  FILE* iff = fopen(ifs.c_str(), "r");
  if (iff == NULL) perror("Error opening file.\n");
  else {
    sz = 0;
    char* cs = new char[COM];
    while(fgets(cs, COM, iff)) {
      sz += strlen(cs);
      bfs.pb(cs);
      cs = new char[COM];
    }
  }
  if (TESTTIME) fprintf(stderr, "Get strings from file %f seconds pass in total.\n", (float)(clock() - st) / CLOCKS_PER_SEC);
}

bool cmp_cs(char* a, char* b) {
  int i = 0;
  for (; i < strlen(a) && i < strlen(b); ++i) {
    if (a[i] < b[i]) return true;
    else if (b[i] < a[i]) return false;
  }
  if (i == strlen(a)) return true;
  return false;
}

int main(int argc, char** argv) {
  if (TESTTIME) fprintf(stderr, "Begin test...\n");
  if (argc < 4) return -1;
  char* ifc = argv[1];
  char* ofc = argv[2];
  char* wd = argv[3];
  // read in file
  string ifs = join_path(string(wd), string(ifc));
  string ofs = join_path(string(wd), string(ofc));
  // get vector strings
  get_strings_from_file(ifs);
  // sort
  sort(bfs.begin(), bfs.end(), cmp_cs);
  if (TESTTIME) fprintf(stderr, "Sort strings %f seconds pass in total.\n", (float)(clock() - st) / CLOCKS_PER_SEC);
  // write back to chars
  write_string_to_chs();
  if (TESTTIME) fprintf(stderr, "Write strings %f seconds pass in total.\n", (float)(clock() - st) / CLOCKS_PER_SEC);
  // write chars to ofs
  FILE* off = fopen(ofs.c_str(), "wb");
  fwrite(bf, sizeof(char), sz, off);
  fclose(off);
  if (TESTTIME) fprintf(stderr, "Process takes %f seconds in total.\n", (float)(clock() - st) / CLOCKS_PER_SEC);
  delete[] bf;
  return 0;
}
