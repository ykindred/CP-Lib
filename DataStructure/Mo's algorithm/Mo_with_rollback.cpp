//常规莫队在挪动指针时，总是双指针交替挪动，O(1)更新答案
//回滚莫队的思想是：
//对于一些可离线的查询，我们进行add操作比较简单，del操作比较复杂
//我们在查询的时候，先确保右指针单调挪动，每次右指针挪动前，把左指针回溯到最初始的状态（区块右边界）
//再将指针左移以匹配区间
//注意到这样就只有add操作，我们只需要维护一个操作栈即可正确回溯，注意到左指针最多挪动sqrt(n)次

//给定一个长度为n的序列，与q个查询 ，求范围内相同元素的最大间距
#include<bits/stdc++.h>
using namespace std;
int main(){
    int n;
    cin>>n;
    int k;
    cin>>k;
    vector<int>a(n);
    for(int i = 0 ; i< n;i++) cin>>a[i];
    int q;
    cin>>q;
    vector<tuple<int,int,int>> query(q);
    vector<int>ans(q);
    for(int i = 0 ;i<q;i++){
        auto&[l,r,pos] = query[i];
        cin>>l>>r;
        pos = i;
        l -- ,r --;
    }
    int B = 0;
    while(B*B < n) B++;
    sort(query.begin() , query.end() , [&](auto a,auto b){
        auto[l1,r1,p1] = a ; auto [l2,r2,p2] = b;
        if(l1/B < l2/B) return true;
        else if(l1/B == l2/B && r1<r2) return true;
        return false;
    });
    int p1 = 0 ,p2 = 0;
    vector<pair<int,int>>ap(k+1 , {-1,-1});
    vector<int>ap2(k+1,-1);
    while(p1<q){
        while(p2<q && get<0>(query[p2]) / B == get<0>(query[p1]) / B) p2++;
       // cerr<<p1<<" "<<p2<<endl;
        int nB = get<0>(query[p1]) /B ;
        //nB * B , (nB+1)*B - 1
        int p = min((nB+1)*B - 1 ,n - 1) ;
        int q = p;
        ap[a[p]] = {p,p};
        stack<tuple<int,int,int,int>> st;
        int nowans = 0;
        for(int i = p1 ; i<p2 ;i++){
            auto &[l ,r,pos] = query[i]; 
            //l r在同块中必须特判
            if(l / B == r/B){
                stack<int> st;
                int nowans = 0;
                for(int j = l ;j<= r; j++){
                    if(ap2[a[j]] != -1){
                        nowans = max(nowans , j - ap2[a[j]]);
                    }
                    else {
                        st.push(a[j]);
                        ap2[a[j]] = j;
                    }
                }
                ans[pos] = nowans;
                while(!st.empty()){
                    auto t = st.top();
                    st.pop();
                    ap2[t] = -1;
                }
                continue;
            }          
            //右区间扩张
            while(q<r){
                q++;
                if(ap[a[q]].first == -1){
                    ap[a[q]].first = ap[a[q]].second = q; 
                }
                else {
                    ap[a[q]].second = q;
                    nowans = max(nowans , ap[a[q]].second - ap[a[q]].first);
                }
            }
            ///左区间扩张
            while(p>l){
                p --;
                if(ap[a[p]].first == -1){
                    st.push({a[p] , -1 , -1 ,nowans});
                    ap[a[p]].first =ap[a[p]].second = p;
                }
                else {
                    st.push({a[p] , ap[a[p]].first ,ap[a[p]].second , nowans});
                    ap[a[p]].first = p;
                    nowans = max(nowans , ap[a[p]].second - ap[a[p]].first);
                }
            };
            ans[pos] = nowans;
            //撤销左区间修改
            while(!st.empty()){
                auto[t ,of , os, old_ans] = st.top();
                st.pop();
                ap[t] = {of ,os};
                nowans = old_ans;
            }
            p = min((nB+1)*B - 1 ,n - 1);
        }
        for(auto&[x,y]:ap){x = y = -1;}
        p1 = p2;
    }
    for(int i = 0 ;i<q;i++) cout<<ans[i]<<endl;
}