#ifndef __OY_CENTROID__
#define __OY_CENTROID__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <map>
#include <numeric>
#include <queue>
#include <unordered_map>

namespace OY {
    namespace Centroid {
        using size_type = uint32_t;
        template <typename Key, typename Value>
        using map_type = std::unordered_map<Key, Value>;
        template <typename Tree, size_type MAX_VERTEX>
        struct Centroid {
            static size_type s_buffer[MAX_VERTEX], s_use_count;
            Tree *m_tree;
            size_type *m_max_adj;
            std::pair<size_type, size_type> m_centroid;
            size_type _tree_dfs(size_type a, size_type p) {
                size_type size = 1;
                m_tree->do_for_each_adj_vertex(
                    a, [&](size_type to) {
                        if (to != p) {
                            size_type to_size = _tree_dfs(to, a);
                            size += to_size, m_max_adj[a] = std::max(m_max_adj[a], to_size);
                        }
                    });
                m_max_adj[a] = std::max(m_max_adj[a], m_tree->vertex_cnt() - size);
                if (m_max_adj[a] * 2 <= m_tree->vertex_cnt())
                    if (~m_centroid.first)
                        m_centroid.second = a;
                    else
                        m_centroid.first = a;
                return size;
            }
            Centroid(Tree *tree = nullptr) { reset(tree); }
            void reset(Tree *tree) {
                if (!(m_tree = tree)) return;
                m_max_adj = s_buffer + s_use_count, s_use_count += m_tree->vertex_cnt();
                m_centroid.first = m_centroid.second = -1;
                _tree_dfs(0, -1);
                if (~m_centroid.second && m_centroid.first > m_centroid.second) std::swap(m_centroid.first, m_centroid.second);
            }
            std::pair<size_type, size_type> centroid() const { return m_centroid; }
            size_type max_adjacent(size_type i) const { return m_max_adj[i]; }
        };
        template <typename Tree, size_type MAX_VERTEX>
        size_type Centroid<Tree, MAX_VERTEX>::s_buffer[MAX_VERTEX];
        template <typename Tree, size_type MAX_VERTEX>
        size_type Centroid<Tree, MAX_VERTEX>::s_use_count;
        struct TreeTrie {
            struct TreeTrieNode {
                size_type m_id;
                map_type<size_type, TreeTrieNode *> m_sons;
                TreeTrieNode(size_type id) : m_id(id) {}
            };
            static TreeTrieNode s_root;
            static size_type s_id_count;
            template <typename Tree>
            static void _tree_dfs(size_type a, size_type p, Tree &tree, std::vector<size_type> &parent, std::vector<size_type> &deg) {
                parent[a] = p;
                if (~p) deg[p]++;
                tree.do_for_each_adj_vertex(a, [&](size_type to) {if(to!=p)_tree_dfs(to,a,tree,parent,deg); });
            }
            template <typename Tree>
            static std::vector<size_type> get(Tree &tree, size_type root) {
                std::vector<size_type> res(tree.vertex_cnt()), parent(tree.vertex_cnt()), deg(tree.vertex_cnt());
                std::queue<size_type> Q;
                _tree_dfs(root, -1, tree, parent, deg);
                for (size_type i = 0; i != tree.vertex_cnt(); i++)
                    if (!deg[i]) Q.push(i);
                while (!Q.empty()) {
                    size_type a = Q.front();
                    Q.pop();
                    std::vector<size_type> children;
                    tree.do_for_each_adj_vertex(a, [&](size_type to) { if(to!=parent[a])children.push_back(res[to]); });
                    std::sort(children.begin(), children.end());
                    TreeTrieNode *cur = &s_root;
                    for (size_type id : children) {
                        auto it = cur->m_sons.emplace(id, nullptr).first;
                        if (!it->second) it->second = new TreeTrieNode(-1);
                        cur = it->second;
                    }
                    if (!~cur->m_id) cur->m_id = s_id_count++;
                    res[a] = cur->m_id;
                    size_type p = parent[a];
                    if (~p && !--deg[p]) Q.push(p);
                }
                return res;
            }
            template <size_type MAX_VERTEX, typename Tree>
            static std::pair<size_type, size_type> get(Tree &tree) {
                auto centroid = Centroid<Tree, MAX_VERTEX>(&tree).centroid();
                centroid.first = get(tree, centroid.first)[centroid.first];
                if (~centroid.second) {
                    centroid.second = get(tree, centroid.second)[centroid.second];
                    if (centroid.second < centroid.first) std::swap(centroid.first, centroid.second);
                }
                return centroid;
            }
        };
        TreeTrie::TreeTrieNode TreeTrie::s_root(0);
        size_type TreeTrie::s_id_count = 1;
        template <typename Tree, size_type MAX_VERTEX>
        struct CentroidDecomposition {
            using bool_tree_type = typename Tree::bool_tree_type;
            static size_type s_tag[MAX_VERTEX];
            Tree *m_tree;
            bool_tree_type m_cdtree;
            size_type _tree_dfs1(size_type a) {
                size_type size = 1;
                s_tag[a] = 1;
                m_tree->do_for_each_adj_vertex(a, [&](size_type to) { if (!s_tag[to]) size += _tree_dfs1(to); });
                s_tag[a] = 0;
                return size;
            }
            size_type _tree_dfs2(size_type a, size_type tot, size_type &centroid) {
                size_type size = 1;
                s_tag[a] = 1;
                m_tree->do_for_each_adj_vertex(a, [&](size_type to) {
                    if (!s_tag[to]) {
                        size_type to_size = _tree_dfs2(to, tot, centroid);
                        size += to_size, s_tag[a] = std::max(s_tag[a], to_size);
                    }
                });
                s_tag[a] = std::max(s_tag[a], tot - size);
                if (s_tag[a] * 2 <= tot) centroid = a;
                s_tag[a] = 0;
                return size;
            }
            CentroidDecomposition(Tree *tree = nullptr) { reset(tree); }
            void reset(Tree *tree) {
                if (!(m_tree = tree)) return;
                m_cdtree.resize(tree->vertex_cnt());
                size_type root = find_centroid(0);
                m_cdtree.prepare(), m_cdtree.set_root(root);
            }
            size_type find_centroid(size_type i) {
                size_type tot = _tree_dfs1(i), centroid = i;
                _tree_dfs2(i, tot, centroid);
                s_tag[centroid] = 1;
                if (centroid == 94) {
                    centroid++;
                    centroid--;
                }
                m_tree->do_for_each_adj_vertex(centroid, [&](size_type to) {
                    if (!s_tag[to]) {
                        size_type sub_centroid = find_centroid(to);
                        m_cdtree.add_edge(centroid, sub_centroid);
                    }
                });
                s_tag[centroid] = 0;
                return centroid;
            }
        };
        template <typename Tree, size_type MAX_VERTEX>
        size_type CentroidDecomposition<Tree, MAX_VERTEX>::s_tag[MAX_VERTEX];
    }
}

#endif
