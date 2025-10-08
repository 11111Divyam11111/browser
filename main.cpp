#include <iostream>
#include <fstream>
#include <variant>
#include <string>
#include <vector>
#include <stack>
#include <cassert>
#include <algorithm>
#include <cstdint>
#include <regex>
#include <sstream>
#include <chrono>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "./headers/html.h"
#include "./headers/css.h"
using namespace std;

void applyStyle(sf::Text& t , vector<pair<string,string>>& s){
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
 
int main(int argc, char* argv[]) {
    if(argc < 3) {
        cout << "Usage: ./prog fileName.txt" << endl;
        return 0;
    }

    // html file
    Node node;
    ifstream inputFile(argv[1]);
    assert(inputFile.is_open() && "HTML file didn't open.");
    node.__readFile__(inputFile);
    inputFile.close();

    // css file
    CSS css;
    ifstream inputCssFile(argv[2]);
    assert(inputCssFile.is_open() && "CSS file didn't open.");
    css.__CssParser__(inputCssFile);
    inputCssFile.close();   
    if(node.root == nullptr){ cout << "No Tree Found." << endl; return 0;}

//------------------------------------------SFML------------------------------------------
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf")) {
        std::cerr << "Could not load font!" << std::endl;
        return -1;
    }
    
    vector<pair<sf::Text,int>> texts;

    unordered_map<string,vector<pair<string,string>>> styles;
    for(auto r : css.Rules.rules){
            for(auto s : r.selectors){
                for(auto d : r.declarations){
                    styles[s.value].push_back({d.value,d.property});
            }
        }
    }   
   
    sf::RenderWindow window(sf::VideoMode(1920,1080), "Browser");
    float y = 20.f; 
    sf::Vector2u size = window.getSize();
    int width = size.x;
    int height = size.y;
    int fontSize = 25;
        
   
    node.__PrintDOMTree__(node.root,0,width,styles,y,texts,font,fontSize);
    sf::Texture texture;
    if (!texture.loadFromFile("me.png", sf::IntRect(0, 0, 32, 32))) {
        return 0;
    }
    texture.setSmooth(true); 
    sf::Sprite rect(texture);

    float moveSpeed = 5.f;
    sf::Vector2f position(1920,0); 
    sf::View view;
    view.reset(sf::FloatRect(0,0,width,height));
    view.setViewport(sf::FloatRect(0,0,1.f,1.f));
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Vector2u size = window.getSize();
        int width = size.x;
        int height = size.y;
        sf::Event event;
        while (window.pollEvent(event)) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
                float x = rect.getPosition().x;
                if(x + moveSpeed <= width){
                    rect.move(moveSpeed * clock.getElapsedTime().asSeconds(),0);
                }
            }  
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
                float x = rect.getPosition().x;
                if(x + moveSpeed > 0){
                    rect.move(-moveSpeed * clock.getElapsedTime().asSeconds(),0);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){               
                rect.move(0,moveSpeed * clock.getElapsedTime().asSeconds()) ;
            }  
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
                float y = rect.getPosition().y;
                if(y + moveSpeed > 0){
                    rect.move(0,-moveSpeed * clock.getElapsedTime().asSeconds()) ;
                }
            }         
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Backspace)){
                rect.setPosition(0,0);
            }

            if (event.type == sf::Event::Closed)
                window.close();
        }

      
        position.x = rect.getPosition().x + 10 - (width/8);
        position.y = rect.getPosition().y + 10 - (height/8);

        if(position.x < 0) position.x = 0;
        if(position.y < 0) position.y = 0;

        view.reset(sf::FloatRect(position.x,position.y,width,height));

        window.setView(view);
        window.draw(rect);
        for (auto& t : texts)
            window.draw(t.first);
       
        window.display();
        window.clear(sf::Color::White);
    }


//------------------------------------------SFML------------------------------------------

    node.deleteTree(node.root);
    node.root = nullptr;

    return 0;
} 