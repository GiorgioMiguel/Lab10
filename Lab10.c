/*
COP 3502 Lab 10 "Tries"
© 2024 Giorgio Torregrosa
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

struct Trie {
    int count;
    struct Trie* children[ALPHABET_SIZE];
};

struct Trie* createTrie() {
    struct Trie* node = (struct Trie*)malloc(sizeof(struct Trie));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void insert(struct Trie* pTrie, char* word) {
    if (!word || *word == '\0') return; // Early return for empty string
    struct Trie* current = pTrie;
    while (*word) {
        if (!islower(*word)) {
            word++; // Skip non-lowercase characters
            continue;
        }
        int index = *word - 'a';
        if (index < 0 || index >= ALPHABET_SIZE) {
            word++; // Skip any invalid characters
            continue;
        }
        if (!current->children[index]) {
            current->children[index] = createTrie();
        }
        current = current->children[index];
        word++;
    }
    current->count++;
}


int numberOfOccurrences(struct Trie* pTrie, char* word) {
    if (!word || *word == '\0') return 0; // Early return for empty string
    struct Trie* current = pTrie;
    while (*word) {
        int index = *word - 'a';
        if (!current->children[index]) {
            return 0;
        }
        current = current->children[index];
        word++;
    }
    return current->count;
}

void deallocateTrie(struct Trie* pTrie) {
    if (!pTrie) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (pTrie->children[i]) {
            deallocateTrie(pTrie->children[i]);
        }
    }
    free(pTrie);
}

int readDictionary(char* filename, char** pInWords) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Unable to open file\n");
        return 0;
    }

    char buffer[100];
    int wordCount = 0;
    while (fgets(buffer, sizeof(buffer), file) && wordCount < 256) { // Prevent buffer overflow
        buffer[strcspn(buffer, "\n")] = 0;
        pInWords[wordCount] = (char*)malloc(strlen(buffer) + 1);
        if (!pInWords[wordCount]) {
            fprintf(stderr, "Memory allocation failed\n");
            fclose(file);
            return 0;
        }
        strcpy(pInWords[wordCount], buffer);
        wordCount++;
    }
    fclose(file);
    return wordCount;
}

int main(void) {
    char* inWords[256];

    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }
    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    char* pWords[] = { "notaword", "ucf", "no", "note", "corg" };
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    deallocateTrie(pTrie);

    for (int i = 0; i < numWords; ++i) {
        free(inWords[i]);
    }

    return 0;
}
