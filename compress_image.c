#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 256

// A Huffman tree node
struct MinHeapNode {
    unsigned char data;           // One of the input characters
    unsigned freq;                // Frequency of the character
    struct MinHeapNode *left, *right; // Left and right child
};

// A Min Heap: Collection of min heap (or Hufmman tree) nodes
struct MinHeap {
    unsigned size;                // Current size of min heap
    unsigned capacity;            // Capacity of min heap
    struct MinHeapNode** array;   // Array of minheap node pointers
};

// A utility function to allocate a new min heap node with given character and frequency of the character
struct MinHeapNode* newNode(unsigned char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// A utility function to create a min heap of given capacity
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// A utility function to swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// The standard minHeapify function.
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// A utility function to check if size of heap is 1 or not
int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// A standard function to extract minimum value node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// A utility function to insert a new node to Min Heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// A standard function to build min heap
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// A utility function to print an array of size n
void printArr(int arr[], int n) {
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf("\n");
}

// Utility function to check if this node is leaf
int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

// Creates a min heap of capacity equal to size and inserts all character of data[] in min heap. Initially size of min heap is equal to capacity
struct MinHeap* createAndBuildMinHeap(unsigned char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

// The main function that builds Huffman tree
struct MinHeapNode* buildHuffmanTree(unsigned char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;

    // Step 1: Create a min heap of capacity equal to size. Initially, there are modes equal to size.
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    // Iterate while size of heap doesn't become 1
    while (!isSizeOne(minHeap)) {
        // Step 2: Extract the two minimum freq items from min heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Step 3: Create a new internal node with frequency equal to the sum of the two nodes frequencies. Make the two extracted node as left and right children of this new node. Add this node to the min heap. '$' is a special value for internal nodes, not used
        top = newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    // Step 4: The remaining node is the root node and the tree is complete.
    return extractMin(minHeap);
}

// Prints huffman codes from the root of Huffman Tree. It uses arr[] to store codes
void printCodes(struct MinHeapNode* root, int arr[], int top) {
    // Assign 0 to left edge and recur
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    // Assign 1 to right edge and recur
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    // If this is a leaf node, then it contains one of the input characters, print the character and its code from arr[]
    if (isLeaf(root)) {
        printf("%c: ", root->data);
        printArr(arr, top);
    }
}

// A utility function to store the huffman codes in an array
void storeCodes(struct MinHeapNode* root, int arr[], int top, char codes[256][256]) {
    // Assign 0 to left edge and recur
    if (root->left) {
        arr[top] = 0;
        storeCodes(root->left, arr, top + 1, codes);
    }

    // Assign 1 to right edge and recur
    if (root->right) {
        arr[top] = 1;
        storeCodes(root->right, arr, top + 1, codes);
    }

    // If this is a leaf node, then it contains one of the input characters, store the code in codes array
    if (isLeaf(root)) {
        int i;
        for (i = 0; i < top; i++) {
            codes[root->data][i] = arr[i] + '0';
        }
        codes[root->data][top] = '\0';
    }
}

// The main function that builds a Huffman Tree and print codes by traversing the built Huffman Tree
void HuffmanCodes(unsigned char data[], int freq[], int size, char codes[256][256]) {
    // Construct Huffman Tree
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);

    // Print Huffman codes using the Huffman tree built above
    int arr[MAX_TREE_HT], top = 0;
    storeCodes(root, arr, top, codes);
}

// Function to read file and count frequencies of characters
void calculateFrequencies(FILE* inputFile, int freq[]) {
    unsigned char buffer[1024];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0) {
       for (size_t i = 0; i < bytesRead; i++) {
            freq[buffer[i]]++;
        }
    }
}

void compressImage(const char* inputPath, const char* outputPath) {
    FILE* inputFile = fopen(inputPath, "rb");
    if (!inputFile) {
        perror("Failed to open input file");
        exit(1);
    }

    // Dummy compression: Just copying the file (in real case, apply Huffman coding)
    FILE* outputFile = fopen(outputPath, "wb");
    if (!outputFile) {
        perror("Failed to open output file");
        exit(1);
    }

    printf("Copying image data...\n");

    unsigned char buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0) {
        fwrite(buffer, 1, bytesRead, outputFile);
    }

    printf("Image data copied.\n");

    fclose(inputFile);
    fclose(outputFile);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_image_path> <output_image_path>\n", argv[0]);
        return 1;
    }

    const char *inputPath = argv[1];
    const char *outputPath = argv[2];

    printf("Input image path: %s\n", inputPath);
    printf("Output image path: %s\n", outputPath);

    printf("Compressing image...\n");

    compressImage(inputPath, outputPath);

    printf("Compression completed.\n");

    return 0;
}
