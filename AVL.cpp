#include <bits/stdc++.h>
using namespace std;

struct Node {
    int key;
    Node *left, *right;
    int height;
    Node(int val) : key(val), left(nullptr), right(nullptr), height(1) {}
};

// Utility functions
int height(Node *n) {
    return n ? n->height : 0;
}
int getBalance(Node *n) {
    return n ? height(n->left) - height(n->right) : 0;
}
void updateHeight(Node *n) {
    n->height = 1 + max(height(n->left), height(n->right));
}

// Rotations with explanation
Node* rightRotate(Node* y) {
    cout << "\nPerforming Right Rotation (LL Case) on Node " << y->key << endl;
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);
    return x;
}

Node* leftRotate(Node* x) {
    cout << "\nPerforming Left Rotation (RR Case) on Node " << x->key << endl;
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);
    return y;
}

// Insert with rotations + messages
Node* insertNode(Node* node, int key) {
    if (!node) {
        cout << "Inserting node " << key << endl;
        return new Node(key);
    }

    if (key < node->key)
        node->left = insertNode(node->left, key);
    else if (key > node->key)
        node->right = insertNode(node->right, key);
    else {
        cout << "Duplicate keys not allowed!\n";
        return node;
    }

    updateHeight(node);
    int balance = getBalance(node);

    cout << "Node " << node->key << " → Balance Factor = " << balance << endl;

    // LL Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // RR Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // LR Case
    if (balance > 1 && key > node->left->key) {
        cout << "Performing Left-Right (LR) Rotation on Node " << node->key << endl;
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // RL Case
    if (balance < -1 && key < node->right->key) {
        cout << "Performing Right-Left (RL) Rotation on Node " << node->key << endl;
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Inorder traversal
void inorder(Node* root) {
    if (!root) return;
    inorder(root->left);
    cout << root->key << " ";
    inorder(root->right);
}

// Search node
bool searchNode(Node* root, int key) {
    if (!root) return false;
    if (key == root->key) return true;
    if (key < root->key) return searchNode(root->left, key);
    return searchNode(root->right, key);
}

// Tree print (sideways)
void printTree(Node* root, string indent = "", bool last = true) {
    if (!root) return;
    cout << indent;
    if (last) {
        cout << "R----";
        indent += "     ";
    } else {
        cout << "L----";
        indent += "|    ";
    }
    cout << root->key << " (BF=" << getBalance(root) << ")\n";
    printTree(root->left, indent, false);
    printTree(root->right, indent, true);
}

// Delete node (simplified)
Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left) current = current->left;
    return current;
}

Node* deleteNode(Node* root, int key) {
    if (!root) return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        cout << "Deleting node " << key << endl;
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = nullptr;
            } else {
                *root = *temp;
            }
            delete temp;
        } else {
            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (!root) return root;

    updateHeight(root);
    int balance = getBalance(root);
    cout << "Node " << root->key << " → Balance Factor = " << balance << endl;

    // LL
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // RR
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // LR
    if (balance > 1 && getBalance(root->left) < 0) {
        cout << "Performing LR rotation on node " << root->key << endl;
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // RL
    if (balance < -1 && getBalance(root->right) > 0) {
        cout << "Performing RL rotation on node " << root->key << endl;
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Main menu
int main() {
    Node* root = nullptr;
    int choice, key;

    while (true) {
        cout << "\n========== AVL TREE MENU ==========\n";
        cout << "1. Insert Node\n2. Delete Node\n3. Search Node\n4. Display Tree\n5. Inorder Traversal\n0. Exit\n";
        cout << "===================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter value to insert: ";
                cin >> key;
                root = insertNode(root, key);
                cout << "\nCurrent AVL Tree:\n";
                printTree(root);
                break;

            case 2:
                cout << "Enter value to delete: ";
                cin >> key;
                if (!searchNode(root, key))
                    cout << "Node not found!\n";
                else {
                    root = deleteNode(root, key);
                    cout << "\nAfter Deletion:\n";
                    printTree(root);
                }
                break;

            case 3:
                cout << "Enter value to search: ";
                cin >> key;
                cout << (searchNode(root, key) ? "Node Found!\n" : "Node Not Found!\n");
                break;

            case 4:
                cout << "\nCurrent Tree Structure:\n";
                printTree(root);
                break;

            case 5:
                cout << "Inorder Traversal: ";
                inorder(root);
                cout << "\n";
                break;

            case 0:
                cout << "Exiting program...\n";
                return 0;

            default:
                cout << "Invalid choice!\n";
        }
    }
}