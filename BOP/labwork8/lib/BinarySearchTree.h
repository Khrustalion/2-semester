#pragma once
#include <functional>
#include <memory>
#include <algorithm>


template<class T>
struct Node{
    T* value;
    Node* right_child = nullptr;
    Node* left_child = nullptr;
    Node* parent = nullptr;
};


struct PreOrder {};

struct InOrder {};

struct PostOrder {};

template<class T, class TypeOrder, class Compare = std::less<T>, class Allocator = std::allocator<T>>
class BinarySearchTree {

private:
Allocator allocator_;
Compare compare_;
int size_bst_ = 0;
Node<T>* head_;
TypeOrder type_order_;

class reverse_iterator_ {
public:

using iterator_category = std::bidirectional_iterator_tag;
using value_type = T;
using difference_type = long;
using pointer = const T*;
using reference = T&;

Node<T>* current_node_;
Node<T>* last_node_;
Node<T>* root_;
TypeOrder type_order_;
bool from_left_tree_ = false;

reverse_iterator_(Node<T>* node, TypeOrder type_order, Node<T>* root) : current_node_(node), type_order_(type_order), root_(root) {
    last_(type_order_);
}

reverse_iterator_(const reverse_iterator_& rhs) : current_node_(rhs.current_node_), type_order_(rhs.type_order_), root_(rhs.root_), from_left_tree_(rhs.from_left_tree_) {
    last_(type_order_);
}

const reverse_iterator_ operator=(reverse_iterator_ rhs) {
    current_node_= rhs.current_node_;
    type_order_ = rhs.type_order_;
    root_ = rhs.root_;
    from_left_tree_ = rhs.from_left_tree_;

    return *this;
}

const reverse_iterator_ operator++() {
    return next_(type_order_);
}

const reverse_iterator_ operator++(int i) {
    reverse_iterator_ tmp(*this);

    ++(*this);

    return tmp;
}

const reverse_iterator_ operator--() {
    return previous_(type_order_);
}

const reverse_iterator_ operator--(int i) {
    reverse_iterator_ tmp(*this);

    --(*this);

    return tmp;
}

bool operator==(const reverse_iterator_& rhs) const {
    if (rhs.current_node_ == current_node_) {
        return true;
    }
    return false;
}

bool operator==(const reverse_iterator_& rhs) {
    if (rhs.current_node_ == current_node_) {
        return true;
    }
    return false;
}

bool operator!=(const reverse_iterator_& rhs) const{
    return !((*this) == rhs);
}

bool operator!=(const reverse_iterator_& rhs) {
    return !((*this) == rhs);
}

T operator*() const {
    return *current_node_->value;
}

~reverse_iterator_() {}


void last_(PreOrder preorder) {
    last_node_ = root_;
}

void last_(InOrder inorder) {
    if (current_node_ && current_node_->left_child) {
        from_left_tree_ = true;
    }

    last_node_ = root_;

    while (last_node_->left_child) {
        if (last_node_->left_child) {
            last_node_ = last_node_->left_child;
        } 
    }
}

void last_(PostOrder postorder) {
    last_node_ = root_;

    while (last_node_->left_child || last_node_->right_child) {
        if (last_node_->left_child) {
            last_node_ = last_node_->left_child;
        } else {
            last_node_ = last_node_->right_child;
        }
    }
}

const reverse_iterator_ next_(PreOrder preorder) {
    if (current_node_ == last_node_) {
        current_node_ = nullptr;
        return *this;
    }
    if (current_node_->parent->left_child == current_node_) {
        current_node_ = current_node_->parent;
        return *this;
    }
    current_node_ = current_node_->parent;
    if (!current_node_->left_child) {
        return *this;
    }
    current_node_ = current_node_->left_child;
    while (current_node_->right_child || current_node_->left_child) {
        if (current_node_->right_child) {
            current_node_ = current_node_->right_child;
        } else {
            current_node_ = current_node_->left_child;
        }
    }
    return *this;
}

const reverse_iterator_ next_(InOrder inorder) {
    if (current_node_ == last_node_) {
        current_node_ =  nullptr;

        return *this;
    }
    if (current_node_->left_child) {
        current_node_ = current_node_->left_child;
        while (current_node_->right_child) {
            current_node_ = current_node_->right_child;
        }
        return *this;
    }
    while (current_node_->parent) {
        if (current_node_->parent->right_child == current_node_) {
            current_node_ = current_node_->parent;
            return *this;
        } 
        current_node_ = current_node_->parent;
    }
    return *this;
}

const reverse_iterator_ next_(PostOrder postorder) {
    if (current_node_ == last_node_) {
        current_node_ = nullptr;

        return *this;
    }
    if (current_node_->right_child) {
        current_node_ =  current_node_->right_child;

        return *this;
    }
    if (current_node_->left_child) {
        current_node_ = current_node_->left_child;

        return *this;
    }
    while (!(current_node_->parent->left_child && current_node_->parent->right_child == current_node_)) {
        current_node_ = current_node_->parent;
    }
    current_node_ = current_node_->parent->left_child;

    return *this;
}

const reverse_iterator_ previous_(PreOrder preorder) {
    if (current_node_ == nullptr) {
        current_node_ = last_node_;

        return *this;
    };
    if (current_node_->left_child) {
        current_node_ = current_node_->left_child;
        return *this;
    } else if (current_node_->right_child) {
        current_node_ = current_node_->right_child;
        return *this;
    }
    
    while (current_node_->parent) {
        if (current_node_->parent->right_child && current_node_->parent->left_child == current_node_) {
            current_node_ = current_node_->parent->right_child;
            return *this;
        }
        current_node_ = current_node_->parent;
    }
    return *this;
}

const reverse_iterator_ previous_(InOrder inorder) {
    if (current_node_ == nullptr) {
        current_node_ = last_node_;

        return *this;
    }
    if (current_node_->left_child && (!from_left_tree_)) {
            while (current_node_->left_child) {
                current_node_ = current_node_->left_child;
            }
            return (*this);
        } else if (current_node_->right_child) {
            from_left_tree_ = false;
            current_node_ = current_node_->right_child;
            while (current_node_->left_child) {
                current_node_ = current_node_->left_child;
            }
            return (*this);
        } 
        while (current_node_->parent) {
            if (current_node_->parent->left_child == current_node_) {
                from_left_tree_ = true;
                current_node_ = current_node_->parent;
                return (*this);
            }
            current_node_ = current_node_->parent;
        }
        return *this;
}

const reverse_iterator_ previous_(PostOrder postorder) {
    if (current_node_ == nullptr) {
        current_node_ = last_node_;

        return *this;
    }
    if (current_node_->parent && current_node_->parent->right_child && current_node_ != current_node_->parent->right_child) {
            current_node_ = current_node_->parent->right_child;
        while (current_node_->left_child || current_node_->right_child) {
            if (current_node_->left_child) {
                current_node_ = current_node_->left_child;
            } else {
                current_node_ = current_node_->right_child;
            }
        }
        return *this;
    }

    current_node_ = current_node_->parent;
    return *this;
}
};

class iterator_ {
public:

using iterator_category = std::bidirectional_iterator_tag;
using value_type = T;
using difference_type = long;
using pointer = const T*;
using reference = T;

Node<T>* current_node_;
Node<T>* root_;
TypeOrder type_order_;
bool from_left_tree_ = false;

iterator_(Node<T>* node, TypeOrder type_order, Node<T>* root) : current_node_(node), type_order_(type_order), root_(root) {
    first_(type_order_);
}

iterator_(const iterator_& rhs) : current_node_(rhs.current_node_), type_order_(rhs.type_order_), root_(rhs.root_), from_left_tree_(rhs.from_left_tree_) {}

const iterator_ operator=(iterator_ rhs) {
    current_node_= rhs.current_node_;
    type_order_ = rhs.type_order_;
    root_ = rhs.root_;
    from_left_tree_ = rhs.from_left_tree_;

    return *this;
}

const iterator_ operator++() {
    return next_(type_order_);
}

const iterator_ operator++(int i) {
    iterator_ tmp(*this);

    ++(*this);

    return tmp;
}

const iterator_ operator--() const {
    return previous_(type_order_);
}

const iterator_ operator--(int i) const {
    iterator_ tmp(*this);

    --(*this);

    return tmp;
}

bool operator==(const iterator_& rhs) const {
    if (rhs.current_node_ == current_node_) {
        return true;
    }
    return false;
}

bool operator==(const iterator_& rhs) {
    if (rhs.current_node_ == current_node_) {
        return true;
    }
    return false;
}

bool operator!=(const iterator_& rhs) const {
    return !((*this) == rhs);
}

bool operator!=(const iterator_& rhs) {
    return !((*this) == rhs);
}



T operator*() const {
    return *current_node_->value;
}

~iterator_() {}

void first_(PreOrder preorder) {}

void first_(InOrder inorder) {
    if (current_node_ && current_node_->left_child) {
        from_left_tree_ = true;
    }
}

void first_(PostOrder postorder) {}

const iterator_ previous_(PreOrder preorder) {
    if (current_node_ == nullptr) {
        current_node_ = root_;
        while (current_node_->right_child) {
            current_node_ = current_node_->right_child;
        }
        return *this;
    }
    if (current_node_->parent->left_child == current_node_) {
        current_node_ = current_node_->parent;
        return *this;
    }
    current_node_ = current_node_->parent;
    if (!current_node_->left_child) {
        return *this;
    }
    current_node_ = current_node_->left_child;
    while (current_node_->right_child || current_node_->left_child) {
        if (current_node_->right_child) {
            current_node_ = current_node_->right_child;
        } else {
            current_node_ = current_node_->left_child;
        }
    }
    return *this;
}

const iterator_ previous_(InOrder inorder) {
    if (current_node_ == nullptr) {
        current_node_ = root_;
        while (current_node_->right_child) {
            current_node_ = current_node_->right_child;
        }
        return *this;
    }
    if (current_node_->left_child) {
        current_node_ = current_node_->left_child;
        while (current_node_->right_child) {
            current_node_ = current_node_->right_child;
        }
        return *this;
    }
    while (current_node_->parent) {
        if (current_node_->parent->right_child == current_node_) {
            current_node_ = current_node_->parent;
            return *this;
        } 
        current_node_ = current_node_->parent;
    }
    return *this;
}

const iterator_ previous_(PostOrder postorder) {
    if (current_node_ == nullptr) {
        current_node_ = root_;
        return *this;
    }
    if (current_node_->right_child) {
        current_node_ =  current_node_->right_child;
        return *this;
    }
    if (current_node_->left_child) {
        current_node_ = current_node_->left_child;
        return *this;
    }
    while (!(current_node_->parent->left_child && current_node_->parent->right_child == current_node_)) {
        current_node_ = current_node_->parent;
    }
    current_node_ = current_node_->parent->left_child;
    return *this;
}

const iterator_ next_(PreOrder preorder) {
    if (current_node_->left_child) {
        current_node_ = current_node_->left_child;
        return *this;
    } else if (current_node_->right_child) {
        current_node_ = current_node_->right_child;
        return *this;
    }
    
    while (current_node_->parent) {
        if (current_node_->parent->right_child && current_node_->parent->left_child == current_node_) {
            current_node_ = current_node_->parent->right_child;
            return *this;
        }
        current_node_ = current_node_->parent;
    }
    current_node_ = current_node_->parent;
    return *this;
}

const iterator_ next_(InOrder inorder) {
    if (current_node_->left_child && (!from_left_tree_)) {
            while (current_node_->left_child) {
                current_node_ = current_node_->left_child;
            }
            return (*this);
        } else if (current_node_->right_child) {
            from_left_tree_ = false;
            current_node_ = current_node_->right_child;
            while (current_node_->left_child) {
                current_node_ = current_node_->left_child;
            }
            return (*this);
        } 
        while (current_node_->parent) {
            if (current_node_->parent->left_child == current_node_) {
                from_left_tree_ = true;
                current_node_ = current_node_->parent;
                return (*this);
            }
            current_node_ = current_node_->parent;
        }
        current_node_ = current_node_->parent;
        return *this;
}

const iterator_ next_(PostOrder postorder) {
    if (current_node_->parent && current_node_->parent->right_child && current_node_ != current_node_->parent->right_child) {
            current_node_ = current_node_->parent->right_child;
        while (current_node_->left_child || current_node_->right_child) {
            if (current_node_->left_child) {
                current_node_ = current_node_->left_child;
            } else {
                current_node_ = current_node_->right_child;
            }
        }
        return *this;
    }
    current_node_ = current_node_->parent;
    return *this;
}
};

void extract_(Node<T>* current) {
    if (current && current->right_child && current->left_child) {
        Node<T>* next_min = findNextMin_(current);

        if ((next_min->parent->right_child) && (next_min == next_min->parent->right_child)) {
            next_min->parent->right_child = next_min->right_child;
        } else {
            next_min->parent->left_child = next_min->right_child;
        }
        if (next_min->right_child) {
            next_min->right_child->parent = next_min->parent;
        }

        std::construct_at(current->value, *next_min->value);

        --size_bst_;
        allocator_.deallocate(next_min->value, 1);
        delete [] next_min;
    } else if (current && current->right_child) {
        current->right_child->parent = current->parent;

        if ((current != head_) && (current == current->parent->right_child)) {
            current->parent->right_child = current->right_child;
        } else if (current != head_){
            current->parent->left_child = current->right_child;
        }

        if (current == head_) {
            head_ = current->right_child;
        }
        
        --size_bst_;
        allocator_.deallocate(current->value, 1);
        delete [] current;
    } else if (current && current->left_child) {
        current->left_child->parent = current->parent;

        if ((current != head_) && (current == current->parent->right_child)) {
            current->parent->right_child = current->left_child;
        } else if (current != head_) {
            current->parent->left_child = current->left_child;
        }

        if (current == head_) {
            head_ = current->left_child;
        }
        
        --size_bst_;
        allocator_.deallocate(current->value, 1);
        delete [] current;
    } else if (current) {
        if ((current != head_) && (current == current->parent->right_child)) {
            current->parent->right_child = nullptr;
        } else if (current != head_) {
            current->parent->left_child = nullptr;
        }

        if (current == head_) {
            head_ = nullptr;
        }
        
        --size_bst_;
        allocator_.deallocate(current->value, 1);
        delete [] current;
    }
}

void extract_(iterator_ it) {
    extract_(it.current_node_);
}

void extract_(T value) {
    auto current = find(value);

    extract_(current);
}

template<class InputIt>
const iterator_ extract_(InputIt first, InputIt last) {
    while (first != last) {
        this->extract_(*first);
        ++first;
    }
    return iterator_(first_(type_order_), type_order_, head_);
}


Node<T>* first_reverse_iterator_(PreOrder preorder) const {
    Node<T>* tmp = head_;

    while (tmp->left_child || tmp->right_child) {
        if (tmp->right_child) {
            tmp = tmp->right_child;
        } else {
            tmp = tmp->left_child;
        }
    }
    return tmp;
}

Node<T>* first_reverse_iterator_(InOrder inorder) const {
    Node<T>* tmp = head_;

    while (tmp->right_child) {
        tmp = tmp->right_child;
    }
    return tmp;
}

Node<T>* first_reverse_iterator_(PostOrder postorder) const {
    return head_;
}

Node<T>* first_iterator_(PreOrder preorder) const {
    return head_;
}

Node<T>* first_iterator_(InOrder inorder) const {
    Node<T>* tmp = head_;
    while (tmp->left_child) {
        tmp = tmp->left_child;
    }
    return tmp;
}

Node<T>* first_iterator_(PostOrder postorder) const {
    Node<T>* tmp = head_;

    while (tmp->left_child || tmp->right_child) {
        if (tmp->left_child) {
            tmp = tmp->left_child;
        } else {
            tmp = tmp->right_child;
        }
    }
    return tmp;
}

Node<T>* findNextMin_(Node<T>* value) {
    value = value->right_child;

    while (value->left_child) {
        value = value->left_child;
    }
    return value;
}

Node<T>* search_pos(T value) {
    Node<T>* tmp = head_;

    while (true) {
        if ((*tmp->value) == value) {
            break;
        } else if ((tmp->right_child) && (compare_((*tmp->value), value))) {
            tmp = tmp->right_child;
        } else if ((!tmp->right_child) && (compare_((*tmp->value), value))) {
            break;
        } else if ((tmp->left_child) && (!(compare_((*tmp->value), value)))) {
            tmp = tmp->left_child;
        } else {
            break;
        }
    }
    return tmp;
} 

public:

using value_type = T;
using reference = T&;
using const_reference = const T&;
using iterator = iterator_;
using const_iterator_= iterator_;
using reverse_iterator = reverse_iterator_;
using const_reverse_iterator = reverse_iterator_;
using difference_type = size_t;
using size_type = size_t;


BinarySearchTree() {}

template<class InputIt>
BinarySearchTree(InputIt it1, InputIt it2, const Compare& compare) {
    this->insert<InputIt>(it1, it2);
    compare_ = compare;
}

BinarySearchTree(std::initializer_list<T> il) {
    this->insert(il);
}

BinarySearchTree (BinarySearchTree<T, TypeOrder, Compare, Allocator>& copy) : size_bst_(copy.size_bst_), 
                                                                              allocator_(copy.allocator_), 
                                                                              type_order_(copy.type_order_),
                                                                              compare_(copy.compare_) {
    auto it = copy.begin();
    ++it;
    Node<T>* current_node = new Node<T>[1];
    current_node->value = allocator_.allocate(1);
    std::construct_at(current_node->value, *copy.head_->value);

    head_ = current_node;

    for (;it != copy.end(); ++it) {
        if (compare_(*it, *current_node->value)) {
            Node<T>* new_node = new Node<T>[1];

            new_node->value = allocator_.allocate(1);
            std::construct_at(new_node->value, *it);
            new_node->parent = current_node;
            current_node->left_child = new_node;

            current_node = new_node;
        } else {
            while (current_node->parent && (!compare_(*it, *current_node->value))) {
                current_node = current_node->parent;
            }

            Node<T>* new_node = new Node<T>[1];
            
            new_node->value = allocator_.allocate(1);
            std::construct_at(new_node->value, *it);
            new_node->parent = current_node;
            current_node->right_child = new_node;

            current_node = new_node;
        }
    }
}

size_t size() const {
    return size_bst_;
}

bool operator==(BinarySearchTree& rhs) {
    if (this->size() != rhs.size()) {
        return false;
    }

    for (auto it1 = this->begin(), it2 = rhs.begin(); it1 != this->end(); ++it1, ++it2) {
        if (*it1 != *it2) {
            return false;
        }
    }
    return true;
}

bool operator!=(BinarySearchTree& rhs) {
    return !((*this) == rhs);
}

BinarySearchTree& operator=(std::initializer_list<T> il) {
    this->insert(il);
    return *this;
}

void swap(BinarySearchTree rhs) {
    std::swap(this->allocator_, rhs.allocator_);
    std::swap(this->head_, rhs.head_);
    std::swap(this->type_order_, rhs.type_order_);
    std::swap(this->size_bst_, rhs.size_bst_);
}

Compare key_comp() {
    return compare_;
}

Compare value_comp() {
    return compare_;
}

const iterator_ insert(T value) {
    if (!size_bst_) {
        head_ = new Node<T>[1];
        head_->value = allocator_.allocate(1);
        std::construct_at(head_->value, value);
        head_->left_child = nullptr;
        head_->right_child = nullptr;
        head_->parent = nullptr;

        ++size_bst_;
        return iterator_(head_, type_order_, head_);
    } else {
        Node<T>* tmp = search_pos(value);

        if ((*tmp->value) == value) {
            return iterator_(tmp, type_order_, head_);
        } else if (compare_((*tmp->value), value)) {
            Node<T>* new_right_child = new Node<T>[1];
            new_right_child->value = allocator_.allocate(1);

            std::construct_at(new_right_child->value, value);
            new_right_child->parent = tmp;
            new_right_child->left_child = nullptr;
            new_right_child->right_child = nullptr;
            tmp->right_child = new_right_child;


            ++size_bst_;

            return iterator_(new_right_child, type_order_, head_);
        }
        Node<T>* new_left_child = new Node<T>[1];
        new_left_child->value = allocator_.allocate(1);

        std::construct_at(new_left_child->value, value);
        new_left_child->parent = tmp;
        new_left_child->left_child = nullptr;
        new_left_child->right_child = nullptr;
        tmp->left_child = new_left_child;

        ++size_bst_;

        return iterator_(new_left_child, type_order_, head_);
    }
}

template<class InputIt>
void insert(InputIt first, InputIt last) {
    while (first != last) {
        this->insert(*first);
        ++first;
    }
}

void insert(std::initializer_list<T> il) {
    insert(il.begin(), il.end());
}

const iterator_ begin() const {
    if (size_bst_ == 0) {
        return iterator_(nullptr, type_order_, nullptr);
    }
    return iterator_(first_iterator_(type_order_), type_order_, head_);
}

const iterator_ cbegin() const {
    if (size_bst_ == 0) {
        return iterator_(nullptr, type_order_, nullptr);
    }
    return iterator_(first_iterator_(type_order_), type_order_, head_);
}

const iterator_ end() const {
    return iterator_(nullptr, type_order_, head_);
}

const iterator_ cend() const {
    return iterator_(nullptr, type_order_, head_);
}

const reverse_iterator_ rbegin() const {
    if (size_bst_ == 0) {
        return reverse_iterator_(nullptr, type_order_, nullptr);
    }
    return reverse_iterator_(first_reverse_iterator_(type_order_), type_order_, head_);
}

const reverse_iterator_ crbegin() const {
    if (size_bst_ == 0) {
        return reverse_iterator_(nullptr, type_order_, nullptr);
    }
    return reverse_iterator_(first_reverse_iterator_(type_order_), type_order_, head_);
}


const reverse_iterator_ rend() const {
    return reverse_iterator_(nullptr, type_order_, head_);
}

const reverse_iterator_ crend() const {
    return reverse_iterator_(nullptr, type_order_, head_);
}

bool empty() {
    return size_bst_ == 0;
}

size_t erase(T value) {
    size_t v_in_bst = 0;
    if (this->find(value) != this->end()) {
        v_in_bst = 1;
    }
    this->extract_(value);
    
    return v_in_bst;
}

const iterator_ erase(iterator_ it) {
    iterator_ tmp = ++it;
    this->extract(it.current_node_);

    return tmp;
}

template<class InputIt>
const iterator_ erase(InputIt it1, InputIt it2) {
    ++it2;

    extract_(it1, it2);
    return it2;
}

template<class T_r, class TypeOrder_r, class Compare_r = std::less<T>, class Allocator_r = std::allocator<T>>
void merge(BinarySearchTree<T_r, TypeOrder_r, Compare_r, Allocator_r>& rhs) {
    auto start = rhs.begin();

    while (start != rhs.end()) {
        this->insert(*start);
        ++start;
    }

    rhs.clear();
}

size_t count(T value) {
    if (find(value) != this->end()) {
        return 1;
    }
    return 0;
}

void clear() {
    if (size_bst_) {
        deleteBst(head_);
    } 
}

const iterator_ find(T value) {
    Node<T>* tmp = head_;

    while (tmp) {
        if ((*tmp->value) == value) {
            break;
        } else if (compare_((*tmp->value), value)) {
            tmp = tmp->right_child;
        } else {
            tmp = tmp->left_child;
        }
    }

    iterator_ result(tmp, type_order_, head_);
    return result;
}

void deleteBst(Node<T>* current_node) {
    if (current_node) {
        deleteBst(current_node->left_child);
        deleteBst(current_node->right_child);

        allocator_.deallocate(current_node->value, 1);
        delete [] current_node;
    }
    size_bst_ = 0;
}

~BinarySearchTree() {
    if (size_bst_) {
        deleteBst(head_);
    }
}

Node<T>* head() {
    return head_;
}
};
