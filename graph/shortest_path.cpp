// ===== BFS =====
void bfs(int s) {  // 起點
    queue<int> q;  // 這個存走訪名單
    q.push(s);     // s 第一個放入要走訪的名單
    vis[s] = true;
    
    while (!q.empty()) {
        int u = q.front();   // u 是現在走訪的節點
        q.pop();             // 把 u 從走訪名單上踢掉

        for (int v : g[u]) {          // 把 u 的每個鄰居 v 都放進名單中
            if (vis[v] == false) {    // 如果沒有走訪過這個鄰居
                q.push(v);            // 就把這個鄰居丟進去名單裡面
                vis[v] = true;        // 這樣子 v 就算是走訪過囉
                dis[v] = dis[u] + 1;  // 順便算一下 s 到 v 的距離是多少
            }
        }
    }
}

// ===== DFS 偵測無向圖環 =====
int vis[MXN];
bool dfs(int u, int p) {
    if (vis[u]) return true;  // 遇到祖先就代表有環，回傳 true
    vis[u] = 1;
    for (auto v : g[u]) {
        if (v == p)      // 邊連到父節點: 不理他 (因為這是無向圖)
            continue;
        if (dfs(v, u)) 
            return true; // 如果在子孫已經出現環，回傳 true
    }
    return false;
}

// ===== DFS 偵測有向圖環 =====
bool vis[MXN] = {0};                // 初始化為 0 (白點)
bool dfs(int u) {
    if (vis[u] == 1) return true;   // 當走到還沒走完的點，就代表有環
    if (vis[u] == 2) return false;  // 走到已經走完的節點，當沒事發生
    vis[u] = 1;                     // 標記這個點還沒被走完 (白點 -> 灰點)
    for (auto v : g[u]) {
        if (dfs(v)) {
            return true;
        }
    }
    vis[u] = 2;                     // 標記這個點已經被走完 (灰點 -> 黑點)
    return false;
}

// ===== Dijkstra =====
void dijkstra(int s, int t) {      // 起點，終點
    int dis[N];
    for(int i = 0; i < N; i++) {   // 初始化
        dis[i] = INF;              // 值要設為比可能的最短路徑權重還要大的值
    }
    dis[s] = 0;
    priority_queue<pii, vector<pii>, greater<pii>> pq; // 以小到大排序
    pq.push({dis[s], s});
    while(!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        if(vis[u]) continue;
        vis[u] = 1;
        for(auto [v, w] : vec[u]) {
            if(dis[u] + w < dis[v]) {  // 鬆弛
                dis[v] = dis[u] + w;
                pq.push({dis[v], v});
            }
        }
    }
}

// ===== Bellman-Ford =====
void bellman_ford(int n, int m) {
    for(int t = 1; t < n; t++) {   // 注意: 這邊只需要跑 n-1 次，而不是 n 次
        for (int i = 0; i < m; i++) {
            if (dis[edge[i].u] + edge[i].w < dis[edge[i].v]) {
                dis[edge[i].v] = dis[edge[i].u] + edge[i].w;
            }
        }
    }
}

// ===== SPFA =====
struct edge {
    int v, w;
};
int s, n;  // 起點 點數量
vector<edge> E[MXN];
vector<int> dis(MXN, INF), cnt(MXN), inq(MXN);
// cnt記錄最短路經過幾條邊
// inq記錄節點是否在queue裡面
queue<int> Q;

void spfa() {
    dis[s] = 0; 
    Q.push(s); 
    inq[s] = 1;
    while (Q.size()) {
        int u = Q.front(); 
        Q.pop();
        inq[u] = 0;
        for (auto [v, w] : E[u]) {
            if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
                cnt[v] = cnt[u] + 1;
                if (cnt[v] >= n) {
                    // 有負環 因為一般最短路只會經過 n - 1 條邊
                }
                if (!inq[v]) Q.push(v), inq[v] = 1;
            }
        }
    }
}

// ===== Floyd-Warshall =====
int dis[N + 1][N + 1];

void floyd_warshall() {
    for(int i = 1; i <= N; i++) {
        for(int j = 1; j <= N; j++) {
            dis[i][j] = INF;  // INF 就是很大的數
        }
        dis[i][i] = 0;
    }
    // 輸入邊的資訊
    int u, v, w;
    cin >> u >> v >> w;
    dis[u][v] = min(dis[u][v], w);
    
    // Floyd-Warshall 演算法
    for(int k = 1; k <= N; k++) {  // 窮舉中繼點k
        for(int i = 1; i <= N; i++) {
            for(int j = 1; j <= N; j++) {  // 窮舉點對(i,j)
                dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
            }
        }
    }
}
