#include<bits/stdc++.h>
#define ll long long
using namespace std;

int evalution_max;
const int test_number = 30;
const int population_size = 100;
const int elitism_number = 10;
const double crossover_rate = 0.9;
const double mutation_rate = 0.05;

random_device rd;//亂數種子
mt19937 gen(rd());//亂數引擎

struct Ans{
    ll max_profit;
    string solution;
    Ans(): max_profit(-1), solution(""){};
};

Ans Genetic_Algorithm(int n, int capacity, vector<int> &w, vector<int> &v){
    int eva_times = 100;
    vector<vector<bool>> chromosome[2];
    chromosome[0].assign(population_size, vector<bool>(n, 0));//parent
    chromosome[1].assign(population_size, vector<bool>(n, 0));//offspring
    vector<ll> objective_value(population_size);

    function<void()> generate_initial_population = [&](){//隨機產生初始族群
        uniform_int_distribution<int> choose(0, 1);

        for(int i = 0; i < population_size; ++i){
            for(int j = 0; j < n; ++j){
                chromosome[0][i][j] = choose(gen);
            }
        }
    };

    function<void()> evaluation = [&](){//計算objective value
        for(int i = 0; i < population_size; ++i){
            //計算objective value
            //重量超過限制則隨機刪除物品
            vector<int> item;
            ll wsum = 0;
            objective_value[i] = 0;

            for(int j = 0; j < n; ++j){
                if(chromosome[0][i][j]){
                    item.push_back(j);
                    objective_value[i] += v[j];
                    wsum += w[j];
                }
            }

            if(wsum > capacity){
                shuffle(item.begin(), item.end(), gen);
                for(int x: item){
                    chromosome[0][i][x] = 0;
                    objective_value[i] -= v[x];
                    wsum -= w[x];
                    if(wsum <= capacity) break;
                }
            }
        }
    };

    function<void()> selection = [&](){//Tournament selection
        ll fsum = 0;
        uniform_real_distribution<double> spin(0, 1);
        vector<double> EV_psum(population_size + 1, 0);
        vector<vector<bool>> elitism(elitism_number);

        function<int()> roulette = [&](){
            double marker = spin(gen);
            int l = 0, r = population_size;
            while(r - l > 1){
                int mid = (l + r) >> 1;
                if(EV_psum[mid] < marker){
                    l = mid;
                }else{
                    r = mid;
                }
            }
            return r - 1;
        };

        for(int i = 0; i < population_size; ++i){
            fsum += objective_value[i];
        }

        for(int i = 0; i < population_size; ++i){
            EV_psum[i + 1] = (double)objective_value[i] / fsum + EV_psum[i];
        }
        
        //select elitisms
        vector<pair<int, int>> tmp(population_size);//(objective value, index)
        for(int i = 0; i < population_size; ++i){
            tmp[i] = {objective_value[i], i};
        }
        sort(tmp.begin(), tmp.end(), greater<pair<int, int>>());
        for(int i = 0; i < elitism_number; ++i){
            elitism[i] = chromosome[0][tmp[i].second];
        }
        for(int i = 0; i < elitism_number; ++i){
            chromosome[1][i] = elitism[i];
        }

        //Tournament
        for(int i = elitism_number; i < population_size; ++i){
            int candidate1 = roulette();
            int candidate2 = roulette();
            if(objective_value[candidate1] < objective_value[candidate2]) swap(candidate1, candidate2);
            chromosome[1][i] = chromosome[0][candidate1];
        }
    };

    function<void()> crossover = [&](){//uniform crossover
        uniform_real_distribution<double> crossover_or_not(0, 1);
        uniform_int_distribution<int> g_mask(0, 1);

        for(int i = elitism_number + 1; i < population_size; i += 2){//crossover
            if(crossover_or_not(gen) <= crossover_rate){
                //Uniform crossover
                for(int j = 0; j < n; ++j){
                    if(g_mask(gen)){
                        swap(chromosome[1][i - 1][j], chromosome[1][i][j]);
                    }
                }
            }
        }
    };

    function<void()> mutation = [&](){
        uniform_real_distribution<double> mutation_or_not(0, 1);

        for(int i = elitism_number; i < population_size; ++i){
            for(int j = 0; j < n; ++j){
                if(mutation_or_not(gen) <= mutation_rate){
                    //flip
                    chromosome[1][i][j] = (chromosome[1][i][j] ^ 1);
                }
            }
        }
    };

    function<Ans()> get_ans = [&](){
        Ans ans;

        for(int i = 0; i < population_size; ++i){
            if(objective_value[i] > ans.max_profit){
                ans.max_profit = objective_value[i];
                ans.solution.resize(n);
                for(int j = 0; j < n; ++j){
                    ans.solution[j] = chromosome[0][i][j] + '0';
                }
            }
        }

        return ans;
    };

    generate_initial_population();
    evaluation();
    while(eva_times < evalution_max){
        selection();
        crossover();
        mutation();
        swap(chromosome[0], chromosome[1]);
        evaluation();
        eva_times += population_size;
    }

    return get_ans();
}

int main(){
    if(elitism_number > population_size){
        cout << "Population size must greater than the number of elitisms.";
        return 0;
    }
    ifstream in;
    ofstream out;
    string data;

    cout << "Select the test data(dt01, dt02, dt03...):";
    cin >> data;

    if(data == "dt01") evalution_max = 1e3;
    else if(data == "dt02") evalution_max = 1e4;
    else if(data == "dt03") evalution_max = 5e5;
    else{
        cout << "Wrong Input format!";
        return 0;
    }

    //讀入測資
    in.open("dataset/" + data + "/item.txt");
    int n, capacity;
    in >> n >> capacity;
    vector<int> w(n), v(n);
    for(int i = 0; i < n; ++i){
        in >> w[i] >> v[i];
    }
    in.close();

    ll ans_sum = 0;
    Ans ans;
    out.open("ans_" + data + ".txt");
    for(int i = 0; i < test_number; ++i){
        //solve
        ans = Genetic_Algorithm(n, capacity, w, v);
        ans_sum += ans.max_profit;

        //輸出答案
        out << i + 1 << ":\n";
        out << "max profit:" << ans.max_profit << endl;
        out << "solution:" << ans.solution << endl << endl;
    }

    out << fixed << setprecision(0) << "average:" << 1.0 * ans_sum / test_number;
    out.close();

    return 0;
}