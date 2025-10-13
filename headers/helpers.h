#pragma once
#include <fstream>
#include <chrono>
#include <iostream>
using namespace std;

class Helpers{
public:
    string __CovertStreamToString__(ifstream& file);
};