#include <iomanip>
#include <cstdlib>
#include "LexicalAnalyzer.h"

using namespace std;

static string token_names[] = {	"EOF_T"}; 

LexicalAnalyzer::LexicalAnalyzer (char * filename)
{
    // This function will initialize the lexical analyzer class
    input.open(filename, ifstream::in);
    string outfileName = string(filename);
    outfileName += ".lst";
    listing.open(outfileName.c_str(), ofstream::out);
    outfileName = string(filename);
    outfileName += ".dbg";
    debug.open(outfileName.c_str(), ofstream::out);
    
    token = NONE;
    line = "";
    linenum = 0;
    pos = 0;
    lexeme = "";
    errors = 0;
    cs = 1;
}

LexicalAnalyzer::~LexicalAnalyzer ()
{
    // This function will complete the destruction of the lexical analyzer class
}

token_type LexicalAnalyzer::GetToken ()
{
    // This function will find the next lexeme int the input file and return
    // the token_type value associated with that lexeme
    lexeme = "";
    // If position out of current line bounds
    if (pos >= line.length()){
        // Try to get new line from input
        getline(input, line);
        if (input.fail()){
            token = EOF_T;
            return token;
        }
        linenum++;
        pos = 0;
        cout << linenum << " " << line << endl;
    }
    // While it is within bounds of current line
    while (pos < line.length()){
        bool lookupWord = false;
        char c = line[pos++];
        lexeme += c;
        //cout << "lexeme is now: '" << lexeme << "'" << endl;
        if (isalpha(c)) c = 'a';
        if (isdigit(c)) c = '0';
        int col = 0;
        while (c != valid[col] && col < valid.length()) col++;
        //int tempcs = cs;
        cs = stateTable[cs][col];
        //printf ("Current state is %d (from [%d][%d])\n", cs, tempcs, col);
        switch (cs){
            case -1:
                // ERROR
                return ERROR_T;
            case 3:
                // Identifier found for hash table
                putback();
                lookupWord = true;
                break;
            case 5:
                // Found only numerics
                cs = 0;
                putback();
                return INTLIT_T;
            case 8:
            case 9:
            case 10:
            case 11:
            case 13:
            case 14:
            case 16:
            case 17:
            case 20:
            case 21:
            case 22:
            case 23:
            case 26:
            case 27:
            case 28:
            case 29:
            case 31:
            case 32:
            case 34:
            case 35:
            case 37:
            case 38:
            case 40:
            case 41:
            case 43:
            case 44:
            case 45:
            case 46:
            case 47:
            case 48:
            case 49:
            case 50:
            case 51:
            case 52:
            case 53:
            case 54:
            case 56:
            case 58:
            default:
                cs = 1;
                break;
        }
        if (lookupWord){
            // Compare word to hash table
            // is it a keyword/identifier?
        }
            
    }
    return NONE;
}

string LexicalAnalyzer::GetTokenName (token_type t) const
{
    // This function will return the token name string for the token
    for (int i = 0; i < NUM_TOKENS; i++) {
        if (t == i)
            return (this->reference[i]);
    }
    return "[ERROR: not a valid token index]";
}

string LexicalAnalyzer::GetLexeme () const
{
	// This function will return the lexem found by the most recent call to 
	// the get_token function
	return "";
}

void LexicalAnalyzer::ReportError (const string & msg)
{
	// This function will be called to write an error message to a file
}

void LexicalAnalyzer::putback (){
    if (lexeme.length() > 1){
        lexeme = lexeme.substr(0, lexeme.length()-1);
        pos--;
    }
}