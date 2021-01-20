#include <iostream>
#include <cassert>

template <class T>
struct DefComparator {
    int operator()(const T& l, const T& r) {
        if (l < r)
            return -1;
        if (l == r)
            return 0;
        return 1;
    }
};

template <class Key, class Value, class Compare = DefComparator<Key>>
struct AVLTree {

    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        uint8_t height;
        int count;
        Node(const Key& k, const Value& v):
             key(k), value(v), left(nullptr),
             right(nullptr), height(1), count(1) {};
        Node& operator=(const Node& rhs) {
            key = rhs.key;
            value = rhs.value;
            left = rhs.left;
            right = rhs.right;
            height = rhs.height;
            count = rhs.count;
        }
    };

    AVLTree(Compare comp = Compare()):
            root(nullptr), nodes_count(0), comp(comp) {}
    ~AVLTree()  {delete root;}
    Value* find(const Key& key) {
        return find_aux(key, root);
    }
    void insert(const Key& key_, const Value& val) {
        root = insert_aux(key_, val, root);
    }
    void erase(const Key& key_) {
        root = erase_aux(key_, root);
    }
    size_t size() {return nodes_count;}
    Value* find_aux(const Key& key, Node* node) {
        if (!node)
            return nullptr;
        int res = comp(key, node->key);
        if (res == -1) {
            return find_aux(key, node->left);
        } else if (res == 1) {
            return find_aux(key, node->right);
        }
        return &node->value;
    }
    Node* insert_aux(const Key& key,const Value& val, Node* node) {
        if (!node) {
            nodes_count++;
            return new Node(key, val);
        }
        int res = comp(key, node->key);
        if (res == -1) {
            node->left = insert_aux(key, val, node->left);
        } else if (res == 1) {
            node->right = insert_aux(key, val, node->right);
        }
        return balance(node);

    }
    Node* erase_aux(const Key& key, Node* node) {
        if (!node)
            return nullptr;
        int res = comp(key, node->key);
        if (res == -1) {
            node->left = erase_aux(key, node->left);
        } else if (res == 1) {
            node->right = erase_aux(key, node->right);
        } else {
            nodes_count--;
            Node* left = node->left;
            Node* right = node->right;
            delete node;
            if(!right)
                return left;
            Node *min_node;
            Node* buf = find_remove_min(right, &min_node);
            min_node->right = buf;
            min_node->left = left;

            return balance(min_node);
        }
        return balance(node);
    }

    Node* find_remove_min(Node* node, Node** min) {
        if (!node->left) {
            *min = node;
            return node->right;
        }
        node->left = find_remove_min(node->left, min);
        return balance(node);
    }

    uint8_t height(Node* node) {
        if (!node)
            return 0;
        return node->height;
    }
    int count(Node *node) {
        return node ? node->count : 0;
    }
    void fix_height(Node* node) {
        if (!node)
            return;
        node->count = count(node->left) + count(node->right) + 1;
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }
    uint8_t bfactor(Node* node) {
        return height(node->right) - height(node->left);
    }
    Node* balance(Node* node) {
        fix_height(node);
        int bf = bfactor(node);
        if (bf == 2) {
            if (bfactor(node->right) < 0)
                node->right = rotate_right(node->right);
            return rotate_left(node);
        } else if (bf == -2) {
            if (bfactor(node->left) > 0)
                node->left = rotate_left(node->left);
            return rotate_right(node);
        }
        return node;
    }
    Node* rotate_left(Node *p) {
        Node *q = p->right;
        p->right = q->left;
        q->left = p;
        fix_height(p);
        fix_height(q);
        return q;
    }

    Node *rotate_right(Node  *p) {
        Node *q = p->left;
        p->left = q->right;
        q->right = p;
        fix_height(p);
        fix_height(q);
        return q;
    }

    Node* root;
    size_t nodes_count;
    Compare comp;

};

int main() {
    /*AVLTree<int, int> tree1;
    for (size_t i = 0; i < 100; ++i) {
        tree1.insert(i, i);
        assert(*tree1.find(i) == i);
    }
    std::cout << "OK\n";
    for (size_t i = 0; i< 100; ++i) {
        assert(*tree1.find(i) == i);
        tree1.erase(i);
        assert(tree1.find(i) == nullptr);
    }
    assert(tree1.size() == 0);
    std::cout << "OK\n";*/

    int n;
    std::cin >> n;
    int val, stat = 0;
    auto *tree = new AVLTree<int, int>();
    for (int i = 0; i < n; ++i) {
        std::cin >> val;
        if (val >= 0) {
            tree->insert(val, val);
        } else {
            tree->erase(-1 * val);
        }
    }

    delete tree;
    return 0;
}