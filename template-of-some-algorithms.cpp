#include <bits/stdc++.h>

// Fast in-out stream helps a lot.
namespace io    {

    // Read an integer x (use "__int128").
    // x must less than 2 ^ 127 - 1, and also greater than - 2 ^ 127.
    inline __int128 rd_int()  {
        int x = 0, w = 1;   char ch;
        do  {
            if(ch == '-')   w = -1;
            ch = getchar();
        }	while(!isdigit(ch));
        do  {
            x = (x << 3) + (x << 1) + ch - '0', ch = getchar();
        }	while(isdigit(ch));
        return x * w;
    }

    // Print an integer x (use "__int128").
    // x must less than 2 ^ 127 - 1, and also greater than - 2 ^ 127.
    inline void prt_int(__int128 x)   {
        static int s[45], d = 0;
        do  s[++d] = x % 10, x /= 10;   while(x);
        do  putchar(s[d--] + '0');   	while(d);
    }

    // Read an float number x (use "long double").
    // x must greater than -1.2 * 10 ^ -4932, and also less than 1.2 * 10 ^ 4932.
    inline long double rd_dbl()	{
        long double x = 0;  int w = 1, m = 0;  char ch;
        do  {
            if(ch == '-')   w = -1;
            ch = getchar();
        }   while(!isdigit(ch));
        do  {
            x = x * 10 + ch - '0', ch = getchar();
        }   while(isdigit(ch));
        if(ch != '.')   return x * w;
        ch = getchar();
        do  {
            x = x + pow(10, --m) * (ch - '0'), ch = getchar();
        }   while(isdigit(ch));
        return w ? x : -x;
    }

    // Print an float number x (use "long double").
    // x must greater than -1.2 * 10 ^ -4932, and also less than 1.2 * 10 ^ 4932.
    // f means print f figures after the decimal point.
    inline void prt_dbl(long double x, __int128 f)   {
        prt_int((__int128)x), x -= (__int128)x;
        if(x)   putchar('.');
        do  {
            x = x * 10, putchar((__int128)x + '0'), x -= (__int128)x;
        }   while(--f);
    }
}

// Disjoint Set Union aka "DSU".
namespace dsu   {

    // MAXN is the possible maximum number.
    const int MAXN = 100005;
    int fa[MAXN];

    // Initialize DSU.
    void init() {for(int i = 1; i <= MAXN; ++i) fa[i] = i;}

    // Get the father of the element x.
    int gf(int x)   {return fa[x] == x ? x : fa[x] = gf(fa[x]);}

    // Check the relationship of the elements x and y.
    // The function returns true when x and y are in the same set, false in the opposite situation.
    bool chk(int u, int v)  {return ((fa[u] = gf(fa[u])) == (fa[v] = gf(fa[v])));}

    // Put the elements x and y in the same set.
    void unt(int u, int v)    {fa[(fa[u] = gf(fa[u]))] = (fa[v] = gf(fa[v]));}
}

// Segment Tree is a common data structure which could solve a lot of problems, such as RMQs.
// This is a simple example of Segment Tree. This code could be accepted in Luogu P3372.
namespace segment_t {

    #define int long long
    #define ls (x << 1)
    #define rs ((x << 1) | 1)
    #define mid ((p[x].l + p[x].r) >> 1)
    const int MAXN = 100005;

    // n is the number of a_i, and m is the number of operations.
    int n, m, a[MAXN];

    // Segment Tree needs about 4X memory.
    struct node {
        int l, r, sum, tag;
    }   p[(MAXN << 2) + 10];

    // Push up. Collect information of x's son nodes, and update the information of node x.
    inline void push_up(int x)   {
        p[x].sum = p[ls].sum + p[rs].sum;
    }

    // Push down. Give away the lazy tags of x to its son nodes.
    inline void push_down(int x)    {
        if(!p[x].tag)   return;
        p[ls].sum += (p[ls].r - p[ls].l + 1) * p[x].tag, p[rs].sum += (p[rs].r - p[rs].l + 1) * p[x].tag;
        p[ls].tag += p[x].tag, p[rs].tag += p[x].tag, p[x].tag = 0;
    }

