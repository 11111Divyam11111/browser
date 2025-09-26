#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <cassert>

using namespace std;

struct Node {
    string tag_name, text;
    vector<Node> children;

    Node(string tag_name, string text, vector<Node> c = {}) 
        : tag_name(tag_name), text(text), children(c) {}
};

// Build DOM tree
void __TAG_OPEN__(const string& tag) {
    cout << tag;
}

void __TEXT__(const string& text) {
    cout << text;
}

void __TAG_CLOSE__(const string& tag) {
    cout << tag;
}


void __readFile__(ifstream &file) {
    char cur1, cur2;
    if (!file.get(cur1)) return;
    if (!file.get(cur2)) return;

    while(file) {
        string openTag="", text="", closeTag="";

        // Opening tag
        if(cur1 == '<' && cur2 != '/') {
            openTag += cur1;
            openTag += cur2;
            while(file.get(cur2) && cur2 != '>') openTag += cur2;
            openTag += '>';
            __TAG_OPEN__(openTag);
            file.get(cur1); // move to text
        }

        // Text
        while(cur1 != '<' && file) {
            text += cur1;
            file.get(cur1);
        }
        if(!text.empty()) __TEXT__(text);

        // Closing tag
        if(cur1 == '<') {
            file.get(cur2);
            if(cur2 == '/') {
                closeTag += "<";
                closeTag += "/";
                while(file.get(cur2) && cur2 != '>') closeTag += cur2;
                closeTag += '>';
                __TAG_CLOSE__(closeTag);
                file.get(cur1); 
            }
        }
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


    return 0;
}
