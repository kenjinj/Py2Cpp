/* 
 * File:   GrammarGen.cpp
 * Author: Kenji Johnson
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
#include <algorithm>

using namespace std;

// Functions to split a string into a vector using a delimeter
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

// Data type for parsed input lines
struct Rule{
    string start;
    vector<string> results;
};

// Data types to store firsts and follows for each NT key
struct FandF{
    string key;
    vector<string> firsts;
    vector<string> follows;
};

// Helper function for sorting
bool ruleListCompare (FandF a, FandF b){
    return (a.key < b.key);
}

// Vector for parsed input
vector<Rule> inputList;
// Vector of unique non-terminals and its firsts and follows
vector<FandF> ruleList;
// Metrics for number of times rules applied
int aCount, bCount, cCount, dCount, eCount, fCount;

// Helper function to find index of non-terminal(string) in FF vector
int findKeyIndex(string toFind){
    for (int i = 0; i < ruleList.size(); i++){
        if (toFind == ruleList[i].key) return i;
    }
    return -1;
}

// Needs 2 parameters for when Rule B applies Rule A using 2 lines
bool ruleA(int listIndex, int checkIndex){
    if (inputList[checkIndex].results[0].front() != '<'){
        bool exists = false;
        // Check if already in list
        for (int i = 0; i < ruleList[findKeyIndex(inputList[listIndex].start)].firsts.size(); i++)
            if (inputList[checkIndex].results[0] == ruleList[findKeyIndex(inputList[listIndex].start)].firsts[i]) exists = true;
        if (!exists){
            ruleList[findKeyIndex(inputList[listIndex].start)].firsts.push_back(inputList[checkIndex].results[0]);
            aCount++;
            return true;
        }
    }
    return false;
}

void ruleB(int listIndex){
    // Does the rule at listIndex have a non terminal 'first'?
    if (inputList[listIndex].results[0].front() == '<'){
        vector<string> NTList;
        NTList.push_back(inputList[listIndex].results[0]);
        while (NTList.size() > 0){
            string current = NTList.back();
            NTList.pop_back();
            for (int j = 0; j < inputList.size(); j++){
                if (inputList[j].start == current){
                    bool wasTerminal = ruleA(listIndex, j);
                    // If checking first non-terminal's rule also had a non-terminal, add to list to check
                    if (!wasTerminal)
                        NTList.push_back(inputList[j].results[0]);
                    else
                        bCount++;
                }
            }
        }
    }
}

void ruleC(int listIndex){
    // For rules that only go to empty set (lambda), the start of rules(index) follow set is also in firsts set
    if (inputList[listIndex].results.size() == 1){
        if (inputList[listIndex].results[0] == "{}" || inputList[listIndex].results[0] == "LAMBDA_T"){
            int index = findKeyIndex(inputList[listIndex].start);
            if (index != -1){
                for (int i = 0; i < ruleList[index].follows.size(); i++){
                    bool exists = false;
                    for (int j = 0; j < ruleList[index].firsts.size(); j++)
                        if (ruleList[index].firsts[j] == ruleList[index].follows[i]) exists = true;
                    if (!exists){
                        ruleList[index].firsts.push_back(ruleList[index].follows[i]);
                        cCount++;
                    }
                }
            }
        }
    }
}

void ruleD(int listIndex){
    // Add terminals that follow a non-terminal to that non-terminal's follow set
    string last, current;
    for (int i = 0; i < inputList[listIndex].results.size(); i++){
        if (i == 0) current = inputList[listIndex].results[i];
        else {
            last = current;
            current = inputList[listIndex].results[i];
            if (last.back() == '>' && current.front() != '<'){
                bool exists = false;
                // Check if already in list
                int index = findKeyIndex(last);
                if (index != -1){
                    for (int j = 0; j < ruleList[index].follows.size(); j++)
                        if (current == ruleList[index].follows[j]) exists = true;
                    if (!exists){
                        ruleList[index].follows.push_back(current);
                        dCount++;
                    }
                }
            }
        }    
    }
}

void ruleE(int listIndex){
    // Add Firsts of current to Follows of last
    string last, current;
    for (int i = 0; i < inputList[listIndex].results.size(); i++){
        if (i == 0) current = inputList[listIndex].results[i];
        else {
            last = current;
            current = inputList[listIndex].results[i];
            if (last.back() == '>' && current.front() == '<'){
                int indexC = findKeyIndex(current);
                int indexL = findKeyIndex(last);
                if (indexC != -1 && indexL != -1){
                    for (int j = 0; j < ruleList[indexC].firsts.size(); j++){
                        bool exists = false;
                        for (int k = 0; k < ruleList[indexL].follows.size(); k++)
                            if (ruleList[indexC].firsts[j] == ruleList[indexL].follows[k]) exists = true;
                        if (!exists){
                            ruleList[indexL].follows.push_back(ruleList[indexC].firsts[j]);
                            eCount++;
                        }
                    }
                }
            }
        }
    }
}

void ruleF(int listIndex){
    // The follows of the start of a rule(A), are also the follows of the end of that rules expansion(B)
    if (inputList[listIndex].results.back().back() == '>'){
        int indexA = findKeyIndex(inputList[listIndex].start);
        int indexB = findKeyIndex(inputList[listIndex].results.back());
        if (indexA != -1 && indexB != -1){
            for (int i = 0; i < ruleList[indexA].follows.size(); i++){
                bool exists = false;
                for (int j = 0; j < ruleList[indexB].follows.size(); j++)
                    if (ruleList[indexA].follows[i] == ruleList[indexB].follows[j]) exists = true;
                if (!exists){
                    ruleList[indexB].follows.push_back(ruleList[indexA].follows[i]);
                    fCount++;
                }
            }
        }
    }
    
}

/*
 * Main function
 */
