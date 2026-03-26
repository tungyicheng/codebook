struct Binary_Indexed_Tree{
    int n;
    vector<long long> bit;
    int lowbit(int x){
        return x&-x;
    }
    void init(int _n){
        n = _n+1;
        bit = vector<long long>(n,0);
    }
    void update(int x,int v){
        for(; x<n; x+=lowbit(x)){
            bit[x] += v;
        }
    }
    long long query(int x){
        long long ret = 0;
        for(; x>0; x-=lowbit(x)){
            ret += bit[x];
        }
        return ret;
    }
}BIT;

// ===== 離散化 =====
// vector<int> arr;
vector<int> tmp(arr);
sort(tmp.begin(), tmp.end());
tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
for (int i = 0; i < n; i++)
    arr[i] = lower_bound(tmp.begin(), tmp.end(), arr[i]) - tmp.begin();

// ===== Implementation 1: 逆序对计数 =====
// 统计数组中的逆序对数量
// 逆序对: 对于 i < j，如果 a[i] > a[j]，则 (i,j) 为一个逆序对
void solve1(){  
    int n;
    cin >>n;
    int ans = 0;
    BIT.init(1000005);
    // 遍历数组
    for(int i = 1 ; i <= n; i ++ ){
        int a;
        cin >> a;
        if(a==0){
            continue;
        }
        else {
            // 查询有多少个已经出现过且比 a 小的数
            ans += BIT.query(a-1);
            // 将当前数字 a 加入到 BIT 中
            BIT.update(a,a);
        }
    }
    cout << ans << endl;
}

// ===== Implementation 2: 区间更新与单点查询 =====
// 支持对一个区间内的所有元素加上同一个值
// 以及查询单个位置的值
void solve2(){
    int n,q;
    cin >> n >> q;
    int pre = 0;
    int a;
    BIT.init(n);
    // 读取初始数组，存储差分值
    for(int i = 1;i <= n; i++){
        cin >> a;
        BIT.update(i,a-pre);
        pre = a;
    }
    // 处理 q 个查询
    for(int i = 0 ;i<q;i++){
        int t;
        cin >> t;
        if(t == 1){
            // 区间更新: 对 [a, b] 区间加上 u
            int b,u;
            cin >> a >> b >> u;
            BIT.update(a,u);
            BIT.update(b+1,-u);
        }
        else{
            // 单点查询: 查询位置 a 的值
            cin >> a;
            cout << BIT.query(a)<<endl;
        }
    }
}

// ===== Implementation 3: 离散化 + 平衡查询 =====
// 用两个数组 a[i] 和 b[i] 表示某种属性
// 离散化后，检查是否满足某个平衡条件
void solve3(){
    int n,k;
    cin >> n>> k;
    vector<pair<int , int>> prefect(n+1);
    vector<int> a(n+1),b(n+1),tmp(n+1,0);
    for(int i = 1 ;i<=n;i++){
        cin >> a[i] >> b[i];
    }

    // 对 a[] 进行离散化处理
    for(int i = 1; i <= n; i++) tmp[i] = a[i];
    sort(tmp.begin() + 1, tmp.begin() + n + 1);
    int len = unique(tmp.begin() + 1, tmp.begin() + n + 1) - (tmp.begin() + 1);
    for(int i = 1; i <= n; i++)
        a[i] = lower_bound(tmp.begin() + 1, tmp.begin() + 1 + len, a[i]) - (tmp.begin() + 1) + 1;

    // 对 b[] 进行离散化处理
    for(int i = 1; i <= n; i++) tmp[i] = b[i];
    sort(tmp.begin() + 1, tmp.begin() + n + 1);
    int lenb = unique(tmp.begin() + 1, tmp.begin() + n + 1) - (tmp.begin() + 1);
    for(int i = 1; i <= n; i++)
        b[i] = lower_bound(tmp.begin() + 1, tmp.begin() + 1 + lenb, b[i]) - (tmp.begin() + 1) + 1;

    // 组合成对并排序
    for(int i  = 1;i<=n;i++){
        prefect[i] = {a[i],b[i]};
    }
    sort(all(prefect));
    
    vector<int> corn(n+1);
    BIT.init(n);
    int num = 0;
    int conr;
    for(int i = 1 ;i <= n;i++){
        // 统计前面有多少个 b 值小于当前 b[i]
        conr = BIT.query(prefect[i].second-1);
        BIT.update(prefect[i].second,1);

        // 计算平衡度: 左边比当前小的数 vs 右边比当前小的数
        int gate = abs(conr -(n-1-conr));
        if(gate >= k){
            num++;
        } 
    }
    cout << num << endl;
}

// ===== Implementation 4: 高级逆序对计数 =====
// 计算对于每个元素，有多少个数对 (i,j)，其中 i < j
// 且 a[i], a[j] 相对于当前元素形成某种关系
void solve4(){
    int n;
    cin >> n;
    vector<int> a(n+1,0);
    for(int i = 1;i<=n;i++){
        cin >> a[i];
    }
    
    // 离散化: 将原数组值映射到 1~len 的范围
    vector<int> tmp(a.begin()+1,a.end()); 
    sort(tmp.begin(), tmp.end());
    tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
    for (int i = 1; i <= n; i++)
        a[i] = (lower_bound(tmp.begin(), tmp.end(), a[i]) - tmp.begin())+1;
    
    // 计算 left[i]: 在位置 i 左边有多少个数比 a[i] 大
    vector<int> left(n+1,0),right(n+1,0);
    BIT.init(n);
    for(int i =1 ;i<=n;i++){
        left[i] = (i-1) - BIT.query(a[i]);
        BIT.update(a[i],1);
    }
    
    // 计算 right[i]: 在位置 i 右边有多少个数比 a[i] 小
    BIT.init(n);
    for(int i = n;i>0;i--){
        right[i] = (a[i]>1)?BIT.query(a[i]-1):0;
        BIT.update(a[i],1);
    }
    
    // 答案 = sum(left[i] * right[i])
    // 表示有多少个三元组 (i,j,k) 满足 i<j<k，且 a[i]>a[j]<a[k]
    int ans = 0;
    for(int i = 1;i<=n;i++){
        ans += right[i]*left[i];
    }
    cout << ans;
}

