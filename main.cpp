#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

using namespace std;

struct ElementData{
    string tag_name;
};

struct text{
    string tdata;
};

struct Node{
    variant<ElementData,text> node;
    vector<Node> children;
};

Node makeText(string textData){
    cout << "   text data : " << textData << endl;
    Node n;
    n.node = text{textData};
    n.children = {};

    return n;
};

Node makeElement(string tagData , vector<Node> child){
    cout <<"Element data : " << tagData << endl;
    Node n;
    n.node = ElementData{tagData};
    n.children = child;

    return n;
}

int main(){

    Node mytext = makeText("This is my test code");
    Node myElem = makeElement("h1", {
        makeElement("div", {
            makeElement("p", {})
        })
    });


    return 0;
}