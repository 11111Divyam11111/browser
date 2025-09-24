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
   int t = 0;


   void __tagAndText__(string tag , string text){
    cout << "<"<<tag<<">"<<text<<"<"<<tag<<">"<<endl;
   }

   
   void __tree__(Node a){
        queue<Node> q;
        q.push(a);
        t++;
        while(!q.empty()){
            auto x = q.front();
            t++;
            q.pop();
            for(auto a : x.children){
                for(int i=0;i<t*2;i++)cout<<" ";
                __tagAndText__(a.tag_name , a.text);
                q.push(a);
            }
        }
        return;
   }
};

int main(){

    Node div("p","p inside div",{});
    Node div4("div4","div4 inside div3",{div,div});
    Node div5("div5","div5 inside div3",{div,div});
    Node div6("div6","div6 inside div3",{div});
    Node div3("div3","div3 inside div2",{div4,div,div,div,div,div5,div6});
    Node div2("div2","div2 inside header",{div3,div});
    Node div1("div1","div1 inside header",{div,div});
    Node html("h1","header",{div1, div2,div});
    Node root("root","root",{html});
    root.__tree__(root);

    return 0;
}