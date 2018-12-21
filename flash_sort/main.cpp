#include <unistd.h>
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
#include <queue>
#include <future>

using namespace std;

typedef long long ll;
typedef unsigned int uint;
typedef vector<string> vts;
typedef vector<char*> vtc;

#define PR(x) cerr << #x << ": " << x << endl;
#define COMPILE true
#define TESTTIME true
#define pb push_back
#define COM 129
#define STEP 27
#define BUCKET_SIZE 5
#define LARGE 14350000
#define THREADNUM 6
#define num(x) ((x) - 'a' + 1)
#define SEGMENT 12
#define WRITE_SEGMENT 100000000
#define BASE_SIZE 1000000000

// global variable
time_t st = time(NULL);
vtc bfs;
vtc bks[LARGE];
char* bf[SEGMENT];
vtc bkm[SEGMENT];
vtc fin;
ll sz;
int tha[THREADNUM + 1];
// for input element
char *ifs, *ofs, *wkd;
int sf_len = 0;

struct Node {
  char* addr;
  int rnk;
  Node(char* a, int r) : addr(a), rnk(r) { }
};
bool operator<(Node c, Node d);

int cs_len(char* s);
const char* constr(int rnk);
void write_file(int rk);
void read_file(string ifs);
bool not_end(char c);
bool cmp_cs(char* a, char* b);
void get_chars_array(int rk);
void sort_string(int rk);
void prt(char* a);
void prt(char* a, int len);
void raw(char* s, char* m, int& clen);
void raw(char* s, char* m, int& clen, int& slen);
void merge_and_write(FILE* off);
void sort_bucket(int st);
char* radix_sort(char* a, int sz);
void bucket_sort();
void sort_chars();
void sort_write_small_file(int rnk);
void split_sort_file(string ifs, string wkd);
void cutout(ifstream& f, char* a, int size, int& real_size);
void read_chars(ifstream& f, char* s, int size, int& real_size);
bool write_chars(char* s, int size, FILE* off);
void merge_big_file(string ofs);

int main(int argc, char** argv) {
  if (TESTTIME) fprintf(stderr, "Begin test...\n");
  if (argc < 4) return -1;
  ifs = argv[1];
  ofs = argv[2];
  wkd = argv[3];
  // split file to small file
  split_sort_file(ifs, wkd);
  // read file to segment
  merge_big_file(ofs);
  if (TESTTIME) fprintf(stderr, "Write strings %f seconds pass in total.\n", difftime(time(NULL), st));
  //fclose(off);
  //for (int i = 0; i < SEGMENT; ++i)
  //  delete[] bf[i];
  fprintf(stderr, "Process takes %f seconds in total.\n", difftime(time(NULL), st));
  return 0;
}

int cs_len(char* s) {
  for (int i = 0; ; ++i) {
    if (s[i] == '\n') return i + 1;
  }
  return 0;
}

const char* constr(int rnk) {
  string s;
  s = string(wkd) + string("tmp_") + to_string(rnk);
  return s.c_str();
}

void split_sort_file(string ifs, string wdk) {
  ifstream iff(ifs, ifstream::binary);
  // split big file into segments according BASE_SIZE
  sf_len = 0;
  ll esz = BASE_SIZE / SEGMENT;
  for (int i = 0; i < SEGMENT; ++i)
    bf[i] = new char[esz + 1];
  ll tot = 0;
  while (iff) {
    // read in a BASE_SIZE
    int ck = 0;
    for (int i = 0; i < SEGMENT; ++i) bf[i][0] = '\0';
    while (iff && ck < SEGMENT) {
      int csz = 0;
      iff.seekg(tot);
      read_chars(iff, bf[ck], esz, csz);
      tot += csz;
      ++ck;
    }
    sort_write_small_file(sf_len);
    ++sf_len;
  }
  for (int i = 0; i < sf_len; ++i) delete bf[i];
  if (TESTTIME) fprintf(stderr, "After split sort file, time pass: %f seconds.\n", difftime(time(NULL), st));
}

void read_file(string ifs) {
  ifstream iff(ifs, ifstream::binary);
  iff.seekg(0, iff.end);
  sz = iff.tellg();
  int cp = 0;
  int CSIZE = (ll)sz / SEGMENT * 2;
  iff.seekg(0);
  for (int i = 0; i < SEGMENT; ++i) {
    bf[i] = new char[CSIZE];
    if (i < SEGMENT - 1) 
      iff.read(bf[i], CSIZE / 2); 
    else 
      iff.read(bf[i], CSIZE);
    int j = iff.gcount() - 1;
    for (; 0 <= j; --j) {
      if (bf[i][j] == '\n') { cp += j + 1; break; }
      else bf[i][j] = '\0';
    } 
    //iff.seekg(j + 1 - iff.gcount(), ios_base::cur);
    iff.seekg(cp);
  }
  if (TESTTIME) fprintf(stderr, "Read data %f seconds pass in total.\n", difftime(time(NULL), st));
  iff.close();
}

