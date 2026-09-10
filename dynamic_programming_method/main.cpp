#include<bits/stdc++.h>
using namespace std;

struct Ans{
    int max_profit;
    string solution;
    Ans(): max_profit(-1), solution(""){};
};

Ans Dynamic_Programing(int n, int capacity, vector<int> &w, vector<int> &v){
    Ans ans;
    vector<int> dp(capacity + 1, 0);
    vector<long long> sol(capacity + 1, 0);//n must less or equal than 64

    for(int i = 0; i < n; ++i){
        for(int j = capacity; j - w[i] >= 0; --j){
            if(dp[j] < dp[j - w[i]] + v[i]){
                dp[j] = dp[j - w[i]] + v[i];
                sol[j] = (sol[j - w[i]] | (1LL << i));
            }
        }
    }

    ans.max_profit = dp[capacity];
    for(long long k = sol[capacity], i = 0; i < n; k >>= 1, ++i){
        if(k & 1) ans.solution.push_back('1');
        else ans.solution.push_back('0');
    }

    return ans;
}

int main(){
    ifstream in;
    ofstream out;
    cout << "Select the test data(dt01, dt02, dt03...):";
    string data;
    cin >> data;

    //讀入測資
    in.open("dataset/" + data + "/item.txt");
    int n, capacity;
    in >> n >> capacity;
    vector<int> w(n), v(n);
    for(int i = 0; i < n; ++i){
        in >> w[i] >> v[i];
    }
    in.close();

    //solve
    Ans ans = Dynamic_Programing(n, capacity, w, v);

    //輸出答案
    out.open("ans_" + data + ".txt");
    out << "max profit:" << ans.max_profit << endl;
    out << "solution:" << ans.solution;
    out.close();

    return 0;
}