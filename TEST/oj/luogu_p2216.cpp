#include "DS/ZkwTree2d.h"
#include "IO/FastIO.h"

/*
[P2216 [HAOI2007] 理想的正方形](https://www.luogu.com.cn/problem/P2216)
*/
struct Pair {
    uint32_t min, max;
};
struct MinMaxNode {
    using value_type = Pair;
    using modify_type = Pair;
    static value_type op(const value_type &x, const value_type &y) { return Pair{std::min(x.min, y.min), std::max(x.max, y.max)}; }
    Pair m_val;
    const value_type &get() const { return m_val; }
    void set(uint32_t val) { m_val.min = m_val.max = val; }
    void set(Pair val) { m_val = val; }
};
int main() {
    uint32_t m, n, k;
    cin >> m >> n >> k;
    OY::Zkw2D::Tree<MinMaxNode, 1 << 22> S(m, n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });

    uint32_t min_dif = 0x3f3f3f3f;
    for (uint32_t i = 0; i + k <= m; i++) {
        for (uint32_t j = 0; j + k <= n; j++) {
            auto [min, max] = S.query(i, i + k - 1, j, j + k - 1);
            min_dif = std::min(min_dif, max - min);
        }
    }
    cout << min_dif;
}