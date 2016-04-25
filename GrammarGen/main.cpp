/* 
 * File:   main.cpp
 * Author: Kenji
 *
 * Created on April 25, 2016, 12:00 AM
 */

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace std;

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

struct Rule{
    string start;
    vector<string> results;
};

struct FandF{
    vector<string> firsts;
    vector<string> follows;
};

/*
 * 
 */
int main(int argc, char** argv) {
    ifstream input;	
    vector<Rule> inputList;
    unordered_map<string, FandF> rule_map;
    
    input.open(argv[1], ifstream::in);
    Rule current;
    
    string line = "";
    vector<string> temp;
    // Grammar.part1 : <file_input> -> NL_T <file_input>
    // get input from in file
    getline(input, line);
    while (!input.fail()){
        temp = split(line, ' ');
        current.start = temp[2];
        // erase
        temp.erase (temp.begin(), temp.begin()+4);
        //if (!temp[0].empty()) temp.erase(temp.begin());
        current.results = temp;
        // add to vector
        inputList.push_back(current);
        // get next line
        getline(input, line);
    }
    input.close();
    
    for (int i = 0; i < inputList.size(); i++){
        FandF temp;
        rule_map.insert({inputList[i].start, temp});
    }
    
    return 0;
}

