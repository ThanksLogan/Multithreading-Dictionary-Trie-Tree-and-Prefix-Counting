//
// Created by forem on 2/21/2023.
//
//
// Created by Logan Foreman on 1/24/23.
//REDID: 825056655
//

#include <cstring>
#include <string.h>
#include "dictionarytree.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <queue>
#include "shared.h"

// dictionary.cpp
//Dictionary::Dictionary() {
/*
dictentry(){
    root = new dictentry;
    // Initialize all pointers in the next array to NULL
    for (int i = 0; i < 29; i++) {
        root->next[i] = nullptr;
    }
    root->visited = false;
}
 */
int getCharIndex(char c) {
    switch (c) {
        case '\'':
            return 26;
        case '-':
            return 27;
        case '_':
            return 28;
        default:
            return tolower(c) - 'a';
    }
}
bool dictentry::add(const char *wordBeingInserted) {
    // Check if the wordBeingInserted is already present
    if (wordBeingInserted == nullptr) {
        return false;
    }

    // Code to add the node to the data structure
    //must start at root node, add a letter by creating a new node using the index of the created array
    dictentry *current = this;
    for (int i = 0; i <= strlen(wordBeingInserted); i++) {
        int index;
        if (i == strlen(wordBeingInserted)){
            current->isWord = true;
            index = 29; // this is terminator index
        } else {
            index = getCharIndex(wordBeingInserted[i]);
    }// index corresponds to the letter in next[]

        if (!current->next[index]) { //if we're not at the root, new node needs to be created for next letter
            current->next[index] = new dictentry(); //instantiates with the class dictNode for a leaf node
        }
        current = current->next[index]; //assigns new node to a letter (based on index array)
        /*
        else if (current->next[index] == nullptr) {
            // If the child node doesn't exist, create a new one
            current->next[index] = new dictentry;
            current->next[index]->letter = wordBeingInserted[i];
            for (int j = 0; j < 26; j++) {
                current->next[index]->next[j] = nullptr;
            }
            current->next[index]->isWord = false;
        }
        current = current->next[index];
         */
    }
    return true;
}

dictentry* dictentry::findEndingNodeOfAStr(const char *strBeingSearched) {
    dictentry *current = this;
    for (int i = 0; strBeingSearched[i] != '\0'; i++) {
        int index; //= tolower(strBeingSearched[i]) - 'a';
        index = getCharIndex(strBeingSearched[i]);
        if (!current->next[index]) {
            return nullptr;
        }
        current = current->next[index];
    }
    return current;
}

void dictentry::countWordsStartingFromANode(int &count, dictentry *node) {
    /*
     * //node denoted as visited to keep track of recurring tree parsing
    node -> visited = true;
    if (node->next[29]) { //index 29 is the null terminator \0 //!node->next[29]
        count++; //increment count once we reach a new word from the given prefix
    }

    //loop traverses a through _, excluding null terminator since we are only looking for valid leaf nodes
    //if a leaf node is no longer found (i=29), the for loop ends and we have found all words
    for (int i = 0; i < 29; i++) { //since there are 29 letters, we have these many children possibilities
        if (node->next[i] != nullptr ) { //
            countWordsStartingFromANode(count, node->next[i]);
            //^ this recursively calls our method with the next node and count as parameter
        }
    }
     */
    node -> visited = true;
    if (node->next[29]) {
        count++;//increment count once we reach a new word from the given prefix
    }

    //loop traverses a through _, excluding null terminator since we are only looking for valid leaf nodes
    //if a leaf node is no longer found (i=29), the for loop ends and we have found all words
    for (int i = 0; i < 29; i++) {
        if (node->next[i] != nullptr) {
            countWordsStartingFromANode(count, node->next[i]);
        }
    }
}
/*
Dictionary::Dictionary() {
    root = new dictNode;
    // Initialize all pointers in the next array to NULL
    for (int i = 0; i < 29; i++) {
        root->next[i] = nullptr;
    }
}

bool Dictionary::add(const char * wordBeingInserted) {
    // Check if the wordBeingInserted is already present
    if (wordBeingInserted == nullptr) {
        return false;
    }

    //Code to add the node to the data structure
    //must start at root node, add a letter by creating a new node using the index of the created array
    dictNode *current = root; //denotes pointer to root, where we must begin
    //letters a-z are index 0-25, and specials are 26-28. terminator /0 is 29.
    int index; //takes track of index in letter array index next[]
    for (int i = 0; i <= strlen(wordBeingInserted); i++) {
        if (wordBeingInserted[i] == '\'') {
            index = 26; // '
        }
        else if (wordBeingInserted[i] == '-') {
            index = 27; // -
        }
        else if (wordBeingInserted[i] == '_') {
            index = 28; // _
        }
        else {
            index = tolower(wordBeingInserted[i]) - 'a';//this assigns a letter to an index based on ASCII
        }
        if (i == strlen(wordBeingInserted)){
            //if we're at the end of the given word from vocab, we add a
            // terminator char node to denote the end
            index = 29; // this is terminator index
        }
        if (!current->next[index]) { //if we're not at the root, new node needs to be created for next letter
            current->next[index] = new dictNode(); //instantiates with the class dictNode for a leaf node
        }
        current = current->next[index]; //assigns new node to a letter (based on index array)
    }
    return true;
}
dictNode* Dictionary::findEndingNodeOfAStr(const char *strBeingSearched) {
    dictNode* current = root; //node to keep track of where we are in the tree
    int index; //corresponds to next[]
    for (int i = 0; strBeingSearched[i] != '\0'; i++) { //goes down until a null terminator is found
        if (strBeingSearched[i] == '\'') {
            index = 26; //index for '
        }
        else if (strBeingSearched[i] == '-') {
            index = 27; //index for -
        }
        else if (strBeingSearched[i] == '_') {
            index = 28; // index for _
        }
        else {
            index = tolower(strBeingSearched[i]) - 'a'; //uses ASCII value of a to find index of alphabet letter
        }
        if (!current->next[index]) {
            return nullptr;
        }
        current = current->next[index];
        // current is a dictNode, where at the end right before returning it points to an index
    }
    return current; //returns of type dictNode
}

void Dictionary::countWordsStartingFromANode(int &count, dictNode *node) {

    //node denoted as visited to keep track of recurring tree parsing
    node -> visited = true;
    if (node->next[29]) { //index 29 is the null terminator \0 //!node->next[29]
        count++; //increment count once we reach a new word from the given prefix
    }

    //loop traverses a through _, excluding null terminator since we are only looking for valid leaf nodes
    //if a leaf node is no longer found (i=29), the for loop ends and we have found all words
    for (int i = 0; i < 29; i++) { //since there are 29 letters, we have these many children possibilities
        if (node->next[i] != nullptr ) { //
            countWordsStartingFromANode(count, node->next[i]);
            //^ this recursively calls our method with the next node and count as parameter
        }
    }
}

void Dictionary::loadFile(const char *filename) {
    std::ifstream inputFile(filename);

}
*/




