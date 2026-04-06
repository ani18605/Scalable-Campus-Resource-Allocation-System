#include "SegmentTree.h"

SegmentTree::SegmentTree(int size) {
    n = size;
    if (n > 0) {
        tree.resize(4 * n);
        build(1, 0, n - 1);
    }
}

void SegmentTree::push_up(int node) {
    tree[node].ideal = SegMetrics::merge(tree[2 * node].ideal, tree[2 * node + 1].ideal);
    tree[node].actual = SegMetrics::merge(tree[2 * node].actual, tree[2 * node + 1].actual);
}

void SegmentTree::push_down(int node) {
    if (tree[node].lazy != 0) {
        int lz = tree[node].lazy;
        for (int child : {2 * node, 2 * node + 1}) {
            tree[child].lazy = lz;
            if (lz == 1) { // MAINT_ON
                tree[child].actual.set_val(0, tree[child].ideal.size);
            } else { // MAINT_OFF
                tree[child].actual = tree[child].ideal;
            }
        }
        tree[node].lazy = 0;
    }
}

void SegmentTree::build(int node, int l, int r) {
    tree[node].lazy = 0;
    if (l == r) {
        // Initially all rooms free, not occupied, no maintenance
        tree[node].ideal.set_val(1, 1);
        tree[node].actual.set_val(1, 1);
        return;
    }
    int mid = l + (r - l) / 2;
    build(2 * node, l, mid);
    build(2 * node + 1, mid + 1, r);
    push_up(node);
}

void SegmentTree::update_maintenance(int node, int l, int r, int ql, int qr, bool state) {
    if (ql <= l && r <= qr) {
        tree[node].lazy = state ? 1 : -1;
        if (state) {
            tree[node].actual.set_val(0, tree[node].ideal.size);
        } else {
            tree[node].actual = tree[node].ideal;
        }
        return;
    }
    push_down(node);
    int mid = l + (r - l) / 2;
    if (ql <= mid) update_maintenance(2 * node, l, mid, ql, qr, state);
    if (qr > mid) update_maintenance(2 * node + 1, mid + 1, r, ql, qr, state);
    push_up(node);
}

void SegmentTree::set_maintenance(int ql, int qr, bool state) {
    if (n == 0 || ql > qr || ql < 0 || qr >= n) return;
    update_maintenance(1, 0, n - 1, ql, qr, state);
}

void SegmentTree::update_occupied(int node, int l, int r, int idx, bool is_occupied) {
    if (l == r) {
        tree[node].ideal.set_val(is_occupied ? 0 : 1, 1);
        if (tree[node].lazy == 1) {
            tree[node].actual.set_val(0, 1);
        } else {
            tree[node].actual = tree[node].ideal;
        }
        return;
    }
    push_down(node);
    int mid = l + (r - l) / 2;
    if (idx <= mid) update_occupied(2 * node, l, mid, idx, is_occupied);
    else update_occupied(2 * node + 1, mid + 1, r, idx, is_occupied);
    push_up(node);
}

void SegmentTree::set_occupied(int idx, bool is_occupied) {
    if (idx < 0 || idx >= n) return;
    update_occupied(1, 0, n - 1, idx, is_occupied);
}

int SegmentTree::query_consecutive(int node, int l, int r, int target) {
    if (tree[node].actual.max_consec < target) return -1;
    if (l == r) return l;

    push_down(node);
    int mid = l + (r - l) / 2;
    
    // Prefer left child
    if (tree[2 * node].actual.max_consec >= target) {
        return query_consecutive(2 * node, l, mid, target);
    }
    // Try spanning middle
    if (tree[2 * node].actual.suffix_free + tree[2 * node + 1].actual.prefix_free >= target) {
        return mid - tree[2 * node].actual.suffix_free + 1;
    }
    // Try right child
    return query_consecutive(2 * node + 1, mid + 1, r, target);
}

int SegmentTree::find_consecutive(int target) {
    if (n == 0 || target <= 0) return -1;
    return query_consecutive(1, 0, n - 1, target);
}

int SegmentTree::get_total_free() {
    if (n == 0) return 0;
    return tree[1].actual.total_free;
}
