#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int count;
};

// Initializes a trie node
struct TrieNode* createTrieNode() {
    struct TrieNode* node = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if (node) {
        node->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

// Inserts a word into the trie
void insert(struct TrieNode* root, char* word) {
    struct TrieNode* node = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (node->children[index] == NULL) {
            node->children[index] = createTrieNode();
        }
        node = node->children[index];
    }
    node->count++;
}

// Computes the number of occurrences of a word in the trie
int numberOfOccurrences(struct TrieNode* root, char* word) {
    struct TrieNode* node = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (node->children[index] == NULL) {
            return 0;  // Word not found
        }
        node = node->children[index];
    }
    return node->count;
}

// Deallocates memory for the trie
void deallocateTrie(struct TrieNode* root) {
    if (root == NULL) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i] != NULL) {
            deallocateTrie(root->children[i]);
        }
    }
    free(root);
}

// Reads the dictionary from a file and returns the number of words read
int readDictionary(char* filename, char** pInWords) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    int numWords = 0;
    char word[256];
    while (fscanf(file, "%s", word) != EOF) {
        pInWords[numWords] = strdup(word);
        numWords++;
    }
    fclose(file);
    return numWords;
}

int main(void) {
    char* inWords[256];
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct TrieNode* pTrie = createTrieNode();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    deallocateTrie(pTrie);

    return 0;
}
