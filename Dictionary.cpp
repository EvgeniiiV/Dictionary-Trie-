#include <iostream>
using namespace std;
#include "Dictionary.h"
#include <string>

Dictionary::Dictionary()
{    
    root = getNode();      
}

Dictionary::TrieNode* Dictionary::getNode(void)
{
    TrieNode* pNode = new TrieNode;
    pNode->isWordEnd = false;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        pNode->children[i] = NULL;

    return pNode;
}    
    void Dictionary::insert(const string key)
    {
        struct TrieNode* pCrawl = root;

        for (int level = 0; level < key.length(); level++)
        {
            int index = CHAR_TO_INDEX(key[level]);
            if (!pCrawl->children[index])
                pCrawl->children[index] = getNode();

            pCrawl = pCrawl->children[index];
        }

        
        pCrawl->isWordEnd = true;
    }
    
    Dictionary::TrieNode* Dictionary::search(const string key)
    {
        int length = key.length();
        TrieNode* pCrawl = root;
        for (int level = 0; level < length; level++)
        {
            int index = CHAR_TO_INDEX(key[level]);
            if (!pCrawl->children[index])
                return nullptr;
            pCrawl = pCrawl->children[index];
        }
        
        return  pCrawl;
    }
    // Returns 0 if current node has a child
    // If all children are NULL, return 1.
    bool Dictionary::isLastNode(struct TrieNode* root)
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
            if (root->children[i])
                return 0;
        return 1;
    }

    // Recursive function to print auto-suggestions for given
// node.
    void Dictionary::suggestionsRec(TrieNode* root, string currPrefix, int& counter)
    {
        // found a string in Trie with the given prefix
        if (root->isWordEnd)
        {
           cout << counter << " " << currPrefix << endl;            
           addsuggestions(currPrefix, counter);
           counter++;
        }

        // All children struct node pointers are NULL
        if (isLastNode(root))
            return;

        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            if (root->children[i])
            {
                // append current character to currPrefix string
                currPrefix.push_back(97 + i);

                // recur over the rest
                suggestionsRec(root->children[i], currPrefix, counter);
                // remove last character
                currPrefix.pop_back();
            }
        }
    }   
      

    // print suggestions for given query prefix.
    int Dictionary::printAutoSuggestions(const string query)
    {
        TrieNode* pCrawl = search(query);
        if (!pCrawl) return 0;

        // If prefix is present as a word.
        bool isWord = (pCrawl->isWordEnd == true);

        // If prefix is last node of tree (has no
        // children)
        bool isLast = isLastNode(pCrawl);

        // If prefix is present as a word, but
        // there is no subtree below the last
        // matching node.
        if (isWord && isLast)
        {
            cout << query << endl;
            return -1;
        }
        //for suggestionsRec
        int counter = 0;
        // If there are are nodes below last
        // matching character.
        if (!isLast)
        {
            string prefix = query;
            suggestionsRec(pCrawl, prefix, counter);
            return 1;
        }
    }

    //collects suggestions to choose
    void Dictionary::addsuggestions(string currPrefix, int index)
    {        
        if (index == SIZE)
        { cout << "Error: Out of memory" << endl;        
        exit(0); }
        suggestion[index] = currPrefix; 
    }
    //prints chosen suggestion 
    void Dictionary::print(int index)
    {
        cout << suggestion[index] << endl;
    }


    int main()
    {
        Dictionary d;
        d.insert("hello");
        d.insert("dog");
        d.insert("hell");
        d.insert("doggy");
        d.insert("dogs");
        d.insert("dogma");
        d.insert("hel");
        d.insert("help");
        d.insert("helps");
        d.insert("helping");

        while (true)
        {
            cout << "Enter a prefix (press key 1 to quit)" << endl;
            string prefix;
            prefix.clear();
            cin.clear();           
            cin >> prefix;
            if (prefix == "1") return 0;

            int comp = d.printAutoSuggestions(prefix);

            if (comp == -1)
            {
                cout << "No other strings found with this prefix\n";
                continue;
            }

            else if (comp == 0)
            {
                cout << "No string found with this prefix\n";
                continue;
            }

            cout << "Choose an option" << endl;
            int choice;
            cin.clear();
            cin >> choice;
            d.print(choice);

        }
        return 0;
    }
