//
// Created by logan foreman on 2/21/2023.
//
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
#include "dictionarytree.h"

using namespace std;

/*
 * thread entry point, argument must be pointer to a shared data structure
 * which contains the commmunication channel and thread parameters
 *
 * readPrefixToQueue thread is responsible for reading words/prefix from
 * a file (file name passed from the shared buffer); and putting each
 * prefix in a queue
 */




void *readprefix(void* voidPtr) {
    SHARED_DATA *sharedData = (SHARED_DATA *)voidPtr;

    while(!sharedData->isPopulated){}
    pthread_mutex_lock(&sharedData-> mutex_numThreadsRunning);
    sharedData->numThreadsRunning++;
    pthread_mutex_unlock(&sharedData-> mutex_numThreadsRunning);

    const char* separators = " \n\r!\"#$%&()*+,./0123456789:;<=>?@[\\]^`{|}~"; //add a '_'?

    const char* file = sharedData->filePath[TESTFILEINDEX];
    ifstream infile(file);
    string line;
    if (!infile.is_open()) {
        perror("Error: could not open file");//std::cerr << "Error: could not open file  " << file << std::endl;
        pthread_exit(NULL);
    }
    //std::cout << "\n\n About to start READPREFIX\n\n "; // << sharedData->wordCountInFile[TESTFILEINDEX]
    while (getline(infile, line))
    {

        char *str = new char[line.length() + 1];
        strcpy(str, line.c_str());

        char *token = strtok(str, separators);
        while (token != NULL)
        {
            string prefix(token);
            //cout << (prefix);
            pthread_mutex_lock(&sharedData->mutex_prefixQueue);
            sharedData->prefixQueue.push(prefix);
            pthread_cond_signal(&sharedData->cv_prefixQueue);
            pthread_mutex_unlock(&sharedData->mutex_prefixQueue);
            pthread_mutex_lock(&sharedData->mutex_wordCountInFile);
            sharedData->wordCountInFile[TESTFILEINDEX] += 1;
            //std::cout << "\n\n inside readPrefix. wordCountInFile =   " << sharedData->wordCountInFile[TESTFILEINDEX];
            //std::cout << std::endl;
            pthread_mutex_unlock(&sharedData->mutex_wordCountInFile);
            token = strtok(NULL, separators);
        }
        delete[] str;
    }


    pthread_mutex_lock(&sharedData-> mutex_numThreadsRunning);
    sharedData->numThreadsRunning--;
    pthread_mutex_unlock(&sharedData-> mutex_numThreadsRunning);
    //TODO
    pthread_mutex_lock(&sharedData->mutex_taskCompleted);
    sharedData->taskCompleted = true;
    pthread_mutex_unlock(&sharedData->mutex_taskCompleted);
    pthread_exit(NULL);
    return nullptr;
}