#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <ctime>
#include <thread>

using namespace std;

typedef long long ll;
typedef vector<string> vts;
typedef vector<char*> vtc;

#define PR(x) cerr << #x << ": " << x << endl;
#define COMPILE true
#define TESTTIME true
#define pb push_back
#define COM 129
#define STEP 27
#define BUCKET_SIZE 5
#define LARGE 14349000
#define THREADNUM 6
#define num(x) ((x) - 'a' + 1)

// global variable
clock_t st = clock();
vtc bfs;
vtc bks[LARGE];
char* bf;
ll sz;
int tha[THREADNUM + 1];

//void split_to_string() {
//  string cs;
//  for (ll i = 0; i < sz; ++i) {
//    cs += bf[i];
//    if (bf[i] == '\n') { bfs.pb(cs); cs = ""; }
//  }
//  if (!cs.empty()) bfs.pb(cs);
//}
//

int cs_len(char* s) {
  for (int i = 0; ; ++i) {
    if (s[i] == '\n') return i + 1;
  }
  return 0;
}

void write_string(FILE* f) {
  string ans;
  for (int i = 0; i < LARGE; ++i) 
    for (auto& j: bks[i])
      fwrite(j, sizeof(char), cs_len(j), f);
}

//void write_string_to_chs() {
//  ll bias = 0;
//  bf = new char[sz];
//  for (auto& i: bfs) {
//    for (auto& j: i) {
//      sprintf(bf + bias, "%s\n", j.c_str());
//      bias += j.length() + 1;
//    }
//  }
//}

string join_path(string l, string r) {
  if (l[l.length() - 1] == '/') return l + r;
  else return l + '/' + r;
}

void read_file(string ifs) {
  ifstream iff(ifs, ifstream::binary);
  iff.seekg(0, iff.end);
  sz = iff.tellg();
  PR(sz);
  iff.seekg(0);
  if (TESTTIME) fprintf(stderr, "Befor read, %f seconds pass in total.\n", (float)(clock() - st) / CLOCKS_PER_SEC);
  bf = new char[sz];
  iff.read(bf, sz);
  if (TESTTIME) fprintf(stderr, "Read data %f seconds pass in total.\n", (float)(clock() - st) / CLOCKS_PER_SEC);
  iff.close();
}

int get_bucket_no(ll st, ll ed) {
  int res = 0;
  for (ll i = st; i < st + BUCKET_SIZE; i++) {
    int c = (ed <= i) ? 0 : num(bf[i]);
    res = res * STEP + c;
  }
  return res;
}

void track_bucket() {
  ll bef = 0, cur = 0;
  int rank = 0;
  while (cur < sz) {
    if (bf[cur] == '\n') {
      rank = get_bucket_no(bef, cur);
      bks[rank].pb(&bf[bef]);
      bef = ++cur;
    } else ++cur;
  }
}

void write_to_bfs() {
  bfs.pb(bf);
  for (int i = 0; i < sz; ++i) {
    if (bf[i] == '\n') bfs.pb(bf + i + 1);
  }
  bfs.pop_back();
}

void get_strings(string ifs) {
  read_file(ifs);
  // get chs
  track_bucket();
  if (TESTTIME) fprintf(stderr, "Get strings from file %f seconds pass in total.\n", (float)(clock() - st) / CLOCKS_PER_SEC);
}

bool not_end(char c) {
  return c != '\n';
}

bool TEMP = false;
void prt(char* a) {
  cerr << (ll)a << endl;
  for (int i = 0; not_end(a[i]); ++i) {
    cerr << a[i];
  }
  cerr << endl;
}

bool cmp_cs(char* a, char* b) {
  //if (TEMP) {
  //  cerr << "begin:\n";
  //  prt(a);
  //  prt(b);
  //  cerr << "end.\n";
  //}
  int i = 0;
  for (; not_end(a[i]) && not_end(b[i]); ++i) {
    if (a[i] < b[i]) return true;
    else if (b[i] < a[i]) return false;
  }
  if (!not_end(a[i])) return true;
  return false;
}


void sort_bucket(int st) {
  for (int i = tha[st]; i < tha[st + 1]; i++) {
    //cerr << "i:" << i << endl;
    //if (i == 414270) {
    //  TEMP = true;
    //  for(auto& it: bks[i]) prt(it);
    //}
    stable_sort(bks[i].begin(), bks[i].end(), cmp_cs);
  }
}

int main(int argc, char** argv) {
  if (TESTTIME) fprintf(stderr, "Begin test...\n");
  if (argc < 4) return -1;
  char* ifs = argv[1];
  char* ofs = argv[2];
  char* wd = argv[3];
  for (int i = 0; i < LARGE; i++) bks[i].clear();
  tha[0] = 0;
  for (int i = 1; i < THREADNUM; i++) tha[i] = tha[i - 1] + LARGE / THREADNUM;
  tha[THREADNUM] = LARGE;
  char c[100];
  sprintf(c, "sort %s -o %s", ifs, ofs);
  system(c);
  return 0;
}
