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

        // --- tratăm stringurile " ... "
        if(z=='"'){
            chestia.clear();
            chestia += '"';
            bool inchis=false;
            int lin_start = lin;
            while(in.get(z)){
                // dacă găsim ghilimea de închidere -> string normal
                if(z=='"'){
                    chestia += '"';
                    inchis = true;
                    break;
                }
                // dacă apar newline în interior
                if(z=='\n'){
                    lin++; // contorizăm linia
                    // conform cerinței: șirurile *pot* fi multiline, dar dacă vrem
                    // să semnalăm un string neînchis la sfârșitul liniei (comportament cerut),
                    // tratăm newline fără încheiere ca eroare lexicală de șir neînchis
                    // și recuperează pointerul la primul caracter nenul (după whitespace).
                    cout<<"Eroare, nu e inchis stringul la linia "<<lin_start<<endl;
                    // consumăm până la primul caracter nenul (nu-space/tab/newline)
                    char nxt;
                    while(in.get(nxt)){
                        if(nxt=='\n') { lin++; continue; }
                        if(nxt==' ' || nxt=='\t') continue;
                        // am găsit primul caracter util: punem înapoi ca să fie citit la următorul apel
                        in.unget();
                        break;
                    }
                    // pregătim token de eroare: lungime 0, pointer deja poziționat
                    token.val = "";          // nu returnăm șirul parțial
                    token.tip = "eroare";
                    token.lung = 0;
                    token.lin = lin_start;
                    return token;
                }
                // în mod normal, acumulăm caractere în string
                chestia += z;
            } // end while get

            if(!inchis){
                // dacă am ieșit din loop fără inchidere (EOF) -> eroare, dar s-ar opri
                cout<<"Eroare, nu e inchis stringul la linia "<<lin_start<<endl;
                // la EOF nu mai avem nimic de scanat: token de eroare cu lung=0
                token.val = "";
                token.tip = "eroare";
                token.lung = 0;
                token.lin = lin_start;
                return token;
            } else {
                // string corect închis
                token.val = chestia;
                token.tip = "string_literal";
                token.lung = chestia.length();
                token.lin = lin_start;
                return token;
            }
        }

        // --- COMENTARII ---
        if(z == '/'){
            if(in.peek() == '/'){
                in.get(z); // consumam al doilea '/'
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
                in.get(z); // consumam '*'
                chestia = "/*";
                bool closed = false;
                int lin_start = lin;
                while(in.get(z)){
                    chestia += z;
                    if(z=='\n') lin++;
                    if(z=='*' && in.peek()=='/'){
                        in.get(z);
                        chestia += z; // adaugam '/'
                        closed = true;
                        break;
                    }
                }
                if(!closed){
                    cout<<"Eroare, comentariu neinchis la linia "<<lin_start<<endl;
                    // conform cerintei: la eroare lexicală scanam in continuare pana la primul non-whitespace
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
            // verificam operatori din 2 caractere (ex: ==, !=, <=, >=)
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

        // eroare literala: caracter necunoscut
        cout<<"Eroare, caracter necunoscut '"<<z<<"' la linia "<<lin<<endl;
        // conform cerintei: seteaza lungimea tokenului curent la 0 si avanseaza la primul caracter nenul
        // punem token de eroare cu lung=0 si pozitia curenta (lin)
        token.val = string(1,z);
        token.tip = "eroare";
        token.lung = 0;
        token.lin = lin;

        // consumam pana la primul caracter nenul (nu spatiu/tab/newline) si punem inapoi acel caracter
        char nxt;
        while(in.get(nxt)){
            if(nxt=='\n'){ lin++; continue; }
            if(nxt==' ' || nxt=='\t') continue;
            in.unget();
            break;
        }
        return token;
    } // end while in.get

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
    do{
        token=iaurmatorul(in,line);
        if(token.tip !="end_of_file")
            {
                cout<< " '"<<token.val<<"' , "<<token.tip
                    <<" ; "<<token.lung<<" ; linia " <<token.lin<<endl;
            }
    } while(token.tip !="end_of_file");
    in.close();
    return 0;
}
