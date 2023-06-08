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
void preorder(struct Node* root, FILE* output_txt);
void freeTree(struct Node* root);

struct Node* createNode(int label) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->label = label;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct Node* insert(struct Node* root, int label) {
    // If the tree is empty, create a new node and return it as the root
    if (root == NULL)
        return createNode(label);

    // Splay the tree to bring the closest node to the root
    root = splay(root, label);

    // If the label is already present, return the root
    if (root->label == label)
        return root;

    // Create a new node
    struct Node* newNode = createNode(label);

    // If the label is less than the root's label, make the new node the left child
    // and the root's left child the left child of the new node
    if (label < root->label) {
        newNode->right = root;
        newNode->left = root->left;
        root->left = NULL;
    }
        // If the label is greater than the root's label, make the new node the right child
        // and the root's right child the right child of the new node
    else {
        newNode->left = root;
        newNode->right = root->right;
        root->right = NULL;
    }

    // Set the new node as the root
    return newNode;
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

struct Node *splay(struct Node *root, int label) {
    // root is NULL or label is present at root
    if (root == NULL || root->label == label)
        return root;

    // label is in left subtree
    if (root->label > label) {
        // label is not in tree
        if (root->left == NULL)
            return root;

        // Zig-Zig
        if (root->left->label > label) {
            // First recursively bring the label as root of left-left
            root->left->left = splay(root->left->left, label);

            // Do first rotation for root
            root = zig_rotate(root);
        }
            // Zig-Zag
        else if (root->left->label < label){
            // First recursively bring the label as root of left-right
            root->left->right = splay(root->left->right, label);

            // Do first rotation for root->left
            if (root->left->right != NULL)
                root->left = zag_rotate(root->left);
        }

        // Do second rotation for root
        return (root->left == NULL) ? root : zig_rotate(root);
    }
        // label is in right subtree
    else {
        // label is not in tree
        if (root->right == NULL)
            return root;

        // Zag-Zig
        if (root->right->label > label) {
            // Bring the label as root of right-left
            root->right->left = splay(root->right->left, label);

            // Do first rotation for root->right
            if (root->right->left != NULL)
                root->right = zig_rotate(root->right);
        }
            // Zag-Zag
        else if (root->right->label < label) {
            // Bring the label as root of right-right and do first rotation
            root->right->right = splay(root->right->right, label);
            root = zag_rotate(root);
        }

        // Do second rotation for root
        return (root->right == NULL) ? root : zag_rotate(root);
    }
}

struct Node* search(struct Node* root, int label) {
    root = splay(root, label);
    if (root->label != label)
        printf("Node %d does not exist\n", label);
    return root;
}

void preorder(struct Node* root, FILE* output_txt) {
    if (root != NULL) {
        printf(" %d", root->label);
        fprintf(output_txt, " %d", root->label);

        if (root->left != NULL) {
            printf(" -");
            fprintf(output_txt, " -");
            preorder(root->left, output_txt);
        }

        if (root->right != NULL) {
            printf(" -");
            fprintf(output_txt, " -");
            preorder(root->right, output_txt);
        }
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
    char labelsFileName[50], viewedFileName[50];
    FILE* output_txt;
    output_txt = fopen("output.txt", "w");
    if (output_txt == NULL) {
        printf("Failed to open output file.\n");
        return 1;
    }

    fprintf(output_txt, "Input file for label numbers:");
    printf("Input file for label numbers:");
    scanf("%s", labelsFileName);
    fprintf(output_txt, "\n");
    printf("\n");

    FILE* inputFile = fopen(labelsFileName, "r");
    if (inputFile == NULL) {
        printf("Failed to open the input file.\n");
        return 1;
    }

    struct Node* root = NULL;
    int label;

    while (fscanf(inputFile, "%d", &label) == 1) {
        root = insert(root, label);
        printf("Tree:");
        fprintf(output_txt, "Tree: ");
        preorder(root, output_txt);
        printf("\n");
        fprintf(output_txt, "\n");
    }

    fclose(inputFile);


    printf("Input file for viewed products:");
    fprintf(output_txt, "Input file for viewed products:");
    scanf("%s", viewedFileName);
    fprintf(output_txt, "\n");
    printf("\n");

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

        preorder(root, output_txt);
        printf("\n");
        fprintf(output_txt, "\n");
    }

    fclose(viewedFile);

    freeTree(root);

    return 0;
}
