
//OSMAN BERK AN 28849
//HW3

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "BinarySearchTree.h"
#include "HashTable.h"
#include <thread>
#include <chrono>

using namespace std;





//reading files and there is new rule : Every non-alphabetical character is considered as a separator (different than HW2).
//This rule applies for both the text in input files and queried words.
//For example; "face2face" = "face.face" = "face:facE" = "face face"

void CreateTreeAndHashtable(BinarySearchTree<WordItem> &myTree, HashTable<WordItem> &myHashTable, const string &filename) {  //processing file by file and insert the unique words when they have read first time. else, increase the count in that file

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
                cleanedWord.clear();  //After add to tree and table, need to clear the cleaned word

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
                 myHashTable.insert(wordItem);
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
        // both hashtable and tree finished

    cleanedWord.clear();
    inputFile.close(); //close the input file

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
        CreateTreeAndHashtable(wordTree, myHashTable, allFiles[i]);
    }
    cout << endl;
    myHashTable.afterPreprocessing(); // output final outcome of preprocessed files // unique word count, current load ratio


    //SEARCHING QUERY

    cin.ignore();
    cin.clear();
    cout << "Enter queried words in one line: ";

    string query;

    getline(cin,query);

    int outputCountForBST=0;
    int outputCountForHT = 0;   //for just output query result 1 time not 100 time
    int k = 100;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        // QueryDocuments(with BST);
        outputCountForBST++;
        BSTQuery(wordTree, allFiles, query,outputCountForBST);
    }
    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);
    //cout << "\nTime: " << BSTTime.count() / k << "\n";
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        // QueryDocuments (with hashtable);
        outputCountForHT++;
        hashQuery(myHashTable, allFiles, query, outputCountForHT);
    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);
    //cout << "\nTime: " << HTTime.count() / k << "\n";

    cout << "\nTime: " << BSTTime.count() / k << "\n";

    cout << "\nTime: " << HTTime.count() / k << "\n";

    cout << "Speed Up: " << (double)BSTTime.count()/HTTime.count() << endl;   //bst / hashtable = speed up


    return 0;
}
