// Codeforce 1 - KSM + 位操作
const int mod = 1e9+7;
int ksm(int b,int e){
    int res = 1; b%=mod;
    while(e>0){
        if(e%2==1) res = (res*b)%mod;
        b = (b*b)%mod; e/=2;
    }
    return res;
}
void solve_cf1(){
    int n,k; cin >> n >> k;
    vector<int> a(n);
    vector<vector<int>> one(n);
    int mxbit = 0;
    for(int i = 0; i < n; i++){
        cin >> a[i];
        for(int b = 0; (a[i] >> b) > 0; b++)
            if((a[i] >> b) & 1){
                one[i].push_back(b);
                mxbit = max(mxbit, b);
            }
    }
    int ans = 0;
    for(int j = mxbit;j>=0;j--){
        int cost = 0;
        for(int x = 0 ; x < n ; x++){
            auto it = upper_bound(one[x].begin(),one[x].end(),j);
            if(it == one[x].begin()){ cost = LLONG_MAX; break; }
            it--; cost += (j-*it);
            if(cost > k) break;
        }
        if(cost <=k){
            for(int x = 0 ; x < n ; x++){
                auto it = upper_bound(one[x].begin(),one[x].end(),j);
                it--; one[x].erase(it);
            }
            k-=cost; int leftshift = k/n;
            ans += ksm(2,j+leftshift);
            k-=n*leftshift;
        }
    }
    cout << ans% ((int)1e9 + 7);
}
// Codeforce 2 - DP 騎士路徑計數
int dp[1000005][4][4] = {0};
bool check(int x,int y){
    return x>=0 && y >=0 && x<1000000 && y < 4;
}
void solve_cf2(){
    const int mod = 1000000007;
    int n; cin >> n;
    int dx[4] = { 2 , 2 , 1 , 1 };
    int dy[4] = {-1 , 1 ,-2 , 2 };
    memset(dp, 0, sizeof(dp));
    for(int i = 0;i<4;i++){
        int nx = 0 + dx[i];
        int ny = 0 + dy[i];
        if(check(nx,ny)) dp[nx][ny][i] = 1;
    }
    for(int i = 1; i < n; i++) {
        for(int j = 0; j < 4; j++) {
            for(int lk = 0; lk < 4; lk++) {
                if(dp[i][j][lk] == 0) continue;
                for(int k = 0; k < 4; k++) {
                    if(k == lk) continue;
                    int nx = i + dx[k];
                    int ny = j + dy[k];
                    if(check(nx, ny))
                        dp[nx][ny][k] = (dp[nx][ny][k] + dp[i][j][lk]) % mod;
                }
            }
        }
    }
    int ans = 0;
    for(int i = 0;i<4;i++) ans = (ans + dp[n-1][3][i])%mod;
    cout << ans << endl;
}
