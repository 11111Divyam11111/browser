#include "css.h"
#include <cctype>

using namespace std;

string CSS::__selectorType__(char c) const {
    switch (c) {
        case '.': return "Class";
        case '#': return "Id";
        case '*': return "Universal";
        default:  return "Tag";
    }
}

void CSS::__CssComment__(const string& s, size_t& i) {   
    size_t n = s.size();
    if (i + 1 >= n || s[i] != '/' || s[i+1] != '*') return;
    i += 2;
    while (i + 1 < n) {
        if (s[i] == '*' && s[i+1] == '/') { i += 2; return; }
        ++i;
    }  
}

void CSS::__SelectorParser__(const string& s, size_t& i) {   
    size_t n = s.size();   
    while (i < n && isspace((unsigned char)s[i])) ++i;
    while (i < n && s[i] != '{') {      
        if (s[i] == '/' && i + 1 < n && s[i+1] == '*') break; 
        selecValue.push_back(s[i]);
        ++i;
    }   
    while (!selecValue.empty() && isspace((unsigned char)selecValue.back())) selecValue.pop_back();
}

void CSS::__DeclarationsParser__(const string& s, size_t& i) {  
    size_t n = s.size();
    if (i < n && s[i] == '{') ++i;
    
    if (!selecValue.empty()) _sel.type = __selectorType__(selecValue[0]);
    _sel.value = selecValue;
    allRule.selectors.push_back(_sel);
    decla = true;
    
    prop.clear();
    val.clear();
    colon = false;
}

void CSS::__PropertyParser__(const string& s, size_t& i) {
    size_t n = s.size();

    while (i < n && isspace((unsigned char)s[i])) ++i;
    while (i < n && s[i] != ':' && s[i] != '}' && s[i] != ';') {
        prop.push_back(s[i]);
        ++i;
    }
  
    while (!prop.empty() && isspace((unsigned char)prop.back())) prop.pop_back();
}

void CSS::__PropertyValueSeparator__(const string& s, size_t& i) {
  
    if (i < s.size() && s[i] == ':') ++i;
    colon = true;
  
    while (i < s.size() && isspace((unsigned char)s[i])) ++i;
}

void CSS::__ValueParser__(const string& s, size_t& i) {
   
    size_t n = s.size();
    while (i < n && s[i] != ';' && s[i] != '}') {
        val.push_back(s[i]);
        ++i;
    }
   
    while (!val.empty() && isspace((unsigned char)val.back())) val.pop_back();
}

void CSS::__PropertyValueEnded__(const string& s, size_t& i) {
    colon = false;
    cssImp++;   
  
    while (!prop.empty() && isspace((unsigned char)prop.front())) prop.erase(prop.begin());
    while (!val.empty() && isspace((unsigned char)val.front())) val.erase(val.begin());

    _del.property = prop;
    _del.value = val;

    string decValueType;
    if (regex_match(val, hexColor)) {
        decValueType = "Color";
    } else if (regex_match(val, numberWithUnit)) {
        decValueType = "Unit";
    } else if (regex_match(val, plainNumber)) {
        decValueType = "Numeric";
    } else {
        decValueType = "Keyword/String";
    }
    _del.valueType = decValueType;
    allRule.declarations.push_back(_del);   
    prop.clear();
    val.clear();
    if (i < s.size() && s[i] == ';') ++i; 
}

void CSS::__DeclarationEndParser__(const string& s, size_t& i) {   
    if (i < s.size() && s[i] == '}') ++i;
    Rules.rules.push_back(allRule);
  
    cssImp = 0;
    decla = false;
    colon = false;
    checked = false;
    selecValue.clear();
    _sel.type.clear();
    _sel.value.clear();
    _del.property.clear();
    _del.value.clear();
    allRule.declarations.clear();
    allRule.selectors.clear();
}

void CSS::__CssParser__(const string& s) {
    size_t i = 0;
    size_t n = s.size();

    cssImp = 0;
    checked = decla = colon = false;
    selecValue.clear();
    prop.clear();
    val.clear();
    allRule = Rule();
    Rules.rules.clear();

    while (i < n) {      
        if (isspace((unsigned char)s[i])) { ++i; continue; }       
        if (s[i] == '/' && i + 1 < n && s[i+1] == '*') {
            __CssComment__(s, i);
            continue;
        }
        if (!decla && s[i] != '{') {
            __SelectorParser__(s, i);
            continue;
        }
        if (!decla && s[i] == '{') {
            __DeclarationsParser__(s, i);
            continue;
        }
        if (decla) {
            if (s[i] == '}') {
                __DeclarationEndParser__(s, i);
                continue;
            } else if (s[i] == ':') {
                __PropertyValueSeparator__(s, i);
                continue;
            } else if (s[i] == ';') {
                __PropertyValueEnded__(s, i);
                continue;
            } else {
                // property or value char
                if (!colon) {
                    __PropertyParser__(s, i);
                } else {
                    __ValueParser__(s, i);
                }
                continue;
            }
        }       
        ++i;
    }
        
    if (decla && !prop.empty() && !val.empty()) {
        _del.property = prop;
        _del.value = val;
        string decValueType;
        if (regex_match(val, hexColor)) decValueType = "Color";
        else if (regex_match(val, numberWithUnit)) decValueType = "Unit";
        else if (regex_match(val, plainNumber)) decValueType = "Numeric";
        else decValueType = "Keyword/String";
        _del.valueType = decValueType;
        allRule.declarations.push_back(_del);
        Rules.rules.push_back(allRule);

        
    }
}
