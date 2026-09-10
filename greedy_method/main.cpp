#include<bits/stdc++.h>
using namespace std;

struct Ans{
    int max_profit;
    string solution;
    Ans(): max_profit(0), solution(""){};
};

struct Item{
    int idx, w, v;
    bool operator<(Item b){
        return (double)v / (double)w > (double)b.v / (double) b.w;
    }
};

Ans Greedy(int n, int capacity, vector<Item> item){
    Ans ans;
    int weight_sum = 0;

    ans.solution.resize(n);

    //將CP值高的排在前面
    sort(item.begin(), item.end());
    for(int i = 0; i < n; ++i){
        //還放得下的話繼續放入
        if(weight_sum + item[i].w <= capacity){
            weight_sum += item[i].w;
            ans.max_profit += item[i].v;
            ans.solution[item[i].idx] = '1';
        }else{
            ans.solution[item[i].idx] = '0';
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
    vector<Item> item(n);
    for(int i = 0; i < n; ++i){
        item[i].idx = i;
        in >> item[i].w >> item[i].v;
    }
    in.close();

    //solve
    Ans ans = Greedy(n, capacity, item);

    //輸出答案
    out.open("ans_" + data + ".txt");
    out << "max profit:" << ans.max_profit << endl;
    out << "solution:" << ans.solution;
    out.close();

    return 0;
}