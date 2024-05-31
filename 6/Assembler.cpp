#include <bits/stdc++.h>
using namespace std;

map<string, int> table;
int line = 0;
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

int TypeOfCommand(const string& str) {
    if (str[0] == '@') {
        return isalpha(str[1]) ? 0 : 1;
    }
    return 2;
}

void CreateSymbol(const string& str) {
    string symbol = str.substr(1, str.find(')') - 1);
    table[symbol] = line;
}

string ExtraAddress(const string& str) {
    int address = stoi(str.substr(1));
    return bitset<15>(address).to_string();
}

string ExtraSymbol(const string& str) {
    string symbol = str.substr(1);
    if (table.find(symbol) == table.end()) {
        table[symbol] = var++;
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

string DesttoBinary(const string& dest) {
    string res = "000";
    if (dest.find('A') != string::npos) res[0] = '1';
    if (dest.find('D') != string::npos) res[1] = '1';
    if (dest.find('M') != string::npos) res[2] = '1';
    return res;
}

string JumptoBinary(const string& jump) {
    static const unordered_map<string, string> Jump_map = {
        {"JGT", "001"}, {"JEQ", "010"}, {"JGE", "011"},
        {"JLT", "100"}, {"JNE", "101"}, {"JLE", "110"},
        {"JMP", "111"}
    };

    auto it = Jump_map.find(jump);
    return it != Jump_map.end() ? it->second : "000";
}

string ALU(const string& str) {
    int eq = str.find('=');
    int semi = str.find(';');
    
    string comp = (eq != string::npos) ? str.substr(eq + 1, semi - eq - 1) : str.substr(0, semi);
    comp = ComptoBinary(comp);

    string dest = (eq != string::npos) ? DesttoBinary(str.substr(0, eq)) : "000";
    string jump = (semi != string::npos) ? JumptoBinary(str.substr(semi + 1)) : "000";

    return comp + dest + jump;
}

string Parser(const string& str) {
    int commandType = TypeOfCommand(str);
    switch (commandType) {
        case 0: return '0' + ExtraSymbol(str) + "\n";
        case 1: return '0' + ExtraAddress(str) + "\n";
        case 2: return "111" + ALU(str) + "\n";
        default: return "";
    }
}

int main() {
    string filename;
    cin >> filename;
    ifstream file(filename);
    string str;
    init();

    // First pass: symbol table creation
    while (getline(file, str)) {
        if (str.empty() || str[0] == '/' || str[0] == ' ' || str[0] == 13 || str[0] == 10) continue;
        if (str[0] == '(') {
            CreateSymbol(str);
        } else {
            line++;
        }
    }
    file.clear();
    file.seekg(0, ios::beg);

    string name = filename.substr(0, filename.find('.')) + ".hack";
    ofstream ofile(name);

    // Second pass: parsing and writing output
    while (getline(file, str)) {
        if (str.empty() || str[0] == '/' || str[0] == '('  || str[0] == ' '|| str[0] == 13 || str[0] == 10) continue;
        ofile << Parser(str);
    }

    file.close();
    ofile.close();
    return 0;
}
