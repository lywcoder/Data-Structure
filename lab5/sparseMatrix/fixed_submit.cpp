#include <iostream>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <limits>

using namespace std;

// 题目要求按 32 位 int 回绕计算；unsigned 运算可以安全地实现取模。
static_assert(numeric_limits<int>::digits == 31 &&
              numeric_limits<unsigned int>::digits == 32,
              "This program requires 32-bit int.");

int signedValue(unsigned int bits) {
    if (bits <= static_cast<unsigned int>(numeric_limits<int>::max())) {
        return static_cast<int>(bits);
    }
    return -1 - static_cast<int>(numeric_limits<unsigned int>::max() - bits);
}

int addValue(int a, int b) {
    return signedValue(static_cast<unsigned int>(a) +
                       static_cast<unsigned int>(b));
}

int mulValue(int a, int b) {
    return signedValue(static_cast<unsigned int>(a) *
                       static_cast<unsigned int>(b));
}

struct Triad {
    int row;
    int col;
    int val;

    Triad(int r, int c, int v) : row(r), col(c), val(v) {}
};

bool positionLess(const Triad& a, const Triad& b) {
    return a.row < b.row || (a.row == b.row && a.col < b.col);
}

class SMatrix {
private:
    int rows;
    int cols;
    vector<Triad> data;

public:
    SMatrix() : rows(0), cols(0) {}

    void clear();
    void reset();
    void readFromTriad();
    void print() const;
    void transpose();
    SMatrix add(const SMatrix& a) const;
    SMatrix mul(const SMatrix& b) const;
};

void SMatrix::clear() {
    rows = 0;
    cols = 0;
    data.clear();
}

void SMatrix::reset() {
    clear();  // 先清空，再读入新尺寸。
    cin >> rows >> cols;
    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols; ++j) {
            int v;
            cin >> v;
            if (v != 0) data.emplace_back(i, j, v);
        }
    }
}

void SMatrix::readFromTriad() {
    clear();
    int t;
    cin >> rows >> cols >> t;
    data.reserve(static_cast<size_t>(t));
    for (int k = 0; k < t; ++k) {
        int x, y, v;
        cin >> x >> y >> v;
        if (v != 0) data.emplace_back(x, y, v);
    }

    // 题目输入按行主序给出；额外兼容乱序的三元组。
    if (!is_sorted(data.begin(), data.end(), positionLess)) {
        sort(data.begin(), data.end(), positionLess);
    }
}

void SMatrix::print() const {
    cout << rows << ' ' << cols << '\n';
    size_t p = 0;
    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols; ++j) {
            if (j > 1) cout << ' ';
            if (p < data.size() && data[p].row == i && data[p].col == j) {
                cout << data[p].val;
                ++p;
            } else {
                cout << 0;
            }
        }
        cout << '\n';
    }
}

void SMatrix::transpose() {
    SMatrix res;
    res.rows = cols;
    res.cols = rows;
    res.data.reserve(data.size());
    for (const auto& t : data) {
        res.data.emplace_back(t.col, t.row, t.val);
    }
    sort(res.data.begin(), res.data.end(), positionLess);
    swap(*this, res);
}

SMatrix SMatrix::add(const SMatrix& a) const {
    if (rows != a.rows || cols != a.cols) {
        cout << -1 << '\n';
        return a;  // 按题意，不合法时用 Q 替换 P。
    }

    SMatrix res;
    res.rows = rows;
    res.cols = cols;
    size_t i = 0, j = 0;
    while (i < data.size() && j < a.data.size()) {
        const Triad& t0 = data[i];
        const Triad& t1 = a.data[j];
        if (positionLess(t0, t1)) {
            res.data.push_back(t0);
            ++i;
        } else if (positionLess(t1, t0)) {
            res.data.push_back(t1);
            ++j;
        } else {
            int sum = addValue(t0.val, t1.val);
            if (sum != 0) res.data.emplace_back(t0.row, t0.col, sum);
            ++i;
            ++j;
        }
    }
    while (i < data.size()) res.data.push_back(data[i++]);
    while (j < a.data.size()) res.data.push_back(a.data[j++]);
    return res;
}

SMatrix SMatrix::mul(const SMatrix& b) const {
    if (cols != b.rows) {
        cout << -1 << '\n';
        return b;
    }

    SMatrix res;
    res.rows = rows;
    res.cols = b.cols;
    if (data.empty() || b.data.empty()) return res;

    // b 的第 r 行存放在 [rowPos[r], rowPos[r+1])。
    vector<size_t> rowPos(static_cast<size_t>(b.rows) + 2, 0);
    for (const auto& t : b.data) ++rowPos[t.row + 1];
    for (int r = 1; r <= b.rows; ++r) {
        rowPos[r + 1] += rowPos[r];
    }

    vector<int> temp(static_cast<size_t>(res.cols) + 1, 0);
    vector<int> mark(static_cast<size_t>(res.cols) + 1, 0);
    vector<int> used;
    size_t aIdx = 0;

    while (aIdx < data.size()) {
        int row = data[aIdx].row;
        used.clear();

        // 一次处理 P 的一整行：P(row,j) * Q(j,k)。
        while (aIdx < data.size() && data[aIdx].row == row) {
            const Triad& ta = data[aIdx];
            int j = ta.col;
            for (size_t p = rowPos[j]; p < rowPos[j + 1]; ++p) {
                const Triad& tb = b.data[p];
                int k = tb.col;
                if (mark[k] != row) {
                    mark[k] = row;
                    temp[k] = 0;
                    used.push_back(k);
                }
                temp[k] = addValue(temp[k], mulValue(ta.val, tb.val));
            }
            ++aIdx;
        }

        // 触及列的顺序未必递增，必须排序后保存这一行。
        sort(used.begin(), used.end());
        for (int k : used) {
            if (temp[k] != 0) res.data.emplace_back(row, k, temp[k]);
        }
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int w;
    if (!(cin >> w)) return 0;
    SMatrix P;
    while (w--) {
        int op;
        cin >> op;
        if (op == 1) {
            P.reset();
        } else if (op == 2) {
            SMatrix Q;
            Q.readFromTriad();
            P = P.mul(Q);
        } else if (op == 3) {
            SMatrix Q;
            Q.readFromTriad();
            P = P.add(Q);
        } else if (op == 4) {
            P.print();
        } else if (op == 5) {
            P.transpose();
        }
    }
    return 0;
}
