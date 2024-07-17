#include <iostream>
#include <lib/BinarySearchTree.h>
#include <vector>
#include <string>

template<class T>
void inorder(Node<T>* root) {
    if (root != nullptr) {
        inorder(root->left_child);
        std::cout << "std::pair<int, int>(" << (*root->value) / 10 << ", " << (*root->value)%10 << "), ";
        inorder(root->right_child);
    }
}

template<class T>
void preorder(Node<T>* root) {
    if (root != nullptr) {
        std::cout << "std::pair<int, int>(" << (*root->value) / 10 << ", " << (*root->value)%10 << "), ";
        preorder(root->left_child);
        preorder(root->right_child);
    }
}

template<class T>
void postorder(Node<T>* root) {
    if (root != nullptr) {
        postorder(root->left_child);
        postorder(root->right_child);
        std::cout << "std::pair<int, int>(" << (*root->value) / 10 << ", " << (*root->value)%10 << "), ";
    }
}

int main() {
    std::vector<int> a{1, 5, 0, 3, -1, 2};
    BinarySearchTree<int, InOrder> bst{a.begin(), a.end(), std::less<int>()};

    return 0;
}
