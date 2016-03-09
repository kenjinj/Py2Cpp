#include <iomanip>
#include <cstdlib>
#include "LexicalAnalyzer.h"

using namespace std;

static string token_names[] = {	"EOF_T"}; 

LexicalAnalyzer::LexicalAnalyzer (char * filename)
{
    // This function will initialize the lexical analyzer class
    file = filename;
    size_t found = file.find_first_of(".");
    fstub = file.substr (0,found); 
    input.open(filename, ifstream::in);
    string outfileName = string(fstub);
    outfileName += ".lst";
    listing.open(outfileName.c_str(), ofstream::out);
    outfileName = string(fstub);
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
    // This function will find the next lexeme in the input file and return
    // the token_type value associated with that lexeme
    cout << "Resetting token, t = " << GetTokenName(token) << endl;
    token = NONE;
    cout << "Token is now = " << GetTokenName(token) << endl;
    // If position out of current line bounds
    if (pos >= line.length()){
        // Try to get new line from input
            cout << "GETTING NEW LINE\n";
        getline(input, line);
        if (input.fail()){
            token = EOF_T;
            return token;
        }
        linenum++;
        pos = 0;
        cs = 1;
        lexeme = "";
        // Write line number and input text to files
        listing << setw(5) << linenum << "  " << line << endl;
        debug << setw(5) << linenum << "  " << line << endl;
    }
    // While it is within bounds of current line
    while (pos <= line.length() && token == NONE){
        char c = line[pos++];
        if (lexeme.length() == 0 && c == 0)
            break;
        if (lexeme.length() != 0 || c != ' ')
            lexeme += c;
        if (isalpha(c)) c = 'a';
        if (isdigit(c)) c = '0';
        int col = 0;
        while (c != valid[col] && col < valid.length() - 1) col++;
        col++;
            cout << "Current state is: " << cs << endl;
            cout << "Found character in column: " << col << endl;
        cs = stateTable[cs][col];
            cout << "State is now: " << cs << endl;
        switch (cs){
            case -1:
                // ERROR
                listing << "Error: Invalid character '" << c << "' at " << linenum << "," << pos << endl;
                debug << "Error: Invalid character '" << c << "' at " << linenum << "," << pos << endl;
                token = ERROR_T;
                break;
            case 3:
                // Identifier found for hash table lookup
                cs = 1;
                putback(c);
                token = IDENT_T;
                break;
            case 5:
                // Valid is only numerics
                cs = 1;
                putback(c);
                token = INTLIT_T;
                break;
            case 8:
                // Valid is numerics and a decimal
                cs = 1;
                putback(c);
                token = FLTLIT_T;
                break;
            case 9:
                // Found newline character
                cs = 1;
                token = NL_T;
                break;
            case 10:
                // Found carriage token = character
                cs = 1;
                token = CR_T;
                break;
            case 11:
                // Found tab character
                cs = 1;
                token = TAB_T;
                break;
            case 13:
                // Found +=
                cs = 1;
                token = PLUSEQ_T;
                break;
            case 14:
                // Valid is only + character
                cs = 1;
                putback(c);
                token = PLUS_T;
                break;
            case 16:
                // Found -=
                token = MINUSEQ_T;
                break;
            case 17:
                // Valid is only - character
                cs = 1;
                putback(c);
                token = MINUS_T;
                break;
            case 20:
                // Found **=
                cs = 1;
                token = EXPEQ_T;
                break;
            case 21:
                // Valid is ** characters
                cs = 1;
                putback(c);
                token = EXP_T;
                break;
            case 22:
                // Found *=
                cs = 1;
                token = MULTEQ_T;
                break;
            case 23:
                // Valid is only * character
                cs = 1;
                putback(c);
                token = MULT_T;
                break;
            case 26:
                // Found //=
                cs = 1;
                token = IDIVEQ_T;
                break;
            case 27:
                // Valid is // characters
                cs = 1;
                putback(c);
                token = IDIV_T;
                break;
            case 28:
                // Found /=
                cs = 1;
                token = DIVEQ_T;
                break;
            case 29:
                // Valid is only / character
                cs = 1;
                putback(c);
                token = DIV_T;
                break;
            case 31:
                // Found %=
                token = MODEQ_T;
                break;
            case 32:
                // Valid is only % character
                cs = 1;
                putback(c);
                token = MOD_T;
                break;
            case 34:
                // Found <=
                cs = 1;
                token = LTE_T;
                break;
            case 35:
                // Valid is only < character
                cs = 1;
                putback(c);
                token = LT_T;
                break;
            case 37:
                // Found >=
                cs = 1;
                token = GTE_T;
                break;
            case 38:
                // Valid is only > character
                cs = 1;
                putback(c);
                token = GT_T;
                break;
            case 40:
                // Found ==
                cs = 1;
                token = EQUAL_T;
                break;
            case 41:
                // Valid is only = character
                cs = 1;
                putback(c);
                token = ASSIGN_T;
                break;
            case 43:
                // Found !=
                cs = 1;
                token = NOTEQ_T;
                break;
            case 44:
                // Found (
                cs = 1;
                token = LPAREN_T;
                break;
            case 45:
                // Found )
                cs = 1;
                token = RPAREN_T;
                break;
            case 46:
                // Found [
                cs = 1;
                token = LBRACKET_T;
                break;
            case 47:
                // Found ]
                cs = 1;
                token = RBRACKET_T;
                break;
            case 48:
                // Found {
                cs = 1;
                token = LBRACE_T;
                break;
            case 49:
                // Found }
                cs = 1;
                token = RBRACE_T;
                break;
            case 50:
                // Found @
                cs = 1;
                token = AT_T;
                break;
            case 51:
                // Found ,
                cs = 1;
                token = COMMA_T;
                break;
            case 52:
                // Found :
                cs = 1;
                token = COLON_T;
                break;
            case 53:
                // Valid is only . character
                cs = 1;
                putback(c);
                token = PERIOD_T;
                break;
            case 54:
                // Found ;
                cs = 1;
                token = SEMI_T;
                break;
            case 56:
                // Found one " followed by some amount of 
                // characters and then another "
                cs = 1;
                token = STRLIT_T;
                break;
            case 58:
                // Found one ' followed by some amount of 
                // characters and then another '
                cs = 1;
                token = STRLIT_T;
                break;
        }
        if (token != NONE){
            debug << "\t\t" << GetTokenName(token) << "\t\t" << GetLexeme() << endl;
                cout << "Token (" << token << ") is: " << GetTokenName(token) << endl;
                cout << "Lexeme is: " << lexeme << endl;
            lexeme_last = lexeme;
            lexeme = "";
        }
            cout << endl;
    }
    return token;
}

string LexicalAnalyzer::GetTokenName (token_type t) const
{
    // This function will return the token name string for the token
    if (t == -1)
        return "NONE";
    for (int i = 0; i < NUM_TOKENS; i++) {
        if (t == i)
            return reference[i];
    }
    return "";
}

string LexicalAnalyzer::GetLexeme () const
{
    // This function will return the lexeme found by the most recent call to 
    // the get_token function
    if (lexeme[0] == '\n')
        return "\\n";
    if (lexeme[0] == '\r')
        return "\\r";
    if (lexeme[0] == '\t')
        return "\\t";
    return lexeme;
}

void LexicalAnalyzer::ReportError (const string & msg)
{
    // This function will be called to write an error message to a file
    cout << errors << msg << file << endl;
    listing << endl << ".     .     .\n" << errors << msg << file << endl;
    debug << endl << ".     .     .\n" << errors << msg << file << endl;
}

void LexicalAnalyzer::putback (char last){
    if (lexeme.length() > 1){
        lexeme = lexeme.substr(0, lexeme.length()-1);
        if (last != ' ')
            pos--;
    }
}