#include <algorithm>
#include <queue>

#include "sphinx.h"

using namespace std;

int count_components(int N, vector<vector<int>> &e, vector<int> &col) {
  int cnt = 0;
  vector<bool> vis(N, false);
  queue<int> q;
  for (int i = 0; i < N; ++i) {
    if (vis[i]) {
      continue;
    }
    ++cnt;
    vis[i] = true;
    q.push(i);
    while (!q.empty()) {
      int cur = q.front();
      q.pop();
      for (int nxt : e[cur]) {
        if (!vis[nxt] && col[nxt] == col[cur]) {
          vis[nxt] = true;
          q.push(nxt);
        }
      }
    }
  }
  return cnt;
}

vector<vector<int>> find_components(int N, vector<vector<int>> &e) {
  vector<vector<int>> comps = {{0}};
  vector<int> ord(N);
  for (int u = 1; u < N; ++u) {
    ord.assign(N, N);
    int n = comps.size();
    vector<int> col(N, N);
    ord[u] = col[u] = -1;
    for (int i = 0; i < n; ++i) {
      for (int v : comps[i]) {
        ord[v] = -1;
        col[v] = i;
      }
    }
    int expected = count_components(N, e, col);
    int cnt = expected - perform_experiment(ord);
    if (cnt == 0) {
      comps.push_back({u});
      continue;
    }
    int lo = 0, hi = n;
    vector<int> comps_to_merge;
    while (cnt > 0) {
      while (lo + 1 < hi) {
        int mid = (lo + hi) / 2;
        ord.assign(N, N);
        col.assign(N, N);
        ord[u] = col[u] = -1;
        for (int i = mid; i < hi; ++i)
          for (int v : comps[i]) {
            ord[v] = -1;
            col[v] = i;
          }
        expected = count_components(N, e, col);
        if (perform_experiment(ord) < expected) {
          lo = mid;
        } else {
          hi = mid;
        }
      }
      comps_to_merge.push_back(lo);
      lo = 0, --hi;
      --cnt;
    }
    int to = comps_to_merge.back();
    comps_to_merge.pop_back();
    for (int from : comps_to_merge) {
      for (int v : comps[from]) {
        comps[to].push_back(v);
      }
      comps.erase(comps.begin() + from);
    }
    comps[to].push_back(u);
  }

  return comps;
}

void solve(vector<int> &A, vector<int> &FC, int N, vector<vector<int>> &e,
           vector<vector<int>> &comps) {
  vector<int> ord(N), col(N);
  for (int f = 0; f < N; ++f) {
    int lo = 0, hi = A.size();
    while (true) {
      ord.assign(N, f);
      col.assign(N, N);
      for (int i = lo; i < hi; ++i)
        for (int u : comps[A[i]]) {
          ord[u] = -1;
          col[u] = i;
        }
      int cnt = count_components(N, e, col) - perform_experiment(ord);
      if (cnt == 0) break;
      while (lo + 1 < hi) {
        int mid = (lo + hi) / 2;
        ord.assign(N, f);
        col.assign(N, N);
        for (int i = mid; i < hi; ++i)
          for (int u : comps[A[i]]) {
            ord[u] = -1;
            col[u] = i;
          }
        if (perform_experiment(ord) < count_components(N, e, col)) {
          lo = mid;
        } else {
          hi = mid;
        }
      }
      FC[A[lo]] = f;
      lo = 0, --hi;
      if (cnt == 1) break;
    }
  }
}

vector<int> find_colours(int N, vector<int> X, vector<int> Y) {
  vector<vector<int>> e(N);
  int M = X.size();
  for (int i = 0; i < M; ++i) {
    e[X[i]].push_back(Y[i]);
    e[Y[i]].push_back(X[i]);
  }

  auto comps = find_components(N, e);
  int n = comps.size();
  if (n == 1) {
    for (int f = 0; f < N; ++f) {
      vector<int> ord(N, f);
      ord[0] = -1;
      if (perform_experiment(ord) == 1) return vector<int>(N, f);
    }
  }

  vector<int> cid(N, -1);
  for (int c = 0; c < n; ++c)
    for (int u : comps[c]) cid[u] = c;
  vector<vector<int>> adj(n, vector<int>(n, 0));
  for (int i = 0; i < M; ++i) {
    int u = cid[X[i]], v = cid[Y[i]];
    if (u == v) continue;
    adj[u][v] = adj[v][u] = 1;
  }

  vector<int> vis(n, 0);
  queue<int> q;
  for (int i = 0; i < n; ++i) {
    if (vis[i]) continue;
    q.push(i);
    vis[i] = 1;
    while (!q.empty()) {
      int cur = q.front();
      q.pop();
      for (int nxt = 0; nxt < n; ++nxt) {
        if (adj[cur][nxt] && vis[nxt] == 0) {
          vis[nxt] = (vis[cur] == 1 ? 2 : 1);
          q.push(nxt);
        }
      }
    }
  }
  vector<int> A, B;
  for (int i = 0; i < n; ++i) {
    if (vis[i] == 1)
      A.push_back(i);
    else
      B.push_back(i);
  }

  vector<int> FC(n, -1);
  solve(A, FC, N, e, comps);
  solve(B, FC, N, e, comps);

  vector<int> F(N);
  for (int i = 0; i < N; ++i) F[i] = FC[cid[i]];
  return F;
}