bool not_end(char c) {
  return c != '\n';
}

bool cmp_cs(char* a, char* b) {
  int i = 0;
  for (; not_end(a[i]) && not_end(b[i]); ++i) {
    if (a[i] < b[i]) return true;
    else if (b[i] < a[i]) return false;
  }
  if (!not_end(a[i])) return true;
  return false;
}

void get_chars_array(int rk) {
  bkm[rk].pb(bf[rk]);
  for (int i = 0; bf[rk][i] != '\0'; ++i) 
    if (bf[rk][i] == '\n') 
      bkm[rk].pb(bf[rk] + i + 1);
  bkm[rk].pop_back();
}

void sort_string(int rk) {
  // get chars array
  get_chars_array(rk);
  stable_sort(bkm[rk].begin(), bkm[rk].end(), cmp_cs);
}

void prt(char* a) {
  cerr << (ll)a << endl;
  for (int i = 0; not_end(a[i]); ++i) {
    cerr << a[i];
  }
  cerr << endl;
}

void prt(char* a, int len) {
  for (int i = 0; i < len; ++i) cerr << a[i];
  cerr << endl;
}

void prtt(char* a, int total, int len) {
  for (int i = 1; i <= len; ++i) cerr << a[total - i];
  cerr << endl;
}

void raw(char* s, char* m, int& clen) {
  int i = 0;
  for (; not_end(s[i]); ++i) m[clen++] = s[i];
  m[clen++] = '\n';
}

void raw(char* s, char* m, int& clen, int& slen) {
  int i = 0;
  for (; not_end(s[i]); ++i) m[clen++] = s[i];
  m[clen++] = '\n';
  slen = i + 1;
}

void merge_and_write(FILE* off) {
  priority_queue<Node> q;
  int where[SEGMENT];
  int csz[SEGMENT];
  memset(where, 0, sizeof where);
  for (int i = 0; i < SEGMENT; ++i) {
    if (!bkm[i].empty()) q.push(Node(bkm[i][0], i));
    csz[i] = bkm[i].size();
  }
  future<bool> sft = async(launch::async, [&]{
    return true;
  });
  char* mid;
  int clen = 0;
  int tlen = 0;
  int slen = 0;
  while (!q.empty()) {
    clen = 0;
    mid = new char[WRITE_SEGMENT + COM];
    while (!q.empty() && clen < WRITE_SEGMENT) {
      Node t = q.top();
      q.pop();
      int tr = t.rnk;
      raw(t.addr, mid, clen, slen);
      tlen += slen;
      if (where[tr] < csz[tr] - 1) {
        where[tr]++;
        q.push(Node(bkm[tr][where[tr]], tr));
      }
    }
    if (sft.get()) {
      int cc = clen;
      char* tmp = mid;
      sft = async(launch::async, [&]{
        fwrite(tmp, sizeof(char), cc, off);
        delete[] tmp;
        return true;
      });
    }
  }
  sft.wait();
}

void sort_bucket(int st) {
  for (int i = tha[st]; i < tha[st + 1]; i++) {
    stable_sort(bks[i].begin(), bks[i].end(), cmp_cs);
  }
}

bool operator<(Node c, Node d) {
  char *a = c.addr, *b = d.addr;
  int i = 0;
  for (; a[i] != '\n' && b[i] != '\n'; ++i) {
    if (b[i] < a[i]) return true;
    else if (a[i] < b[i]) return false;
  }
  if (b[i] == '\n') return true;
  return false;
}

int get_bucket_no(char* w, int turn) {
  int right = COM - turn * BUCKET_SIZE;
  int left = max(0, right - BUCKET_SIZE);
  int len = cs_len(w);
  if (len <= left) return 0;
  int res = 0, i = left;
  for (; not_end(w[i]) && i < right; ++i) 
    res = res * STEP + num(w[i]);
  while (i++ < right) 
    res = res * STEP;
  return res;
}

