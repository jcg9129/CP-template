#include "IO/FastIO.h"
#include "MATH/CombinationTable.h"
#include "MATH/StaticModInt64.h"
#include "MATH/StaticMontgomeryModInt64.h"

/*
[P1595 信封问题](https://www.luogu.com.cn/problem/P1595)
*/
/**
 * 本题为错排数模板题
 */

int main() {
    using mint = OY::StaticModInt64<9223372036854775783, true>;
    // using mint = OY::StaticMontgomeryModInt64<9223372036854775783, true>;

    uint32_t n;
    cin >> n;
    OY::CombinationTable<mint, 20> table(n);

    cout << table.perm_staggered(n) << endl;
}