    // Build the tree. 
    void build(int x, int l, int r)    {
        p[x].l = l, p[x].r = r;
        if(l == r)  {
            p[x].sum = a[l];
            return;
        }
        build(ls, l, mid), build(rs, mid + 1, r), push_up(x);
    }
    
    // Modify the range. a_i + k, when l <= i <= r.
    void modify(int x, int l, int r, int k) {
        if(l <= p[x].l && p[x].r <= r)  {
            p[x].sum += (p[x].r - p[x].l + 1) * k, p[x].tag += k;   return;
        }
        push_down(x);
        if(mid >= l)        modify(ls, l, r, k);
        if(r >= mid + 1)    modify(rs, l, r, k);
        push_up(x);
    }

    // Make a query. This function return the sum of a_i, when l <= i <= r.
    int query(int x, int l, int r)  {
        if(l <= p[x].l && p[x].r <= r)  return p[x].sum;
        push_down(x);   int ans = 0;
        if(mid >= l)        ans += query(ls, l, r);
        if(r >= mid + 1)    ans += query(rs, l, r);
        return ans;
    }
    #undef int
    #undef mid
    #undef ls
    #undef rs
}

// President Tree is a data structure and is another variety of Segment Tree which could solve "range k-th problems".
namespace psdent_t    {
    
    // mid is the midpoint of query range.
    // query_sum is the sum of the node p's left son.
    #define mid ((l + r) >> 1)
    #define query_sum (p[p[rver].ls].sum - p[p[lver].ls].sum)
    
    // Discretization is usually necessary.
    const int MAXN = 200000;
    int n, m, a[MAXN + 10], refr[MAXN + 10], rlen, tot, root[MAXN + 10];

    // President Tree needs a lot of memory (usually 32X).
    struct node {
        int l, r, ls, rs, sum;
    }   p[(MAXN << 5) + 10];

    // Build the tree.
    // Use dir to be the id of the local node.
    int build(int l, int r) {
        int dir = ++tot; p[dir].l = l, p[dir].r = r;
        if(l == r)  return dir;
        p[dir].ls = build(l, mid), p[dir].rs = build(mid + 1, r);   return dir;
    }

    // Update. Insert a new node to the segment tree.
    // Use ls & rs to memorize the left son and the right son.
    int update(int root, int l, int r, int k)    {
        int dir = ++tot; p[dir] = p[root], ++p[dir].sum;
        if(l == r)  return dir;
        k <= mid ? p[dir].ls = update(p[dir].ls, l, mid, k) : p[dir].rs = update(p[dir].rs, mid + 1, r, k);
        return dir;
    }

    // Make a query. As if using prefixs, lver & rver keep the range info.
    // l & r keeps the left and right limits of value a_i.
    int query(int rver, int lver, int l, int r, int k) {
        if(l == r)  return l;
        return k > query_sum ? query(p[rver].rs, p[lver].rs, mid + 1, r, k - query_sum) : query(p[rver].ls, p[lver].ls, l, mid, k);
    }

    // Initailize. Discret the a_i, then insert them to the tree one by one.
    void init() {
        n = io::rd_int(), m = io::rd_int(); for(int i = 1; i <= n; ++i) a[i] = io::rd_int(), refr[i] = a[i];
        sort(refr + 1, refr + 1 + n), rlen = unique(refr + 1, refr + 1 + n) - refr - 1;
        for(int i = 1; i <= n; ++i) a[i] = lower_bound(refr + 1, refr + 1 + rlen, a[i]) - refr;
        root[0] = build(1, rlen);
        for(int i = 1; i <= n; ++i) root[i] = update(root[i - 1], 1, rlen, a[i]);
    }
    #undef mid
    #undef query_sum
}

int main()  {}
