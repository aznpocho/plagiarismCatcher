#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <queue>
#include <map>
#include "TextList.h"

using namespace std;

/*function... might want it in some class?*/
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

/* Function: int hashFunc
 * input: string and hash table size
 * output: idx of hash table
*/
int hashFunc (string key, int arraySize){
    unsigned int hashVal = 0;
    unsigned int letter;
    for (int j = 0; j < key.length(); j++){
        letter = int(key[j]);
        // algorithm implemented avoids integer overflow
        hashVal = (hashVal * 37 + letter) % arraySize;
    }
    return hashVal;
}

/* Function: check if a number is prime
 * input: number
 * output: true or false
 * Warning: not really fast...
 */
bool isPrime (unsigned int number){
    if (number <= 1){
        return (false);
    }
    for (unsigned int i = 2; i*i <= number; i++){
        if (number % i == 0){
            // not prime
            return (false);
        }
    }
    // number is prime
    return (true);
}

/*
 *
 */
int find_nextPrime(unsigned int number){
    // while the number is not prime
    number += 1;
    while (!isPrime(number)){
        number += 1;
    }
    return number;
}

/* Function: Use 1 text file, and compare to the rest of the text files
 * input: vector file, starting file
 *
 */
int find_hash_table_size(vector<string> &files, string dir){
    string word;
    ifstream myfile;
    unsigned int total_word_count = 0;
    for (int i = 0; i < files.size(); i++){
        // open text file
        myfile.open(dir+"/"+files[i]);

        // getting word count of text file
        istream_iterator<string> in{ myfile }, end;
        total_word_count += distance(in, end);

        // done, close text file
        myfile.close();
    }

    total_word_count *= 1.3;
    if (!isPrime(total_word_count)){
        total_word_count = find_nextPrime(total_word_count);
    }

    return(total_word_count);
}

/* Function: sort_plagiarized_text | sort the plagiarized texts by least to greatest
 * input: 2D list of texts and plagiarized n-sequence strings
 * output: none
 */
void sort_plagiarized_text (int* text_2d[], int file_size){
    // create map
    multimap<int,string> plagiarized_map;
    multimap<int,string>::iterator itr;

    string text_pair;

    // need to pass # of plagiarized allowed
    for (int i = 0; i < file_size; i++){
        for (int j = 0; j < file_size; j++){
            if (text_2d[i][j] >= 200){
                text_pair = to_string(i)+"-"+to_string(j);
                plagiarized_map.insert(pair<int,string>(text_2d[i][j],text_pair));
            }
        }
    }

    // start at end of multimap
    itr = plagiarized_map.end();
    itr --; // To get to actual last index of multimap
    for (itr; itr != plagiarized_map.begin(); itr--){
        cout << itr->first << ": " <<itr->second << "\n";
    }
    cout << plagiarized_map.begin()->first << ": " << plagiarized_map.begin()->second << "\n";
}

void get_plagiarism_comb(int* text_2d[], TextNode* head){ // fills out the 2D list
    vector<int> text_idx;

    // populate text indexes from linked list
    while (head != NULL){
        // push text indexes into a vector
        text_idx.push_back(head->text_idx);
        head = head->next;
    }

    // populate 2d list of text plagiarism
    // get all combinations of 2 texts within vector of text indexes
    for (int i = text_idx.size()-1; i > 0; i--){
        for (int j = i-1; j >= 0; j--){
            text_2d[text_idx[i]][text_idx[j]] += 1;
        }
    }
}

int size_TextNode_ll(TextNode* head){
    int size = 0;
    while (head != NULL){
        size += 1;
        head = head -> next;
    }
    return (size);
}

/* Function: get_plagiarism_texts -> get combinations of plagiarism
 * input: array of pointers to TextNodes
 * output: sorted array
 */
void get_plagiarism_texts (TextNode* arr_name[], int const arraySize, int const file_size){
    cout << "\n" << "get_plagiarism_texts: ";

    // create 2D list for text files, fill with 0's
    int *text_2d[file_size];
    for (int i = 0; i < file_size; i++){ // populate each row with columns
        text_2d[i] = new int[file_size];
        for (int j = 0; j < file_size; j++){ // fill the 2D list with 0's
            text_2d[i][j] = 0;
        }
    }

    // go through every linked list in array
    for (int i = 0; i < arraySize; i++){
        // check if NULL or size of linked list is <= 1
        if ((arr_name[i] != NULL) && (size_TextNode_ll(arr_name[i]) > 1)) {
            // inserting # of plagiarism into 2D list
            get_plagiarism_comb(text_2d, arr_name[i]);
        }
    }
    cout << "# of Plagiarized sequences: " <<text_2d[0][1] << endl;
    cout << "Print out plagiarized texts, least to greatest" << endl;
    sort_plagiarized_text (text_2d, file_size);
}

