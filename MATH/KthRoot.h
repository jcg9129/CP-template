/*
最后修改:
20240620
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_KTHROOT__
#define __OY_KTHROOT__

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numeric>

namespace OY {
    namespace KthRoot {
        static constexpr double einv[] = {0.33333333333333326, 0.24999999999999997, 0.19999999999999998, 0.16666666666666663, 0.14285714285714282, 0.12499999999999999, 0.11111111111111109, 0.099999999999999992, 0.090909090909090898, 0.083333333333333315, 0.076923076923076913, 0.071428571428571411, 0.066666666666666652, 0.062499999999999993, 0.058823529411764698, 0.055555555555555546, 0.052631578947368411, 0.049999999999999996, 0.047619047619047609, 0.045454545454545449, 0.043478260869565209, 0.041666666666666657, 0.039999999999999994, 0.038461538461538457, 0.037037037037037028, 0.035714285714285705, 0.034482758620689648, 0.033333333333333326, 0.032258064516129024};
        static constexpr uint64_t pow3[] = {1853020188851841, 5559060566555523, 16677181699666569, 50031545098999707, 150094635296999121, 450283905890997363, 1350851717672992089, 4052555153018976267, 12157665459056928801};
        static constexpr uint64_t pow2_[] = {18445618199572250625, 17878103347812890625, 6568408355712890625};
        inline uint64_t fast_pow(uint64_t a, uint64_t k) {
            uint64_t res = 1;
            while (k) {
                if (k & 1) res *= a;
                k >>= 1, a *= a;
            }
            return res;
        }
        inline uint64_t solve(uint64_t x, uint64_t k) {
            if (!x) return 0;
            if (k > 63 || !(x >> k)) return 1;
            if (k > 40) return 2;
            if (k >= 32) return 2 + (x >= pow3[k - 32]);
            if (k == 1) return x;
            if (k == 2) return std::sqrt((long double)x);
            double res = std::pow(x, einv[k - 3]);
            if (fast_pow(res + 0.05, k) - 1 < x) res += 0.05;
            return res;
        }
    }
}

#endif