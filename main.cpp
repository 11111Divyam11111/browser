#include <iostream>
#include <fstream>
#include <variant>
#include <optional>
#include <string>
#include <vector>
#include <stack>
#include <cassert>
#include <algorithm>
#include <cstdint>
#include <regex>
#include <sstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

// -------------------------HTML PARSER-------------------------   

        pair<string,int> wrap(auto text, size_t line_length)
        {
            std::istringstream words(text);
            std::ostringstream wrapped;
            std::string word;
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

       

        void deleteTree(Node* node) {
            if (!node) return;

            for (auto child : node->children) {
                deleteTree(child); 
            }

            delete node; 
        }
// -------------------------HTML PARSER-------------------------


// ---------------------------CSS Style------------------------

    struct Selector {
        string type = "";
        string value = "";
    };

    struct Declaration {
        string property;
        string value;
        string valueType;
    };

    struct Rule {
        vector<Selector> selectors;
        vector<Declaration> declarations;
    };

    struct StyleSheet {
        vector<Rule> rules;
    };
// ---------------------------CSS Style-------------------------

// ---------------------------CSS PARSER-------------------------
// ------------------------------CSS HELPERS------------------------------
    string __selectorType__(auto c){
        switch (c){
            case '.':
                return "Class";
            case '#':
                return "Id";
            case '*':
                return "Universal";
            default:
                return "Tag";
        }
    }

// ------------------------------CSS HELPERS------------------------------
    // ---------------------------CSS VALUES---------------------------
    int cssImp = 0;
    bool checked = false , decla = false , colon = false;
    string selecValue = "" , prop = "" , val = "" ,decValueType;
    Selector _sel;
    Declaration _del;
    Rule allRule;
    StyleSheet Rules;
    std::regex plainNumber("^[0-9]+$");       // e.g., 400, 600
    std::regex numberWithUnit("^[0-9]+(px|em|%)$"); // e.g., 12px, 45em, 14%
    std::regex hexColor("^#([0-9a-fA-F]{3}|[0-9a-fA-F]{6})$");

    // ---------------------------CSS VALUES---------------------------


	void __SelectorParser__(ifstream& file,char& c){
        if(_sel.type == ""){ 
            string s = __selectorType__(c);
            _sel.type = s;
        }
        selecValue += c;
	}
	
	void __DeclarationsParser__(ifstream& file,char& c){
        _sel.value = selecValue;      
        allRule.selectors.push_back(_sel);
        decla = true;
	}
	
    void __PropertyParser__(ifstream& file , char& c){
        prop += c;
    }

    void __PropertyValueSeparator__(ifstream& file , char& c){
        colon = true;
    }
    
    void __ValueParser__(ifstream& file , char& c){
        val += c;
    }

    void __PropertyValueEnded__(ifstream& file, char& c) {
        colon = false;
        cssImp++;

        _del.property = prop;
        _del.value = val;        
        
        string decValueType = "";
        if (std::regex_match(val, hexColor)) {
            decValueType = "Color";
        } 
        else if (std::regex_match(val, numberWithUnit)) {
            decValueType = "Unit";
        } 
        else if (std::regex_match(val, plainNumber)) {
            decValueType = "Numeric";
        } 
        else {
            decValueType = "Keyword/String";
        }
        _del.valueType = decValueType;
        allRule.declarations.push_back(_del);
        prop = "";
        val = "";
        decValueType = "";
    }
    
    void __DeclarationEndParser__(ifstream& file, char& c){
        Rules.rules.push_back(allRule);
        cssImp = 0;
        decla = false;
        colon  = false;
        checked = false;
        selecValue = "" , _sel.type = "" , _sel.value = "";
        _del.property = "" , _del.value = "";
        allRule.declarations = {};
        allRule.selectors = {};
    } 

	void __CssParser(ifstream& file){
		char c;
		while(file){
			file >> c;
			if(c != '{' && decla == false){ 
                __SelectorParser__(file,c);
            }
            else if(c == '{') {
                __DeclarationsParser__(file,c);
            }
            else if(c != '{' && c!='}' && decla == true){
                if(c!=':' and colon == false){
                    __PropertyParser__(file,c);
                }else if(c == ':'){
                    __PropertyValueSeparator__(file,c);
                }else if(c != ':' and c!=';' and colon == true){
                    __ValueParser__(file,c);
                }else if(c == ';' and colon == true){
                    __PropertyValueEnded__(file,c);
                }
            }
            else if(c=='}'){
                __DeclarationEndParser__(file,c);
            }
		}
	}

// ---------------------------CSS PARSER-------------------------


int main(int argc, char* argv[]) {
    if(argc < 3) {
        cout << "Usage: ./prog fileName.txt" << endl;
        return 0;
    }

    // html file
    ifstream inputFile(argv[1]);
    assert(inputFile.is_open() && "HTML file didn't open.");
    __readFile__(inputFile);
    inputFile.close();

    // css file
    ifstream inputCssFile(argv[2]);
    assert(inputCssFile.is_open() && "CSS file didn't open.");
    __CssParser(inputCssFile);
    cout<<endl;
    inputCssFile.close();   
    cout << "----------------------------" << endl;

        for(auto a : Rules.rules){
            for(auto i : a.selectors){
                cout << "For selector " << i.type <<i.value<< " we have these properties : " << endl;
                for(auto j : a.declarations){
                    cout << "    ";
                    cout << j.property << " " << j.value << " " << j.valueType << endl; 
                }
            }   
        }
   
    cout << "----------------------------" << endl;

    if(root == nullptr){ cout << "No Tree Found." << endl; return 0;}


//------------------------------------------SFML------------------------------------------
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf")) {
        std::cerr << "Could not load font!" << std::endl;
        return -1;
    }

    std::vector<pair<sf::Text,int>> texts;
 
    sf::RenderWindow window(sf::VideoMode(1920,1080), "Browser");
    float y = 20.f; 
    sf::Vector2u size = window.getSize();
    int width = size.x;
    function<void(Node*, int)> __PrintDOMTree__ = [&](Node* x, int depth) {
        // screen size width = 1920 , max number of char in 1 line = 190
        // t = fontSize*2 + charParLine <= 190
        // x = 0.1*fontSize + 0.1*charParLine
        // total lines = t/x
        int fontSize = 25; 
        int charPerLine = (width/10) - 2*fontSize;
        int lineSpace = 15;
        pair<string,int> strWord = wrap(x->text,charPerLine);
        auto value = strWord.first;
        int numOfWords = strWord.second;
        int totalChars = numOfWords * 2;
        if (!value.empty()) {
            if(x->tag_name == "h1"){
                sf::Text t(value, font, 35);
                t.setFillColor(sf::Color::Red);
                t.setStyle(sf::Text::Bold | sf::Text::Underlined);
                int totalLine = (totalChars*4)/charPerLine;
                if(totalLine == 0) totalLine++;
                t.setPosition(860.0f, y);    
                y += totalLine * 1.0f * (lineSpace+fontSize);  
                texts.push_back({t, numOfWords});
            }
            else{
                sf::Text t(value, font, fontSize);
                t.setFillColor(sf::Color::Black);
                int totalLine = (totalChars*4)/charPerLine;
                if(totalLine == 0) totalLine++;
                t.setPosition(10.f, y);    
                y += totalLine * 1.0f * (lineSpace+fontSize-totalLine-1);  
                texts.push_back({t, numOfWords});
            }
        }
        for (auto a : x->children) {
            __PrintDOMTree__(a, depth + 1);
        }
    };

    __PrintDOMTree__(root, 0);
    cout << "Number of different text/para : " << texts.size() << endl;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        for (auto& t : texts)
            window.draw(t.first);
        window.display();
    }


//------------------------------------------SFML------------------------------------------

    deleteTree(root);
    root = nullptr;

    return 0;
}
