# 0/1 背包問題：四種求解方法

中山大學「演算法」作業。
以 **brute-force**、**greedy**、**dynamic programming** 和 **genetic algorithm** 四種方法求解 0/1 背包問題。

## 執行方式

使用 GNU G++ 編譯，並在對應方法目錄下執行。例如：

```sh
cd dynamic_programming_method
g++ -std=c++17 -O2 main.cpp -o knapsack
./knapsack
```

執行後輸入 `dt01`、`dt02` 或 `dt03`，程式會讀取 `dataset/` 中的測資，並將結果寫入（覆寫）對應的 `ans_dt*.txt`。
