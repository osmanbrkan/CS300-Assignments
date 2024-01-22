

//OSMAN BERK AN 28849 CS300 HOMEWORK2


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "strutils.h"
#include "AvlTree.h"
#include <thread>

using namespace std;

struct DocumentItem {
    string documentName;
    int count;

    bool operator == (const DocumentItem& rhs)const{
        return documentName == rhs.documentName;

    }

};

struct WordItem {
    string word;
    // List of DocumentItem’s. In order to keep the documents
    //you can again use the BST that you are going to implement.
    vector<DocumentItem> details;



    // Comparison operators for WordItem
    bool operator<(const WordItem& other) const {
        return word < other.word;
    }

    bool operator == (const WordItem& other) const{
        return word == other.word;
    }



};



void processFileAndCreateTree(AvlTree<WordItem> &myTree, const string &filename){  //processing file by file and insert the unique words when they have read first time. else, increase the count in that file

    ifstream inputFile(filename);

    if (!inputFile.is_open()) {  //checking file have opened
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string word;


    while (inputFile >> word) {     //Read string by string and make it case-insensitive
        string cleanedWord;
        for (char c : word) {
            if (isalpha(c)) {
                cleanedWord += tolower(c);
            }
        }


        if (!cleanedWord.empty()) {    //if there is a word


            const WordItem& foundItem = myTree.find(WordItem{cleanedWord, {} }); // find WordItem with cleaned word in tree


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
                // for tracking all wordItems maybe use in another func
                //allWords.push_back(wordItem);
            }
            else{   // if that word is in tree before  (as a string), then check for is it in the same document or not.

                vector<DocumentItem>& details = const_cast<WordItem&>(foundItem).details;  // make vector for WordItem's document details

                bool documentFound = false;
                for (DocumentItem& doc : details) {
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

    }
    inputFile.close();  //closing the file

}



void searchQuery(AvlTree<WordItem> &myTree, const vector<string> & allFiles, const string& query){   //Query part that checking the words included in files and checking the counts. then output it as wanted

    // Read a line of strings from the console
    //I am checking the "ENDOFINPUT" case in main

    if(query.substr(0,6) == "REMOVE"){    // if query is like REMOVE word
        string deleteWord = query.substr(7);  // will deleted word string is that

        string lowerDeleteWord;
        for (char c : deleteWord) {
            if (isalpha(c)) {
                lowerDeleteWord += tolower(c);  // make it case-insensitive
            }
        }

        WordItem deleteWordItem;
        deleteWordItem.word = lowerDeleteWord;



        const WordItem& willDelete = myTree.find(deleteWordItem);  // find that wordItem with the word in query.

        myTree.remove(willDelete);  //remove it from my tree
        cout << lowerDeleteWord << " has been REMOVED" << endl;

        return; //function finished, return to main and ask an input again

    }

    else{  //if query is like normal query, need to separate word with iss   // for ex: capitol impoverishment



        istringstream iss(query);
        string word;

        int wordCount = 0;
        vector<string> words;   // store query words in string for maybe use in somewhere

        while(iss>>word){

            string xx;
            for (char c : word) {
                if (isalpha(c)) {
                    xx += tolower(c);  // case-insensitive read again
                }
            }

            if (!xx.empty()) {
                words.push_back(xx);  // words in the query
                wordCount++;  // check the word count in query
            }


        }


        //CHECKS NO DOCUMENT FIND CASE IN HERE
        bool allWordsFound = true; // if it is false, then output like No document found.
        for (const string& queryWord : words) {  // query words from words vector
            WordItem queryWordItem{queryWord, {}};  // my WordItem includes query word string
            queryWordItem.word = queryWord;

            const WordItem & foundWord = myTree.find(queryWordItem);

            bool foundInAnyDocument = false;

            for (const DocumentItem& doc : foundWord.details) {

                if (doc.count > 0) {  // Check if the word is found at least once in any document
                    foundInAnyDocument = true;

                }
            }

            if (!foundInAnyDocument) {  // if one of the words cannot found in any document, then output will be "no document contains the given query"
                allWordsFound = false;
                //break;  // Exit early if any word is not found in any document
            }
        }
        if (!allWordsFound) {
            cout << "No document contains the given query" << endl;
            return;  // Exit the function
            // and then continue to while loop in main and ask an input again
        }



        //THEN CHECK FOR OUTPUTTING EXISTED WORDS


        bool foundInAnyDocument = false;

        for (const string& fileName : allFiles) {  // I stored all files that I need to for query use in main, because outputting is document by document

            //bool lastWordInSentence = f
            // alse;

            bool foundInCurrentDocument = false;

            int count = 0; // for checking that all words are exists in that document or not

            string outputString; // I will cout my output with that for that Document

            for (int i=0; i<words.size(); i++) {  // check the words in query
                const string &queryWord = words[i]; // go word by word in query for documents
                WordItem queryWordItem;
                queryWordItem.word = queryWord;

                const WordItem& foundWord = myTree.find(queryWordItem);  // find the WordItem with same string word


                for (const DocumentItem& doc : foundWord.details) {  // WordItem's docItem vector

                    if (doc.documentName == fileName) { // if it is same as query files
                        if (!foundInCurrentDocument) {  // just output the documents that contains the words of query

                            outputString = "in Document "  + fileName;
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
            if(count == words.size()){  // if all words can be found at that document.
                cout << outputString;
                cout << endl;
            }
        }


        if (!foundInAnyDocument) {  //check element found again for sure  //maybe unnecessary
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



    AvlTree<WordItem> wordTree(WordItem{"", {}});  //initialize myTree
    vector<string> allFiles;  // all files that I will read in input

    // Process each file

    cin.ignore();  // Ignore any remaining characters from previous input
    cin.clear();

    if(numFiles > 0){
        for (int i = 0; i < numFiles; i++) {

            cout << "Enter " << i+1  << ". file name: ";

            cin >> fileName;
            allFiles.push_back(fileName);

            processFileAndCreateTree(wordTree, fileName);  //process files and insert the wordItems into tree



        }
    }
    //TREE MADE




    cout << endl;

    //SEARCHING QUERY

    cin.ignore();
    cin.clear();

    cout << "Enter queried words in one line: ";

    string query;

    getline(cin,query);

    if(query == "ENDOFINPUT"){   //finish getting input

        wordTree.makeEmpty();  //clear the tree memory before program closing.
        return 0;
    }
    else{
        while(query != "ENDOFINPUT"){
            searchQuery(wordTree, allFiles, query); //outputting query or remove word from tree
            cout << endl;
            cout << "Enter queried words in one line: ";
            getline(cin,query);


        }
    }

    wordTree.makeEmpty();   // make all tree empty before program finishes
    return 0;
}



