#include <bits/stdc++.h>

using namespace std ;

const int N = 1e5 + 10 ;
int n,q;
array<int, 3> arr[N];
int uni[N], sz[N], l[N], r[N], mn[N], mnp[N][2];
long long sum[N];
long long ans;

int uni_find(int x)
{
    return uni[x]=(uni[x]==x?x:uni_find(uni[x]));
}

long long calc(int x)
{
    int a=l[x];
    return sum[x] + (sz[x]%2 ? min({mn[x], mnp[x][a&1]}) : 0);
}

void unio(int x,int y)
{
    x=uni_find(x); y=uni_find(y);
    if(x==y) return;
    ans-=calc(x);
    ans-=calc(y);
    sz[x]+=sz[y];
    l[x]=min(l[x], l[y]);
    r[x]=max(r[x], r[y]);
    mn[x]=min({mn[x], mn[y]});
    mnp[x][0]=min(mnp[x][0], mnp[y][0]);
    mnp[x][1]=min(mnp[x][1], mnp[y][1]);
    sum[x]+=sum[y];
    uni[y]=x;
    ans+=calc(x);
}

vector<long long> calculate_costs(vector<int> W, vector<int> A, vector<int> B, vector<int> E) {
	n=W.size(); q=E.size();
    for(int i=0;i<n;i++)
    {
        arr[i]={W[i], A[i], B[i]};
    }
    sort(arr,arr+n);
    for(int i=0;i<n;i++)
    {
        uni[i]=l[i]=r[i]=i;
        sz[i]=1;
        mn[i]=(1<<30);
        sum[i]=arr[i][2]-arr[i][1];
        mnp[i][0]=mnp[i][1]=(1<<30);
        mnp[i][i&1] = -sum[i];
        ans+=A[i];
    }
    vector<array<int, 3> > eds;
    for(int i=0;i<n-1;i++)
    {
        eds.push_back({arr[i+1][0]-arr[i][0], -2, i});
        if(i+2<n)
            eds.push_back({arr[i+2][0]-arr[i][0], -1, i});
    }
    for(int i=0;i<q;i++)
        eds.push_back({E[i], 0, i});
    sort(eds.begin(), eds.end());
    vector<long long> ret(q);
    for(auto ed:eds) {
        if(ed[1]==0) ret[ed[2]]=ans;
        else if(ed[1]==-2) {
            unio(ed[2], ed[2]+1);
        }
        else {
            int x=uni_find(ed[2]), y=uni_find(ed[2]+2);
            assert(x==y);
            ans-=calc(x);
            int cost=arr[ed[2]+1][1] - arr[ed[2]+1][2];
            mn[x]=min(mn[x], cost);
            ans+=calc(x);
        }
    }
    return ret;
}