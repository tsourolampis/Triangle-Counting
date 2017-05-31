#include <cstdio>
#include <cstring>
#include "IO.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;


// Counts triangles exactly
// "Efficient Triangle Counting in Large Graphs via Degree-based Vertex Partitioning"
// Authors: Mihail N. Kolountzakis, Gary L. Miller, Richard Peng, Charalampos E. Tsourakakis
// Copyright (C) Mihail N. Kolountzakis, Gary L. Miller, Richard Peng, Charalampos E. Tsourakakis
// Compile: g++ -o triExact triExact.cc
// Usage: Assume toy.txt is the input graph (first line n m, followed by m lines)
// ./compress < toy.txt > toycompressed.txt (Available at https://github.com/tsourolampis/Graph-Compression)
// ./triExact < toy_compressed.txt


typedef long long ll;
typedef double T;

int startTime;

const int MAXV =       20000000;
const int MAXE =      400000000;
const int QUERYBUFFER = 20000000;
const int KEEPPROB = 100;
int *eList = new(int [MAXE]);
int V;
int eStart[MAXV], eEnd[MAXV];
int degree[MAXV];

const int NOEDGE = -1;
char seen[MAXV];
char countAfter[MAXV];

int queryNext[QUERYBUFFER];
int queryList[QUERYBUFFER];
int queryStart[MAXV];
int queryTotal;

ll answer;

void datain() {
  printf("exact counting, edge sampling rate = %d\n", KEEPPROB);
  int u, v, tot;
  IO::read(V);
  tot = 0;
  for(int u = 0; u < V; ++u) {
    degree[u] = 0;
    eStart[u] = tot;
    int deg;
    IO::read(deg);
    while(deg--) {
      IO::read(eList[tot]);
      if(rand() % 100 < KEEPPROB) {
        tot++;
        degree[u]++;
      }
    }
    eEnd[u] = tot;
  }
  printf("Total number of edges: %d\n", tot);
}

void initializeQuery() {
  queryTotal = 0;
  for(int u = 0; u < V; ++u) {
    queryStart[u] = NOEDGE;
  }
}

void answerQueryBatch() {
  for(int u = 0; u < V; ++u) {
    if(queryStart[u] != NOEDGE) {
      for(int p = eStart[u]; p < eEnd[u] ; p++) {
        seen[eList[p]] = 1;
      }
      for(int p = queryStart[u]; p != NOEDGE; p = queryNext[p]) {
        if (seen[queryList[p]]) answer++;
      }
      for(int p = eStart[u]; p < eEnd[u] ; p++) {
        seen[eList[p]] = 0;
      }
    }
  }
  initializeQuery();
}

inline void query(int u, int v) {
  if(u > v) swap(u, v);
  queryList[queryTotal] = v;
  queryNext[queryTotal] = queryStart[u];
  queryStart[u] = queryTotal;
  queryTotal++; 
  if(queryTotal == QUERYBUFFER) {
    answerQueryBatch();
  }
}

int *reverseList = new(int [MAXE]);
int reverseStart[MAXV], reverseEnd[MAXV];

void addReverseEdge(int u, int v) {
  reverseList[reverseEnd[u]] = v;
  reverseEnd[u]++; 
}

ll countCandidates() {
  ll total = 0, totalReverse = 0;
  memset(reverseStart, 0, sizeof(reverseStart));

  for(int u = 0; u < V; ++u) {
    ll count1 = ll(degree[u]) * ll(degree[u] - 1) / ll(2);
    ll count2 = 0;
    for(int p = eStart[u]; p < eEnd[u]; ++p) {
      count2 += ll(degree[eList[p]]);
    }
    if(count1 * 3LL < count2) {
      total += ll(count1);
      countAfter[u] = 1;
    }
    else {
      total += ll(count2);
      countAfter[u] = 0;
      for(int p = eStart[u]; p < eEnd[u]; ++p) {
        reverseStart[eList[p]] ++;
      }
      totalReverse += degree[u];
    }
  }
  int temp = 0, s = 0;
  for(int u = 0; u < V; ++u) {
    temp = reverseStart[u];
    reverseStart[u] = reverseEnd[u] = s;
    s += temp;
  }
  for(int u = 0; u < V; ++u) {
    if(!countAfter[u]) {
      for(int p = eStart[u]; p < eEnd[u]; ++p) {
        addReverseEdge(eList[p], u);
      }
    }
  }
  printf("Total number of candidates: %lld\n", total);
  printf("Number of Reverse Edges added: %lld\n", totalReverse);
  return total;
}
 
T cube(T x) { return x*x*x;}

void solve() {
  initializeQuery();
  answer = 0;
  printf("starting queries at: %lu\n", clock()-startTime);
  for(int u = 0; u < V; ++u) {
    for(int p1 = reverseStart[u]; p1 < reverseEnd[u]; ++p1) {
      for(int p2 = eStart[u]; p2 < eEnd[u]; ++p2) {
        query(reverseList[p1], eList[p2]);
      }
    }
    if(countAfter[u]) {
      for(int p1 = eStart[u]; p1 < eEnd[u]; ++p1) {
        for(int p2 = eStart[u]; p2 < p1; ++p2) {
          query(eList[p1], eList[p2]);
        }
      }
    }
  }
  answerQueryBatch();
 printf("answered queries at: %lu\n", clock()-startTime);
  printf("# of triangles encountered: %lld\n", answer);
  printf("approx # of triangles in original:\n*\n%lf\n", T(answer) * cube(T(100) / T(KEEPPROB)));
}

int main() {
  srand(time(0));
    startTime = clock();
    datain();
    printf("input finished at: %lu\n", clock()-startTime);
    ll candidates = countCandidates();
    printf("done calculating which type at: %lu\n", clock()-startTime);
    solve();
  return 0;
}

