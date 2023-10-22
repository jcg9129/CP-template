#include "GRAPH/Prim_naive.h"
#include "IO/FastIO.h"

int main() {
    OY::PrimNaive::Graph<int, 1000, 1000> G(4, 5);
    G.add_edge(0, 1, 100);
    G.add_edge(1, 2, 20);
    G.add_edge(2, 3, 40);
    G.add_edge(0, 2, 110);
    G.add_edge(2, 3, 30);

    auto res = G.calc<true>();
    auto &&sol = res.first;
    bool flag = res.second;
    if (!flag) {
        cout << "There is no MST\n";
    } else {
        cout << "There is MST\n";
        cout << "MST total cost: " << sol.total_cost() << endl;
        cout << "used edges:\n";

        // PrimHeap 版本的找树边较为费劲，只能先标注出来，再在遍历邻接表的时候去输出
        std::vector<bool> used(5);
        sol.do_for_used_edges([&](int index) { used[index] = true; });
        for (int from = 0; from < 4; from++) G(from, [&](int index, int to, int cost) {
            if (used[index] && from < to) cout << "use No." << index << " edge, from " << from << " to " << to << ", cost = " << cost << endl;
        });
    }
}
/*
#输出如下
There is MST
MST total cost: 150
used edges:
use No.0 edge, from 0 to 1, cost = 100
use No.2 edge, from 1 to 2, cost = 20
use No.8 edge, from 2 to 3, cost = 30

*/