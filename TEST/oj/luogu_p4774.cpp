#include "DS/AVL.h"
#include "IO/FastIO.h"
#include "MATH/LinearModEquations.h"

/*
[P4774 [NOI2018] 屠龙勇士](https://www.luogu.com.cn/problem/P4774)
*/
/**
 * 本题为线性同余方程组模板题
 * 血量 a， 砍 x 刀每刀 attack 血，使得怪物被打到小于等于 0；怪物每次回血 p，回到 0
 * 也就是说 a - attack * x + p * y = 0，y 未知
 * 也就是 attack * x = a (mod p)
 * 最后记得，除了保证同余，还要满足刀数使得怪物打到小于等于 0
 */

static constexpr uint32_t T = 5, N = 100000, M = 100000;
uint64_t a[N], p[N];
uint32_t reward[N], attack[M];
int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t n, m;
        cin >> n >> m;
        for (uint32_t i = 0; i != n; i++) cin >> a[i];
        for (uint32_t i = 0; i != n; i++) cin >> p[i];
        for (uint32_t i = 0; i != n; i++) cin >> reward[i];
        for (uint32_t i = 0; i != m; i++) cin >> attack[i];

        std::sort(attack, attack + m);
        auto swords = OY::AVLMultiset<uint64_t, std::less<uint64_t>, (N + M) * T>::from_sorted(attack, attack + m);

        OY::LinearModEquations lme;
        bool flag = true;
        uint64_t min_need = 0;
        for (uint32_t i = 0; i != n; i++) {
            auto it = swords.smaller_bound(a[i] + 1);
            if (it->is_null()) it = swords.kth(0);
            uint64_t use = it->get();
            swords.erase_by_key(use);
            swords.insert_by_key(reward[i]);
            if (!lme.add_equation(use, a[i], p[i])) {
                flag = false;
                break;
            }
            min_need = std::max(min_need, (a[i] + use - 1) / use);
        }
        if (!flag)
            cout << "-1\n";
        else {
            auto [rem, div] = lme.query();
            uint64_t ans = rem;
            if (ans < min_need) ans += (min_need - ans + div - 1) / div * div;
            cout << ans << endl;
        }
    }
}