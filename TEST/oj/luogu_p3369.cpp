#include "DS/FHQTreap.h"
#include "IO/FastIO.h"

/*
[P3369 【模板】普通平衡树](https://www.luogu.com.cn/problem/P3369)
*/
int main() {
    uint32_t n;
    cin >> n;
    OY::FHQTreap<int> S;
    for (uint32_t i = 0; i < n; i++) {
        char op;
        int x;
        cin >> op >> x;
        if (op == '1')
            S.insert_by_key(x);
        else if (op == '2')
            S.erase_by_key(x);
        else if (op == '3')
            cout << S.rank(x) + 1 << endl;
        else if (op == '4')
            cout << S.kth(x - 1)->get() << endl;
        else if (op == '5')
            cout << S.smaller_bound(x)->get() << endl;
        else
            cout << S.upper_bound(x)->get() << endl;
    }
}