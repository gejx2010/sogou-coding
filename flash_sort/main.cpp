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
#define BUCKET_SIZE 4
#define LARGE 1050000
#define THREADNUM 6
#define num(x) ((x) - 'a' + 1)
#define SEGMENT 12
#define WRITE_SEGMENT 10000000

// global variable
clock_t st = clock();
vtc bfs;
vtc bks[LARGE];
char* bf[SEGMENT];
vtc bkm[SEGMENT];
vtc fin;
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

string join_path(string l, string r) {
  if (l[l.length() - 1] == '/') return l + r;
  else return l + '/' + r;
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
  if (TESTTIME) fprintf(stderr, "Read data %f seconds pass in total.\n", (float)(clock() - st) / CLOCKS_PER_SEC);
  iff.close();
}

//int get_bucket_no(ll st, ll ed) {
//  int res = 0;
//  int cb = 0x60;
//  for (ll i = st; i < st + BUCKET_SIZE; i++) {
//    res <<= 5;
//    if (i < ed) 
//      res += (bf[i] - cb);
//  }
//  return res;
//}

//void track_bucket() {
//  ll bef = 0, cur = 0;
//  int rank = 0;
//  while (cur < sz) {
//    if (bf[cur] == '\n') {
//      rank = get_bucket_no(bef, cur);
//      bks[rank].pb(&bf[bef]);
//      bef = ++cur;
//    } else ++cur;
//  }
//}

//void write_to_bfs() {
//  bfs.pb(bf);
//  for (int i = 0; i < sz; ++i) {
//    if (bf[i] == '\n') bfs.pb(bf + i + 1);
//  }
//  bfs.pop_back();
//}

bool not_end(char c) {
  return c != '\n';
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

void get_chars_array(int rk) {
  int bef = 0;
  for (int i = 0; bf[rk][i] != '\0'; ++i) {
    if (bf[rk][i] == '\n') {
      bkm[rk].pb(bf[rk] + bef);
      bef = i + 1;
    }
  }
}

void write_file(int rk) {
  char name[20];
  sprintf(name, "test_%d", rk);
  FILE* off = fopen(name, "wb");
  for (uint i = 0; i < bkm[rk].size(); ++i) fwrite(bkm[rk][i], sizeof(char), cs_len(bkm[rk][i]), off);
  fclose(off);
}

void sort_string(int rk) {
  // get chars array
  get_chars_array(rk);
  sort(bkm[rk].begin(), bkm[rk].end(), cmp_cs);
  // write_file(rk);
  if (TESTTIME) fprintf(stderr, "Get strings from file %f seconds pass in total.\n", ((float)clock() - st) / CLOCKS_PER_SEC);
}

struct Node {
  char* addr;
  int rank;
  Node(char* a, int r) : addr(a), rank(r) { }
};

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

bool TEMP = false;
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
    q.push(Node(bkm[i][0], i));
    csz[i] = bkm[i].size();
  }
  future<bool> sft = async(launch::async, [&]{
    return true;
  });
  char* mid = new char[WRITE_SEGMENT + COM];
  int clen = 0;
  int tlen = 0;
  int slen = 0;
  while (!q.empty()) {
    Node t = q.top();
    q.pop();
    int tr = t.rank;
    raw(t.addr, mid, clen, slen);
    tlen += slen;
    //prt(t.addr);
    //int abc;
    //cin >> abc;
    //PR(tr);
    //prt(t.addr);
    if ((WRITE_SEGMENT < clen || tlen == sz) && sft.get()) {
      int cc = clen;
      char* tmp = mid;
      sft = async(launch::async, [&]{
        //prtt(tmp, cc, 500);
        //sleep(5);
        fwrite(tmp, sizeof(char), cc, off);
        delete[] tmp;
        return true;
      });
      clen = 0;
      mid = new char[WRITE_SEGMENT + COM];
    }
    //PR(where[tr]);
    //PR(csz[tr]);
    if (where[tr] < csz[tr] - 1) {
      where[tr]++;
      q.push(Node(bkm[tr][where[tr]], tr));
    }
  }
  sft.get();
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
  for (int i = 0; i < LARGE; i++) bks[i].clear();
  tha[0] = 0;
  for (int i = 1; i < THREADNUM; i++) tha[i] = tha[i - 1] + LARGE / THREADNUM;
  tha[THREADNUM] = LARGE;
  // read file to segment
  read_file(ifs);
  // sort each small segment
  int ec = 0;
  thread t[THREADNUM];
  while (ec < SEGMENT) {
    for (int i = 0; i < THREADNUM; ++i) 
      t[i] = thread(sort_string, ec + i);
    for (int i = 0; i < THREADNUM; ++i)
      t[i].join();
    ec += THREADNUM;
  }
  if (TESTTIME) fprintf(stderr, "Sort strings %f seconds pass in total.\n", ((float)clock() - st) / CLOCKS_PER_SEC);
  // write back to chars
  FILE* off = fopen(ofs, "wb");
  // merge and write
  merge_and_write(off);
  if (TESTTIME) fprintf(stderr, "Write strings %f seconds pass in total.\n", (float)(clock() - st) / CLOCKS_PER_SEC);
  fclose(off);
  for (int i = 0; i < SEGMENT; ++i)
    delete[] bf[i];
  if (TESTTIME) fprintf(stderr, "Process takes %f seconds in total.\n", (float)(clock() - st) / CLOCKS_PER_SEC);
  return 0;
}
