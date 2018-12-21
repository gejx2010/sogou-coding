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
#include <future>
#include <queue>

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
#define THREADNUM 10
#define SEGMENT 10
#define WRITE_SEGMENT 100000000
#define num(x) ((x) - 'a' + 1)

// global variable
time_t st = time(NULL);
vtc bfs;
char* bf[SEGMENT];
vtc bkm[SEGMENT];
ll sz;
char *ifs, *ofs;

struct Node {
  char* addr;
  int rnk;
  Node(char* a, int r) : addr(a), rnk(r) { }
};
bool operator<(Node c, Node d);

void raw(char* s, char* m, int& clen);
void raw(char* s, char* m, int& clen, int& slen);
void cutout(ifstream& f, char* a, int size, int& real_size);
void read_chars(ifstream& f, char* s, int size, int& real_size);
void read_file();
void get_chars_array(int rk);
void sort_string(int rk);
void bucket_sort();
void sort_chars();
void merge_segment();
void quick_sort_and_write(FILE*);
void merge_and_write(FILE* off);
void prt(char* a);

int main(int argc, char** argv) {
  if (TESTTIME) fprintf(stderr, "Begin test...\n");
  if (argc < 4) return -1;
  ifs = argv[1];
  ofs = argv[2];
  // read into bf
  read_file();
  if (TESTTIME) fprintf(stderr, "read file %f seconds pass in total.\n", difftime(time(NULL), st));
  // sort chars
  sort_chars();
  if (TESTTIME) fprintf(stderr, "sort chars %f seconds pass in total.\n", difftime(time(NULL), st));
  // merge and write
  FILE* off = fopen(ofs, "wb");
  merge_and_write(off);
  fclose(off);
  if (TESTTIME) fprintf(stderr, "merge and write %f seconds pass in total.\n", difftime(time(NULL), st));
  for (int i = 0; i < SEGMENT; ++i) delete[] bf[i];
  return 0;
}

int cs_len(char* s) {
  for (int i = 0; ; ++i) {
    if (s[i] == '\n') return i + 1;
  }
  return 0;
}

void read_file() {
  ifstream iff(ifs, ifstream::binary);
  iff.seekg(0, iff.end);
  sz = iff.tellg();
  if (TESTTIME) fprintf(stderr, "Befor read, %f seconds pass in total.\n", difftime(time(NULL), st));
  ll esz = sz / SEGMENT + COM;
  for (int i = 0; i < SEGMENT; ++i) bf[i] = new char[esz + 1];
  int ck = 0;
  int tot = 0;
  while (ck < SEGMENT) {
    int csz = 0;
    iff.seekg(tot);
    read_chars(iff, bf[ck], esz, csz);
    tot += csz;
    ++ck;
  }
  if (TESTTIME) fprintf(stderr, "Read data %f seconds pass in total.\n", difftime(time(NULL), st));
  iff.close();
}

bool not_end(char c) {
  return c != '\n';
}

void prt(char* a) {
  cerr << (ll)a << endl;
  for (int i = 0; not_end(a[i]); ++i) {
    cerr << a[i];
  }
  cerr << endl;
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

void read_chars(ifstream& f, char* s, int size, int& real_size) {
    f.read(s, size);
    cutout(f, s, size, real_size);
}

void cutout(ifstream& f, char* a, int size, int& real_size) {
  for (int j = f.gcount() - 1; 0 <= j; --j) 
    if (a[j] == '\n') {
      real_size = j + 1;
      a[j + 1] = '\0';
      break;
    }
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

void merge_segment() {
  for (int i = 0; i < SEGMENT; i++) 
    for (auto& j: bkm[i]) 
      bfs.pb(j);
}

void quick_sort_and_write(FILE* off) {
  // get segment to one 
  merge_segment();
  // sort whole
  stable_sort(bfs.begin(), bfs.end(), cmp_cs);
  if (TESTTIME) fprintf(stderr, "quick sort after merge %f seconds pass in total.\n", difftime(time(NULL), st));
  // write it into file
  int tot = bfs.size();
  char* mid;
  future<bool> sft = async(launch::async, [&]{
    return true;
  });
  int clen = 0;
  int rnk = 0;
  while (rnk < tot) {
    clen = 0;
    mid = new char[WRITE_SEGMENT + COM];
    while (rnk < tot && clen < WRITE_SEGMENT) {
      raw(bfs[rnk], mid, clen);
      rnk++;
    }
    if (sft.get()) {
      int cc = clen;
      char* tmp = mid;
      sft = async(launch::async, [=]{
        fwrite(tmp, sizeof(char), cc, off);
        delete[] tmp;
        return true;
      });
    }
  }
  sft.get();
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
      sft = async(launch::async, [=]{
        fwrite(tmp, sizeof(char), cc, off);
        delete[] tmp;
        return true;
      });
    }
  }
  sft.wait();
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

