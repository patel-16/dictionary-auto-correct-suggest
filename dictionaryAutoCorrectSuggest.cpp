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
//              // \\  
//          ...    ... 
//           //       \\   
//         ...      ... 
//
// dp[i][j] -> here i is implied by the depth of the trie
// all j's we iterate over...
// now what is dp[i][j]
// minimum edit distance for string1(0..i)
// and string2(0..j)
// NOTE - First finish edit distance bottom up
// in the  personal dsa repo first...
// then I think this one will click??
void getTopMatchesFromDictionaryRecursion(
    // string& current, -> implied from path, see currentTriePathString
    // int currentIndex (i) -> implied by trie depth,
    string& inputTarget, // while this is target, 
    // we assume manipulation for this 
    std::vector<int>& dpPrevRow,
    int errorTolerance,
    std::string currentTriePathString,
    TrieNode* currentDictionaryTrieNode,
    vector<pair<int, string>>& editDistWordMap) {


    // Iterate through all possible out going characters
    // of the current trie node ( 'a'...'z')
    for (int charIndex = 0; charIndex < 26; charIndex+=1) {


        if (currentDictionaryTrieNode->alphabetList[charIndex] != nullptr) {
            

            char currentLetter = 'a' + charIndex;

            // dp row for each existing character...
            // dp [i][j] , but i is implied here 
            // only j is variable
            // since its a trie..for each of the character
            // it is made as there are 26 characters at 
            // 'i' th level
            std::vector<int> dpCurrRow(inputTarget.size() + 1);

            // dp[i][0] = dp[i-1][0] + 1;
            dpCurrRow[0] = dpPrevRow[0] + 1;

            // this is for the currentLetter
            // as we declared above a variable with same name
            int minCostForCurrentLetterString = dpCurrRow[0];

            // 1 based indexing for dp..
            for (int j = 1; j <= inputTarget.size(); j++)
            {
                // now what is dp[i][j]
                // minimum edit distance for string1(0..i-1) included
                // and string2(0..j-1) included
                // so, we need dp[i][j] based on previous states
                // .. 
                // we think of dpPrevRow as dp[i-1][0...inputTargetSize]
                if (currentLetter == inputTarget[j - 1]) {
                    // dp[i][j] = dp[i-1][j-1] 
                    dpCurrRow[j] = dpPrevRow[j - 1] ;
                } else {
                    // replace case - dp[i][j] = 1 + dp[i-1][j-1] replacing in string 2 
                    // delete case - dp[i][j] = 1 + dp[i][j-1] delete from string 2
                    // add case - dp[i][j] = 1 + dp[i-1][j] add to string 2
                    // and dp[i-1] -> dpPrevRow so 
                    dpCurrRow[j] = 1 + min(dpPrevRow[j-1], min(dpCurrRow[j-1], dpPrevRow[j]));
                }

                minCostForCurrentLetterString = min(dpCurrRow[j], minCostForCurrentLetterString);

            }

            // this outer condition check is for more traversal
            // of trie
            if (minCostForCurrentLetterString <= errorTolerance) {
                std::string newTriePathString = currentTriePathString + currentLetter;

                // using a trie which handles count
                // so just !=0 should be ok for now
                // this internal condition is for checking if the
                // input query word ends here  
                // and edit distance is within tolerance
                // aka - ( dp[i] [inputTargetSize] < errorTolerance)
                // then we can 
                // add it to the result, yay !
                if (currentDictionaryTrieNode -> alphabetList[charIndex] -> countEnd != 0
                    && dpCurrRow[inputTarget.size()] <= errorTolerance
                ) {
                    editDistWordMap.push_back({dpCurrRow[inputTarget.size()], newTriePathString});
                } 

                getTopMatchesFromDictionaryRecursion(
                    inputTarget, 
                    dpCurrRow, 
                    errorTolerance, 
                    newTriePathString, 
                    currentDictionaryTrieNode -> alphabetList[charIndex],
                    editDistWordMap
                );
            }

        }

    }


}


// looks like we cant return array like
// string[] in c++
// so use pointer or vector
vector<pair<int, string>> getTopMatchesFromDictionary(string input, Trie* dictionaryTrie) {

    vector<pair<int, string>> editDistWordMap;

    int errorTolerance = 2;

    TrieNode* root = dictionaryTrie -> root;

    vector<int> initialDpVector (input.size() + 1, 0);
    
    // Why All 0s Fails
    // In Levenshtein distance, 
    // Row 0 represents the base case: 
    // comparing an empty prefix ("") against 
    // your target word.
    // If your target word is "hwo" (length 3), 
    // and your current trie prefix is empty (""), 
    // how many edits does it take to turn "" into "hwo"?
    // It takes 3 insertions.
    
    // Therefore, base row (the root's prevRow) must 
    // represent the cost of transforming an empty string into
    // every prefix of your target word. 
    // It needs to count up: 0, 1, 2, 3, ... N.
    for (int j = 0; j <= input.size(); ++j) {
        initialDpVector[j] = j; // This sets it to: [0, 1, 2, 3]
    }
    
    getTopMatchesFromDictionaryRecursion(
        input, 
        initialDpVector, 
        errorTolerance,
        "",
        root,
        editDistWordMap
    );

    // sort so min dist words come up
    sort(editDistWordMap.begin(), editDistWordMap.end());

    return editDistWordMap;
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
    vector<pair<int,string>> topMatches = getTopMatchesFromDictionary(input, &trieDictionaryObject);

    cout <<"\n RESULTS - \n"; 
    cout<< "edit distance, word\n"; 
    for (int i = 0; i < min(10, (int) topMatches.size()); i++)
    {
        cout << topMatches[i].first <<", "<< topMatches[i].second<<"\n";
    }
    

    return 0;

}