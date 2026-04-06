#include "TimeTree.h"
#include <algorithm>

// 24 * 60 = 1440 minutes.
TimeTree::TimeTree() {
    n = 1440;
    tree.assign(4 * n, 0);
    lazy.assign(4 * n, 0);
}

void TimeTree::push_down(int node) {
    if (lazy[node] != 0) {
        tree[2 * node] += lazy[node];
        lazy[2 * node] += lazy[node];
        tree[2 * node + 1] += lazy[node];
        lazy[2 * node + 1] += lazy[node];
        lazy[node] = 0;
    }
}

void TimeTree::update(int node, int l, int r, int ql, int qr, int val) {
    if (ql <= l && r <= qr) {
        tree[node] += val;
        lazy[node] += val;
        return;
    }
    push_down(node);
    int mid = l + (r - l) / 2;
    if (ql <= mid) update(2 * node, l, mid, ql, qr, val);
    if (qr > mid) update(2 * node + 1, mid + 1, r, ql, qr, val);
    tree[node] = std::max(tree[2 * node], tree[2 * node + 1]);
}

int TimeTree::query(int node, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) {
        return tree[node];
    }
    push_down(node);
    int mid = l + (r - l) / 2;
    int res = 0;
    if (ql <= mid) res = std::max(res, query(2 * node, l, mid, ql, qr));
    if (qr > mid) res = std::max(res, query(2 * node + 1, mid + 1, r, ql, qr));
    return res;
}

bool TimeTree::book_seats(int start_min, int end_min, int seats, int capacity) {
    if (start_min >= end_min) return false;
    int current_max = query(1, 0, n - 1, start_min, end_min - 1);
    if (current_max + seats > capacity) {
        return false;
    }
    update(1, 0, n - 1, start_min, end_min - 1, seats);
    return true;
}

int TimeTree::max_seats_used(int start_min, int end_min) {
    if (start_min >= end_min) return 0;
    return query(1, 0, n - 1, start_min, end_min - 1);
}
