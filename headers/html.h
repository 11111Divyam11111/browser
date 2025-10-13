#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdint>
#include <regex>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <unordered_map>

using namespace std;

class Node{   
public:     
    Node() : tag_name(""), text(""), children({}) {}
    Node(const string& tag_name, const string& text, const vector<Node*>& c = {})
    : tag_name(tag_name), text(text), children(c) {};    


    void __TAG_OPEN__(string& tag);
    void __TEXT__(string& text);
    void __TAG_CLOSE__(string& tag);
    void __readFile__(string& file);
    static void deleteTree(Node* node);
    pair<string,int> wrap(string& text, size_t line_length);
    void __PrintDOMTree__(Node* x, int depth,int& width , unordered_map<string,vector<pair<string,string>>>&styles,float &y,vector<pair<sf::Text,int>>& texts,sf::Font& font,int& fontSize);
    void applyStyle(sf::Text& t , vector<pair<string,string>>& s);
    ~Node(){};

private:
    string tag_name,text;
    vector<Node*> children;
public:
    static stack<Node*> Tree;
    static Node* root;
};