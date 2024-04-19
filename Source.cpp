#include <iostream>
#include <cstdlib> // This library helps us with generating random numbers
#include <ctime>   // We need this to seed the random generator based on the current time
#include <iomanip> // Helps in formatting the output so it looks nice
#include <chrono>  // Lets us measure time, which is super useful for seeing how fast our tree operates

struct Node {
    int data;         // Each node holds a single integer
    Node* left;       // Pointer to the left child, if it exists
    Node* right;      // Pointer to the right child, if it exists
    Node(int value) : data(value), left(nullptr), right(nullptr) {}
    // Constructor that initializes the node with the given value and no children
};

class BST {
public:
    bool verbose;     // I use this flag to decide when to show detailed messages

    // Constructor sets up an empty tree and defaults verbose to 'true'
    BST() : root(nullptr), verbose(true) {}
    ~BST() { destroyTree(root); } // Destructor takes care of cleaning up the whole tree

    // Adds a node with the specified value to the BST
    void add(int data) {
        root = insert(root, data);
    }

    // Removes a node with the specified value from the BST
    void remove(int data) {
        root = remove(root, data);
    }

    // Prints a visual representation of the tree
    void display() {
        std::cout << "BST Structure:\n";
        printTree(root);
    }

    // Returns the maximum value in the BST; useful for checking the upper bound
    int findMaximum() {
        return findMax(root);
    }

    // Manages performance testing by turning off verbose output temporarily
    void testPerformance() {
        verbose = false;  // Turn off detailed logging to speed up the test
        testPerformanceHelper(100);
        testPerformanceHelper(1000);
        testPerformanceHelper(10000);
        testPerformanceHelper(100000);
        verbose = true;   // Restore detailed logging after testing
    }

private:
    Node* root; // The root node of the tree

    // Recursive method to insert a new value into the subtree
    Node* insert(Node* node, int data) {
        if (!node) {
            if (verbose) std::cout << "Insert " << data << " here.\n";
            return new Node(data);
        }
        if (data < node->data) {
            if (verbose) std::cout << "Go left from " << node->data << "\n";
            node->left = insert(node->left, data);
        }
        else if (data > node->data) {
            if (verbose) std::cout << "Go right from " << node->data << "\n";
            node->right = insert(node->right, data);
        }
        return node;
    }

    // Recursive method to remove a value from the subtree
    Node* remove(Node* node, int data) {
        if (!node) return nullptr; // Nothing to do if node is not found
        if (data < node->data) {
            node->left = remove(node->left, data);
        }
        else if (data > node->data) {
            node->right = remove(node->right, data);
        }
        else {
            // Node with one child or no child
            if (!node->left) return node->right;
            else if (!node->right) return node->left;
            // Node with two children: Get the inorder successor
            Node* temp = findMin(node->right);
            node->data = temp->data;
            node->right = remove(node->right, temp->data);
        }
        return node;
    }

    // Finds the maximum value in the subtree
    int findMax(Node* node) {
        if (node == nullptr) return -1; // If the tree is empty, return an error code
        while (node->right != nullptr) {
            node = node->right;
        }
        return node->data;
    }

    // Helper method to find the minimum node in a subtree
    Node* findMin(Node* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    // Prints each node of the tree in a nicely formatted way using recursion
    void printTree(Node* node, int space = 0) {
        if (!node) return;
        space += 5; // Increase space between levels
        printTree(node->right, space); // Process the right child first
        std::cout << std::setw(space) << node->data << "\n"; // Print the current node's data
        printTree(node->left, space); // Process the left child
    }

    // Cleans up all nodes from the tree to prevent memory leaks
    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    // Internal method to add a bunch of numbers and time how long it takes
    void testPerformanceHelper(int numElements) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numElements; i++) {
            add(rand() % (numElements * 10));  // Add a bunch of random numbers
        }
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = finish - start;
        std::cout << "Added " << numElements << " elements in " << elapsed.count() << " ms\n";
    }
};

int main() {
    BST tree;
    srand(static_cast<unsigned>(time(nullptr))); // Seed the random number generator

    int choice, value;

    // Main menu loop for user interaction
    while (true) {
        std::cout << "\n1. Add Node\n2. Remove Node\n3. Display Tree\n4. Find Maximum\n5. Run Performance Test\n6. Exit\nEnter your choice: ";
        std::cin >> choice;
        switch (choice) {
        case 1:
            std::cout << "Enter value to add: ";
            std::cin >> value;
            tree.add(value);
            break;
        case 2:
            std::cout << "Enter value to remove: ";
            std::cin >> value;
            tree.remove(value);
            break;
        case 3:
            tree.display();
            break;
        case 4:
            std::cout << "Maximum value in BST: " << tree.findMaximum() << std::endl;
            break;
        case 5:
            tree.testPerformance();
            break;
        case 6:
            std::cout << "Exiting program.\n";
            return 0;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}