/* Function: Go through each file
 * input: vector of strings/files
 */
void hash_all_texts(string dir, vector<string> &files, int n){
    // getting hash table size
    unsigned int arraySize = find_hash_table_size(files,dir);
    cout << "arraySize: " << arraySize << endl;

    // variables for "for loop"
    string word; // used for each word in text files
    ifstream myfile; // text file
    vector<string> n_list; // list of strings
    cout << "Right before creating array of ptrs" << endl;
    TextNode* arr_name[arraySize]; // array of ptrs that point to TextNodes
    cout << "Was able to create array of ptrs" << endl;
    // setting all the pointers to null
    for (int i = 0; i < arraySize; i++){
        arr_name[i] = NULL;
    }
    unsigned int hashVal; // hash idx value for given key
    string nlen_string; // holds concatenated strings

    // go through all text files and hash
    for (int i = 0; i < files.size(); i++){
        // open file
        myfile.open(dir+"/"+files[i]);

        // fill up the n_list with n number of words
        while(n_list.size() != (n-1)){
            getline(myfile,word, ' ');
            // only alphanumerical characters
            word.erase(remove_if(word.begin(), word.end(), [](char c) { return !isalnum(c); } ), word.end());
            n_list.push_back(word);
        }

        // loop that goes through every single string/"word" in file
        if (myfile.is_open()){
            while (getline(myfile,word, ' ')){
                nlen_string = "";
                // only alphanumerical characters and push to list
                word.erase(remove_if(word.begin(), word.end(), [](char c) { return !isalnum(c); } ), word.end());
                n_list.push_back(word);
                // concatenating strings
                for (int i = 0; i < n_list.size(); i++){
                    nlen_string += n_list[i]; // concatenated string
                }
                transform(nlen_string.begin(), nlen_string.end(), nlen_string.begin(), [](char c){return tolower(c);});
                cout << "Right before hashFunc" << endl;
                // get hashVal for nlen_string(key)
                hashVal = hashFunc(nlen_string, arraySize); // e.g. index of hashTable
                // store in hash table
                addText(arr_name[hashVal], i);
                // pop beginning n_list
                n_list.erase(n_list.begin());
            }
        }
        myfile.close();
    }
    cout << "Out of for loop, linked list is built" << endl;
    get_plagiarism_texts(arr_name, arraySize, files.size());
}

int main(int argc,char *argv[])
{
    // dir is the directory
    //string dir = string("sm_doc_set");
    //string dir = string("med_doc_set");
    string dir = string("big_doc_set");

    vector<string> files = vector<string>();

    // getting all files in the directory
    getdir(dir,files);

    // delete child and parent directory in files vector
    files.erase(files.begin());
    files.erase(files.begin());

    // prints out the files in file vector
    // # text_name.txt
    for (unsigned int i = 0;i < files.size();i++) {
        cout << i << " " << files[i] << endl;
    }

    /*
    ifstream myfile;
    string word;
    myfile.open(dir+"/"+files[0]);
    getline(myfile,word, ' ');
    cout << "while loop: " << endl;
    while(getline(myfile,word, ' ')) {
        cout << word << endl;
    }*/

    int n = 6;

    // hashing all the text files
    hash_all_texts(dir, files, n);

    cout << "\n" << "Done with reading text files" << endl;

    cout << "\n" << "Reading text file... " << files[0] << "\n";

    cout << "\n" << "Done reading 1st text file" << endl;

    /* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ TESTING AREA @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
    // testing area
    string test, test2;
    test = "hello";
    test2 = "pickles";
    cout << test2 << endl;
    cout << int(test2[0]) << endl;
    test2 = "yes";
    cout << test2 << endl;
    test = test + test2;
    cout << test << endl;

    string test3;
    test3 += test2;
    cout << test3 << endl;

    int num = 5043;
    int num2 = num * 1.45;
    cout << num2 << endl;

    cout << "Pointer of strings array" << endl;

    string* word_list_n[3];
    string string_1 = "hello";
    string string_2 = "world";
    word_list_n[0] = &string_1;
    cout << *(word_list_n[0]) << endl;
    word_list_n[1] = &string_2;
    cout << *(*(word_list_n+1)) << endl;
    cout << word_list_n[0] << endl;

    vector<int> intvec;
    intvec.push_back(5);
    intvec.push_back(6);
    cout << intvec[0] << endl;
    intvec.erase(intvec.begin());
    cout << intvec[0] << endl;

    cout << "Testing 2D list" << endl;
    int *num3[5];
    num3[0] = new int[5];
    num3[0][4] = 44;
    cout << num3[0][4] << endl;
    cout << num3[0][4] << endl;

    int x = 45;
    int y = 20;
    int z = 10;

    return 0;
}