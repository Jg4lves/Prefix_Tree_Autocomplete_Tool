// To compile: gcc-o main main.c
// To run: ./main input output

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TrieNode {
    struct TrieNode* children[26];
    int isEndOfWord;
} TrieNode;

TrieNode* createNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->isEndOfWord = 0;
    for (int i = 0; i < 26; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void insert(TrieNode* root, const char* word) {
    TrieNode* node = root;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!node->children[index]) {
            node->children[index] = createNode();
        }
        node = node->children[index];
    }
    node->isEndOfWord = 1;
}

void findSuggestions(TrieNode* root, const char* prefix, const char* currentWord, FILE* output, int* suggestionsFound) {
    if (root->isEndOfWord && strncmp(currentWord, prefix, 3) == 0) {
        if (*suggestionsFound > 0) {
            fprintf(output, ",");
        }
        fprintf(output, "%s", currentWord);
        (*suggestionsFound)++;
    }

    for (int i = 0; i < 26; i++) {
        if (root->children[i]) {
            char nextChar = 'a' + i;
            char nextWord[1000];
            strcpy(nextWord, currentWord);
            strncat(nextWord, &nextChar, 1);
            findSuggestions(root->children[i], prefix, nextWord, output, suggestionsFound);
        }
    }
}

int main(int argc, char* argv[]) {
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    TrieNode* root = createNode();

    int numTerms;
    fscanf(input, "%d", &numTerms);

    for (int i = 0; i < numTerms; i++) {
        char term[100];
        fscanf(input, "%s", term);
        insert(root, term);
    }

    int numRequests;
    fscanf(input, "%d", &numRequests);

    for (int i = 0; i < numRequests; i++) {
        char request[100];
        fscanf(input, "%s", request);

        fprintf(output, "%s:", request);
        char currentWord[1000] = "";
        int suggestionsFound = 0;
        findSuggestions(root, request, currentWord, output, &suggestionsFound);
        if (suggestionsFound == 0) {
            fprintf(output, "-");
        }
        fprintf(output, "\n");
    }

    fclose(input);
    fclose(output);
    return 0;
}