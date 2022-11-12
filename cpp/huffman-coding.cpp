// Credits: GeeksforGeeks

#include <iostream>
#include <cstdlib>
using namespace std;

// This constant can be avoided by explicitly calculating height of Huffman Tree.
#define MAX_TREE 100

// Huffman Tree Node
struct MinHeapNode
{
	// One of the input characters.
	char data;
	unsigned frequency;
	struct MinHeapNode *left;
	struct MinHeapNode *right;
};

struct MinHeap
{
	unsigned size;
	unsigned capacity;
	struct MinHeapNode** array;
};

// A utility function to allocate a new min heap node with given character and its frequency.
struct MinHeapNode* newNode(char data, unsigned freq)
{
	struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
	
	temp->left = temp->right = NULL;
	temp->data = data;
	temp->frequency = freq;
	
	return temp;
}

// A utility function to create a min heap of given capacity.
struct MinHeap* createMinHeap(unsigned capacity)
{
	struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
	
	// Current size is 0.
	minHeap->size = 0;
	
	minHeap->capacity = capacity;
	
	minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
	
	return minHeap;
}

// A utility function to swap two min heap nodes.
void swapMinHeapNodes(struct MinHeapNode** a, struct MinHeapNode** b)
{
	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

// The standard minHeapify function.
void minHeapify(struct MinHeap* minHeap, int idx)
{
	int smallest = idx;
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;
	
	if (left < minHeap->size && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency) 
	{
		smallest = left;
	}
	
	if (right < minHeap->size && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency)
	{
		smallest = right;
	}
	
	if (smallest != idx)
	{
		swapMinHeapNodes(&minHeap->array[smallest], &minHeap->array[idx]);
		minHeapify(minHeap, smallest);
	}
}

int isSizeOne(struct MinHeap* minHeap)
{
	return (minHeap->size == 1);
}

// A standard function to extract minimum value node from the heap.
struct MinHeapNode* extractMin(struct  MinHeap* minHeap)
{
	struct MinHeapNode* temp = minHeap->array[0];
	minHeap->array[0] = minHeap->array[minHeap->size - 1];
	
	--minHeap->size;
	
	minHeapify(minHeap, 0);
	
	return temp;
}

// A utility function to insert a new node to min heap.
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode)
{
	++minHeap->size;
	int i = minHeap->size - 1;
	
	while (i && minHeapNode->frequency < minHeap->array[(i - 1) / 2]->frequency)
	{
		minHeap->array[i] = minHeap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}
	
	minHeap->array[i] = minHeapNode;
}

// A standard function to build min heap.
void buildMinHeap(struct MinHeap* minHeap)
{
	int n = minHeap->size - 1;
	int i;
	
	for (i = (n - 1) / 2; i >= 0; --i)
	{
		minHeapify(minHeap, i);
	}
}

// A utility function to print an array of size n.
void printArr(int arr[], int n)
{
	int i;
	for (i = 0; i < n; ++i)
	{
		cout<< arr[i];
	}
	
	cout<<"\n";
}

// A utility function to check if this node is leaf.
int isLeaf(struct MinHeapNode* root)
{
	return !(root->left) && !(root->right);
}

// Creates a min heap of capacity equal to size and inserts all characters of data[] in min heap.
// Initially sets the size equal to the given capacity.
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size)
{
	struct MinHeap* minHeap = createMinHeap(size);
	
	for (int i = 0; i < size; ++i)
	{
		minHeap->array[i] = newNode(data[i], freq[i]);
	}
	
	minHeap->size = size;
	buildMinHeap(minHeap);
	
	return minHeap;
}

// The main function that builds the Huffman Tree.
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size)
{
	struct MinHeapNode *left, *right, *top;
	
	// Step 1: Create a main heap of capacity equal to size.
	struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);
	
	// Iterate while the heap has more than one nodes.
	while (!isSizeOne(minHeap))
	{
		// Step 2: Extract the two minimum frequency items from min heap.
		left = extractMin(minHeap);
		right = extractMin(minHeap);
		
		// Step 3: Create a new internal node with frequency equal to sum of the two nodes' frequencies.
		// And bind the extracted nodes as childs of this new node.
		// (We'll use '$' as a special character for our internal nodes.)
		top = newNode('$', left->frequency + right->frequency);
		
		top->left = left;
		top->right = right;
		
		insertMinHeap(minHeap, top);
	}
	
	// Step 4: The remaining node is the root and the tree is complete.
	return extractMin(minHeap);
}

// Prints Huffman Codes from the root of Huffman Tree. Uses arr[] to store codes.
void printCodes(struct MinHeapNode* root, int arr[], int top)
{
	// Assign 0 to left edge and recur.
	if (root->left)
	{
		arr[top] = 0;
		printCodes(root->left, arr, top + 1);
	}
	
	// Assign 1 to right edge and recur.
	if (root->right)
	{
		arr[top] = 1;
		printCodes(root->right, arr, top + 1);
	}
	
	// If the node is a leaf, then it contains one of the input characters.
	// Print the character and its code from arr[].
	if (isLeaf(root))
	{
		if (root->data == '\n')
		{
			cout<< "\\n" <<": ";
		}
		else
		{
			cout<< root->data <<": ";
		}
		
		printArr(arr, top);
	}
}

// The main function that builds a Huffman Tree, traverses the entire tree and prints the codes.
void huffmanCodes(char data[], int freq[], int size)
{
	// Construct Huffman Tree.
	struct MinHeapNode* root = buildHuffmanTree(data, freq, size);
	
	// Print Huffman Codes using the tree.
	int arr[MAX_TREE],  top = 0;
	
	printCodes(root, arr, top);
}

int main()
{
	char arr[] = { 'a', 'e', 'i', 's', 't', 'p', '\n' };
	int freq[] = { 10, 15, 12, 3, 4, 13, 1 };
	
	int size = sizeof(arr) / sizeof(arr[0]);
	
	huffmanCodes(arr, freq, size);
	
	return 0;
}
