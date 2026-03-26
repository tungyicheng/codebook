// ===== 拓撲排序 - 基礎版本 =====
vector<int> edge[MAXN], ans;  
int deg[MAXN];      

void topo(int n) {  
    queue<int> q; 
    
    for (int i = 1; i <= n; i++)
        if (!deg[i]) 
            q.push(i);
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        ans.push_back(u); 
        
        for (int v : edge[u]) {
            deg[v]--; 
            if (!deg[v]) 
                q.push(v);
        }
    }
}

// ===== 拓撲排序 - 完整版本 with 檢查 =====
void solve(){
    int n, m;
    cin >> n >> m;
    vector<int> edge[m + 5];
    vector<int> deg(n + 5, 0);
    vector<int> ans;
    
    for(int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;    // 點 u -> v
        edge[u].push_back(v);
        deg[v]++;         // v 的入度 +1
    }   
    
    queue<int> que;
    for(int i = 1; i <= n; i++) {
        if(deg[i] == 0) que.push(i); // 度數為 0 時推入 queue 中
    }
    
    while (!que.empty()) {
        int u = que.front();
        que.pop();
        ans.push_back(u); // 結果存 ans
        
        for (int i : edge[u]) {
            deg[i]--;     // 每次走訪完都先幫鄰居的入度 -1
            if (!deg[i])  // 如果入度 == 0 則加入此節點
                que.push(i);
        }
    }
    
    if(ans.size() != n) {
        cout << "IMPOSSIBLE";  // 有環，無法進行拓撲排序
    }
    else {
        for(int i = 0; i < n; i++) {
            cout << ans[i] << " \n"[i == n];
        }
    }
}

// ===== 拓撲排序 + DP (計數路徑) =====
void solve_dp(){
    int n, m;
    cin >> n >> m;
    
    vector<int> edge[n + 5];
    vector<int> deg(n + 5, 0);
    vector<long long> dp(n + 5, 0);
    
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        edge[u].push_back(v);
        deg[v]++;
    }
    
    dp[1] = 1;  // 初始起點
    
    queue<int> q;
    for (int i = 1; i <= n; i++) {
        if (deg[i] == 0) {
            q.push(i);
        }
    }
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int i : edge[u]) {
            dp[u] %= (int)1e9 + 7;
            dp[i] = dp[i] + dp[u];  // DP 轉移: 累加所有入邊的路徑數
            dp[i] %= (int)1e9 + 7;
            deg[i]--;
            
            if (deg[i] == 0)
                q.push(i);
        }
    }   
    
    dp[n] %= (int)1e9 + 7;
    cout << dp[n];  // 輸出到達終點的路徑數
}
