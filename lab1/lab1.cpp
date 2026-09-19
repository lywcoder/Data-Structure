//1. The value of subset
// #include <iostream>
// #include <vector>

// using namespace std;

// int n;
// vector<long long> a;
// long long ans = 0;

// //idx : 当前遍历到的原数组下标
// //len : 当前构建的子序列长度
// //current_val : 当前子序列的积累得到的子集价值

// void dfs(int idx, int len, long long current_val) {
//     // 递归边界：已经遍历完了所有元素
//     if (idx == n) {
//         ans ^= current_val; //当前自序列的子集价值多加
//         return;
//     }

//     // 选择1：不选择当前的元素
//     // 长度不变，价值不变，继续下一个元素
//     dfs(idx + 1, len, current_val);

//     // 选择2：选入当前的元素，长度加一，子集价值增加
//     // 长度加一，价值增加
//     // len + 1 就是题目中的i
//     dfs(idx + 1, len + 1, current_val + (len + 1) * a[idx]);
// }

// int main() {
//     cin >> n;
//     a.resize(n);

//     for (int i = 0; i < n; ++i) {
//         cin >> a[i];
//     }

//     //从第零个元素开始：初始长度为0，初始价值为0
//     dfs(0, 0, 0);

//     cout << ans << endl;
//     return 0;
// }

//---
//2.求所有排列价值的按位或（STL全排列）
// 核心是原数组要求原排列（求所有打乱顺序的可能性），计算价值，并且将价值进行按位或进行

#include <iostream>
#include <vector>
#include <algorithm>

// STL中的next_permutation就是排列顺序的可能性

using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> a(n);

    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    // 首先需要排序，否则没法生成全部排列
    sort(a.begin(), a.end());

    long long ans = 0; //记录按位或的最终结果

    // do-while循环进行按位或的结果
    do {
        long long current_val = 0;

        // 计算当前排列的价值
        for (int i = 0; i < n; i++) {
            current_val += (a[i] ^ (i + 1));
        }

        // 将当前排列的价值按位或到总答案之中

        ans |= current_val;
    } while (next_permutation(a.begin(), a.end()));

    cout << ans << endl;
    return 0;
}

// #include <iostream>
// #include <vector>

// using namespace std;

// int n;
// vector<int> a, current_perm;
// vector<bool> used;
// long long ans = 0;

// void dfs(int pos) {
//     if (pos > n) {
//         long long current_val = 0;
//         for (int i = 1; i <= n; i++) {
//             current_val += (current_perm[i - 1] ^ i);
//         }
//         ans |= current_val;
//         return;
//     }

//     for (int i = 0; i < n; ++i) {
//         if (!used[i]) {
//             used[i] = true;
//             current_perm.push_back(a[i]);
//             dfs(pos + 1);
//             current_perm.pop_back();
//             used[i] = false;
//         }
//     }
// }

// int main() {
//     cin >> n;
//     a.resize(n);
//     used.resize(n, false);
//     for (int i = 0; i < n; ++i) {
//         cin >> a[i];
//     }

//     dfs(1);
//     cout << ans << endl;
//     return 0;
// }