int main(int argc, char** argv) {
    ifstream input;	
    FILE * report;
    FILE * grammarFF;
    input.open(argv[1], ifstream::in);
    report = fopen("GrammarGenReport.txt", "w");
    grammarFF = fopen("GrammarFirstsFollows.txt", "w");
    fprintf(report, "Starting generation of firsts and follows on file '%s'\n\n", argv[1]);
    
    Rule current;
    
    aCount = bCount = cCount = dCount = eCount = fCount = 0;
    
    string line = "";
    vector<string> temp;
    // Grammar.part1 : <file_input> -> NL_T <file_input>
    // get input from in file
    getline(input, line);
    fprintf(report, "Input format:\n%s\n\n", line.c_str());
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
    
    // Get each unique rule start
    for (int i = 0; i < inputList.size(); i++){
        FandF temp;
        bool found = false;
        for (int j = 0; j < ruleList.size() && !found; j++){
            found = false;
            if (inputList[i].start == ruleList[j].key) found = true;
        }
        if (!found){
            temp.key = inputList[i].start;
            ruleList.push_back(temp);  
        }
    }
    
    // Find non-dependent firsts & follows
    for (int i = 0; i < inputList.size(); i++){
        // firsts
        ruleA(i, i);
        ruleB(i);
        // follows
        ruleD(i);
    }
    // fix counter
    aCount -= bCount;
    fprintf(report, "Non-dependent Report (Rules A, B, D):\n\t%d new firsts, %d new follows\n", aCount + bCount, dCount);
    
    // Find follows dependent on firsts, need to recheck after final firsts.
    for (int i = 0; i < inputList.size(); i++){
        ruleE(i);
    }
    
    // Find follows dependent on other follows, need to recheck after final follows.
    for (int i = 0; i < inputList.size(); i++){
        ruleF(i);
    }
    fprintf(report, "F&F-dependent Report (Rules E, F):\n\t%d new follows\n", eCount + fCount);
    
    /* Initial follows found */
    fprintf(report, "Initial set of follows found\n");
    
    // Find firsts dependent on follows, need to recheck after final follows
    for (int i = 0; i < inputList.size(); i++){
        ruleC(i);
    }
    fprintf(report, "Follows-dependent Report (Rule C):\n\t%d new firsts\n", cCount);
    
    /* Initial firsts found */
    fprintf(report, "Initial set of firsts found\n");
    
    // Find remaining F&F with inter dependencies, finished when no change detected
    fprintf(report, "F&F-Dependent Report (Rules E, F, C in loop):\n");
    int firstsTemp;
    int followsTemp; 
    do{
        firstsTemp = cCount;
        followsTemp = eCount + fCount;
        for (int i = 0; i < inputList.size(); i++){
            ruleE(i);
            ruleF(i);
        }
        for (int i = 0; i < inputList.size(); i++){
            ruleC(i);
        }
        fprintf(report, "\t%d new firsts, %d new follows\n", cCount - firstsTemp, eCount + fCount - followsTemp);
    } while (firstsTemp != cCount && followsTemp != eCount + fCount);
    
    // Sort for readability
    sort(ruleList.begin(), ruleList.end(), ruleListCompare);
    for (int i = 0; i < ruleList.size(); i++){
        sort(ruleList[i].firsts.begin(), ruleList[i].firsts.end());
        sort(ruleList[i].follows.begin(), ruleList[i].follows.end());
    }
    
    // Print results
    for (int i = 0; i < ruleList.size(); i++){
        fprintf(grammarFF, "%s\n\tFirsts: ", ruleList[i].key.c_str());
        for (int j = 0; j < ruleList[i].firsts.size(); j++)
            fprintf(grammarFF, "%s ", ruleList[i].firsts[j].c_str());
        fprintf(grammarFF, "\n\tFollows: ");
        for (int j = 0; j < ruleList[i].follows.size(); j++)
            fprintf(grammarFF, "%s ", ruleList[i].follows[j].c_str());
        fprintf(grammarFF, "\n\n");
    }
    
    fprintf(report, "\nFinished finding all firsts and follows\n");
    fprintf(report, "Rule A applied %d times.\nRule B applied %d times.\n", aCount, bCount);
    fprintf(report, "Rule C applied %d times.\nRule D applied %d times.\n", cCount, dCount);
    fprintf(report, "Rule E applied %d times.\nRule F applied %d times.\n", eCount, fCount);
    
    fclose(report);
    fclose(grammarFF);
    
    cout << "GrammarGen execution complete.\n\tReport saved to GrammarGenReport.txt\n\tList of firsts and follows saved to GrammarFirstsFollows.txt\n";
    
    return 0;
}

