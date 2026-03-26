#define int long long
#define INF 1e18
#define pii pair<int,int>

struct Edge {
    int u, v, w;
};

// ===== Implementation 1: Dijkstra - 求最短路径距离 =====
// 从源点到所有点的最短路径
void solve1(){
    int n,m;
    cin >> n >> m;
    vector<int> dis(n+5,0),vis(n+5,0);
    vector<vector<pii>>  vec(n+5); // pair 存 {終點,權重}
    for(int i = 0 ;i<=n;i++){
        dis[i] = INF;
    }
    dis[1] = 0;
    
    // 读取边的信息
    for(int i = 0 ;i<m;i++){
        int a,b,c;
        cin >> a >> b >> c;
        vec[a].push_back(make_pair(b,c));
    }

    // 使用优先队列实现 Dijkstra，按距离从小到大排序
    priority_queue<pii,vector<pii>,greater<pii>> pq;
    pq.push({dis[1],1});
    
    while(pq.empty() == 0){
        int u = pq.top().second;
        pq.pop();
        
        // 若已访问过则跳过
        if(vis[u])continue;
        vis[u] = 1;
        
        // 对所有相邻节点进行松弛操作
        for(int i = 0 ;i  <vec[u].size();i++){
            int v = vec[u][i].first;
            int w = vec[u][i].second;
            if(dis[u]+w<dis[v]){
                dis[v]=dis[u]+w;
                pq.push({dis[v],v});
            }
        }
    }
    
    // 输出所有点的最短距离
    for(int i = 1 ;i<=n;i++){
        cout << dis[i]<<" ";
    }
}

// ===== Implementation 2: Dijkstra - 记录最短路径 =====
// 记录每个点的前驱边编号，用于重构最短路径
void solve2(){
    int n,m;
    cin >> n >> m;

    vector<int> dis(n+5), where_he_from(n+5), vis(n+5,0);

    // 存储 {终点, {权重, 边编号}}
    vector<vector<pair<int,pair<int,int>>>> vec(n+5);

    for(int i=1;i<=n;i++) dis[i]=INF;
    dis[1]=0;

    // 读取无向边
    for(int i=1;i<=m;i++){
        int a,b,c;
        cin >> a >> b >> c;
        vec[a].push_back({b,{c,i}});
        vec[b].push_back({a,{c,i}});
    }

    priority_queue<pii,vector<pii>,greater<pii>> pq;
    pq.push({0,1});

    while(!pq.empty()){
        int u = pq.top().second;
        pq.pop();

        if(vis[u]) continue;
        vis[u] = 1;

        // 对相邻节点进行松弛，并记录前驱边
        for(int i = 0 ;i<vec[u].size();i++){
            int v = vec[u][i].first;
            int w = vec[u][i].second.first;
            int edge_id = vec[u][i].second.second;
            
            if(dis[u]+w<dis[v]){
                where_he_from[v] = edge_id;
                dis[v]=dis[u]+w;
                pq.push({dis[v],v});
            }
        }
    }

    // 输出每个节点（除了源点）的前驱边编号
    for(int i=2;i<=n;i++){
        cout << where_he_from[i] << " ";
    }
}

// ===== Implementation 3: Floyd-Warshall 算法 =====
// 求所有点对之间的最短路径
void solve3(){
    int n,m,q;
    cin >> n >> m >> q;
    int N = n;
    int dis[N + 1][N + 1];
    
    // 初始化：同一点距离为 0，其他为无穷大
    for(int i = 1; i <= N; i++){
        for(int j = 1; j <= N; j++){
            dis[i][j] = INF;
        }
        dis[i][i] = 0;
    }
    
    // 读取边信息（双向边，取较小值）
    while(m--){
        int a, b, c;
        cin >> a >> b >> c;
        dis[a][b]=min(dis[a][b], c);
        dis[b][a]=min(dis[b][a], c);
    }

    // Floyd-Warshall 主算法：枚举中间点 k
    for(int k=1;k<=N;k++){
        // 枚举所有点对 (i,j)
        for(int i=1;i<=N;i++){
            for(int j=1;j<=N;j++){
                // 尝试通过点 k 的路径
                dis[i][j]=min(dis[i][j],dis[i][k]+dis[k][j]);
            }
        }
    }
    
    // 处理 q 个查询
    while(q--){
        int a,b;
        cin >> a >> b;
        if(dis[a][b]!=INF){
            cout << dis[a][b]<<endl;
        }
        else cout << -1 << endl;
    }
}

// ===== Implementation 4: Bellman-Ford 算法 =====
// 可以处理负权边，并检测负环
void solve4(){    
    const long long INF2 = (1LL<<60);
    int n,m;
    cin >> n >> m;
    
    struct Edge {
        int u, v, w;
    };
    vector<Edge> e(m);
    vector<int> dis(n + 1, INF2),neg(n+1,0);
    
    // 读取边，权重取负（用于处理最大值问题）
    for (int i = 0; i < m; i++){
        cin >> e[i].u >> e[i].v >> e[i].w;
        e[i].w = -e[i].w;
    }
    dis[1]=0;
    
    // 第一阶段：放松所有边 n-1 次
    for (int t = 1; t < n; t++) {
        for (int i = 0; i < m; i++) {
            int u = e[i].u, v = e[i].v, w = e[i].w;
            if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
            }
        }
    }
    
    // 第二阶段：再松弛一次，检测负环
    // 如果还能松弛则存在负环，标记受影响的节点
    for (int t = 1; t < n; t++) {
        for (int i = 0; i < m; i++) {
            int u = e[i].u, v = e[i].v, w = e[i].w;
            if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
                neg[v] = 1;
            }
            // 从负环影响的点出发的边也受负环影响
            if (neg[u]) neg[v] = 1;
        }
    }
    
    // 如果终点受到负环影响，输出 inf；否则输出最短路径
    if(neg[n]){
        cout <<"inf";
    }
    else cout << -dis[n];
}
