#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <vector>
#include <algorithm>

struct SegMetrics {
    int total_free;
    int prefix_free;
    int suffix_free;
    int max_consec;
    int size;

    void set_val(int val, int sz) {
        size = sz;
        if (val) {
            total_free = prefix_free = suffix_free = max_consec = sz;
        } else {
            total_free = prefix_free = suffix_free = max_consec = 0;
        }
    }
    
    static SegMetrics merge(const SegMetrics& left, const SegMetrics& right) {
        SegMetrics res;
        res.size = left.size + right.size;
        res.total_free = left.total_free + right.total_free;
        
        res.prefix_free = left.prefix_free;
        if (left.prefix_free == left.size) res.prefix_free += right.prefix_free;
        
        res.suffix_free = right.suffix_free;
        if (right.suffix_free == right.size) res.suffix_free += left.suffix_free;
        
        res.max_consec = std::max({left.max_consec, right.max_consec, left.suffix_free + right.prefix_free});
        return res;
    }
};

class SegmentTree {
    struct Node {
        SegMetrics ideal;
        SegMetrics actual;
        int lazy; // 0 = none, 1 = MAINT_ON, -1 = MAINT_OFF
    };

    int n;
    std::vector<Node> tree;

    void push_down(int node);
    void push_up(int node);
    void build(int node, int l, int r);
    void update_maintenance(int node, int l, int r, int ql, int qr, bool state);
    void update_occupied(int node, int l, int r, int idx, bool is_occupied);
    int query_consecutive(int node, int l, int r, int target_length);

public:
    SegmentTree(int size);
    
    // Set maintenance state on a range
    void set_maintenance(int ql, int qr, bool state);
    
    // Update occupied state of a single room
    void set_occupied(int idx, bool is_occupied);
    
    // Query starting index of 'target_length' consecutive free rooms. Returns -1 if not found.
    int find_consecutive(int target_length);
    
    // Query total free rooms in the block
    int get_total_free();
};

#endif // SEGMENT_TREE_H
