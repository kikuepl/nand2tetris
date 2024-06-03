#include <bits/stdc++.h>
using namespace std;

unordered_map<string, int> table;
int pos = 0;
int var = 16;

void init() {
    table["R0"] = 0;
    table["R1"] = 1;
    table["R2"] = 2;
    table["R3"] = 3;
    table["R4"] = 4;
    table["R5"] = 5;
    table["R6"] = 6;
    table["R7"] = 7;
    table["R8"] = 8;
    table["R9"] = 9;
    table["R10"] = 10;
    table["R11"] = 11;
    table["R12"] = 12;
    table["R13"] = 13;
    table["R14"] = 14;
    table["R15"] = 15;

    table["SP"] = 0;
    table["LCL"] = 1;
    table["ARG"] = 2;
    table["THIS"] = 3;
    table["THAT"] = 4;
    table["SCREEN"] = 16384;
    table["KBD"] = 24576;
}

bool check(string str){
    if (str[2] == ' ' || str[0] == '\n' || str[0] == '\r' || str[0] == '\0'){
        return 1;
    }else
    {
        return 0;
    }
}

int TypeOfCommand(string str) {
    if (str[0] == '@') {
        return isalpha(str[1]) ? 0 : 1;
    }
    return 2;
}

void CreateSymbol(string str) {
    string symbol = str.substr(1, str.find(')') - 1);
    table[symbol] = pos;
}

string ExtraAddress(string str) {
    int address = stoi(str.substr(1));
    return bitset<15>(address).to_string();
}

string ExtraSymbol(string str) {
    string symbol = str.substr(1);
    if (table.find(symbol) == table.end()) {
        table[symbol] = var;
        var++;
    }
    return bitset<15>(table[symbol]).to_string();
}

string ComptoBinary(const string& comp) {
    static const unordered_map<string, string> Comp_map = {
        {"0", "0101010"}, {"1", "0111111"}, {"-1", "0111010"},
        {"D", "0001100"}, {"A", "0110000"}, {"M", "1110000"},
        {"!D", "0001101"}, {"!A", "0110001"}, {"!M", "1110001"},
        {"-D", "0001111"}, {"-A", "0110011"}, {"-M", "1110011"},
        {"D+1", "0011111"}, {"A+1", "0110111"}, {"M+1", "1110111"},
        {"D-1", "0001110"}, {"A-1", "0110010"}, {"M-1", "1110010"},
        {"D+A", "0000010"}, {"D+M", "1000010"}, {"D-A", "0010011"},
        {"D-M", "1010011"}, {"A-D", "0000111"}, {"M-D", "1000111"},
        {"D&A", "0000000"}, {"D&M", "1000000"}, {"D|A", "0010101"},
        {"D|M", "1010101"}
    };

    auto it = Comp_map.find(comp);
    return it != Comp_map.end() ? it->second : "0000000";
}

string DesttoBinary(string dest) {
    string res = "000";
    if (dest.find('A') != string::npos) res[0] = '1';
    if (dest.find('D') != string::npos) res[1] = '1';
    if (dest.find('M') != string::npos) res[2] = '1';
    return res;
}

string JumptoBinary(string jump) {
    static unordered_map<string, string> Jump_map = {
        {"JGT", "001"}, {"JEQ", "010"}, {"JGE", "011"},
        {"JLT", "100"}, {"JNE", "101"}, {"JLE", "110"},
        {"JMP", "111"}
    };

    auto it = Jump_map.find(jump);
    return it != Jump_map.end() ? it->second : "000";
}

string ALU(string str) {
    int eq = str.find('=');
    int semi = str.find(';');

    string comp = (eq != string::npos) ? str.substr(eq + 1, semi - eq - 1) : str.substr(0, semi);
    comp = ComptoBinary(comp);

    string dest = (eq != string::npos) ? DesttoBinary(str.substr(0, eq)) : "000";
    string jump = (semi != string::npos) ? JumptoBinary(str.substr(semi + 1)) : "000";
    return comp + dest + jump;
}

string Parser(string str) {
    int commandType = TypeOfCommand(str);
    // str = str.substr(0, str.size());
    switch (commandType) {
        case 0: return '0' + ExtraSymbol(str) + "\n";
        case 1: return '0' + ExtraAddress(str) + "\n";
        case 2: return "111" + ALU(str) + "\n";
        default: return "";
    }
}

void strtrim(string &str) {
    if (str.empty()) return;

    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == string::npos) {
        str.clear();
        return;
    }

    size_t end = str.find_last_not_of(" \t\n\r");

    str = str.substr(start, end - start + 1);
}



int main(){
    string filename;
    cin >> filename;
    ifstream file(filename);

    string make_filename = "";
    for(char c : filename){
        if(c == '.') { break ;}
        make_filename += c;
    }
    ofstream ofile(make_filename + ".hack");
    string str;
    stringstream buf;

    if (file) {
        buf << file.rdbuf();
        str = buf.str();
    }else
    {
        cout << "filename : " << filename << "は存在しません" << endl;
        return 0;
    }
    init();
    int i=0;
    string text = "";
    while (str[i]){
        string line = "";
        while(str[i] && str[i] != '\n') { line += str[i++]; }
        strtrim(line);
        i++;
        if(check(line)) { continue ;}
        if (line[0] == '(') { CreateSymbol(line) ;}
        else { pos++ ;}
        text += line;
        // cout << line;
    }
    i = 0;
    while (str[i]){
        string line = "";
        while(str[i] && str[i] != '\n') { line += str[i++]; }
        i++;
        strtrim(line);
        if (check(line)) { continue ;}
        if (line[0] == '(') { continue ;}
        ofile << Parser(line);
    }
    file.close();
    return 0;
}