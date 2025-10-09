#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <cctype>
using namespace std;

struct Token{
    string val;
    string tip;
    int lung;
    int lin;
};

unordered_set<string> KeyWords={
    "as",
    "break",
    "const",
    "continue",
    "crate",
    "else",
    "enum",
    "extern",
    "false",
    "fn",
    "for",
    "if",
    "impl",
    "in",
    "let",
    "loop",
    "match",
    "mod",
    "move",
    "mut",
    "pub",
    "ref",
    "return",
    "self",
    "Self",
    "static",
    "struct",
    "super",
    "trait",
    "true",
    "type",
    "unsafe",
    "use",
    "where",
    "while",
    "async",
    "await",
    "dyn"



};

unordered_set<string> ReservedKeyWords={
    "abstract",
    "alignof",
    "become",
    "box",
    "do",
    "final",
    "macro",
    "override",
    "priv",
    "typeof",
    "unsized",
    "virtual",
    "yield"
};

unordered_set<string> WeakKeyWords={
    "r#static",
    "macro_rules",
    "raw",
    "safe",
    "union"
};

unordered_set<char> Operatori={
    '+',
    '-',
    '*',
    '/',
    '%',
    '!',
    '=',
    '<',
    '>',
    '&',
    '|',
    '^',
    '~',
    '?'
};

unordered_set<char> Delimitatoare={
    '(',
    ')',
    '{',
    '}',
    '[',
    ']',
    ';',
    ',',
    '.',
    ':'
};

bool esteDelim(char x){
    return Delimitatoare.find(x) != Delimitatoare.end();
}

bool esteOperator(char x){
    return Operatori.find(x) != Operatori.end();
}

bool esteKeyWord(const string &y){
    return KeyWords.find(y) != KeyWords.end();
}

Token iaurmatorul(ifstream &in, int &lin){
    Token token;
    token.val="";
    token.tip="end_of_file";
    token.lung=0;
    token.lin=lin;

    char z;
    string chestia;

    while(in.get(z)){
        if(isspace(z)) {
            if(z== '\n' ) lin++;
            continue;
        }

        if(isalpha(z) || z=='_'){
            chestia+=z;
            while(in.peek()!=EOF && (isalnum(in.peek()) || in.peek()=='_')){
                in.get(z);
                chestia+=z;
            }
            token.val=chestia;
            token.tip=esteKeyWord(chestia) ? "keyword" : "identifier";
            token.lung=chestia.length();
            token.lin=lin;
            return token;
        }

        if (isdigit(z)){
            chestia+=z;
            bool esteFloat=false;
            while(in.peek()!=EOF && (isdigit(in.peek()) || in.peek()=='.')){
                if(in.peek()=='.') esteFloat=true;
                in.get(z);
                chestia+=z;
            }
            token.val=chestia;
            token.tip=esteFloat ? "float_literal" : "integer_literal";
            token.lung=chestia.length();
            token.lin=lin;
            return token;
        }

        if(z==' " ')
        {
            chestia+=z;
            while(in.get(z)){
                chestia+=z;
                if (z==' " ') break;
                if(z== '\n' )
                {
                    cout<<"eroare, nu e inchis stringul la linia" <<lin<<endl;
                    break;
                }
            }
            token.val=chestia;
            token.tip="string_literal";
            token.lung=chestia.length();
            token.lin=lin;
            return token;
        }

        if(z == '/'){
            if(in.peek() == '/'){
                in.get(z);
                while(in.peek() != '\n' && in.peek() != EOF)
                    in.get(z);
                continue;
            } else if (in.peek() == '*'){
                in.get(z);
                bool closed=false;
                    if(z=='\n') lin++;
                    if(z=='*' && in.peek()=='/'){
                        in.get(z);
                        closed=true;
                        break;
                    }
                if(!closed){
                    cout<<"Eroare, comentariu neinchis la linia "<<lin<<endl;
                } else {
                    chestia+=z;
                    token.val=chestia;
                    token.tip="operator";
                    token.lung=1;
                    token.lin=lin;
                    return token;
                    }
                }
            }

            if(esteDelim(z)){
                token.val=string(1,z);
                token.tip="delimiter";
                token.lung=1;
                token.lin=lin;
                return token;
            }

            if(esteOperator(z)){
                token.val=string(1,z);
                token.tip="operator";
                token.lung=1;
                token.lin=lin;
                return token;
            }

            cout<<"Eroare, caracter necunoscut '"<<z<<"' la linia "<<lin<<endl;
}
    token.tip="end_of_file";
    return token;
}

int main(){
    ifstream in("test.rs");
    if(!in.is_open()){
        cout<<"Eroare, n-am putut deschide fisierul :("<<endl;
        return -1;
    } else {
        cout<<"Fisier deschis cu succes!"<<endl;
    }

    int line=1;
    Token token;
    do{
        token=iaurmatorul(in,line);
        if(token.tip !="end_of_file")
            {
                cout<< " ' "<<token.val<<" ' , "<<token.tip
                    <<" ; "<<token.lung<<" ; l " <<token.lin<<endl;
            }
    } while(token.tip !="end_of_file");
    in.close();
    return 0;
}