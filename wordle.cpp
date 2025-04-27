#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// Add prototypes of helper functions here

void findWords(
    const string& pattern,
    string& word,
    string floatingChars,
    size_t position,
    const set<string>& dictionary,
    set<string>& validWords);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    set<string> results;
    string currentWord = in;
    findWords(in, currentWord, floating, 0, dict, results);
    return results;
}

// Define any helper functions here

void findWords(
    const string& pattern,
    string& word,
    string floatingChars,
    size_t position,
    const set<string>& dictionary,
    set<string>& validWords)
{
    if (position == pattern.length()) {
        if (floatingChars.empty() && dictionary.find(word) != dictionary.end()) {
            validWords.insert(word);
        }
        return;
    }
    
    if (pattern[position] != '-') {
        char fixedChar = pattern[position];
        word[position] = fixedChar;
        
        size_t idx = floatingChars.find(fixedChar);
        if (idx != string::npos) {
            string updatedFloating = floatingChars;
            updatedFloating.erase(idx, 1);
            findWords(pattern, word, updatedFloating, position + 1, dictionary, validWords);
        } else {
            findWords(pattern, word, floatingChars, position + 1, dictionary, validWords);
        }
        return;
    }
    
    // For blank positions, try different letters
    // Count remaining blank positions
    size_t blanksRemaining = 0;
    for (size_t i = position; i < pattern.length(); i++) {
        if (pattern[i] == '-') {
            blanksRemaining++;
        }
    }
    
    // try each letter
    for (char letter = 'a'; letter <= 'z'; letter++) {
        word[position] = letter;
        // Check if floating
        size_t floatingIdx = floatingChars.find(letter);
        if (floatingIdx != string::npos) {
            // using required floating
            string remainingFloating = floatingChars;
            remainingFloating.erase(floatingIdx, 1);
            // recurse if enough blank positions for float
            if (blanksRemaining - 1 >= remainingFloating.length()) {
                findWords(pattern, word, remainingFloating, position + 1, dictionary, validWords);
            }
        } else {
            // using regular letter
            // recurse if enough blank positions for float
            if (blanksRemaining - 1 >= floatingChars.length()) {
                findWords(pattern, word, floatingChars, position + 1, dictionary, validWords);
            }
        }
    }
}
