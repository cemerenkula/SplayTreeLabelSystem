#include <stdio.h>
#include <stdlib.h>

struct Node {
    int label;
    struct Node* left;
    struct Node* right;
};

struct Node* createNode(int label);
struct Node* insert(struct Node* root, int label);
struct Node* search(struct Node* root, int label);
struct Node* splay(struct Node* root, int label);
struct Node* zag_rotate(struct Node* x);
struct Node* zig_rotate(struct Node* x);
void preorder(struct Node* root);
void freeTree(struct Node* root);

struct Node* createNode(int label) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->label = label;
    newNode->left = newNode->right = NULL;
    return newNode;
}

struct Node* insert(struct Node* root, int label) {
    if (root == NULL)
        return createNode(label);

    root = splay(root, label);

    if (label < root->label) {
        struct Node* newNode = createNode(label);
        newNode->left = root->left;
        root->left = NULL;
        newNode->right = root;
        return newNode;
    }
    else if (label > root->label) {
        struct Node* newNode = createNode(label);
        newNode->right = root->right;
        root->right = NULL;
        newNode->left = root;
        return newNode;
    }
    else {
        return root;
    }
}




struct Node* zag_rotate(struct Node* x) {
    struct Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

struct Node* zig_rotate(struct Node* x) {
    struct Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

struct Node* splay(struct Node* root, int label) {
    if (root == NULL || root->label == label)
        return root;

    if (label < root->label) {
        if (root->left == NULL)
            return root;

        if (label < root->left->label) {
            root->left->left = splay(root->left->left, label);
            if (root->left->left != NULL)
                root = zig_rotate(root);
        }
        else if (label > root->left->label) {
            root->left->right = splay(root->left->right, label);
            if (root->left->right != NULL)
                root->left = zag_rotate(root->left);
        }

        return (root->left == NULL) ? root : zig_rotate(root);
    }
    else {
        if (root->right == NULL)
            return root;

        if (label < root->right->label) {
            root->right->left = splay(root->right->left, label);
            if (root->right->left != NULL)
                root->right = zig_rotate(root->right);
        }
        else if (label > root->right->label) {
            root->right->right = splay(root->right->right, label);
            if (root->right->right != NULL)
                root = zag_rotate(root);
        }

        return (root->right == NULL) ? root : zag_rotate(root);
    }
}

struct Node* search(struct Node* root, int label) {
    return splay(root, label);
}

void preorder(struct Node* root) {
    if (root != NULL) {
        printf("%d ", root->label);
        preorder(root->left);
        preorder(root->right);
    }
}

void freeTree(struct Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

int main() {
    char inputFileName[100], viewedFileName[100], outputFileName[100];
    printf("Enter the name of the input file containing the label numbers of the products: ");
    scanf("%s", inputFileName);

    FILE* inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("Failed to open the input file.\n");
        return 1;
    }

    struct Node* root = NULL;
    int label;

    while (fscanf(inputFile, "%d", &label) == 1) {
        root = insert(root, label);
        preorder(root);
        printf("\n");
        // Save preorder traversal to a txt file
    }

    fclose(inputFile);

    preorder(root);
    printf("\n");
    // Save final preorder traversal to a txt file

    printf("Enter the name of the input file containing the label numbers viewed by customers: ");
    scanf("%s", viewedFileName);

    FILE* viewedFile = fopen(viewedFileName, "r");
    if (viewedFile == NULL) {
        printf("Failed to open the viewed file.\n");
        freeTree(root);
        return 1;
    }

    while (fscanf(viewedFile, "%d", &label) == 1) {
        struct Node* result = search(root, label);
        if (result == NULL)
            printf("Node with label %d does not exist in the tree.\n", label);
        else
            root = result;

        preorder(root);
        printf("\n");
        // Save preorder traversal to a txt file
    }

    fclose(viewedFile);

    preorder(root);
    printf("\n");
    // Save final preorder traversal to a txt file

    freeTree(root);

    return 0;
}
