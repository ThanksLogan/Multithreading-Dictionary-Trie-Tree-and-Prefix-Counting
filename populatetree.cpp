//
// Created by forem on 2/21/2023.
//
#include "populatetree.h"
#include "shared.h"
#include <queue>
#include <stdio.h>
#include <cstring>
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
#include <sstream>
#include <thread>

using namespace std;
void* populatetree(void* ptr) {
    // INITIALIZE SHARED DATA
    SHARED_DATA* sharedData = (SHARED_DATA *)ptr; //Dictionary* dict = sharedData->dict;

    // INCREASE NUMBER OF THREADS RUNNING ( for DOCUMENTATION)
    pthread_mutex_lock(&sharedData-> mutex_numThreadsRunning);
    sharedData->numThreadsRunning++;
    pthread_mutex_unlock(&sharedData-> mutex_numThreadsRunning);


    // Open the vocabulary file
    const char* file = sharedData->filePath[DICTSRCFILEINDEX];
    std::ifstream infile(file, std::ios_base::binary);
    if (!infile.is_open()) {
        perror("Error: could not open file");
        //std::cerr << "Error: could not open file  " << file << std::endl;
        pthread_exit(NULL);
    }
    /*
    // Determine the line endings in the file
    std::string lineEnding;
    std::getline(infile, lineEnding, '\n');
    if (lineEnding.back() == '\r') {
        lineEnding = "\r\n"; // Windows-style CRLF line endings
    } else {
        lineEnding = "\n"; // Unix-style LF line endings
    }
     */
    // Set universal newline mode
    infile.unsetf(std::ios::skipws);
    infile >> std::noskipws;

    // Loop through each word in the file and add it to the dict tree
    long charCount = 0;
    std::string line;
    while (std::getline(infile, line, '\n')) {
        std::istringstream stream(line);
        std::string word;
        charCount = line.length() + 1; //word.length + 1
        while (std::getline(stream, word, ' ')) { //stream >> word
            pthread_mutex_lock(&sharedData->mutex_numOfCharsReadFromFile);
            sharedData->numOfCharsReadFromFile[DICTSRCFILEINDEX]+=charCount;
            pthread_mutex_unlock(&sharedData->mutex_numOfCharsReadFromFile);
            sharedData->dictRootNode->add(word.c_str());
            sharedData->numWordsProcessed++;
        }
        //this_thread::sleep_for(chrono::nanoseconds(1));
    }

    pthread_mutex_lock(&sharedData->mutex_wordCountInFile);
    sharedData->wordCountInFile[DICTSRCFILEINDEX] = sharedData->numWordsProcessed;
    pthread_mutex_unlock(&sharedData->mutex_wordCountInFile);
    // Close the file and decrement the number of running threads
    infile.close();
    //pthread_cond_signal(&sharedData->populated);

    // SIGNAL THAT TREE IS DONE BEING POPULATED
    pthread_mutex_lock(&sharedData-> mutex_numThreadsRunning);
    sharedData->numThreadsRunning--;
    pthread_mutex_unlock(&sharedData-> mutex_numThreadsRunning);
    //this_thread::sleep_for(chrono::milliseconds(10000));
    pthread_mutex_lock(&sharedData->mutex_populated);
    sharedData->isPopulated = true;
    pthread_mutex_unlock(&sharedData->mutex_populated);

    pthread_exit(NULL);
    return nullptr;
}
