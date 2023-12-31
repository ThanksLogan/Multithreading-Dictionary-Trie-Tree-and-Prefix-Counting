//Author: Logan Foreman on 2/20/2023
//REDID: 825056655

#include <stdio.h>
#include <pthread.h>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include "shared.h"
#include "populatetree.h"
#include <cstring>
#include "readprefix.h"
#include "countprefix.h"
#include "dictionarytree.h"
#include <sys/types.h>
#include <sys/stat.h>


using namespace std;

int main(int argc, char **argv)
{
    setbuf(stdout, NULL);
    bool verbose = true;  // chatty or quiet program
    int option;   /* command line switch */
    /*
    int p; //progress marks
    int h; //hash marks
    int n; //numprefixes to show*/
    int count = 100;
    const char *file1 = "";  // input file name
    const char *file2 = "";  // input file name
    const char *outputFile = "countprefix_output.txt";  // output file name

    // use getopt to process command-line arguments

    // shared data to be used for communication between threads
    SHARED_DATA sharedData;
    sharedData.dictRootNode = new dictentry();
    //sharedData.dict = new Dictionary();

    pthread_cond_init(&sharedData.cv_prefixQueue, NULL);
    //sharedData.dictRootNode = new dictentry();
    sharedData.prefixQueue = std::queue<std::string>();
    pthread_cond_init(&sharedData.populated, NULL);
    pthread_mutex_init(&sharedData.mutex_populated, NULL);
    //TODO
    pthread_mutex_init(&sharedData.mutex_taskCompleted, NULL);
    pthread_mutex_init(&sharedData.mutex_numThreadsRunning, NULL);
    pthread_mutex_init(&sharedData.mutex_prefixCount, NULL);
    pthread_mutex_init(&sharedData.mutex_prefixQueue, NULL);
    pthread_mutex_init(&sharedData.mutex_numOfCharsReadFromFile, NULL);
    pthread_mutex_init(&sharedData.mutex_wordCountInFile, NULL);
    pthread_mutex_init(&sharedData.mutex_dictRootNode, NULL);
    pthread_mutex_init(&sharedData.mutex_prefixesFound, NULL);
    pthread_mutex_init(&sharedData.mutex_numWordsProcessed, NULL);
    //pthread_mutex_t mutex_numThreadsRunning = PTHREAD_MUTEX_INITIALIZER;
    //pthread_mutex_t mutex_dictRootNode = PTHREAD_MUTEX_INITIALIZER;
    //pthread_mutex_t mutex_numOfCharsReadFromFile = PTHREAD_MUTEX_INITIALIZER;
    //pthread_mutex_t mutex_wordCountInFile = PTHREAD_MUTEX_INITIALIZER;
    //pthread_mutex_t mutex_numOfProcessedPrefixes = PTHREAD_MUTEX_INITIALIZER;
    //pthread_mutex_t mutex_prefixQueue = PTHREAD_MUTEX_INITIALIZER;
    //pthread_mutex_t mutex_prefixCount = PTHREAD_MUTEX_INITIALIZER;

    sharedData.numOfProgressMarks = DEFAULT_NUMOF_MARKS;
    sharedData.hashmarkInterval = DEFAULT_HASHMARKINTERVAL;
    sharedData.minNumOfWordsWithAPrefixForPrinting = DEFAULT_MINNUM_OFWORDS_WITHAPREFIX;

    struct stat sb;

    while ( (option = getopt(argc, argv, "p:h:n:")) != -1) {
        switch (option) {
            case 'p': // progress marks optional argument
                // process p
                sharedData.numOfProgressMarks = atoi(optarg );
                if (sharedData.numOfProgressMarks < 10 || sharedData.numOfProgressMarks >50){
                    cout << "Number of progress marks must be a number and at least 10 and less than or equal to 50" << endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 'h':
                // process h
                sharedData.hashmarkInterval = atoi(optarg );
                if (sharedData.hashmarkInterval <= 0 || sharedData.hashmarkInterval > 10){
                    cout << "Hash mark interval for progress must be a number, greater than 0, and  \n"
                            "less than or equal to 10" << endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 'n':
                // process n
                sharedData.minNumOfWordsWithAPrefixForPrinting = atoi(optarg);
                if (sharedData.minNumOfWordsWithAPrefixForPrinting<1){
                    cout << "[-n arg] Must be greater than 0"<<endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case '?':
                cout << "Unrecognized argument: -" << optopt;
                exit(EXIT_FAILURE);

            case ':':
                cout << "Argument -" << optopt << " needs a value" << endl;
                break;
            default:
                cerr << "Usage: " << argv[0] << "vocabulary.txt testfile1.txt [-p pMarks] [-h hMarks] [-n numPrefixes] \n";
                exit(1);
        }
    }
    // Assign default values if optional arguments are not provided

    int idx = optind;
    // If no input file is provided, print the usage message and exit
    if (optind >= argc) {
        cerr << "Usage: " << argv[0] << "vocabulary.txt testfile1.txt [-n progressMarks] [-h hashMarks] [-n numPrefixes\n";
        exit(EXIT_FAILURE);
    }
    /* If idx < argc, there are mandatory arguments to process */
    if (idx < argc) {
        /* Process first positional argument as file path for sharedData.filePath[0] */
        file1 = argv[idx];
        idx++;

        /* Check if there is a second positional argument */
        if (idx < argc) {
            /* Process second positional argument as file path for sharedData.filePath[1] */
            file2 = argv[idx];
            idx++;
            //outputFile = argv[idx + 3];
        }
        else {
            /* If there is no second positional argument, print error message and exit */
            cerr << "Error: missing second file path argument\n";
            exit(EXIT_FAILURE);
        }/*
        if (idx < argc){
            const char* n = "-n";
            if(strcmp(argv[idx], n) == 0){
                idx++;
                sscanf(argv[idx], "%d", &count);
                idx++;
            }
        }
        if (idx < argc){
            const char* o = "-o";
            if(strcmp(argv[idx], o) == 0){
                idx++;
                outputFile = argv[idx];
            }
            */
    }

    // Set the input file path in sharedData
    sharedData.filePath[DICTSRCFILEINDEX] = file1;
    sharedData.filePath[TESTFILEINDEX] = file2;

    ifstream vFile (file1, std::ifstream::in);
    if (vFile.is_open()){
        if (stat(file1,&sb) == 0){
            sharedData.totalNumOfCharsInFile[DICTSRCFILEINDEX] = sb.st_size;
        }
        else {
            cout <<"bad system call for " << file1;
            ::exit(EXIT_FAILURE);
        }
    }
    else if (!vFile.is_open()){
        cout << "Unable to open <<" << file1 << ">>";
        ::exit(EXIT_FAILURE);
    }
    vFile.close();
    ifstream tFile (file2, std::ifstream::in);
    if (tFile.is_open()){
        if (stat(file2,&sb) == 0){
            sharedData.totalNumOfCharsInFile[TESTFILEINDEX] = sb.st_size;
        }
        else {
            cout <<"bad system call for " << file2;
            ::exit(EXIT_FAILURE);
        }
    }
    else if (!tFile.is_open()){
        cout << "Unable to open <<" << file2 << ">>";
        ::exit(EXIT_FAILURE);
    }
    tFile.close();

    // initialize sharedData attributes / variables
    sharedData.isPopulated = false;
    //TODO
    sharedData.taskCompleted = false;
    sharedData.verbose = verbose;
    sharedData.count = count;
    sharedData.wordCountInFile[DICTSRCFILEINDEX] = 0;
    sharedData.wordCountInFile[TESTFILEINDEX] = 0;
    sharedData.numWordsProcessed = 0;
    sharedData.totalPrefixes = 0;
    sharedData.prefixesFound = 0;
    sharedData.numThreadsRunning = 0;
    sharedData.numOfProcessedPrefixes = 0;


    // create and start populatetree thread
    pthread_t populateTreeThread;
    pthread_t readPrefixThread;
    pthread_t countPrefixThread;


    if (pthread_create(&populateTreeThread, NULL, &populatetree, &sharedData)) {
        cerr << "Error: failed to create thread for populatetree" << endl;
        exit(-1);
    }

    // create and start readprefix thread
    if (pthread_create(&readPrefixThread, NULL, &readprefix, &sharedData)) {
        cerr << "Error: failed to create thread for readprefix" << endl;
        exit(-1);
    }

    // create and start countprefix thread
    if (pthread_create(&countPrefixThread, NULL, &countprefix, &sharedData)) {
        cerr << "Error: failed to create thread for countprefix" << endl;
        exit(-1);
    }

    //cout << "There are " << sharedData.numWordsProcessed << " words in " << file1 << ".\n";

    // Print progress bar
   // printProgressBar(sharedData.numOfProgressMarks, sharedData.hashmarkInterval, sharedData.numWordsProcessed, sharedData.numW);
    int progressBarWidth = sharedData.numOfProgressMarks;
    int hashes = sharedData.hashmarkInterval;
    while (sharedData.numOfCharsReadFromFile[DICTSRCFILEINDEX] == sharedData.totalNumOfCharsInFile[DICTSRCFILEINDEX]){
        //this_thread::sleep_for(chrono::milliseconds(1));
    }
    bool newLetter = false;
    int progress = 0;
    int temp = 0;
    int totalMarks = 0;
    // !sharedData.isPopulated
    while (!sharedData.isPopulated) {//sharedData.numOfCharsReadFromFile[DICTSRCFILEINDEX] != sharedData.totalNumOfCharsInFile[DICTSRCFILEINDEX]
        /*
            double percent = (double) sharedData.numOfCharsReadFromFile[DICTSRCFILEINDEX] /
                    (double) sharedData.totalNumOfCharsInFile[DICTSRCFILEINDEX];

            double interval = 1.0 / (double) sharedData.numOfProgressMarks;
            //cout << "\n\n out here \n\n";
            while (totalMarks * interval < percent) {
                totalMarks++;
                if(totalMarks % sharedData.hashmarkInterval == 0){
                    cout << "#" << flush;
                }
                else {
                    cout << "-" << flush;
                }
                this_thread::sleep_for(chrono::nanoseconds(1));
                cout.flush();
            }

        }
         */
    }
    while(totalMarks < sharedData.numOfProgressMarks){
        totalMarks++;
        if (totalMarks % sharedData.hashmarkInterval ==0){
            cout << "#";
        }
        else{
            cout << "-";
        }
        cout.flush();
    }
    cout << endl;
    cout << "There are " << sharedData.wordCountInFile[DICTSRCFILEINDEX] << " words in " << file1 << ".\n";

    if (pthread_join(populateTreeThread, NULL)) {
        if (errno != ESRCH) {
            cerr << "Error: failed to join thread for populatetree: " << strerror(errno) << endl;
            exit(-1);
        }
    }



    //cout << "There are " << sharedData.numWordsProcessed << " words in " << file1 << ".\n";

    //cout << "There are " << sharedData.prefixesFound << " words in " << file2 << ".\n";
    /*
    this_thread::sleep_for(chrono::seconds(2));
    cout << "There are " << sharedData.numWordsProcessed << " words in " << file1 << ".\n";

    this_thread::sleep_for(chrono::seconds(2));
    cout << "There are " << sharedData.prefixesFound << " words in " << file2 << ".\n";

    this_thread::sleep_for(chrono::seconds(2));
    cout << "There are " << sharedData.prefixesFound << " words in " << file2 << ".\n";
    this_thread::sleep_for(chrono::seconds(2));
    cout << "There are " << sharedData.prefixesFound << " words in " << file2 << ".\n";
    this_thread::sleep_for(chrono::seconds(2));
    cout << "There are " << sharedData.prefixesFound << " words in " << file2 << ".\n";
    this_thread::sleep_for(chrono::seconds(2));
    cout << "There are " << sharedData.prefixesFound << " words in " << file2 << ".\n";
    this_thread::sleep_for(chrono::seconds(2));
    cout << "There are " << sharedData.prefixesFound << " words in " << file2 << ".\n";
    this_thread::sleep_for(chrono::seconds(2));
    */

    if (pthread_join(readPrefixThread, NULL)) {
        if (errno != ESRCH) {
            cerr << "Error: failed to join thread for readprefix: " << strerror(errno) << endl;
            exit(-1);
        }
    }
    //print progress bar for testfile1.txt
    totalMarks = 0;

    //while(sharedData.numOfCharsReadFromFile[TESTFILEINDEX] != sharedData.totalNumOfCharsInFile[TESTFILEINDEX]){
    //while(!sharedData.taskCompleted){}
    //while (sharedData.numOfCharsReadFromFile[TESTFILEINDEX] == sharedData.totalNumOfCharsInFile[TESTFILEINDEX]){
    //    this_thread::sleep_for(chrono::milliseconds(1));
    //}
    while (!sharedData.isPopulated){
    }

    while(sharedData.isPopulated && sharedData.numOfProcessedPrefixes < sharedData.wordCountInFile[TESTFILEINDEX]){
        /*
        double percent = (double ) sharedData.numOfProcessedPrefixes /
                         (double) sharedData.wordCountInFile[TESTFILEINDEX];

        double interval = 1.0 / (double) sharedData.numOfProgressMarks;
        //cout << sharedData.numOfProcessedPrefixes << endl;
        while (totalMarks * interval < percent) {
            totalMarks++;
            if(totalMarks % sharedData.hashmarkInterval == 0){
                cout << "#" << flush;
            }
            else {
                cout << "-" << flush;
            }
            this_thread::sleep_for(chrono::nanoseconds(1));
            cout.flush();
        }
        //cout << "hello";
        */
    }

    while(totalMarks < sharedData.numOfProgressMarks){
        totalMarks++;
        if (totalMarks % sharedData.hashmarkInterval ==0){
            cout << "#" << flush;
        }
        else{
            cout << "-" << flush;
        }
        cout.flush();
    }
    cout << endl;
    cout << "There are " << sharedData.numOfProcessedPrefixes << " words in " << file2 << ".\n";
    if (pthread_join(countPrefixThread, NULL)) {
        if (errno != ESRCH) {
            cerr << "Error: failed to join thread for countprefix: " << strerror(errno) << endl;
            exit(-1);
        }
    }
    //cout << " Should end by here ";

}



// output results
//cout << "Dictionary tree populated with " << sharedData.numWordsProcessed << " words." << endl;


