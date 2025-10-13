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

void Node::__readFile__(string &file) {
    size_t i = 0;
    while (i < file.size()) {
        string openTag = "", text = "", closeTag = "";
        while (i < file.size() && isspace(file[i])) i++;

        if (i < file.size() && file[i] == '<' && i + 1 < file.size() && file[i + 1] != '/') {
            i++; 
            while (i < file.size() && file[i] != '>') {
                openTag += file[i];
                i++;
            }
            __TAG_OPEN__(openTag);
            if (i < file.size()) i++; 
            continue;
        }

        while (i < file.size() && file[i] != '<') {
            text += file[i];
            i++;
        }
        if (!text.empty() && text.find_first_not_of(" \t\n\r") != string::npos)
            __TEXT__(text);

 
        if (i < file.size() && file[i] == '<' && i + 1 < file.size() && file[i + 1] == '/') {
            i += 2; 
            while (i < file.size() && file[i] != '>') {
                closeTag += file[i];
                i++;
            }
            __TAG_CLOSE__(closeTag);
            if (i < file.size()) i++; 
            continue;
        }

        i++;
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
         if (val.second == "text-decoration") {
            if(val.first == "underline"){
                t.setStyle(sf::Text::Underlined);
                continue;
            }else if(val.first == "line-through"){
                t.setStyle(sf::Text::StrikeThrough);
                continue;
            }
            continue;
        } 
        if (val.second == "color") {
            string clr = val.first;
            clr.erase(clr.begin());
            string str = clr;
            int r, g, b;
            sscanf(str.c_str(), "%02x%02x%02x", &r, &g, &b);
            sf::Uint8 R = r;
            sf::Uint8 G = g;
            sf::Uint8 B = b;
            t.setFillColor(sf::Color{R,G,B});
            continue;
        }        
        if (val.second == "font-size") {
            string sz = val.first;
            sz.erase(sz.size()-1);
            sz.erase(sz.size()-1);
            int m = stoi(sz);
            t.setCharacterSize(m);
            continue;
        } 
        continue;
    }
}

void Node::__PrintDOMTree__(Node* x, int depth,int& width , unordered_map<string,vector<pair<string,string>>>&styles,float &y,vector<pair<sf::Text,int>>& texts,sf::Font& font,int& fontSize) {
    // screen size width = 1920 , max number of char in 1 line = 190
    // t = fontSize*2 + charParLine <= 190
    // x = 0.1*fontSize + 0.1*charParLine
    // total lines = t/x
    int charPerLine = 2*(width/(fontSize+2));
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
                applyStyle(t,styles["*"]);   
                applyStyle(t,styles[x->tag_name]);
                
            //--------------Styling--------------

            int totalLine = (totalChars*4)/charPerLine;
            if(totalLine == 0) totalLine++;
            t.setPosition(10.f, y+10.f);    
            y += totalLine * 1.3f * (lineSpace+fontSize);  
            texts.push_back({t, numOfWords});
        }
        else{
            //--------------Styling--------------
                sf::Text t(value, font, fontSize); 
                t.setFillColor(sf::Color::Black);
                applyStyle(t,styles["*"]);                   
                applyStyle(t,styles[x->tag_name]);
                                
            //--------------Styling--------------

            int totalLine = (totalChars*4)/charPerLine;
            if(totalLine == 0) totalLine++;
            t.setPosition(40.f, y);    
            y += totalLine * 1.3f * (lineSpace+fontSize-totalLine-1);  
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