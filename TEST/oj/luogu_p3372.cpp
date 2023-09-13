#include "DS/SegTree.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[P3372 【模板】线段树 1](https://www.luogu.com.cn/problem/P3372)
*/
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::ZkwLazySumTree tree(n, [](auto...) {
        int64_t x;
        cin >> x;
        return x;
    });
    // OY::SegLazySumTree tree(n, [](auto...) {
    //     int64_t x;
    //     cin >> x;
    //     return x;
    // });
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r;
            int64_t x;
            cin >> l >> r >> x;
            tree.add(l - 1, r - 1, x);
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << tree.query(l - 1, r - 1) << '\n';
        }
    }
}