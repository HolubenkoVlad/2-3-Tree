#include "stdio.h"

#define MAX_INDEX 2

typedef struct Tree {
	struct Node* root;
}Tree;

typedef struct Node {
	int size;
	int key[2];
	struct Node* nodes[3];
	struct Node* parent;
}Node;

struct Tree* create_tree() {
	struct Tree* tree = (struct Tree*)malloc(sizeof(Tree));
	tree->root = NULL;
	return tree;
}

struct Node* create_node(Node* parent_node, int number) {
	Node* node = (Node*)malloc(sizeof(Node));
	node->parent = parent_node;
	node->size = 1;
	node->key[node->size - 1] = number;
	return node;
}

void add(Tree*, int);

void search_node(Tree*, Node*, int);

void split_node(Tree*, Node*, Node*, int);

int minmax(Node*, int);

int search(Node*, int);

void print_tree(Tree*);

int main() {
	struct Tree* tree = create_tree();
	int arr[12] = { 15,16,17,14,20,9,24,7,18,50,19, 76 };
	for (int i = 0; i < 12; i++)
		add(tree, arr[i]);
	//print_tree(tree);
	return 0;
}

void print_tree(Tree* tree) {
	Node* current_node=tree->root;
	int index_node = 0;
	for (int i = 0;; i++) {
		printf("level: %d \t", i);
		while (1) {	
			if (current_node->nodes[0] == current_node->nodes[1] && current_node->parent->nodes[current_node->parent->size] == current_node)
				break;
			for (int j = 0; j < current_node->size; j++) {
				printf("|%d|", current_node->key[j]);
			}
			if (current_node->parent == NULL) {
				current_node = current_node->nodes[index_node++];
				printf("\n");
				break;
			}
			if (current_node->parent->nodes[current_node->parent->size] == current_node) {
				index_node = 0;
				current_node = current_node->parent->nodes[index_node]->nodes[index_node];
				printf("\n");
				break;
			}
			else {
				printf("\t");
				current_node = current_node->parent->nodes[index_node++];
			}
		}
	}
}

int minmax(Node* node, int number) {
	if (number < node->key[0])
		return node->key[0];
	else if (number < node->key[1])
		return number;
	else return node->key[1];
}

void split_node(Tree* tree, Node* left, Node* right, int number) {
	if (left->parent == NULL && left == tree->root)
	{
		Node* new_node = create_node(left->parent, minmax(left, number));
		if (left->nodes[0] != left->nodes[1] && right->nodes[0] != right->nodes[1])
			for (int i = 0; i < MAX_INDEX; i++) {
				left->nodes[i]->parent = left;
				right->nodes[i]->parent = right;
			}
		left->parent = new_node;
		right->parent = new_node;
		new_node->nodes[0] = left;
		new_node->nodes[1] = right;
		tree->root = new_node;
		return;
	}
	if ((left->parent)->size == MAX_INDEX) {
		Node* new_parent;
		for (int i = 0; i <= left->parent->size; i++) {
			if (left->parent->nodes[i] == left) {
				if (i == 0) {
					new_parent = create_node((left->parent)->parent, left->parent->key[left->parent->size - 1]);
					for (int k = i; k < MAX_INDEX; k++)
						new_parent->nodes[k] = left->parent->nodes[k + 1];
					left->parent->size--;
					left->parent->key[i] = number;
					left->parent->nodes[i + 1] = right;
					split_node(tree, left->parent, new_parent, left->parent->key[left->parent->size]);
				}
				else if (i == 1) {
					new_parent = create_node((left->parent)->parent, left->parent->key[left->parent->size - 1]);
					new_parent->nodes[0] = right;
					new_parent->nodes[1] = left->parent->nodes[left->parent->size];
					left->parent->size--;
					left->parent->nodes[i] = left;
					split_node(tree, left->parent, new_parent, number);
				}
				else {
					left->parent->size--;
					new_parent = create_node((left->parent)->parent, number);
					new_parent->nodes[0] = left;
					new_parent->nodes[1] = right;
					split_node(tree, left->parent, new_parent, left->parent->key[left->parent->size]);
				}
			}
		}
	}
	else {
		if (left->parent->key[left->parent->size-1] < number)
			left->parent->key[left->parent->size] = number;
		else {
			left->parent->key[left->parent->size] = left->parent->key[left->parent->size - 1];
			left->parent->key[left->parent->size-1] = number;
		}
		left->parent->size++;
		for (int i = 0; i <= left->parent->size; i++) {
			if (left->parent->nodes[i] == left||left->parent->nodes[i]==right) {
				if (i == 0)
					left->parent->nodes[MAX_INDEX] = left->parent->nodes[MAX_INDEX - 1];
				left->parent->nodes[i] = left;
				left->parent->nodes[i + 1] = right;
				return;
			}
		}
	}
}

void choose_split_block(Tree* tree, Node* node, int number) {
	if (number < node->key[0]) {
		int temp_number = node->key[0];
		node->key[0] = node->key[1];
		node->size--;
		split_node(tree, create_node(node->parent, number), node, temp_number);
	}
	else if (number < node->key[1]) {
		node->size--;
		split_node(tree, node, create_node(node->parent, node->key[1]), number);
	}
	else {
		node->size--;
		split_node(tree, node, create_node(node->parent, number), node->key[1]);
	}
}

void search_node(Tree* tree, Node* current_node, int number) {
	if (current_node->nodes[0] == current_node->nodes[1])
		if (current_node->size == MAX_INDEX) {
			choose_split_block(tree, current_node, number);
			return;
		}
		else {
			current_node->size++;
			if (current_node->key[current_node->size - 2] < number)
				current_node->key[current_node->size - 1] = number;
			else {
				current_node->key[current_node->size - 1] = current_node->key[current_node->size - 2];
				current_node->key[current_node->size - 2] = number;
			}
			return;
		}
	for (int i = 0; i < current_node->size; i++) {
		if (number < current_node->key[i]) {
			search_node(tree, current_node->nodes[i], number);
			break;
		}
		else if (i == current_node->size - 1) {
			search_node(tree, current_node->nodes[i + 1], number);
			break;
		}
	}
}

void add(Tree* tree, int number) {
	if (tree->root == NULL) {
		tree->root = create_node(tree->root, number);
	}
	else search_node(tree, tree->root, number);
}

int search(Node* current_node, int number) {
	while (current_node->nodes[0]!=current_node->nodes[1]) {
		for (int i = 0; i < current_node->size; i++) {
			if (number == current_node->key[i])
				return 1;
			if (number < current_node->key[i]) {
				current_node = current_node->nodes[i];
				break;
			}
			else if (i == current_node->size - 1) {
				current_node = current_node->nodes[i+1];
				break;
			}
		}
	}
	return 0;
}