

//this file will IMPLEMENT THE .h file, dictionary.h (#include dictionary.h)
#include "readprefix.h"
#include <queue>
#include <stdio.h>
#include <cstring>
#include "shared.h"
#include <string>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <cstring>
#include <string.h>
#include <iostream>
#include <fstream>
#include "dictionarytree.h"
#include <queue>
#include <cstring>
#include "dictionarytree.h"

#include "countprefix.h"


void* countprefix(void* ptr) {
    SHARED_DATA* sharedData = (SHARED_DATA*)ptr;//Dictionary* dict = sharedData-> dict;

    std::ofstream outputFile;
    outputFile.open("countprefix_output.txt");

    while(!sharedData->isPopulated){}
    pthread_mutex_lock(&sharedData-> mutex_numThreadsRunning);
    sharedData->numThreadsRunning++;
    pthread_mutex_unlock(&sharedData-> mutex_numThreadsRunning);
    while(!sharedData->taskCompleted){} //TODO
    long charCount = 0;
    //std::cout << "\n\n\nAbout to start COUNTPREFIX\n\n\n";
    while (sharedData->isPopulated &&(!sharedData->prefixQueue.empty() || sharedData -> numThreadsRunning >= 1 )) {// || sharedData->numThreadsRunning > 0

        pthread_mutex_lock(&sharedData->mutex_prefixQueue);
        //std::cout << "here2";

        //while (sharedData->prefixQueue.empty() && sharedData->numThreadsRunning >= 1) {
            //waiting
            //pthread_cond_wait(&sharedData->cv_prefixQueue, &sharedData->mutex_prefixQueue);
       // }/*
        if (sharedData->prefixQueue.empty()) {
            pthread_mutex_unlock(&sharedData->mutex_prefixQueue);
            break;
        }
        std::string prefix = sharedData->prefixQueue.front();
        charCount = prefix.length() + 1;
        sharedData->prefixQueue.pop();
        pthread_mutex_unlock(&sharedData->mutex_prefixQueue);
        pthread_mutex_lock(&sharedData->mutex_numOfProcessedPrefixes);
        sharedData->numOfProcessedPrefixes++;
        pthread_mutex_unlock(&sharedData->mutex_numOfProcessedPrefixes);
        pthread_mutex_lock(&sharedData->mutex_numOfCharsReadFromFile);
        sharedData->numOfCharsReadFromFile[TESTFILEINDEX]+=charCount;
        pthread_mutex_unlock(&sharedData->mutex_numOfCharsReadFromFile);
        //Dictionary dict;
        dictentry* node = (sharedData->dictRootNode->findEndingNodeOfAStr(prefix.c_str()));
        if (node != NULL) {
            int count = 0;
            sharedData->dictRootNode->countWordsStartingFromANode(count, node);
            //sharedData->totalPrefixes++;
            if (count >= sharedData->minNumOfWordsWithAPrefixForPrinting) {
                //std::cout << "here";
                outputFile << prefix << " " << count << std::endl;
                //printf("%s %d\n", prefix.c_str(), count);
            }
        }
    }
    //pthread_mutex_lock(&sharedData->mutex_wordCountInFile);
    //sharedData->wordCountInFile[TESTFILEINDEX] = sharedData->numOfProcessedPrefixes;
    //pthread_mutex_unlock(&sharedData->mutex_wordCountInFile);
    pthread_mutex_lock(&sharedData->mutex_numThreadsRunning);
    sharedData->numThreadsRunning--;
    pthread_mutex_unlock(&sharedData->mutex_numThreadsRunning);
    outputFile.close();
    //std::cout << "\nFinished counting prefixes\n";
    pthread_exit(NULL);
    return nullptr;
}

/*
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
*/
/*
int main(int argc, char **argv) {

    // VOCABULARY FILE READING METHOD
    //std::cout << "Beginning File Read and Adding Words to Tree" << std::endl;
    Dictionary dict;
    std::ifstream dictstream(argv[1]); // open file for parsing
    //std::ifstream dictstream("vocabulary.txt"); //for testing
    std::string line;
    const char *delimiters = "\n\r !\"#$%&()*+,./0123456789:;<=>?@[\\]^`{|}~";
    // iterate over dictionary file line by line
    while (std::getline(dictstream, line)) {
        char *word = strtok(const_cast<char *>(line.c_str()), delimiters);
        while (word != nullptr) {
            // call add method to insert word to build the dictionary tree
            dict.add(word);
            //std::cout << "here" << std::endl;
            // handle error from insertion result

            //read next word
            word = strtok(nullptr, delimiters);
        }
    }
    dictstream.close();

    //TESTFILE READING METHOD and FILE WRITING
    //std::cout << "Beginning Test File Read and Searching/Counting Word Prefixes" << std::endl;
    std::ifstream teststream(argv[2]); //open test file
    //std::ifstream teststream("testfile1.txt");
    //std::ofstream outputFile("outputPrefixes.txt");

    // iterate over test file by line
    while (std::getline(teststream, line)) {
        char *word = strtok(const_cast<char *>(line.c_str()), delimiters); //word with delimiters
        while (word != nullptr) {
            // search for word in dictionary tree
            //std::cout << word << std::endl;
            dictNode *node = dict.findEndingNodeOfAStr(word);
            //searches for the word in the tree we created, and fetches the ending node

            // if word is found, count and print the number of words starting from the node
            if (node) { //makes sure the node exists
                int count = 0;
                dict.countWordsStartingFromANode(count, node);
                //std::cout << count << std::endl;
                std::cout << word << " " << count << std::endl;
                //outputFile << word ;
                //outputFile << " " << count << std::endl;
            } else {
                std::cout << word << " 0" << std::endl;
            }
            // read next word
            word = strtok(nullptr, delimiters);
        }
    }
    teststream.close();
    //outputFile.close();


    return 0;

}
*/
