#include "html.h"

stack<Node*> Node::Tree;
Node* Node::root = nullptr;

void Node::__TAG_OPEN__(string& tag) {
    Node* t = new Node(tag , "" ,  {});
    Tree.push(t);
}

void Node::__TEXT__(string& text) {
    if(!Tree.empty()) Tree.top()->text += text;
}

void Node::__TAG_CLOSE__(string& tag) {
    Node* child = nullptr; 
    if(!Tree.empty()) child = Tree.top();
    if(!Tree.empty()) Tree.pop();
    if(!Tree.empty()){
        Tree.top()->children.push_back(child);
    }else{
        root = child;
    }
}

void Node::__readFile__(ifstream &file) {
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

pair<string,int> Node::wrap(string&  text, size_t line_length){
    istringstream words(text);
    ostringstream wrapped;
    string word;
    int bwords = 0;
    if (words >> word) {
        wrapped << word;
        size_t space_left = line_length - word.length();
        while (words >> word) {
            bwords++;
            if (space_left < word.length() + 1) {
                wrapped << '\n' << word;
                space_left = line_length - word.length();
            } else {
                wrapped << ' ' << word;
                space_left -= word.length() + 1;
            }
        }
    }
    return {wrapped.str(),bwords+1};
}

void Node::applyStyle(sf::Text& t , vector<pair<string,string>>& s){
    for(auto& val : s){
        if (val.second == "color") {
            string clr = val.first;
            clr.erase(clr.begin());
            string str = clr;
            int r, g, b;
            sscanf(str.c_str(), "%02x%02x%02x", &r, &g, &b);
            sf::Uint8 R = r;
            sf::Uint8 G = b;
            sf::Uint8 B = b;
            t.setFillColor(sf::Color{R,G,B});
        } 
        if (val.second == "text-decoration") {
            if(val.first == "underline"){
                t.setStyle(sf::Text::Underlined);
            }else if(val.first == "line-through"){
                t.setStyle(sf::Text::StrikeThrough);
            }
        } 
        if (val.second == "font-size") {
            string sz = val.first;
            sz.erase(sz.size()-1);
            sz.erase(sz.size()-1);
            int m = stoi(sz);
            t.setCharacterSize(m);
        } 
    }
}

void Node::__PrintDOMTree__(Node* x, int depth,int& width , unordered_map<string,vector<pair<string,string>>>&styles,float &y,vector<pair<sf::Text,int>>& texts,sf::Font& font,int& fontSize) {
    // screen size width = 1920 , max number of char in 1 line = 190
    // t = fontSize*2 + charParLine <= 190
    // x = 0.1*fontSize + 0.1*charParLine
    // total lines = t/x
    int charPerLine = (width/10) - 2*fontSize;
    int lineSpace = 20;
    pair<string,int> strWord = wrap(x->text,charPerLine);
    auto value = strWord.first;
    int numOfWords = strWord.second;
    int totalChars = numOfWords * 2;
    if (!value.empty()) { 
        if(x->tag_name[0] == 'h'){
            sf::Text t(value, font, 35);
            //--------------Styling--------------                   
                if(x->tag_name[1] == '2') t.setCharacterSize(30);
                if(x->tag_name[1] == '3') t.setCharacterSize(27);
                if(x->tag_name[1] == '4') t.setCharacterSize(24);
                if(x->tag_name[1] == '5') t.setCharacterSize(21);
                t.setFillColor(sf::Color::Black);
                applyStyle(t,styles[x->tag_name]);                   
                t.setStyle(sf::Text::Bold);
                
            //--------------Styling--------------

            int totalLine = (totalChars*4)/charPerLine;
            if(totalLine == 0) totalLine++;
            t.setPosition(10.f, y);    
            y += totalLine * 1.0f * (lineSpace+fontSize);  
            texts.push_back({t, numOfWords});
        }
        else{
            //--------------Styling--------------
                sf::Text t(value, font, fontSize); 
                t.setFillColor(sf::Color::Black);
                applyStyle(t,styles[x->tag_name]);
                                
            //--------------Styling--------------

            int totalLine = (totalChars*4)/charPerLine;
            if(totalLine == 0) totalLine++;
            t.setPosition(40.f, y);    
            y += totalLine * 1.0f * (lineSpace+fontSize-totalLine-1);  
            texts.push_back({t, numOfWords});
        }
    }
    for (auto a : x->children) {
        __PrintDOMTree__(a, depth + 1,width,styles,y,texts,font,fontSize);
    }
}; 

void Node::deleteTree(Node* node) {
    if (!node) return;

    for (auto child : node->children) {
        deleteTree(child); 
    }

    delete node; 
};