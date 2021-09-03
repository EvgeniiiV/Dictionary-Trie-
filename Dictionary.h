// based on https://www.geeksforgeeks.org/auto-complete-feature-using-trie/
#pragma once
#include <iostream>
using namespace std;
#define SIZE 20 //maximum of suggestions
#define ALPHABET_SIZE (26)
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

class Dictionary
{
public:
    struct TrieNode
    {
        TrieNode* children[ALPHABET_SIZE];       
        bool isWordEnd;
    };
    Dictionary();   
    struct TrieNode* getNode(void);
    void insert(const string key);
    bool search(TrieNode* root, const string key);
    bool isLastNode(struct TrieNode* root);
    void suggestionsRec(TrieNode* root, string currPrefix, int& counter);
    int printAutoSuggestions(const string query);
    void addsuggestions(string currPrefix, int index);
    void print(int index);
    
private:
    TrieNode* root;   
    string suggestion [SIZE];
};
