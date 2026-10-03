/*List 10 closest words from a dictionary based on edit distance */

// Now a naive way is to calculate edit distance for each word in the dictionary
// and then get the top 10 words which have minimum distance

// But if a dictionary is very large ?

// thats where trie seems to be used..
#include<string>
#include<iostream>
#include "trieWithCount.h"

using namespace std;

// later refactor to 
// use strategy pattern
// for various match finding ways..

// current - current candidate string under investigation
// input - user input (aka target string)
// but here I think we need to leverage
// edit dist (a,b) = edit dist(b,a) 
// for. (This symmetry assumes you are using 
// uniform operation costs 
// e.g., inserting, deleting, and substituting all cost 1)
//              ... 
//              / \ 
//          ...    ... 
//           /       \ 
//         ...      ... 
//
// dp[i][j] -> here i is implied by the depth of the trie
// all j's we iterate over...
// NOTE - First finish edit distance bottom up
// in the  personal dsa repo first...
// then I think this one will click??
void getTopMatchesFromDictionaryRecursion(
    string& current, 
    // int currentIndex (i) -> implied by trie depth,
    string& inputTarget, // while this is target, 
    // we assume manipulation for this 
    int errorTolerance,
    TrieNode* dictionaryTrieNode) {


    // Iterate through all possible out going characters
    // of the current trie node ( 'a'...'z')
    for (int charIndex = 0; charIndex < 26; charIndex+=1) {

        if (dictionaryTrieNode->alphabetList[charIndex] != nullptr) {
            char currentLetter = 'a' + charIndex;
            // std::vector<int> dpCurrRow();
        }

    }


}


// looks like we cant return array like
// string[] in c++
// so use pointer or vector
vector<string> getTopMatchesFromDictionary(string input, Trie* dictionaryTrie) {

    vector<vector<int, string>> editDistWordMap;

    int errorTolerance = 2;

    TrieNode* root = dictionaryTrie -> root;



    // sort so min dist words come up
    sort(editDistWordMap.begin(), editDistWordMap.end());

    return vector<string>();
}

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

// Assume you have a Trie class defined elsewhere
// Trie trie;

void loadDictionary(const std::string& filename, Trie& trie) {
    // how do file streams work ??
    // lets see later
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }



    std::string word;
    while (std::getline(file, word)) {
        // Clean up the word: remove trailing carriage returns if on Windows
        // TODO() - how the hell does this work?
        // some lambda??
        word.erase(std::remove_if(word.begin(), word.end(), [](char c) {
            return !isalpha(c);
        }), word.end());
        
        if (!word.empty()) {
            // Convert to lowercase if needed
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            trie.insertWord(word);
        }
    }
    // why needed to close file stream explicitly ??
    // lets check later
    file.close();
    std::cout << "Dictionary loaded successfully!\n";
}

int main() {

    Trie trieDictionaryObject;

    // using txts from
    // https://github.com/dolph/dictionary
    loadDictionary("unix-words.txt", trieDictionaryObject);

    cout << "please enter input query to look for top matches - \n";
    std::string input;
    std::cin >> input;

    cout <<"Exact present - " << trieDictionaryObject.countWordsEqualTo(input, false)<<"\n";

    cout<<"\n";

    // should pass pointer only 
    // while this will not create new topMatches 
    // vector...c++ has a feature to handle but still
    // cleaner looking logic is more intuitive
    vector<string> topMatches = getTopMatchesFromDictionary(input, &trieDictionaryObject);

    return 0;

}