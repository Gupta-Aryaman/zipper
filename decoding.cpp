#include "classes.cpp"
#include "mapOfNumbers.cpp"
#include<iostream>

int binaryToDecimal(string str)
{
    int dec_num = 0;
    int power = 0 ;
    int n = str.length() ;
   
      for(int i = n-1 ; i>=0 ; i--){
      if(str[i] == '1'){
        dec_num += (1<<power) ;
      }
      power++ ;
    }
   
    return dec_num;
}

void findNytCode(AdaptiveHuffmanTreeNode* head, AdaptiveHuffmanTreeNode* nyt, string &nytCode, string temp){
    if(head == NULL){
        return;
    }
    if(head->left == NULL && head->right == NULL && head==nyt){
        nytCode = temp;
        return;
    }
    findNytCode(head->left,nyt, nytCode, temp+"0");
    findNytCode(head->right,nyt, nytCode, temp+"1");
}

void updateTree(string current_char, AdaptiveHuffmanTree* tree){
    AdaptiveHuffmanTreeNode* newNode = new AdaptiveHuffmanTreeNode(1);
    newNode->parent = tree->nyt->parent;
    newNode->left = tree->nyt;
    newNode->right = new AdaptiveHuffmanTreeNode(1, current_char);
    newNode->right->parent = newNode;
    tree->nyt->parent->left = newNode;
    tree->nyt->parent = newNode;

    newNode = newNode->parent;
    while(newNode!=NULL){
        if(newNode->left->weight > newNode->right->weight){
            AdaptiveHuffmanTreeNode* temp = newNode->left;
            newNode->left = newNode->right;
            newNode->right = temp;
        }
        newNode->weight = newNode->left->weight + newNode->right->weight;
        newNode=newNode->parent;
    }
    string nytCode = "";
    findNytCode(tree->head,tree->nyt, nytCode, "");
    tree->nytCode = nytCode;
}

string decode(string input){
    //2 parameters e and r
    // m = 94 = 2^e + r
    //e = 6, r = 30
    int e = 6;
    int r = 30;
    AdaptiveHuffmanTree* tree = new AdaptiveHuffmanTree();
    string output = "";

    map<int, string> mp = mapOfNumbers();

    int i;
    string temp = "";
    string current_char = "";

    for(i = 0; i < e; i++){
        temp += input[i];
    }
    //is bit value < r?
    if(binaryToDecimal(temp)<r){
        // i = i+1;
        temp += input[i];
        output += mp[binaryToDecimal(temp)+1]; 
        current_char = mp[binaryToDecimal(temp)+1];
        i = i+1;
    }
    else{
        output += mp[binaryToDecimal(temp) + r + 1];
        current_char = mp[binaryToDecimal(temp) + r + 1];
    }
    AdaptiveHuffmanTreeNode* head = new AdaptiveHuffmanTreeNode(1);
    tree->head = head;
    tree->head->left = tree->nyt;
    tree->nyt->parent = tree->head;
    tree->head->right = new AdaptiveHuffmanTreeNode(1, current_char);
    tree->head->right->parent = tree->head;
    tree->nytCode = "0";

    while(i<input.size()){
        temp = "";
        string tempNytCode = "";
        for(int j = i; j < i + tree->nytCode.size(); j++){
            tempNytCode += input[j];
        }
        //if nyt code, i.e. element does not exist in tree
        if(tempNytCode == tree->nytCode){
            
            i = i+tree->nytCode.size();
            int j;

            for(j= i; j < i + e; j++){
                temp += input[j];
            }
            // cout << temp << " "<<j<< endl;
            // break;
            if(binaryToDecimal(temp) < r){
                temp += input[j];
                output += mp[binaryToDecimal(temp) + 1]; 
                current_char = mp[binaryToDecimal(temp) + 1];
                i = j+1;

                //add new node to tree + update the tree
                updateTree(current_char, tree);
            }
            
            else{
                output += mp[binaryToDecimal(temp) + r + 1];
                current_char = mp[binaryToDecimal(temp) + r + 1];
                i = j;

                //add new node to tree + update the tree
                updateTree(current_char, tree);
            }
            
        }
        //if not nyt code, i.e. element already exists in tree
        else{
            AdaptiveHuffmanTreeNode* curr = tree->head;
            while(curr->character==""){
                if(input[i] == '0'){
                    curr = curr->left;
                }
                else{
                    curr = curr->right;
                }
                temp += input[i];
                i++;
            }
            output += curr->character;
            curr->weight+=1;
            while(curr->parent!=NULL){
                curr = curr->parent;
                curr->weight+=1;
            }
            // cout << tree->head->weight << endl;
            // // cout << i << endl;
            // break;
        }
    }

    return output;
}

int main(){
    string s = "010011000011000001000001000001101110000000001110000011001111000000100110111110000010001111110000100101101111111000011010111111110000100011011110111110111101110110111111111000100000111111111100001010011111111111000001111111111111111000010011111101111111111111001000011101111111111111100010001011011110111110110111101101111111101111111111111101111111111110111111111111111000000011111111011101111111111111101101111011111111111111111011111111111111110000010011111111011111110111111111111111110111111111111110110001001111111111111111101110000011101111111011111111111111111011110111111111111110111100100100";
    cout << decode(s) << endl;
    return 0;
}