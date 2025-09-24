#include <iostream>
#include <string>
#include <vector>
#include <typeinfo> 
#include <queue>
using namespace std;

struct Node{
   string tag_name , text;
   vector<Node> children;

   Node(string tag_name , string text , vector<Node> c){
        this->tag_name = tag_name;
        this->text = text;
        this->children = c;
   }

   void __tagAndText__(string tag , string text){
    cout << "<"<<tag<<">"<<text<<"<"<<tag<<">"<<endl;
   }

   
   void __tree__(Node x,int depth){
            for(int i = 0; i < depth * 2; i++)
                cout << " ";
                __tagAndText__(x.tag_name , x.text);    
            for(auto a : x.children){
                __tree__(a,depth+1);
            }
        return;
    }
   
};

int main(){

   
    Node div4("div4","div4 inside div3",{});
    Node div5("div5","div5 inside div3",{});
    Node div6("div6","div6 inside div3",{});
    Node div3("div3","div3 inside div2",{div4,div5,div6});
    Node div2("div2","div2 inside header",{div3});
    Node div1("div1","div1 inside header",{});
    Node html("h1","header",{div1, div2});
    Node root("root","root",{html});
    root.__tree__(root,0);

    return 0;
}