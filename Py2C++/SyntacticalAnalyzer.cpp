#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
    lex = new LexicalAnalyzer (filename);
    token_type t;
    string t_name;
    string l;
    while ((t = lex->GetToken()) != EOF_T)
    {
        // Got token and lexeme, output handled within Lexical Analyzer
        t_name = lex->GetTokenName(t);
        l = lex->GetLexeme();
    }
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
    delete lex;
}