char* radix_sort(char* a, int sz) {
  int turn = 0;
  char* res = new char[sz];
  while (turn * BUCKET_SIZE < COM) {
    // get it into bks
    int st = 0;
    while (st < sz) {
      char* w = a + st;
      int bn = get_bucket_no(w, turn);
      bks[bn].pb(w);
      st += cs_len(w);
    }
    // write back to new chars
    int r = 0;
    for (int i = 0; i < LARGE; ++i) {
      for (auto& j: bks[i]) 
        raw(j, res, r);
      bks[i].clear();
    }
    // switch res and a
    char* mid = a;
    a = res;
    res = mid;
    // add turn
    ++turn;
  }
  delete[] res;
  return a;
}

void bucket_sort() {
  thread t[THREADNUM];
  int ec = 0;
  while (ec < SEGMENT) {
    for (int i = 0; i < THREADNUM; ++i) t[i] = thread(sort_string, ec + i);
    for (int i = 0; i < THREADNUM; ++i) t[i].join();
    ec += THREADNUM;
  }
}

void sort_chars() {
  for (int i = 0; i < SEGMENT; ++i) bkm[i].clear();
  bucket_sort();
}

void sort_write_small_file(int rnk) {
  if (TESTTIME) fprintf(stderr, "Begin sort_write_small_file function, rnk: %d, time pass: %f seconds.\n", rnk, difftime(time(NULL), st));
  sort_chars();
  if (TESTTIME) fprintf(stderr, "after sort chars, time pass: %f seconds.\n", difftime(time(NULL), st));
  FILE* off = fopen(constr(rnk), "wb");
  merge_and_write(off);
  if (TESTTIME) fprintf(stderr, "after write small chars, rnk: %d, time pass: %f seconds.\n", rnk, difftime(time(NULL), st));
  fclose(off);
}

void cutout(ifstream& f, char* a, int size, int& real_size) {
  for (int j = f.gcount() - 1; 0 <= j; --j) 
    if (a[j] == '\n') {
      real_size = j + 1;
      a[j + 1] = '\0';
      break;
    }
}

void read_chars(ifstream& f, char* s, int size, int& real_size) {
    f.read(s, size);
    cutout(f, s, size, real_size);
}

bool write_chars(char* s, int size, FILE* off) {
  fwrite(s, 1, size, off);
  delete[] s;
  if (TESTTIME) fprintf(stderr, "after write WRITE_SEGMENT time pass: %f seconds.\n", difftime(time(NULL), st));
  return true;
}

void merge_big_file(string ofs) {
  if (TESTTIME) fprintf(stderr, "Begin merge big file, time pass: %f seconds.\n", difftime(time(NULL), st));
  FILE* off = fopen(ofs.c_str(), "wb");
  int sub_size = BASE_SIZE / sf_len;
  ifstream f[sf_len];
  char* s[sf_len];
  for (int i = 0; i < sf_len; ++i) s[i] = new char[sub_size];
  int esz[sf_len];
  // get file stream and read first
  for (int i = 0; i < sf_len; ++i) {
    f[i].open(constr(i), ifstream::binary);
    read_chars(f[i], s[i], sub_size, esz[i]);
  }
  // get priority queue and first push
  priority_queue<Node> q;
  for (int i = 0; i < sf_len; ++i) 
    q.push(Node(s[i], i));
  // loop over
  int has_read[sf_len];
  ll total_read[sf_len];
  memset(has_read, 0, sizeof has_read);
  memset(total_read, 0, sizeof total_read);
  future<bool> sft = async(launch::async, [=] {
    return true;
  });
  char* mid;
  while (!q.empty()) {
    int clen = 0;
    mid = new char[WRITE_SEGMENT + COM];
    while (!q.empty() && clen < WRITE_SEGMENT) {
      Node t = q.top();
      int rk = t.rnk;
      int slen = 0;
      raw(t.addr, mid, clen, slen);
      q.pop();
      has_read[rk] += slen;
      if (has_read[rk] == esz[rk]) {
        if (f[rk]) {
          total_read[rk] += has_read[rk];
          f[rk].seekg(total_read[rk]);
          read_chars(f[rk], s[rk], sub_size, esz[rk]);
          has_read[rk] = 0;
        } else continue;
      }
      q.push(Node(s[rk] + has_read[rk], rk));
    }
    if (TESTTIME) fprintf(stderr, "after get WRITE_SEGMENT time pass: %f seconds.\n", difftime(time(NULL), st));
    if (sft.get()) {
      // take down async parameters
      char* cc = mid;
      int csz = clen;
      sft = async(launch::async, write_chars, cc, csz, off);
    }
  }
  sft.get();
  // clean pointers
  for (int i = 0; i < sf_len; ++i) delete[] s[i];
  // close file
  for (int i = 0; i < sf_len; ++i) f[i].close();
  fclose(off);
}
