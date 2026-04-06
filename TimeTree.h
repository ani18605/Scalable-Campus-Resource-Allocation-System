#ifndef TIME_TREE_H
#define TIME_TREE_H

#include <vector>
#include <algorithm>

class TimeTree {
    std::vector<int> tree;
    std::vector<int> lazy;
    int n;

    void push_down(int node);
    void update(int node, int l, int r, int ql, int qr, int val);
    int query(int node, int l, int r, int ql, int qr);

public:
    TimeTree();
    
    // returns true if seats can be booked, false if not enough seats
    bool book_seats(int start_min, int end_min, int seats, int capacity);
    
    // query maximum seats used in an interval
    int max_seats_used(int start_min, int end_min);
};

#endif // TIME_TREE_H
