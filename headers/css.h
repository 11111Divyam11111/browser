#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

class CSS {
public:
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

    
    string __selectorType__(char c) const;
    void __CssComment__(istream& file, char& c);
    void __SelectorParser__(istream& file, char& c);
    void __DeclarationsParser__(istream& file, char& c);
    void __PropertyParser__(istream& file, char& c);
    void __PropertyValueSeparator__(istream& file, char& c);
    void __ValueParser__(istream& file, char& c);
    void __PropertyValueEnded__(istream& file, char& c);
    void __DeclarationEndParser__(istream& file, char& c);

    void __CssParser__(istream& file);

  
    StyleSheet Rules;

private:
    int cssImp = 0;
    bool checked = false, decla = false, colon = false;
    string selecValue = "", prop = "", val = "", decValueType;
    Selector _sel;
    Declaration _del;
    Rule allRule;
  
    regex plainNumber = regex("^[0-9]+$");
    regex numberWithUnit = regex("^[0-9]+(px|em|%)$");
    regex hexColor = regex("^#([0-9a-fA-F]{3}|[0-9a-fA-F]{6})$");
};
