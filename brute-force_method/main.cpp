#include<bits/stdc++.h>
using namespace std;

struct Ans{
    int max_profit;
    string solution;
    Ans(): max_profit(0), solution(""){};
};

Ans Exhaustive_Search(int n, int capacity, vector<int> &w, vector<int> &v){
    Ans ans;

    for(int s = 0; s < (1 << n); ++s){//用二進位表示集合
        int weight_sum = 0, profix = 0;
        for(int i = 0; i < n; ++i){
            //如果第i個物品有選
            if((s >> i) & 1){
                weight_sum += w[i];
                profix += v[i];
            }
        }
        //檢查重量是否合法
        if(weight_sum <= capacity){
            if(profix > ans.max_profit){
                ans.max_profit = profix;
                ans.solution = "";
                for(int i = 0; i < n; ++i){
                    ans.solution += (char)(((s >> i) & 1) + '0');
                }
            }
        }
    }

    return ans;
}

int main(){
    ifstream in;
    ofstream out;
    cout << "Select the dataset(dt01, dt02, dt03...):";
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
    Ans ans = Exhaustive_Search(n, capacity, w, v);

    //輸出答案
    out.open("ans_" + data + ".txt");
    out << "max profit:" << ans.max_profit << endl;
    out << "solution:" << ans.solution;
    out.close();

    return 0;
}