#include <string>
#include <iostream>
#include <queue>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <cmath>

using namespace std;

struct Node {
    Node* left, * right;
    char key;
    int value;
    Node(char c, int v, Node* l = nullptr, Node* p = nullptr) {
        key = c;
        value = v;
        left = l;
        right = p;
    }
    bool IsItLeaf() {
        return key != '\0';
    }
};

struct comparator {
    bool operator() (Node* a, Node* b) {
        if (a->value != b->value) return a->value > b->value;
        if (!a->IsItLeaf() && b->IsItLeaf()) return false;
        if (!b->IsItLeaf() && a->IsItLeaf()) return true;
        if (a->value == b->value) return false;
        return true;
    }
};

Node* CreateHuffmanTree(vector<int> NumbersOfLetters, vector<char> Letters) {

    priority_queue<Node*, vector<Node*>, comparator> nodes;
    for (int i = 0; i < Letters.size(); i++) {
        nodes.push(new Node(Letters[i], NumbersOfLetters[i]));
    }
    Node* root{};
    while (nodes.size() > 1) {
        Node* l1 = nodes.top();
        nodes.pop();
        Node* l2 = nodes.top();
        nodes.pop();
        root = new Node('\0', l1->value + l2->value, l1, l2);
        nodes.push(root);
    }
    return root;
}

map<char,string> EncodeNodes(Node* node, string help, map<char, string> map) {
    if (node == nullptr) return map;
    if (node->key!='\0') {
        cout << node->key << ":" + help << endl;
        map[node->key] = help;
    }
    map = EncodeNodes(node->left, help + '0', map);
    map = EncodeNodes(node->right, help + '1', map);
    return map;
}

void SavingToTxt(string Text) {
    ofstream save("HuffmanCode.txt");
    save << Text;
    save.close();
}


void OpeningFile(string& CodeToUncode, string FileName) {
    string Code;
    ifstream file(FileName.c_str());


    if (!file)
    {
        cout << "Opening of " << FileName << " have failed.";
    }

    while (getline(file, Code)) {
        CodeToUncode += Code;
    }

    file.close();
}

void Reading(vector<int>& NumbersOfLetters, vector<char>& Letters) {
    for (uint16_t i = 0; i < NumbersOfLetters.size(); i++) cout << NumbersOfLetters[i];
    cout << endl;
    for (uint16_t i = 0; i < Letters.size(); i++) cout << Letters[i];
    cout << endl;
}

void Sorting(vector<int>& NumbersOfLetters, vector<char>& Letters) {
    int HelperNumbers, j;
    char HelperLetters;

    for (uint16_t i = 0; i < NumbersOfLetters.size(); i++) {
        HelperNumbers = NumbersOfLetters[i];
        j = i - 1;
        HelperLetters = Letters[i];

        while (j >= 0 && NumbersOfLetters[j] > HelperNumbers) {
            NumbersOfLetters[j + 1] = NumbersOfLetters[j];
            Letters[j + 1] = Letters[j];
            j--;
        }
        NumbersOfLetters[j + 1] = HelperNumbers;
        Letters[j + 1] = HelperLetters;
    }
}

void CalcNumbers(string& CodeToUncode, vector<char>& Letters, vector<int>& NumbersOfLetters) {
    int Number = 1;
    for (uint16_t i = 0; i < CodeToUncode.length(); i++) {
        while (CodeToUncode[i] == '5') i++;
        Letters.push_back(CodeToUncode[i]);
        //cout << Letters[i] << endl << CodeToUncode[i] << endl;
        for (uint16_t j = i + 1; j < CodeToUncode.length(); j++) {
            if (CodeToUncode[j] == CodeToUncode[i]) {
                Number++;
                CodeToUncode[j] = '5';
                //cout<<Number<<endl;
            }
        }
        if (i < CodeToUncode.length()) NumbersOfLetters.push_back(Number);
        Number = 1;
    }

}


int main()
{
    string CodeToUncode;
    string Code;
    string FileName;
    cout << "Welcome to a program implementing Huffman's code.\nPlease Enter your file name:\n";
    cin >> FileName;
    OpeningFile(CodeToUncode, FileName);
    Code = CodeToUncode;
    cout << CodeToUncode << endl;
    vector<int> NumbersOfLetters;
    vector<char> Letters;
    CalcNumbers(CodeToUncode, Letters, NumbersOfLetters);
    //Reading(NumbersOfLetters, Letters);
    Sorting(NumbersOfLetters, Letters);
    //Reading(NumbersOfLetters, Letters);
    Node* root = CreateHuffmanTree(NumbersOfLetters, Letters);
    map<char, string> EncodedValues;
    map<char,string> map = EncodeNodes(root, "", EncodedValues);
    string EncodedLine = "";
    for (int i = 0; i < Code.length(); i++) {
        EncodedLine += map[Code[i]];
    }
    //cout<<"File size = " << FileSize<<endl;
    //cout << EncodedLine << endl;
    //cout<<endl<<"Rozmiar pliku:"<<
    SavingToTxt(EncodedLine);
    string AfterCompression;
    OpeningFile(AfterCompression, "HuffmanCode.txt");
    double a = Code.length();
    double b = AfterCompression.length();
    double CompressionRatio = (a/b)*100;

    cout <<endl<< "Compression Ratio = "<<CompressionRatio<<"%\n";

    return 0;
}
