
//OSMAN BERK AN 28849

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "BinarySearchTree.h"
#include "HashTable.h"
#include "Sorting.h"
#include <thread>
#include <chrono>
#include <algorithm>


using namespace std;

//reading files and there is new rule : Every non-alphabetical character is considered as a separator (different than HW2).
//This rule applies for both the text in input files and queried words.
//For example; "face2face" = "face.face" = "face:facE" = "face face"


int findPlaceInVec(vector<WordItem> vec, WordItem item){
    for(int i=0; i<vec.size(); i++){
        if(vec[i] == item){
            return i;
        }
    }
    return -1;
}

//creating bst  and hashtable and vector for binary search //same function from hw3
void CreateTreeAndHashtable( BinarySearchTree<WordItem> &myTree, HashTable<WordItem> &myHashTable, const string &filename) {  //processing file by file and insert the unique words when they have read first time. else, increase the count in that file

    //also in that function I am creating my vector for binary search

    ifstream inputFile(filename);

    if (!inputFile.is_open()) {  //checking file have opened
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string cleanedWord;
    char ch;
    while (inputFile.get(ch)) {
        if (isalpha(ch)) {
            cleanedWord += tolower(ch);
        }
        else {  // if it is non alpha char, then take the cleanedWord as the word need to inserted
            if (!cleanedWord.empty()) {    //if there is a word

                const WordItem &ItemInHash = myHashTable.find(WordItem{cleanedWord, {}});
                if(ItemInHash.word != cleanedWord) {
                    DocumentItem docItem;
                    docItem.documentName = filename;
                    docItem.count = 1;

                    vector<DocumentItem> info;
                    info.push_back(docItem);  //make a vector
                    // for ex: WordItem word1{"apple", {{"doc1", 3}, {"doc2", 2}}};
                    WordItem wordItem{cleanedWord, info};
                    wordItem.word = cleanedWord;

                    //myTree.insert(wordItem);
                    myHashTable.insert(wordItem);


                }
                else{ // word is in Hashtable before
                    vector<DocumentItem> &details = const_cast<WordItem &>(ItemInHash).details;  // make vector for WordItem's document details

                    bool documentFound = false;
                    for (DocumentItem &doc: details) {
                        if (doc.documentName == filename) {    //in same document, so increase the count for that doc
                            // Document found, update count
                            doc.count++;
                            documentFound = true;

                        }
                    }
                    if (!documentFound) {   // word is in tree as a string but in another filename, so make new docItem and set its filename and count
                        // Document not found, insert a new DocumentItem
                        DocumentItem docItem;
                        docItem.documentName = filename;
                        docItem.count = 1;
                        details.push_back(docItem);
                    }

                }
                //HASHTABLE finished bot not last word

                //then make same thing for Binary Search Tree
                const WordItem &foundItem = myTree.find(WordItem{cleanedWord, {}}); // find WordItem with cleaned word in tree
                if (foundItem.word != cleanedWord) {   // if there is not that word before in tree
                    // Word not found, insert a new node
                    DocumentItem docItem;
                    docItem.documentName = filename;
                    docItem.count = 1;

                    vector<DocumentItem> info;
                    info.push_back(docItem);  //make a vector
                    // for ex: WordItem word1{"apple", {{"doc1", 3}, {"doc2", 2}}};
                    WordItem wordItem{cleanedWord, info};
                    wordItem.word = cleanedWord;

                    myTree.insert(wordItem);
                    //myHashTable.insert(wordItem);  // insert word into hashtable also if that word is not in tree before



                    //allWords.push_back(wordItem);

                } else {   // if that word is in hashtable before  (as a string), then check for is it in the same document or not.

                    //otherwise the word is in tree and (table of course) then check the doc count etc...
                    vector<DocumentItem> &details = const_cast<WordItem &>(foundItem).details;  // make vector for WordItem's document details

                    bool documentFound = false;
                    for (DocumentItem &doc: details) {
                        if (doc.documentName == filename) {    //in same document, so increase the count for that doc
                            // Document found, update count
                            doc.count++;
                            documentFound = true;

                        }
                    }

                    if (!documentFound) {   // word is in tree as a string but in another filename, so make new docItem and set its filename and count
                        // Document not found, insert a new DocumentItem
                        DocumentItem docItem;
                        docItem.documentName = filename;
                        docItem.count = 1;
                        details.push_back(docItem);
                    }

                }





                cleanedWord.clear();  //After add to tree and table and vector , need to clear the cleaned word

            }
        }

    }
    //all file read and Hashtable, Tree created but,
    //need to read last word of document also (I cannot read it because it never came to non alpha char and so there is a cleanedWord in hand now)

    if (!cleanedWord.empty()) {
        const WordItem &ItemInHash = myHashTable.find(WordItem{cleanedWord, {}});
        if(ItemInHash.word != cleanedWord) {
            DocumentItem docItem;
            docItem.documentName = filename;
            docItem.count = 1;

            vector<DocumentItem> info;
            info.push_back(docItem);  //make a vector
            // for ex: WordItem word1{"apple", {{"doc1", 3}, {"doc2", 2}}};
            WordItem wordItem{cleanedWord, info};
            wordItem.word = cleanedWord;

            //myTree.insert(wordItem);
            myHashTable.insert(wordItem);


        }
        else{ // word is in Hashtable before
            vector<DocumentItem> &details = const_cast<WordItem &>(ItemInHash).details;  // make vector for WordItem's document details

            bool documentFound = false;
            for (DocumentItem &doc: details) {
                if (doc.documentName == filename) {    //in same document, so increase the count for that doc
                    // Document found, update count
                    doc.count++;
                    documentFound = true;

                }
            }
            if (!documentFound) {   // word is in tree as a string but in another filename, so make new docItem and set its filename and count
                // Document not found, insert a new DocumentItem
                DocumentItem docItem;
                docItem.documentName = filename;
                docItem.count = 1;
                details.push_back(docItem);
            }

        }
        //Hashtable with last word of document finished

        //and then here, insert last word into tree
        const WordItem &foundItem = myTree.find(WordItem{cleanedWord, {}});

        if (foundItem.word != cleanedWord) {
            // Word not found, insert a new node
            DocumentItem docItem;
            // ... (initialize docItem as needed)

            vector<DocumentItem> info;
            info.push_back(docItem);

            WordItem wordItem{cleanedWord, info};
            wordItem.word = cleanedWord;


            myTree.insert(wordItem);
            //myHashTable.insert(wordItem);
        } else {
            //otherwise the word is in tree and (table of course) then check the doc count etc...
            vector<DocumentItem> &
            details = const_cast<WordItem &>(foundItem).details;  // make vector for WordItem's document details

            bool documentFound = false;
            for (DocumentItem &doc: details) {
                if (doc.documentName == filename) {    //in same document, so increase the count for that doc
                    // Document found, update count
                    doc.count++;
                    documentFound = true;

                }
            }

            if (!documentFound) {   // word is in tree as a string but in another filename, so make new docItem and set its filename and count
                // Document not found, insert a new DocumentItem
                DocumentItem docItem;
                docItem.documentName = filename;
                docItem.count = 1;
                details.push_back(docItem);
            }
        }

    }
    // both hashtable and tree and vector finished


    cleanedWord.clear();
    inputFile.close(); //close the input file

}

//Binary search function that returns the index of searched item.
int BinarySearch(vector<WordItem> & words, int p, int r, WordItem & toBeSearched) {
    if (p <= r) {
        int mid = (p + r)/2;
        if (words[mid].word == toBeSearched.word)
            return mid ;
        if (words[mid].word > toBeSearched.word)
            return BinarySearch(words, p, mid-1, toBeSearched);
        if (words[mid].word < toBeSearched.word)
            return BinarySearch(words, mid+1, r, toBeSearched);
    }
    return -1;
}

void CreateVectorForBS(vector<WordItem> &myVec, const string & filename){
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {  //checking file have opened
        cerr << "Error opening file: " << filename << endl;
        return;
    }


    string cleanedWord;
    char ch;
    while (inputFile.get(ch)) {
        if (isalpha(ch)) {
            cleanedWord += tolower(ch);
        } else {  // if it is non alpha char, then take the cleanedWord as the word need to inserted
            if (!cleanedWord.empty()) {    //if there is a word
                WordItem wordItem{cleanedWord, {}};
                wordItem.word = cleanedWord;
                int index = findPlaceInVec(myVec,wordItem);

                if(index == -1){ // not exist in vector
                    DocumentItem docItem;
                    docItem.documentName = filename;
                    docItem.count = 1;
                    wordItem.details.push_back(docItem);
                    myVec.push_back(wordItem);
                }
                else{  // item is in vector before
                    //check its doc
                    vector<DocumentItem> &details = myVec[index].details;

                    bool documentFound = false;
                    for (DocumentItem &doc: details) {
                        if (doc.documentName == filename) {    //in same document, so increase the count for that doc
                            // Document found, update count
                            doc.count++;
                            documentFound = true;

                        }
                    }

                    if (!documentFound) {   // word is in tree as a string but in another filename, so make new docItem and set its filename and count
                        // Document not found, insert a new DocumentItem
                        DocumentItem docItem;
                        docItem.documentName = filename;
                        docItem.count = 1;
                        details.push_back(docItem);
                    }

                }
                cleanedWord.clear();

            }
        }

    }

    //add the last word into vec
    if (!cleanedWord.empty()) {    //if there is a word
        WordItem wordItem{cleanedWord, {}};
        wordItem.word = cleanedWord;
        int index = findPlaceInVec(myVec,wordItem);

        if(index == -1){ // not exist in vector
            DocumentItem docItem;
            docItem.documentName = filename;
            docItem.count = 1;
            wordItem.details.push_back(docItem);
            myVec.push_back(wordItem);
        }
        else{  // item is in vector before
            //check its doc
            vector<DocumentItem> &details = myVec[index].details;
            bool documentFound = false;
            for (DocumentItem &doc: details) {
                if (doc.documentName == filename) {    //in same document, so increase the count for that doc
                    // Document found, update count
                    doc.count++;
                    documentFound = true;

                }
            }

            if (!documentFound) {   // word is in tree as a string but in another filename, so make new docItem and set its filename and count
                // Document not found, insert a new DocumentItem
                DocumentItem docItem;
                docItem.documentName = filename;
                docItem.count = 1;
                details.push_back(docItem);
            }

        }
        cleanedWord.clear();
    }

}

void BSQuery(vector<WordItem> & myVector, const vector<string> & allFiles, const string & query, int outputCountBS){

    string currentWord;
    int wordCount = 0;
    vector<string> words;   // store query words
    for (char c: query) {
        if (isalpha(c)) {
            currentWord += tolower(c);
        } else {  // if non alpha char, use it as a separator for collect querying words.
            if (!currentWord.empty()) {
                words.push_back(currentWord);
                wordCount++;
                currentWord.clear();
            }
        }

    }

    // Add the last word of the query, where will not a non alpha char, so we need to add manually after the query finished
    if (!currentWord.empty()) {
        words.push_back(currentWord);
        wordCount++;
    }
    // I created words with query words
    //continue...
    //CHECKS NO DOCUMENT FIND CASE IN HERE
    bool allWordsFound = true; // if it is false, then output like No document found.
    for (const string &queryWord: words) {  // query words from words vector
        WordItem queryWordItem{queryWord, {}};  // my WordItem includes query word string
        queryWordItem.word = queryWord;

        //const WordItem &foundWord = myHashTable.find(queryWordItem);
        int index = BinarySearch(myVector, 0, myVector.size()-1, queryWordItem);
        if (index == -1) { // word is not in vector
            allWordsFound = false;

        }
        if(!allWordsFound && outputCountBS == 1){
            cout << "No document contains the given query" << endl;
            return;
        }
    }

    // if one of the words in query not in vector with any document case finished


    bool foundInAnyDocument = false;

    for (const string &fileName: allFiles) {  // I stored all files that I need to for query use in main, because outputting is document by document

        bool foundInCurrentDocument = false;

        int count = 0; // for checking that all words are exists in that document or not

        string outputString; // I will cout my output with that for that Document

        for (int i = 0; i < words.size(); i++) {  // check the words in query
            const string &queryWord = words[i]; // go word by word in query for documents
            WordItem queryWordItem;
            queryWordItem.word = queryWord;

            //const WordItem &foundWord = myHashTable.find(queryWordItem);  // find the WordItem with same string word
            int ind = BinarySearch(myVector,0,myVector.size()-1,queryWordItem);

            if(ind != -1){
                for (const DocumentItem &doc: myVector[ind].details) {  // WordItem's docItem vector

                    if (doc.documentName == fileName) { // if it is same as query files
                        if (!foundInCurrentDocument) {  // just output the documents that contains the words of query

                            outputString = "in Document " + fileName;
                        }

                        count++;  //word can found in that element
                        outputString += ", " + queryWord + " found " + to_string(doc.count) + " times";


                        foundInCurrentDocument = true;
                        break;
                    }

                }

            }
        }
        if (foundInCurrentDocument) {
            // after check all words in query for that document, put the dot.
            outputString += ".";
            foundInAnyDocument = true;
        }
        if (count == words.size() && outputCountBS == 1) {  // if all words can be found at that document, put outputCount for only output one time
            cout << outputString;

            cout << endl;

        }
    }

    // if the word is not found at least one of the in written files
    if (!foundInAnyDocument && outputCountBS == 1) {
        cout << "No document contains the given query" << endl;
        return;
    }

}

void hashQuery(HashTable<WordItem> &myHashTable, const vector<string> & allFiles, const string& query, int outputCountHash) { //search query for hashtable

    //just in case paste remove case from hw2, same
    if (query.substr(0, 6) == "REMOVE") {    // if query is like REMOVE word
        string deleteWord = query.substr(7);  // will deleted word string is that

        string lowerDeleteWord;
        for (char c: deleteWord) {
            if (isalpha(c)) {
                lowerDeleteWord += tolower(c);  // make it case-insensitive
            }
        }

        WordItem deleteWordItem;
        deleteWordItem.word = lowerDeleteWord;


        const WordItem &willDelete = myHashTable.find(deleteWordItem);  // find that wordItem with the word in query.

        myHashTable.remove(willDelete);  //remove it from my tree
        if(outputCountHash == 1){
            cout << lowerDeleteWord << " has been REMOVED" << endl;
        }

        return; //function finished, return to main and ask an input again

        //not sure that I need to checked REMOVE or not

    }else {
        //SEARCHING CASE

        //example query : Enter queried words in one line: a the-has been   ,   A:

        string currentWord;
        int wordCount = 0;
        vector<string> words;   // store query words
        for (char c: query) {
            if (isalpha(c)) {
                currentWord += tolower(c);
            } else {  // if non alpha char, use it as a separator for collect querying words.
                if (!currentWord.empty()) {
                    words.push_back(currentWord);
                    wordCount++;
                    currentWord.clear();
                }
            }

        }

        // Add the last word of the query, where will not a non alpha char, so we need to add manually after the query finished
        if (!currentWord.empty()) {
            words.push_back(currentWord);
            wordCount++;
        }


        //CHECKS NO DOCUMENT FIND CASE IN HERE
        bool allWordsFound = true; // if it is false, then output like No document found.
        for (const string &queryWord: words) {  // query words from words vector
            WordItem queryWordItem{queryWord, {}};  // my WordItem includes query word string
            queryWordItem.word = queryWord;

            const WordItem &foundWord = myHashTable.find(queryWordItem);

            bool foundInAnyDocument = false;

            for (const DocumentItem &doc: foundWord.details) {

                if (doc.count > 0) {  // Check if the word is found at least once in any document
                    foundInAnyDocument = true;

                }
            }

            if (!foundInAnyDocument) {  // if one of the words cannot found in any document, then output will be "no document contains the given query"
                allWordsFound = false;
                //break;  // Exit early if any word is not found in any document
            }
        }
        if (!allWordsFound && outputCountHash == 1) {  // only output it one time.
            cout << "No document contains the given query" << endl;
            return;  // Exit the function
            // and then continue to while loop in main and ask an input again
        }
        //remove case and there is no document contains the given query cases are finished.


        //THEN CHECK FOR OUTPUTTING EXISTED WORDS


        bool foundInAnyDocument = false;

        for (const string &fileName: allFiles) {  // I stored all files that I need to for query use in main, because outputting is document by document

            bool foundInCurrentDocument = false;

            int count = 0; // for checking that all words are exists in that document or not

            string outputString; // I will cout my output with that for that Document

            for (int i = 0; i < words.size(); i++) {  // check the words in query
                const string &queryWord = words[i]; // go word by word in query for documents
                WordItem queryWordItem;
                queryWordItem.word = queryWord;

                const WordItem &foundWord = myHashTable.find(queryWordItem);  // find the WordItem with same string word


                for (const DocumentItem &doc: foundWord.details) {  // WordItem's docItem vector

                    if (doc.documentName == fileName) { // if it is same as query files
                        if (!foundInCurrentDocument) {  // just output the documents that contains the words of query

                            outputString = "in Document " + fileName;
                        }

                        count++;  //word can found in that element
                        outputString += ", " + queryWord + " found " + to_string(doc.count) + " times";


                        foundInCurrentDocument = true;
                        break;
                    }

                }


            }
            if (foundInCurrentDocument) {
                // after check all words in query for that document, put the dot.
                outputString += ".";
                foundInAnyDocument = true;
            }
            if (count == words.size() && outputCountHash == 1) {  // if all words can be found at that document, put outputCount for only output one time
                cout << outputString;

                cout << endl;

            }
        }


        if (!foundInAnyDocument && outputCountHash == 1) {  //check element found again for sure  //maybe unnecessary
            cout << "No document contains the given query" << endl;
            return;
        }


    }


}

void BSTQuery(BinarySearchTree<WordItem> & myTree, const vector<string> & allFiles, const string& query, int outputCount) {
    if (query.substr(0, 6) == "REMOVE") {    // if query is like REMOVE word
        string deleteWord = query.substr(7);  // will deleted word string is that

        string lowerDeleteWord;
        for (char c: deleteWord) {
            if (isalpha(c)) {
                lowerDeleteWord += tolower(c);  // make it case-insensitive
            }
        }

        WordItem deleteWordItem;
        deleteWordItem.word = lowerDeleteWord;


        const WordItem &willDelete = myTree.find(deleteWordItem);  // find that wordItem with the word in query.

        myTree.remove(willDelete);  //remove it from my tree
        if(outputCount == 1){
            cout << lowerDeleteWord << " has been REMOVED" << endl;
        }

        return; //function finished, return to main and ask an input again

        //not sure that I need to checked REMOVE or not

    }
    else {
        //REAL CASE FOR SEARCHING, remove case is not important in this hw

        //example query : Enter queried words in one line: a the-has been   ,   A:


        string currentWord;
        int wordCount = 0;
        vector<string> words;   // store query words
        for (char c: query) {
            if (isalpha(c)) {
                currentWord += tolower(c);
            } else { // read query char by char and use non alpha chars as a separator
                if (!currentWord.empty()) {
                    words.push_back(currentWord);
                    wordCount++;
                    currentWord.clear();
                }
            }

        }

        // Add the last word of the query
        if (!currentWord.empty()) {
            words.push_back(currentWord);
            wordCount++;
        }


        //CHECKS NO DOCUMENT FIND CASE IN HERE
        bool allWordsFound = true; // if it is false, then output like No document found.
        for (const string &queryWord: words) {  // query words from words vector
            WordItem queryWordItem{queryWord, {}};  // my WordItem includes query word string
            queryWordItem.word = queryWord;

            const WordItem &foundWord = myTree.find(queryWordItem);

            bool foundInAnyDocument = false;

            for (const DocumentItem &doc: foundWord.details) {

                if (doc.count > 0) {  // Check if the word is found at least once in any document
                    foundInAnyDocument = true;

                }
            }

            if (!foundInAnyDocument) {  // if one of the words cannot found in any document, then output will be "no document contains the given query"
                allWordsFound = false;
                //break;  // Exit early if any word is not found in any document
            }
        }
        if (!allWordsFound && outputCount == 1) {  // only output once and return
            cout << "No document contains the given query" << endl;
            return;  // Exit the function
            // and then continue to while loop in main and ask an input again
        }
        //remove case and there is no document contains the given query cases are finished.


        //THEN CHECK FOR OUTPUTTING EXISTED WORDS


        bool foundInAnyDocument = false;

        for (const string &fileName: allFiles) {  // I stored all files that I need to for query use in main, because outputting is document by document

            bool foundInCurrentDocument = false;

            int count = 0; // for checking that all words are exists in that document or not

            string outputString; // I will cout my output with that for that Document

            for (int i = 0; i < words.size(); i++) {  // check the words in query
                const string &queryWord = words[i]; // go word by word in query for documents
                WordItem queryWordItem;
                queryWordItem.word = queryWord;

                const WordItem &foundWord = myTree.find(queryWordItem);  // find the WordItem with same string word


                for (const DocumentItem &doc: foundWord.details) {  // WordItem's docItem vector

                    if (doc.documentName == fileName) { // if it is same as query files
                        if (!foundInCurrentDocument) {  // just output the documents that contains the words of query

                            outputString = "in Document " + fileName;
                        }

                        count++;  //word can found in that element
                        outputString += ", " + queryWord + " found " + to_string(doc.count) + " times";


                        foundInCurrentDocument = true;
                        break;
                    }

                }


            }
            if (foundInCurrentDocument) {
                // after check all words in query for that document, put the dot.
                outputString += ".";
                foundInAnyDocument = true;
            }
            if (count == words.size() && outputCount == 1) {  // if all words can be found at that document, output just one time
                cout << outputString;

                cout << endl;

            }
        }


        if (!foundInAnyDocument && outputCount == 1) {  //check element found again for sure  //maybe unnecessary
            cout << "No document contains the given query" << endl;
            return;
        }

    }


}



int main() {

    string fileName;
    int numFiles = 0;
    cout << "Enter number of input files: ";

    cin >> numFiles;

    vector<WordItem> vectorForBinarySearch;
    BinarySearchTree<WordItem> wordTree(WordItem{"", {}});  //initialize myTree
    HashTable<WordItem> myHashTable(WordItem{"", {}}, 53); // initialize size 53 as in sample runs
    //load factor is always around 0.33 because of the insertion and rehashing in hashtable

    vector<string> allFiles;  // all files that I will read in input

    // Process each file
    cin.ignore();  // Ignore any remaining characters from previous input
    cin.clear();

    if(numFiles > 0){
        for (int i = 0; i < numFiles; i++) {

            cout << "Enter " << i+1  << ". file name: ";

            cin >> fileName;
            allFiles.push_back(fileName);
            //stored all filenames

        }

    }
    for(int i=0; i<allFiles.size(); i++){
        CreateTreeAndHashtable( wordTree, myHashTable, allFiles[i]);  //hashtable and bst created correctly
        CreateVectorForBS(vectorForBinarySearch,allFiles[i]);

    }

    //created tree, hashtable and vector.
    //output after preprocessing
    myHashTable.afterPreprocessing();  // output wants that info after preprocessing // word count and current load


    //all sorts will use same vector

    vector<WordItem> mergeSortVector(vectorForBinarySearch);
    vector<WordItem> heapSortVector(vectorForBinarySearch);
    vector<WordItem> insertionSortVector(vectorForBinarySearch);
    vector<WordItem> quickSortVector(vectorForBinarySearch);
    vector<WordItem> firstQuickSortVector(vectorForBinarySearch);
    vector<WordItem> randomQuickSortVector(vectorForBinarySearch);


    //SORTING  TIMING CALCULATIONS

    int c = 1;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < c; i++) {   //INSERTION SORT
        insertionSort(insertionSortVector);
    }
    auto insertionSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < c; i++) {   //MERGE SORT
        mergeSort(mergeSortVector);
    }
    auto mergeSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < c; i++) {   //HEAP SORT
        heapsort(heapSortVector);
    }
    auto heapSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < c; i++) {   //QUICK SORT, PIVOT WITH MEDIAN3
        quicksort(quickSortVector);
    }
    auto quickSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < c; i++) {   //QUICK SORT, PIVOT RANDOM
        quicksortRandom(randomQuickSortVector);
    }
    auto quickSortRandomTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < c; i++) {   //QUICK SORT, PIVOT FIRST
        quicksortFirst(firstQuickSortVector);
    }
    auto quickSortFirstTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    //SEARCHING QUERY
    cin.ignore();
    cin.clear();
    cout << "Enter queried words in one line: ";

    string query;

    getline(cin,query);


    //SEARCH CALCULATIONS
    int outputCountForBST=0;
    int outputCountForHT = 0;   //for just output query result 1 time not 100 time
    int outputCountForBinarySearch = 0;
    int k = 100;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++){
        // QueryDocuments with Binary Search Tree
        outputCountForBST++;
        BSTQuery(wordTree, allFiles, query,outputCountForBST);
        //double endBST = float(clock() - begin_time);
        //cout << "\nBinary Search Tree Time: " << endBST / k << "\n";
    }
    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++){
        // QueryDocuments with Hash Table
        outputCountForHT++;
        hashQuery(myHashTable, allFiles, query, outputCountForHT);
        //double endHT = float(clock() - begin_time);
        //cout << "\nHash Table Time: " << endHT / k << "\n";
    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++){
        // QueryDocuments with Binary Search
        outputCountForBinarySearch++;
        BSQuery(mergeSortVector,allFiles,query,outputCountForBinarySearch); // can use all vectors here
        //BSQuery(quickSortVector,allFiles,query,outputCountForBinarySearch);

        //double endBS = float(clock() - begin_time);
        //cout << "\nBinary Search Time: " << endBS / k << "\n";
    }
    auto BSTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    cout << endl;

    cout << "Binary Search Tree Time: " << BSTTime.count() / k << endl;

    cout << "Hash Table Time: " << HTTime.count() / k << endl;

    cout << "Binary Search Time: " << BSTime.count() / k << endl;

    cout << endl;

    cout << "Quick Sort(median) Time: " << quickSortTime.count() / c << endl;
    cout << "Quick Sort(random) Time: " << quickSortRandomTime.count() / c << endl;
    cout << "Quick Sort(first) Time: " << quickSortFirstTime.count() / c << endl;
    cout << "Merge Sort Time: " <<  mergeSortTime.count() / c << endl;
    cout << "Heap Sort Time: " <<  heapSortTime.count() / c << endl;
    cout << "Insertion Sort Time: " << insertionSortTime.count() / c << endl;

    cout << endl;

    //COMPARING TIME
    cout << "Speed Up BST/HST: " << (double)BSTTime.count()/HTTime.count()  << endl;

    cout << "Speed Up Merge Sort/Quick Sort(Median): " << (double)mergeSortTime.count()/quickSortTime.count()  << endl;

    cout << "Speed Up Heap Sort/Quick Sort(Median): " << (double)heapSortTime.count()/quickSortTime.count()  << endl;

    cout << "Speed Up Insertion Sort/Quick Sort(Median): " << (double)insertionSortTime.count()/quickSortTime.count()  << endl;
    cout << endl;

    //lastly compare Binary Search with others
    //output as speed up > 1 -> slower one in left side like last hw,

    if((double)BSTime.count()/BSTTime.count() > 1){  //BS time is higher
        cout << "Speed Up Binary Search / Binary Search Tree Time: " << (double)BSTime.count()/BSTTime.count()  << endl;

    }
    else{  //usually this case will come to output -> usually bs is faster than bst
        cout << "Speed Up Binary Search Tree Time / Binary Search : " << (double)BSTTime.count()/BSTime.count()  << endl;
    }

    if((double)BSTime.count()/HTTime.count() > 1){  //BS time is higher
        cout << "Speed Up Binary Search / Hash Table Time: " << (double)BSTime.count()/HTTime.count()  << endl;
    }
    else{
        cout << "Speed Up Hash Table Time / Binary Search: " << (double)HTTime.count()/BSTime.count()  << endl;
    }
    //cout << "Speed Up Binary Search / Binary Search Tree Time: " << (double)BSTime.count()/BSTTime.count()  << endl;

    //cout << "Speed Up Binary Search / Hash Table Time: " << (double)BSTime.count()/HTTime.count()  << endl;

    return 0;
}
