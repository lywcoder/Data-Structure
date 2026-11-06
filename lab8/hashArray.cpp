#include <iostream>

using namespace std;

// 判断 vacant 是否落在从 home 到 curr 的环形路径上（包含 home，不含 curr）
bool is_between(int home, int vacant, int curr) {
    if (home <= curr) {
        return (home <= vacant && vacant < curr);
    } else {
        return (vacant >= home || vacant < curr);
    }
}

int main() {
    // 优化标准输入输出流
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int D, m;
    std::cin >> D >> m;

    int* table = new int[D];
    bool* occupied = new bool[D];
    for (int i = 0; i < D; ++i) {
        occupied[i] = false;
    }

    while (m--) {
        int opt, x;
        cin >> opt >> x;

        if (opt == 0) { // 插入
            bool existed = false;
            for (int i = 0; i < D; ++i) {
                if (occupied[i] && table[i] == x) {
                    existed = true;
                    break;
                }
            }
            if (existed) {
                cout << "Existed\n";
            } else {
                int idx = (x % D + D) % D;
                while (occupied[idx]) {
                    idx = (idx + 1) % D;
                }
                table[idx] = x;
                occupied[idx] = true;
                cout << idx << "\n";
            }
        } else if (opt == 1) { // 查询
            int start = (x % D + D) % D;
            int idx = start;
            int found_idx = -1;
            while (occupied[idx]) {
                if (table[idx] == x) {
                    found_idx = idx;
                    break;
                }
                idx = (idx + 1) % D;
                if (idx == start) break;
            }
            cout << found_idx << "\n";
        } else if (opt == 2) { // 删除
            int start = (x % D + D) % D;
            int idx = start;
            int found_idx = -1;
            while (occupied[idx]) {
                if (table[idx] == x) {
                    found_idx = idx;
                    break;
                }
                idx = (idx + 1) % D;
                if (idx == start) break;
            }

            if (found_idx == -1) {
                cout << "Not Found\n";
            } else {
                int vacant = found_idx;
                occupied[vacant] = false;
                int moved_count = 0;
                int curr = (vacant + 1) % D;

                // 遍历同冲突块内的后续元素进行填补
                while (occupied[curr]) {
                    int k = table[curr];
                    int home = (k % D + D) % D;
                    if (is_between(home, vacant, curr)) {
                        table[vacant] = k;
                        occupied[vacant] = true;
                        occupied[curr] = false;
                        vacant = curr; // 新的空位变为 curr
                        moved_count++;
                    }
                    curr = (curr + 1) % D;
                }
                cout << moved_count << "\n";
            }
        }
    }

    delete[] table;
    delete[] occupied;

    return 0;
}