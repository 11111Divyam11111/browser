#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <cassert>

using namespace std;

struct Node {
    string tag_name, text;
    vector<Node*> children;

    Node(string tag_name, string text, vector<Node*> c = {}) 
        : tag_name(tag_name), text(text), children(c) {}
};

stack<Node*> Tree;
Node* root = nullptr;

void __TAG_OPEN__(string& tag) {
    Node* t = new Node(tag , "" ,  {});
    Tree.push(t);
}

void __TEXT__(string& text) {
    if(!Tree.empty()) Tree.top()->text += text;
}

void __TAG_CLOSE__(string& tag) {
    Node* child = nullptr; 
    if(!Tree.empty()) child = Tree.top();
    if(!Tree.empty()) Tree.pop();
    if(!Tree.empty()){
        Tree.top()->children.push_back(child);
    }else{
        root = child;
    }
}

// isko theek karna h abhi : cur1 ko use karo
void __readFile__(ifstream &file) {
    char cur1, cur2;
    if (!file.get(cur1)) return;
    if (!file.get(cur2)) return;

    while(file) {
        string openTag="", text="", closeTag="";

        // starting tag
        if(cur1 == '<' && cur2 != '/') {
            openTag += cur2;
            while(file.get(cur2) && cur2 != '>') openTag += cur2;
            __TAG_OPEN__(openTag);
            file.get(cur1);
        }

        // text
        while(cur1 != '<' && file) {
            text += cur1;
            file.get(cur1);
        }
        if(!text.empty() && text.find_first_not_of(" \t\n\r") != string::npos ) __TEXT__(text);

        // ending tag    
        if(cur1 == '<') {
            file.get(cur2);
            if(cur2 == '/') {
                while(file.get(cur2) && cur2 != '>') closeTag += cur2;
                __TAG_CLOSE__(closeTag);
                file.get(cur1); 
            }
        }
    }   
}

void __PrintDOMTree__(Node* x,int depth){
    cout << x->text << " ";
    for(auto a : x->children){
        __PrintDOMTree__(a,depth+1);
    }
    
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        cout << "Usage: ./prog fileName.txt" << endl;
        return 0;
    }

    ifstream inputFile(argv[1]);
    assert(inputFile.is_open() && "file didn't open.");
    __readFile__(inputFile);
    inputFile.close();

    __PrintDOMTree__(root,0);

    return 0;
} 