#include "DS/BIT2D.h"
#include "IO/FastIO.h"

/*
[#134. 二维树状数组 2：区间修改，单点查询](https://loj.ac/p/134)
*/
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::BinaryIndexedTree2D::BIT<int64_t, false, 1 << 24> S(n, m);
    char op;
    while (cin >> op) {
        if (op == '1') {
            uint32_t r1, c1, r2, c2;
            int64_t v;
            cin >> r1 >> c1 >> r2 >> c2 >> v;
            auto f = [&](uint32_t v1, uint32_t v2) { return (v1 | (std::bit_ceil((v1 ^ v2) + 1) - 1)); };
            const uint32_t rend = f(r1 - 1, r2), cend = f(c1 - 1, c2);
            for (uint32_t r = r1 - 1; r != rend && r < n; r += S._lowbit(r + 1)) {
                uint32_t cursor = r * m;
                for (uint32_t c = c1 - 1; c != cend && c < m; c += S._lowbit(c + 1)) S.m_sum[cursor + c] += v;
                for (uint32_t c = c2; c != cend && c < m; c += S._lowbit(c + 1)) S.m_sum[cursor + c] -= v;
            }
            for (uint32_t r = r2; r != rend && r < n; r += S._lowbit(r + 1)) {
                uint32_t cursor = r * m;
                for (uint32_t c = c1 - 1; c != cend && c < m; c += S._lowbit(c + 1)) S.m_sum[cursor + c] -= v;
                for (uint32_t c = c2; c != cend && c < m; c += S._lowbit(c + 1)) S.m_sum[cursor + c] += v;
            }
        } else {
            uint32_t r, c;
            cin >> r >> c;
            cout << S.presum(r - 1, c - 1) << endl;
        }
    }
}