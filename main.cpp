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
            token.tip=esteFloat ? "float_constant" : "int_constant";
            token.lung=chestia.length();
            token.lin=lin;
            return token;
        }

        if(z=='"'){
            chestia.clear();
            chestia += '"';
            bool inchis=false;
            int lin_start = lin;
            while(in.get(z)){
                if(z=='"'){
                    chestia += '"';
                    inchis = true;
                    break;
                }
                if(z=='\n'){
                    lin++; 
                    cout<<"Eroare, nu e inchis stringul la linia "<<lin_start<<endl;
                    char nxt;
                    while(in.get(nxt)){
                        if(nxt=='\n') { lin++; continue; }
                        if(nxt==' ' || nxt=='\t') continue;
                        in.unget();
                        break;
                    }
                    token.val = "";          
                    token.tip = "eroare";
                    token.lung = 0;
                    token.lin = lin_start;
                    return token;
                }
                chestia += z;
            } 

            if(!inchis){
                cout<<"Eroare, nu e inchis stringul la linia "<<lin_start<<endl;
                token.val = "";
                token.tip = "eroare";
                token.lung = 0;
                token.lin = lin_start;
                return token;
            } else {

                token.val = chestia;
                token.tip = "string_literal";
                token.lung = chestia.length();
                token.lin = lin_start;
                return token;
            }
        }

        if(z == '/'){
            if(in.peek() == '/'){
                in.get(z); 
                chestia = "//";
                while(in.peek() != EOF && in.peek() != '\n'){
                    in.get(z);
                    chestia += z;
                }
                token.val = chestia;
                token.tip = "comment";
                token.lung = chestia.length();
                token.lin = lin;
                return token;
            } else if (in.peek() == '*'){
                in.get(z); 
                chestia = "/*";
                bool closed = false;
                int lin_start = lin;
                while(in.get(z)){
                    chestia += z;
                    if(z=='\n') lin++;
                    if(z=='*' && in.peek()=='/'){
                        in.get(z);
                        chestia += z; 
                        closed = true;
                        break;
                    }
                }
                if(!closed){
                    cout<<"Eroare, comentariu neinchis la linia "<<lin_start<<endl;
                    char nxt;
                    while(in.get(nxt)){
                        if(nxt=='\n'){ lin++; continue; }
                        if(nxt==' ' || nxt=='\t') continue;
                        in.unget();
                        break;
                    }
                    token.val = "";
                    token.tip = "eroare";
                    token.lung = 0;
                    token.lin = lin_start;
                    return token;
                } else {
                    token.val = chestia;
                    token.tip = "comment";
                    token.lung = chestia.length();
                    token.lin = lin_start;
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
            chestia = string(1,z);
            if(in.peek()!=EOF){
                char urm = in.peek();
                if((z=='=' && urm=='=') ||
                   (z=='!' && urm=='=') ||
                   (z=='<' && urm=='=') ||
                   (z=='>' && urm=='=')){
                    in.get(urm);
                    chestia += urm;
                }
            }
            token.val = chestia;
            token.tip = "operator";
            token.lung = chestia.length();
            token.lin = lin;
            return token;
        }

        cout<<"Eroare, caracter necunoscut '"<<z<<"' la linia "<<lin<<endl;
        token.val = string(1,z);
        token.tip = "eroare";
        token.lung = 0;
        token.lin = lin;


        char nxt;
        while(in.get(nxt)){
            if(nxt=='\n'){ lin++; continue; }
            if(nxt==' ' || nxt=='\t') continue;
            in.unget();
            break;
        }
        return token;
    } 

    token.tip="end_of_file";
    return token;
}

int main(){
    ifstream in("test3.rs");
    if(!in.is_open()){
        cout<<"Eroare, n-am putut deschide fisierul :("<<endl;
        return -1;
    } else {
        cout<<"Fisier deschis cu succes!"<<endl;
    }

    int line=1;
    Token token;
    Token celMaiLung;
    celMaiLung.lung=0;

    do{
        token=iaurmatorul(in,line);
        if(token.tip !="end_of_file")
            {
                cout<< " '"<<token.val<<"' , "<<token.tip
                    <<" ; "<<token.lung<<" ; linia " <<token.lin<<endl;

                if (token.lung > celMaiLung.lung && token.tip != "eroare" && token.tip != "comment") {
                celMaiLung = token; }
            }
        } while(token.tip !="end_of_file");

    cout << "\nCel mai lung token valid este: '" << celMaiLung.val
     << "' (" << celMaiLung.tip << "), lungime = " << celMaiLung.lung
     << ", linia " << celMaiLung.lin << endl;

    in.close();
    return 0;
}
