
//
// Created by Logan Foreman on 2/20/23.
// RedID: 825056655
//

#ifndef DICTIONARYTREE_H
#define DICTIONARYTREE_H

#include "shared.h"

struct SHARED_DATA;
class Dictionary;
class dictentry;

class dictentry {
public:
    static const int NCHILD = 30; // a-z, ', -, _, terminator of word \0
    bool visited = false;
    bool isWord; // true if this node represents the end of a word
    dictentry *next[NCHILD] = {}; // pointers to child nodes (one for each letter of the alphabet)
    bool add(const char *wordBeingInserted);
    dictentry* findEndingNodeOfAStr(const char *strBeingSearched);
    void countWordsStartingFromANode(int &count, dictentry *node);
//private:
  //  dictentry *root;
};
/*
typedef struct dictentry {
    static const int NCHILD = 30; // a-z, ', -, _, terminator of word \0
    //char letter; // the letter stored in this node
    bool isWord; // true if this node represents the end of a word
    bool visited = false;
    dictentry *next[NCHILD]; // pointers to child nodes (one for each letter of the alphabet)
} dictentry;

class Dictionary {
public:
    Dictionary();
    bool add(const char *wordBeingInserted);
    dictentry* findEndingNodeOfAStr(const char *strBeingSearched);
    void countWordsStartingFromANode(int &count, dictentry *node);
    //SHARED_DATA* sharedData;

private:
    dictentry *root;

    //SHARED_DATA* sharedData;
};
 */

#endif //ASSN1_DICTIONARY_H


//main class for dictNode
/*
class dictNode {
public:
    static const int NCHILD = 30; // a-z, ', -, _, terminator of word \0
    dictNode *next[NCHILD]; //this is the array that stores the indices of all our chars we using
    bool visited = false;
};

class Dictionary {
public:
    Dictionary();
    bool add(const char * wordBeingInserted = nullptr);
    dictNode* findEndingNodeOfAStr(const char *strBeingSearched);
    void countWordsStartingFromANode(int &count, dictNode *node);

    void loadFile(const char *filename);

private:
    dictNode *root; //we need this for instantiating / starting at root when we create the tree
};
 */


// ADD NODE METHOD SIGNATURE
//bool add(const char * wordBeingInserted = nullptr);

//Implementation tips:
//1. This method is initially called from the root node to insert the new
//word.
//2. Use recursive calls (or iterative loop) to traverse the tree structure
//to insert the word, one character at a time starting from the first
//character (you need to convert each character from the word to an
//index of a child node), until the \0 null terminator is inserted.



// SEARCH NODE METHOD SIGNATURE
//dictNode* findEndingNodeOfAStr(const char *strBeingSearched);

//Search node operation signature for finding the node (in the tree) that ends a
//prefix, i.e., the node that contains the last character of the prefix being searched:
//Implementation tips:
//1. This method is initially called on the root node to start the search.
//2. Use recursive calls (or iterative loop) to traverse the tree structure
//to find the string, one character at a time starting from the first
//character (you need to convert each character from the word to an
//index of a child node). Returns the node pointer that ends the
//string if found; otherwise, return NULL pointer.
//3. The “strBeingSearched” argument is for passing the string being
//searched.



// COUNT WORD OPERATION SIGNATURE
//void countWordsStartingFromANode(int &count);
// Implementation tips:
//1. This method is initially called on a starting node to start the
//counting.
//2. Starting from the dictnode, use recursive calls (or iterative loop) to
//traverse the whole sub-tree from the node (including the node) to
//count all words that start from the node. Use the count argument
//passed by reference to count.
//f. For searching the tree to count the words that start with a specific string or prefix:
//• First call findEndingNodeOfAStr to find node that ends the string or
//prefix,
//• Then call countWordsStartingFromANode to count the words starting
//from the node returned from findEndingNodeOfAStr

