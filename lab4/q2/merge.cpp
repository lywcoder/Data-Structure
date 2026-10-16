#include <iostream>
#include <algorithm>
#include <vector>

long long xorSum(const std::vector<int>& a) {
    long long ans = 0;
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        ans += (i ^ a[i]);
    }
    return ans;
}

int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<int> a(n), b(m);
    for (int& x : a) std::cin >> x;
    for (int& x : b) std::cin >> x;

    sort(a.begin(), a.end());
    sort(b.begin(), b.end());

    std::vector<int> c;
    c.reserve(n + m);

    int i = 0, j = 0;

    while (i < n && j < m) {
        if (a[i] <= b[j]) {
            c.push_back(a[i++]);
        } else {
            c.push_back(b[j++]);
        }
    }

    while (i < n) c.push_back(a[i++]);
    while (j < m) c.push_back(b[j++]);

    std::cout << xorSum(a) << '\n';
    std::cout << xorSum(b) << '\n';
    std::cout << xorSum(c) << '\n';

    return 0;
}