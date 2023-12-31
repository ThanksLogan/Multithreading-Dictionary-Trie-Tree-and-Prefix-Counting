//
// Created by forem on 2/22/2023.
//

#ifndef SHARED_H
#define SHARED_H
#include <string>
#include <queue>
#include <pthread.h>
#include "dictionarytree.h"

#define NUMOFFILES 2
#define DICTSRCFILEINDEX 0
#define TESTFILEINDEX 1
/* default number of progress marks for representing 100% progress */
#define DEFAULT_NUMOF_MARKS 50

/* minimum number of progress marks for representing 100% progress */
#define MIN_NUMOF_MARKS 10

/* place hash marks in the progress bar every N marks */
#define DEFAULT_HASHMARKINTERVAL  10

/* default minimum number of dictionary words starting from a prefix for printing or writing to
the output */
#define DEFAULT_MINNUM_OFWORDS_WITHAPREFIX  1
/*
typedef struct dictentry {
    std::string word;
    long count;
    struct dictentry *next;
    void findEndingNodeOfAStr(const char *string) {}
} dictentry;
*/
//struct ThreadData;
struct dictentry; // forward declaration
//struct Dictionary;

typedef struct SHARED_DATA{
    //Dictionary *dict;
    dictentry *dictRootNode;
    int numOfProgressMarks;
    int hashmarkInterval;
    int minNumOfWordsWithAPrefixForPrinting;
    const char *filePath[NUMOFFILES];
    long totalNumOfCharsInFile[NUMOFFILES];
    long numOfCharsReadFromFile[NUMOFFILES];
    long wordCountInFile[NUMOFFILES];
    long numOfProcessedPrefixes;
    bool isPopulated;
    bool taskCompleted;
    std::queue<std::string> prefixQueue;
    pthread_cond_t cv_prefixQueue;
    pthread_cond_t populated;
    pthread_mutex_t mutex_populated;
    pthread_mutex_t mutex_dictRootNode;
    pthread_mutex_t mutex_numOfCharsReadFromFile;
    pthread_mutex_t mutex_wordCountInFile;
    pthread_mutex_t mutex_prefixesFound;
    pthread_mutex_t mutex_numOfProcessedPrefixes;
    pthread_mutex_t mutex_numWordsProcessed;
    //TODO
    pthread_mutex_t mutex_taskCompleted;
    pthread_mutex_t mutex_prefixQueue;
    pthread_mutex_t mutex_prefixCount;
    int totalPrefixes;
    int prefixesFound;
    pthread_mutex_t mutex_numThreadsRunning;
    int numThreadsRunning;
    int numWordsProcessed;
    int numWordsTotal;
    int count;
    bool verbose;
} SHARED_DATA;
#endif //A2_SHARED_H
