#include <type_traits>
#include <vector>

namespace olymplib {

using std::size_t;

struct internal_tree_node { 
    //[l..r]
    int l, r;
    internal_tree_node() {};
    internal_tree_node(int l, int r) :l(l), r(r) {};
};

struct internal_tree_query {
    //[l..r]
    int l, r;
    internal_tree_query() {};
    internal_tree_query(int l, int r) :l(l), r(r) {};
};

template <class node, class query, class combiner, class updater, 
    typename std::enable_if<std::is_base_of<internal_tree_node, node>::value>::type* = nullptr,
    typename std::enable_if<std::is_base_of<internal_tree_query, query>::value>::type* = nullptr>
struct segment_tree {
    //contract: (l <= r)
    segment_tree(int l, int r) {
        internal.reserve((r - l + 1) * 4);
        lnode.reserve((r - l + 1) * 4);
        rnode.reserve((r - l + 1) * 4);

        root = build(l, r);

        internal.shrink_to_fit();
        lnode.shrink_to_fit();
        rnode.shrink_to_fit();
    }
    node ask(int l, int r) {
        cached_l = l;
        cached_r = r;
        return ask(root);
    }
    void update(const query& q) {
        cached_l = q.l;
        cached_r = q.r;
        cached_q = q;
        update(root);
    }
private:
    void update(size_t index) {
        int nl = internal[index].l;
        int nr = internal[index].r;
        if (cached_l <= nl && nr <= cached_r) {
            updater()(internal[index], cached_q);
            return;
        }
        int nmid = (nl + nr) >> 1;
        if (cached_r <= nmid) {
            update(lnode[index]);
            update_node(index);
        }
        else if (cached_l > nmid + 1) {
            update(rnode[index]);
            update_node(index);
        }
        else {
            update(lnode[index]);
            update(rnode[index]);
            update_node(index);
        }
    }
    node ask(size_t index) {
        int nl = internal[index].l;
        int nr = internal[index].r;
        if (cached_l <= nl && nr <= cached_r) {
            return internal[index];
        }
        int nmid = (nl + nr) >> 1;
        if (cached_r <= nmid)
            return ask(lnode[index]);
        if (cached_l >= nmid + 1)
            return ask(rnode[index]);
        return combiner()(ask(lnode[index]), ask(rnode[index]));
    }
    int cached_l, cached_r;
    query cached_q;
    std::vector<node> internal;
    std::vector<size_t> lnode, rnode;
    //this should never be called in leaves
    void update_node(size_t index) {
        node tmp(combiner()(internal[lnode[index]], internal[rnode[index]]));
        tmp.l = internal[index].l;
        tmp.r = internal[index].r;
        internal[index] = tmp;
    }
    const size_t build(int l, int r) {
        if (l == r) {
            return get_new_leaf(l);
        } else {
            int mid = (l + r) >> 1;
            size_t lef = build(l, mid);
            size_t rig = build(mid + 1, r);
            size_t id = internal.size();
            internal.push_back(combiner()(internal[lef], internal[rig]));

            internal.back().l = l;
            internal.back().r = r;
            lnode.push_back(lef);
            rnode.push_back(rig);
            return id;
        }
    }
    size_t root;
    size_t get_new_leaf(int index) {        
        internal.push_back(node(index));

        internal.back().l = index;
        internal.back().r = index;

        lnode.push_back(-1);
        rnode.push_back(-1);
        return internal.size() - 1;
    }
};

} // namespace olymplib
