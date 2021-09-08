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
    TrieNode* parent = nullptr;
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
            pCrawl->children[index]->parent = pCrawl;
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
    bool Dictionary::isLastNode(TrieNode* root)
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
            if (root->children[i])
                return 0;
        return 1;
    }

    int Dictionary::suffixCount(TrieNode* root)
    {
        int result = 0;

        // Leaf denotes end of a word
        if (root->isWordEnd)
            result++;

        for (int i = 0; i < ALPHABET_SIZE; i++)
            if (root->children[i])
                result += suffixCount(root->children[i]);

        return result;
    }
    //pseudo-iterative traversal of the trie under the prefix
    void Dictionary::suggestionsRec(TrieNode* current, string currPrefix, int& counter)
    {
        int numOfSuffixes = suffixCount(current);
        int upDown = 1;//for switch
        int next_index = 0; //starting index to find  next letter      
        if (current->isWordEnd)
        {
            cout << counter << " " << currPrefix << endl;
            addsuggestions(currPrefix, counter);
            counter++;
        }

        while (counter < numOfSuffixes)
        {
            switch (upDown)
            {
              case 1:
                if (counter == numOfSuffixes)
                    return;                
                   
               
                if(!isLastNode(current))//not a leaf
                for (int i = 0; i < ALPHABET_SIZE; i++)
                {
                    if  (current && current->children[i])
                    {
                        currPrefix.push_back(97 + i);
                        current = current->children[i];
                        i = 0;
                        if (current->isWordEnd)
                        {
                            cout << counter << " " << currPrefix << endl;
                            addsuggestions(currPrefix, counter);
                            counter++;
                        }
                        if (isLastNode(current)) break;  
                                  
                           
                        
                    }
                }

                upDown = 2;
                break;

             case 2:
                
                current = current->parent;
                next_index = (currPrefix[currPrefix.size() - 1] - 97) + 1;//next letter
                currPrefix.pop_back();
                
                for (next_index; next_index < 26; next_index++)//start with next letter
                {
                    if (current->children[next_index])
                    {                        
                        current = current->children[next_index];
                       
                        upDown = 3;
                        break;
                    }
                }
                
                break;

             case 3:                       
                 
                     currPrefix.push_back(97 + next_index);//take letter
                     if (isLastNode(current) && current->isWordEnd)//if leaf    

                     {
                         cout << counter << " " << currPrefix << endl;
                         addsuggestions(currPrefix, counter);
                         counter++;
                         upDown = 2;
                     }
                     else if (current->isWordEnd)
                     {
                         cout << counter << " " << currPrefix << endl;
                         addsuggestions(currPrefix, counter);
                         counter++;
                         upDown = 1;
                     }
                     else upDown = 1;
                     break;           
                                
            
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
        d.insert("helmet");        
        d.insert("helper");
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
