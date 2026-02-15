#pragma once

#include <cstdlib>

class AVL {
public:
    AVL();

    int find(int data) const;
    int select(size_t i) const;
    int min() const;
    int max() const;
    size_t rank(int data) const;
    void print() const;
    bool insert(int data);
    bool remove(int data);
    bool verify() const;

private:
    struct Node {
        int data;
        size_t height;
        size_t size;
        Node* left;
        Node* right;

        Node(int da) : data(da), height(1), size(1), left(nullptr), right(nullptr) {}
    };

    Node* root;

    inline size_t height(Node* node) const;
    inline size_t size(Node* node) const;
    inline int balance(Node* node) const;
    void update_node_data(Node* node);
    Node* right_rotate(Node* y);
    Node* left_rotate(Node* x);
    Node* rebalance(Node* node);
    Node* insert_rec(Node* node, int data);
    Node* find_min_node(Node* node) const;
    Node* remove_rec(Node* node, int data);
    Node* find_rec(Node* node, int data) const;
    int get_rank_rec(Node* node, int data) const;
    int select_rec(Node* node, size_t i) const;
    void print_rec(Node* node) const;
    bool verify_rec(Node* node) const;
};
