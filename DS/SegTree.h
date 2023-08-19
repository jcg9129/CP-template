#ifndef __OY_SEGTREE__
#define __OY_SEGTREE__

#include <algorithm>
#include <cstdint>
#include <numeric>

namespace OY {
    namespace SegTree {
        using index_type = uint32_t;
        template <typename ValueType>
        struct BaseNode {
            using value_type = ValueType;
            using modify_type = ValueType;
            using node_type = BaseNode<ValueType>;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static void map(const modify_type &modify, node_type *x) { x->m_val += modify; }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType, typename ModifyType, typename SizeType>
        struct LazyNode {
            using value_type = ValueType;
            using modify_type = ModifyType;
            using node_type = LazyNode<ValueType, ModifyType, SizeType>;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static void map(const modify_type &modify, node_type *x, SizeType len) { x->m_val += modify * len; }
            static void com(const modify_type &modify, node_type *x) { x->m_modify += modify; }
            value_type m_val;
            modify_type m_modify;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
            bool has_lazy() const { return bool(m_modify); }
            const modify_type &get_lazy() const { return m_modify; }
            void clear_lazy() { m_modify = 0; }
            void pushup(node_type *lchild, node_type *rchild) { m_val = lchild->m_val + rchild->m_val; }
        };
        template <typename ValueType, typename Operation>
        struct CustomNode {
            using value_type = ValueType;
            using modify_type = ValueType;
            using node_type = CustomNode<ValueType, Operation>;
            static Operation s_op;
            static value_type op(const value_type &x, const value_type &y) { return s_op(x, y); }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType, typename Operation>
        Operation CustomNode<ValueType, Operation>::s_op;
        template <typename ValueType, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy, typename SizeType>
        struct CustomLazyNode {
            using value_type = ValueType;
            using modify_type = ModifyType;
            using node_type = CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, SizeType>;
            static Operation s_op;
            static Mapping s_map;
            static Composition s_com;
            static modify_type s_default_modify;
            static value_type op(const value_type &x, const value_type &y) { return s_op(x, y); }
            static void map(const modify_type &modify, node_type *x, SizeType len) { x->m_val = s_map(modify, x->m_val, len); }
            static void com(const modify_type &modify, node_type *x) { x->m_modify = s_com(modify, x->m_modify); }
            static constexpr bool init_clear_lazy() { return InitClearLazy; }
            value_type m_val;
            modify_type m_modify;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
            const modify_type &get_lazy() const { return m_modify; }
            void clear_lazy() { m_modify = s_default_modify; }
        };
        template <typename ValueType, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy, typename SizeType>
        Operation CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, SizeType>::s_op;
        template <typename ValueType, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy, typename SizeType>
        Mapping CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, SizeType>::s_map;
        template <typename ValueType, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy, typename SizeType>
        Composition CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, SizeType>::s_com;
        template <typename ValueType, typename ModifyType, typename Operation, typename Mapping, typename Composition, bool InitClearLazy, typename SizeType>
        ModifyType CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, SizeType>::s_default_modify;
        struct NoInit {};
#ifdef __cpp_lib_void_t
        template <typename... Tp>
        using void_t = std::void_t<Tp...>;
#else
        template <typename... Tp>
        struct make_void {
            using type = void;
        };
        template <typename... Tp>
        using void_t = typename make_void<Tp...>::type;
#endif
        template <typename Tp, typename = void>
        struct Has_has_lazy : std::false_type {};
        template <typename Tp>
        struct Has_has_lazy<Tp, std::void_t<decltype(std::declval<Tp>().has_lazy())>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_get_lazy : std::false_type {};
        template <typename Tp>
        struct Has_get_lazy<Tp, std::void_t<decltype(std::declval<Tp>().get_lazy())>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename SizeType, typename = void>
        struct Has_pushup : std::false_type {};
        template <typename Tp, typename NodePtr, typename SizeType>
        struct Has_pushup<Tp, NodePtr, SizeType, std::void_t<decltype(std::declval<Tp>().pushup(std::declval<NodePtr>(), std::declval<NodePtr>(), std::declval<SizeType>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushup<Tp, NodePtr, void, std::void_t<decltype(std::declval<Tp>().pushup(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr, typename ModifyType, typename = void>
        struct Has_map : std::false_type {};
        template <typename Tp, typename NodePtr, typename ModifyType>
        struct Has_map<Tp, NodePtr, ModifyType, std::void_t<decltype(Tp::map(std::declval<ModifyType>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_init_clear_lazy : std::false_type {};
        template <typename Tp>
        struct Has_init_clear_lazy<Tp, std::void_t<decltype(Tp::init_clear_lazy())>> : std::true_type {};
        template <typename RangeMapping, bool Complete>
        struct TreeBase {
            RangeMapping m_range_mapping;
            TreeBase(RangeMapping mapping) : m_range_mapping(mapping) {}
        };
        template <>
        struct TreeBase<NoInit, false> {
            TreeBase(NoInit) {}
        };
        template <>
        struct TreeBase<NoInit, true> {
            template <typename InitMapping>
            TreeBase(InitMapping mapping) {}
        };
        template <typename Node, typename RangeMapping = NoInit, bool Complete = false, typename SizeType = uint64_t, index_type MAX_NODE = 1 << 22>
        struct Tree : TreeBase<RangeMapping, Complete> {
            using value_type = typename Node::value_type;
            using modify_type = typename Node::modify_type;
            struct node : Node {
                index_type m_lchild, m_rchild;
                bool is_null() const { return this == s_buffer; }
                node *lchild() const { return s_buffer + m_lchild; }
                node *rchild() const { return s_buffer + m_rchild; }
            };
            static node s_buffer[MAX_NODE];
            static index_type s_use_count;
            index_type m_root;
            SizeType m_size;
            node *_root() const { return s_buffer + m_root; }
            index_type _newnode(SizeType floor, SizeType ceil) const {
                if constexpr (!Complete && !std::is_same<RangeMapping, NoInit>::value) s_buffer[s_use_count].set(TreeBase<RangeMapping, Complete>::m_range_mapping(floor, ceil));
                if constexpr (Has_init_clear_lazy<node>::value)
                    if constexpr (node::init_clear_lazy())
                        s_buffer[s_use_count].clear_lazy();
                return s_use_count++;
            }
            template <typename InitMapping>
            void _initnode(node *cur, SizeType floor, SizeType ceil, InitMapping mapping) {
                if (floor == ceil) {
                    if constexpr (!std::is_same<InitMapping, NoInit>::value) cur->set(mapping(floor));
                } else {
                    SizeType mid = (floor + ceil) >> 1;
                    cur->m_lchild = _newnode(floor, mid);
                    cur->m_rchild = _newnode(mid + 1, ceil);
                    _initnode(cur->lchild(), floor, mid, mapping);
                    _initnode(cur->rchild(), mid + 1, ceil, mapping);
                    _pushup(cur, ceil - floor + 1);
                }
            }
            void _apply(node *cur, const modify_type &modify, SizeType len) const { node::map(modify, cur, len), node::com(modify, cur); }
            void _apply(node *cur, const modify_type &modify) const {
                if constexpr (Has_get_lazy<node>::value)
                    node::map(modify, cur, 1);
                else if constexpr (Has_map<node, node *, modify_type>::value)
                    node::map(modify, cur);
                else
                    cur->set(node::op(modify, cur->get()));
            }
            void _pushdown(node *cur, SizeType floor, SizeType ceil, SizeType mid) const {
                if constexpr (!Complete) {
                    if (!cur->m_lchild) cur->m_lchild = _newnode(floor, mid);
                    if (!cur->m_rchild) cur->m_rchild = _newnode(mid + 1, ceil);
                }
                if constexpr (Has_get_lazy<node>::value) {
                    if constexpr (Has_has_lazy<node>::value)
                        if (!cur->has_lazy()) return;
                    _apply(cur->lchild(), cur->get_lazy(), mid - floor + 1);
                    _apply(cur->rchild(), cur->get_lazy(), ceil - mid);
                    cur->clear_lazy();
                }
            }
            void _pushup(node *cur, SizeType len) const {
                if constexpr (Has_pushup<node, node *, SizeType>::value)
                    cur->pushup(cur->lchild(), cur->rchild(), len);
                else if constexpr (Has_pushup<node, node *, void>::value)
                    cur->pushup(cur->lchild(), cur->rchild());
                else
                    cur->set(node::op(cur->lchild()->get(), cur->rchild()->get()));
            }
            void _modify(node *cur, SizeType floor, SizeType ceil, SizeType i, const value_type &val) {
                if (floor == ceil)
                    cur->set(val);
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    _pushdown(cur, floor, ceil, mid);
                    if (i <= mid)
                        _modify(cur->lchild(), floor, mid, i, val);
                    else
                        _modify(cur->rchild(), mid + 1, ceil, i, val);
                    _pushup(cur, ceil - floor + 1);
                }
            }
            void _add(node *cur, SizeType floor, SizeType ceil, SizeType i, const modify_type &modify) {
                if (floor == ceil)
                    _apply(cur, modify);
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    _pushdown(cur, floor, ceil, mid);
                    if (i <= mid)
                        _add(cur->lchild(), floor, mid, i, modify);
                    else
                        _add(cur->rchild(), mid + 1, ceil, i, modify);
                    _pushup(cur, ceil - floor + 1);
                }
            }
            void _add(node *cur, SizeType floor, SizeType ceil, SizeType left, SizeType right, const modify_type &modify) {
                if (left <= floor && right >= ceil)
                    _apply(cur, modify, ceil - floor + 1);
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    _pushdown(cur, floor, ceil, mid);
                    if (left <= mid) _add(cur->lchild(), floor, mid, left, right, modify);
                    if (right > mid) _add(cur->rchild(), mid + 1, ceil, left, right, modify);
                    _pushup(cur, ceil - floor + 1);
                }
            }
            value_type _query(node *cur, SizeType floor, SizeType ceil, SizeType i) const {
                if (floor == ceil) return cur->get();
                SizeType mid = (floor + ceil) >> 1;
                _pushdown(cur, floor, ceil, mid);
                return i <= mid ? _query(cur->lchild(), floor, mid, i) : _query(cur->rchild(), mid + 1, ceil, i);
            }
            value_type _query(node *cur, SizeType floor, SizeType ceil, SizeType left, SizeType right) const {
                if (left <= floor && right >= ceil) return cur->get();
                SizeType mid = (floor + ceil) >> 1;
                _pushdown(cur, floor, ceil, mid);
                if (left > mid) return _query(cur->rchild(), mid + 1, ceil, left, right);
                if (right <= mid) return _query(cur->lchild(), floor, mid, left, right);
                return node::op(_query(cur->lchild(), floor, mid, left, right), _query(cur->rchild(), mid + 1, ceil, left, right));
            }
            template <typename Judge>
            SizeType _max_right(node *cur, SizeType floor, SizeType ceil, SizeType start, value_type &val, Judge &&judge) const {
                if (start <= floor) {
                    value_type a = start < floor ? node::op(val, cur->get()) : cur->get();
                    if (judge(a))
                        return val = a, ceil;
                    else if (floor == ceil)
                        return floor - 1;
                }
                SizeType mid = (floor + ceil) >> 1;
                _pushdown(cur, floor, ceil, mid);
                if (start <= mid) {
                    SizeType res = _max_right(cur->lchild(), floor, mid, start, val, judge);
                    if (res != mid) return res;
                }
                return _max_right(cur->rchild(), mid + 1, ceil, start, val, judge);
            }
            template <typename Judge>
            SizeType _min_left(node *cur, SizeType floor, SizeType ceil, SizeType start, value_type &val, Judge &&judge) const {
                if (start >= ceil) {
                    value_type a = start > ceil ? node::op(cur->get(), val) : cur->get();
                    if (judge(a))
                        return val = a, floor;
                    else if (floor == ceil)
                        return ceil + 1;
                }
                SizeType mid = (floor + ceil) >> 1;
                _pushdown(cur, floor, ceil, mid);
                if (start > mid) {
                    SizeType res = _min_left(cur->rchild(), mid + 1, ceil, start, val, judge);
                    if (res != mid + 1) return res;
                }
                return _min_left(cur->lchild(), floor, mid, start, val, judge);
            }
            SizeType _kth(node *cur, SizeType floor, SizeType ceil, value_type k) const {
                if (floor == ceil) return floor;
                SizeType mid = (floor + ceil) >> 1;
                _pushdown(cur, floor, ceil, mid);
                if (cur->lchild()->get() > k)
                    return _kth(cur->lchild(), floor, mid, k);
                else
                    return _kth(cur->rchild(), mid + 1, ceil, k - cur->lchild()->get());
            }
            Tree(SizeType length = 0, RangeMapping mapping = RangeMapping()) : TreeBase<RangeMapping, Complete>::TreeBase(mapping) { resize(length); }
            template <typename InitMapping = NoInit, typename = typename std::enable_if<std::is_same<RangeMapping, NoInit>::value && !std::is_same<InitMapping, RangeMapping>::value>::type>
            Tree(SizeType length, InitMapping mapping = InitMapping()) : TreeBase<RangeMapping, Complete>::TreeBase(NoInit()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) : TreeBase<RangeMapping, Complete>::TreeBase(NoInit()) { reset(first, last); }
            template <typename InitMapping = NoInit>
            void resize(SizeType length, InitMapping mapping = InitMapping()) {
                if (m_size = length) {
                    m_root = _newnode(0, m_size - 1);
                    if constexpr (Complete || !std::is_same<InitMapping, NoInit>::value) _initnode(_root(), 0, m_size - 1, mapping);
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](SizeType i) { return *(first + i); });
            }
            void modify(SizeType i, const value_type &val) { _modify(_root(), 0, m_size - 1, i, val); }
            void add(SizeType i, const modify_type &modify) { _add(_root(), 0, m_size - 1, i, modify); }
            void add(SizeType left, SizeType right, const modify_type &modify) { _add(_root(), 0, m_size - 1, left, right, modify); }
            value_type query(SizeType i) const { return _query(_root(), 0, m_size - 1, i); }
            value_type query(SizeType left, SizeType right) const { return _query(_root(), 0, m_size - 1, left, right); }
            value_type query_all() const { return _root()->get(); }
            template <typename Judge>
            SizeType max_right(SizeType left, Judge judge) {
                value_type val;
                return _max_right(_root(), 0, m_size - 1, left, val, judge);
            }
            template <typename Judge>
            SizeType min_left(SizeType right, Judge judge) {
                value_type val;
                return _min_left(_root(), 0, m_size - 1, right, val, judge);
            }
            SizeType kth(value_type k) { return _kth(_root(), 0, m_size - 1, k); }
        };
        template <typename Ostream, typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_NODE>
        Ostream &operator<<(Ostream &out, const Tree<Node, RangeMapping, Complete, SizeType, MAX_NODE> &x) {
            out << "[";
            for (SizeType i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_NODE>
        typename Tree<Node, RangeMapping, Complete, SizeType, MAX_NODE>::node Tree<Node, RangeMapping, Complete, SizeType, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, typename RangeMapping, bool Complete, typename SizeType, index_type MAX_NODE>
        index_type Tree<Node, RangeMapping, Complete, SizeType, MAX_NODE>::s_use_count = 1;
    }
    template <typename Tp, bool Complete, typename SizeType, typename Operation, typename InitOrRangeMapping = SegTree::NoInit, typename TreeType = SegTree::Tree<SegTree::CustomNode<Tp, Operation>, typename std::conditional<Complete, SegTree::NoInit, InitOrRangeMapping>::type, Complete, SizeType>>
    auto make_SegTree(SizeType length, Operation op, InitOrRangeMapping mapping = InitOrRangeMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, bool Complete, typename SizeType, typename InitOrRangeMapping = SegTree::NoInit, typename TreeType = SegTree::Tree<SegTree::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, typename std::conditional<Complete, SegTree::NoInit, InitOrRangeMapping>::type, Complete, SizeType>>
    auto make_SegTree(SizeType length, const Tp &(*op)(const Tp &, const Tp &), InitOrRangeMapping mapping = InitOrRangeMapping()) -> TreeType { return SegTree::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>::s_op = op, TreeType(length, mapping); }
    template <typename Tp, bool Complete, typename SizeType, typename InitOrRangeMapping = SegTree::NoInit, typename TreeType = SegTree::Tree<SegTree::CustomNode<Tp, Tp (*)(Tp, Tp)>, typename std::conditional<Complete, SegTree::NoInit, InitOrRangeMapping>::type, Complete, SizeType>>
    auto make_SegTree(SizeType length, Tp (*op)(Tp, Tp), InitOrRangeMapping mapping = InitOrRangeMapping()) -> TreeType { return SegTree::CustomNode<Tp, Tp (*)(Tp, Tp)>::s_op = op, TreeType(length, mapping); }
    template <typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = SegTree::Tree<SegTree::CustomNode<Tp, Operation>, SegTree::NoInit, true, uint32_t>>
    auto make_SegTree(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = SegTree::Tree<SegTree::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>, SegTree::NoInit, true, uint32_t>>
    auto make_SegTree(Iterator first, Iterator last, const Tp &(*op)(const Tp &, const Tp &)) -> TreeType { return SegTree::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>::s_op = op, TreeType(first, last); }
    template <typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = SegTree::Tree<SegTree::CustomNode<Tp, Tp (*)(Tp, Tp)>, SegTree::NoInit, true, uint32_t>>
    auto make_SegTree(Iterator first, Iterator last, Tp (*op)(Tp, Tp)) -> TreeType { return SegTree::CustomNode<Tp, Tp (*)(Tp, Tp)>::s_op = op, TreeType(first, last); }
    template <typename ValueType, typename ModifyType, bool InitClearLazy, bool Complete, typename SizeType, typename Operation, typename Mapping, typename Composition, typename InitOrRangeMapping = SegTree::NoInit, typename TreeType = SegTree::Tree<SegTree::CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, SizeType>, typename std::conditional<Complete, SegTree::NoInit, InitOrRangeMapping>::type, Complete, SizeType>>
    auto make_lazy_SegTree(SizeType length, Operation op, Mapping map, Composition com, InitOrRangeMapping mapping = InitOrRangeMapping()) -> TreeType { return TreeType(length, mapping); }
    template <typename ValueType, typename ModifyType, bool InitClearLazy, typename Iterator, typename Operation, typename Mapping, typename Composition, typename TreeType = SegTree::Tree<SegTree::CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy, uint32_t>, SegTree::NoInit, true, uint32_t>>
    auto make_lazy_SegTree(Iterator first, Iterator last, Operation op, Mapping map, Composition com) -> TreeType { return TreeType(first, last); }
    template <bool Complete = false, typename SizeType = uint64_t, SegTree::index_type MAX_NODE = 1 << 22>
    using SegSumTree = SegTree::Tree<SegTree::BaseNode<int64_t>, SegTree::NoInit, Complete, SizeType, MAX_NODE>;
    template <bool Complete = false, typename SizeType = uint64_t, SegTree::index_type MAX_NODE = 1 << 22>
    using SegLazySumTree = SegTree::Tree<SegTree::LazyNode<int64_t, int64_t, SizeType>, SegTree::NoInit, Complete, SizeType, MAX_NODE>;
}

#endif