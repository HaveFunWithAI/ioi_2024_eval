#include "message.h"
#include <bits/stdc++.h>

using namespace std;

#define rep(i, n) for(int i = 0, i##__n = (int)(n); i < i##__n; ++i)
#define fer(i, a, b) for(int i = (int)(a), i##__b = (int)(b); i < i##__b; ++i)
#define rof(i, b, a) for(int i = (int)(b), i##__a = (int)(a); i-- > i##__a; )
#define sz(x) (int((x).size()))
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define X first
#define Y second
//#define endl '\n'

template<class P, class Q> inline void smin(P &a, Q b) { if (b < a) a = b; }
template<class P, class Q> inline void smax(P &a, Q b) { if (a < b) a = b; }

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<double, double> pdd;

///////////////////////////////////////////////////////////////////////////

const int plen = 31;
const int maxe = 16;

void send_message(vector<bool> M, vector<bool> C) {
	vector<vector<bool>> A(max(maxe, (sz(M) + 15) / 16 + 2), vector<bool>(plen, false));

	vector<int> nxt(plen, 1);
	rep(b, plen) if(!C[b])
		while(C[(b + nxt[b]) % plen]) nxt[b]++;
	rep(b, plen) if(!C[b]) A[nxt[b] - 1][b] = true;
	
	int pos = 0;
	rep(i, sz(A)) rep(b, plen) if(!C[b] && nxt[b] <= i)
		A[i][b] = (pos < sz(M) ? M[pos] : (pos == sz(M) ? true : false)), pos++;

	rep(i, sz(A)) send_packet(A[i]);
}

vector<bool> receive_message(vector<vector<bool>> A) {
	vector<int> nxt(plen, 0);
	rep(b, plen) rep(i, maxe) if(A[i][b]) { nxt[b] = i + 1; break; }
	vector<bool> C(plen, true);
	rep(b, plen) {
		int cnt = 0;
		vector<bool> mark(plen, false);
		for(int v = (b + nxt[b]) % plen; !mark[v]; v = (v + nxt[v]) % plen) mark[v] = true, cnt++;
		if(cnt == 16) rep(v, plen) C[v] = !mark[v];
	}

	vector<bool> M;
	rep(i, sz(A)) rep(b, plen) if(!C[b] && nxt[b] <= i)
		M.pb(A[i][b]);

	while(!M.back()) M.pop_back();
	M.pop_back();

	return M;
}
