#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <bits/stdc++.h>    //Neu khi compile bao loi dong nay thi // no
using namespace std;

struct Node{
    char ch;
    int fr;
    Node *left, *right;
};

Node* newNode(char ch, int fr, Node* left, Node* right){
    Node* node = new Node();

    node -> ch = ch;
    node -> fr = fr;
    node -> left = left;
    node -> right = right;
    
    return node;
}

//comp for priority_queue
struct comp{
    bool operator()(Node* left, Node* right){
        //increasing order 
        return left -> fr > right -> fr;
    }
};

void encode(Node* root, string str, unordered_map<char,string>&huffmanCode);
void decode(Node* root, int &index, string str);
void buildHuffmanTree(string str);

int main(){
    string str;
    cout<<"Input your string: ";
    getline(cin,str);
    buildHuffmanTree(str);
    //getchar();
}   

void encode(Node* root, string str, unordered_map<char,string>&huffmanCode){
    if(root==nullptr) return;

    //leaf node
    if(!root->left && !root->right){
        if(root->ch!='\0')                      //doesnt assign a code for dummy node
            huffmanCode[root->ch] = str;
    }

    //di trai = 0, di phai = 1
    encode(root->left,str+"0",huffmanCode);
    encode(root->right,str+"1",huffmanCode);
}

void decode(Node* root, int &index, string str){
    if(root==nullptr){
        return;
    }

    //leaf node
    if(!root->left && !root->right){
        cout<<root->ch;
        return;
    }
    index++;
    if(str[index]=='0')
        decode(root->left,index,str);
    else
        decode(root->right,index,str);
}

void buildHuffmanTree(string str){
    //count frequency and store in a map
    unordered_map<char,int> freq;
    for(char ch: str){
        freq[ch]++;
    }

    //priority queue to store nodes of the huffman tree
    priority_queue<Node*,vector<Node*>,comp> pq;
    
    //create a node for each character and push in the priority queue by increasing freq order
    for(auto pair: freq){
        pq.push(newNode(pair.first,pair.second,nullptr,nullptr));
    }
    //only 1 character so add a dummy node with char '\0' with 0 freq to be able to create a tree
    if(pq.size()==1){
        pq.push(newNode('\0',0,nullptr,nullptr));
    }
    //combines 2 minimum nodes until there are only 1 nodes left
    while(pq.size()!=1){
        
        //make the smallest freq node the left node
        //the 2nd smallest freq node the right node
        //and remove them
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();

        int sum = left->fr + right->fr;
        //create a new node with freq equals those 2 left and right node, make it the left node and right node of the
        //new node and push into the queue
        pq.push(newNode('\0',sum,left,right));
    }

    //root of the huffman tree is the only node left
    Node* root = pq.top();

    //traverse the tree and store the code
    unordered_map<char, string> huffmanCode;
    encode(root,"",huffmanCode);

    cout<<"Huffman Codes are:\n"<<'\n';
    for(auto pair: huffmanCode){
        cout<<pair.first<<" "<<pair.second<<'\n';
    }

    cout<<"\nOriginal string was:\n"<<str<<'\n';

    string code = "";
    for(char ch: str){
        code += huffmanCode[ch];
    }

    cout<<"\nEncoded string is:\n"<<code<<'\n';

    int index = -1;
    cout<<"\nDecoded string is: \n";
    while(index < (int)code.size() - 1){
        decode(root,index,code);
    }
    
}