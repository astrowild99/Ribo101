/**
    This is the assembler for Ribo101
    The program takes a file as argument and creates two files
    There must be a sketch.ino file in the directory ../../../MotherBoard/src/
    There must be a BuildAndSend.sh file in the directory ../../../Scripts
*/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <math.h>

#define NOP 0x00
#define LDA 0x01
#define LDB 0x02
#define LDD 0x03
#define STA 0x04
#define STB 0x05
#define ADD 0x06
#define SUB 0x07
#define OUT 0x08
#define INT 0x09
#define ENT 0x0A
#define JMP 0x0B
#define JC  0x0C
#define JZ  0x0D
#define JKI 0x0E
#define LDN 0x0F

#define LCD 0x10
#define LCI 0x11
#define LCM 0x12
#define JNC 0x13
#define JNZ 0x14
#define CMP 0x15
#define ADN 0x16
#define SUN 0X17
#define MAB 0x18
#define MBA 0x19
#define MAD 0x1A
#define MDA 0x1B
#define MBD 0X1C
#define MDB 0X1D
#define HLT 0x1E
#define LDX 0x1F

#define ERROR_READ_FILE 0xFF
#define ERROR_WRITE_FILE 0xFE
#define ERROR_INVALID_OPCODE 0xFD
#define ERROR_INVALID_EXTENSION 0xFC
#define ERROR_FILE_OVERFLOW 0xFB
#define ERROR_NO_COMMAND 0xFA

#define INO_PATH "~/Ribo101/Compiler/MotherBoard/src/sketch.ino"
#define TMP_PATH "~/Ribo101/Compiler/MotherBoard/src/sketch.ino.tmp"
#define SCRIPT_PATH "~/Ribo101/Compiler/MotherBoard/BuildAndSend.sh"
#define PROGRAM_OFFSET_INO 35

using namespace std;

fstream assembly;
fstream executable;
fstream inoMotherBoard;
fstream inoTemp;
string name;
//ram
int address[256] = {};
int currentAddress = 0x00;
//naming
string index[256] = {};
//handling the indexes
bool isFirstInLine = true;
string dumbIndexesRam[256] = {};
string dumbRefRam[256] = {};
//handling the mov
bool isMov = false;
//handling the space in the strings
bool isHandlingStrings = false;
bool isHandlingComment = false;
//handling the libs
int currentRow = 0;
//handling the libs
string libs[256] = {};
int libsIndex = 0;

void writeOnRam(int dato){
    //cout << "STO SCRIVENDO SULLA RAM IL NUMERO: " << dato << " all' indirizzo: " << currentAddress << endl;
    cout << "Address: 0x" << hex << setfill('0') << setw(2) << currentAddress;
    cout << " Data: 0x" << hex << setfill('0') << setw(2) << dato << endl;
    address[currentAddress] = dato;
    currentAddress += 1;
}
void writeOnRam(int dato, int addr){
    //cout << "STO SCRIVENDO SULLA RAM IL NUMERO: " << dato << " all' indirizzo: " << addr << endl;
    cout << "Address: 0x" << hex << setfill('0') << setw(2) << addr;
    cout << " Data: 0x" << hex << setfill('0') << setw(2) << dato << endl;
    address[addr] = dato;
    currentAddress += 1;
}
void writeOnDumb(const char *dumbIndex){
    //cout << "STO SCRIVENDO SUL DUMB INDEXES L'INDICE: " << dumbIndex << " AL FINTO INDIRIZZO: " << currentAddress << endl;
    cout << "Address: 0x" << hex << setfill('0') << setw(2) << currentAddress;
    cout << " Data: 0x" << dumbIndex << endl;
    dumbIndexesRam[currentAddress] = dumbIndex;
    currentAddress += 1;
}
void writeOnRefDumb(const char *dumbIndex){
    cout << "Address: 0x" << hex << setfill('0') << setw(2) << currentAddress;
    cout << " Data: 0x" << dumbIndex << " (ref) " << endl;
    dumbRefRam[currentAddress] = dumbIndex;
    currentAddress += 1;
}

int handleIndex(const char *word){
    /*bool isStored = false;
    for(int i = 0; i < 256; i++){
        if(index[i] == word){
            writeOnRam(i);
            isStored = true;
        }
    }
    if(!isStored){
        index[currentAddress] = word;
        cout << "AGGIUNGO AGLI INDICI IL: " << index[currentAddress] << " CHE PUNTA A: " << currentAddress << endl;
    }*/

    if(isFirstInLine){
        index[currentAddress] = word;
        cout << "Index: " << word << " Address: " << currentAddress << endl;
    }
    else{
        writeOnDumb(word);
    }
}

int handleNumbers(const char *word){
    bool isHex = false;
    string wordT = word;
    int n;
    if((wordT[0] == '0') && (wordT[1] == 'x')){
        wordT.substr(0, 2);
        isHex = true;
    }
    stringstream wordStream(wordT);
    while(true){
        if(isHex) wordStream >> hex;
        wordStream >> n;
        if(!wordStream) break;
    }
    writeOnRam(n);
    return 0;
}

int handleChar(const char *word){
    writeOnRam((int) word[1]);
}

int handleLibs(const char *word){
    string wordT = word;
    wordT = wordT.substr(1, wordT.length());
    libs[libsIndex] = wordT;
    libsIndex += 1;
}

int handleString(const char *word){
    string wordT = word;
    wordT = wordT.substr(1, wordT.length() - 1);
    cout << "Handling String: " << wordT << endl;
    for(int i = 0; i < wordT.length(); i++){
        writeOnRam(wordT[i]);
    }
    if(wordT[wordT.length() - 1] != '"') isHandlingStrings = true;
}

int appendString(const char *word){
    string wordT = word;
    wordT.insert(0, " ");
    for(int i = 0; i < wordT.length(); i++){
        writeOnRam(wordT[i]);
    }
    if(wordT[wordT.length() - 1] == '"') isHandlingStrings = false;
}

int handleRef(const char *word){
    string wordT = word;
    wordT = wordT.substr(1, wordT.length());
    string prefix = ".";
    string send = prefix + wordT;
    writeOnRefDumb(send.c_str());
}

int handleInst(const char *word){
    string wordT = word;
    string inst = "";

    //instruction decode
    if((wordT == "NOP") || (wordT == "nop")){
        writeOnRam(NOP);
        return 0;
    }
    else if((wordT == "LDA") || (wordT == "lda")){
        writeOnRam(LDA);
        return 0;
    }
    else if((wordT == "LDB") || (wordT == "ldb")){
        writeOnRam(LDB);
        return 0;
    }
    else if((wordT == "LDD") || (wordT == "ldd")){
        writeOnRam(LDD);
        return 0;
    }
    else if((wordT == "STA") || (wordT == "sta")){
        writeOnRam(STA);
        return 0;
    }
    else if((wordT == "STB") || (wordT == "stb")){
        writeOnRam(STB);
        return 0;
    }
    else if((wordT == "ADD") || (wordT == "add")){
        writeOnRam(ADD);
        return 0;
    }
    else if((wordT == "SUB") || (wordT == "sub")){
        writeOnRam(SUB);
        return 0;
    }
    else if((wordT == "OUT") || (wordT == "out")){
        writeOnRam(OUT);
        return 0;
    }
    else if((wordT == "INT") || (wordT == "int")){
        writeOnRam(INT);
        return 0;
    }
    else if((wordT == "ENT") || (wordT == "ent")){
        writeOnRam(ENT);
        return 0;
    }
    else if((wordT == "JMP") || (wordT == "jmp")){
        writeOnRam(JMP);
        return 0;
    }
    else if((wordT == "JC") || (wordT == "jc")){
        writeOnRam(JC);
        return 0;
    }
    else if((wordT == "JZ") || (wordT == "jz")){
        writeOnRam(JZ);
        return 0;
    }
    else if((wordT == "JKI") || (wordT == "jki")){
        writeOnRam(JKI);
        return 0;
    }
    else if((wordT == "LDN") || (wordT == "ldn")){
        writeOnRam(LDN);
        return 0;
    }
    else if((wordT == "LCD") || (wordT == "lcd")){
        writeOnRam(LCD);
        return 0;
    }
    else if((wordT == "LCI") || (wordT == "lci")){
        writeOnRam(LCI);
        return 0;
    }
    else if((wordT == "LCM") || (wordT == "lcm")){
        writeOnRam(LCM);
        return 0;
    }
    else if((wordT == "JNC") || (wordT == "jnc")){
        writeOnRam(JNC);
        return 0;
    }
    else if((wordT == "JNZ") || (wordT == "jnz")){
        writeOnRam(JNZ);
        return 0;
    }
    else if((wordT == "CMP") || (wordT == "cmp")){
        writeOnRam(CMP);
        return 0;
    }
    else if((wordT == "ADN") || (wordT == "adn")){
        writeOnRam(ADN);
        return 0;
    }
    else if((wordT == "SUN") || (wordT == "sun")){
        writeOnRam(SUN);
        return 0;
    }
    else if((wordT == "HLT") || (wordT == "hlt")){
        writeOnRam(HLT);
        return 0;
    }
    else if((wordT == "MOV") || (wordT == "mov")){
        isMov = true;
        return 0;
    }
    else if((wordT == "LDX") || (wordT == "ldx")){
        writeOnRam(LDX);
        writeOnRam(0x00);
        return 0;
    }
}

int handleMov(const char *word){
    string wordT = word;
    if((wordT == "a,b") || (wordT == "A,B")){
        writeOnRam(MAB);
    }
    if((wordT == "b,a") || (wordT == "B,A")){
        writeOnRam(MBA);
    }
    if((wordT == "a,d") || (wordT == "A,D")){
        writeOnRam(MAD);
    }
    if((wordT == "d,a") || (wordT == "D,A")){
        writeOnRam(MDA);
    }
    if((wordT == "b,d") || (wordT == "B,D")){
        writeOnRam(MBD);
    }
    if((wordT == "d,b") || (wordT == "D,B")){
        writeOnRam(MDB);
    }
    isMov = false;
}

int decodeWord(const char *word){
    if(isMov == true){
        handleMov(word);
    }
    else if(word[0] == '.'){
        handleIndex(word);
    }
    else if(word[0] == '#'){
        handleLibs(word);
    }
    else if((word[0] == '0') || (word[0] == '1') || (word[0] == '2') || (word[0] == '3') || (word[0] == '4') || (word[0] == '5') || (word[0] == '6') || (word[0] == '7') || (word[0] == '8') || (word[0] == '9')){
        handleNumbers(word);
    }
    else if(word[0] == '\''){
        handleChar(word);
    }
    else if(isHandlingStrings){
        appendString(word);
    }
    else if(word[0] == '"'){
        handleString(word);
    }
    else if(word[0] == '&'){
        handleRef(word);
    }
    else if((word[0] == ';') || isHandlingComment){
        //handling a comment, so running without doing anything
        isHandlingComment = true;
    }
    else{
        handleInst(word);
    }
}

int decodeLine(const char *line){
    isFirstInLine = true;
    string lineT = line;
    string temp = "";
    //exiting from comments:
    isHandlingComment = false;
    for(int i = 0; i <= lineT.length(); i++){
        if((line[i] == ' ') || (i == lineT.length()) ){
            decodeWord(temp.c_str());
            temp = "";
            isFirstInLine = false;
        }
        else{
            temp += line[i];
        }
    }
    currentRow += 1;
}

int appendLibs(){
    //fetching the filenames
    for(int i = 0; i < libsIndex; i++){
        fstream tempFile;
        string temp = libs[i];
        string line = "";
        if(temp.substr(temp.length() - 2, 2) != ".h"){
            cout << "Invalid lib file extension: " << temp << endl;
            return ERROR_READ_FILE;
        }
        tempFile.open(temp.c_str(), ios::in);
        if(!tempFile.is_open()){
            cout << "Unable to load lib: " << temp << endl;
            return ERROR_READ_FILE;
        }
        while(getline(tempFile, line)){
            decodeLine(line.c_str());
        }
    }
}

int decodeFile(){
    //using the first as empty due to the bug with ram
    writeOnRam(NOP);
    string line;
    while(getline(assembly, line)){
        decodeLine(line.c_str());
    }
    if(libsIndex != 0){
        cout << "Appending the Libs" << endl;
        appendLibs();
    }
    cout << "Substituting Indexes: " << endl;
    for(int i = 0; i < 256; i++){
        if(dumbIndexesRam[i] != ""){
            for(int h = 0; h < 256; h++){
                if(index[h] == dumbIndexesRam[i]){
                    writeOnRam(h, i);
                }
            }
        }
    }
    cout << "Substituting Refs: " << endl;
    for(int i = 0; i < 256; i++){
        if(dumbRefRam[i] != ""){
            for(int j = 0; j < 256; j++){
                if(index[j] == dumbRefRam[i]){
                    //todo finish
                    writeOnRam(j, i);
                }
            }
        }
    }
}

int printToExec(){
    executable << "const uint16_t program[256] PROGMEM = {";
    for(int i = 0; i < 256; i++){
        executable << "0x";
        executable << hex << setfill('0') << setw(2) << address[i] << ",";
    }
    executable << "};" << endl;
}

int printToIno(){
    //printToIno
    inoMotherBoard.open(INO_PATH, ios::in| ios::binary);
    if(!inoMotherBoard.is_open()){
        printf("UNABLE TO LOAD .ino FILE \n");
        return ERROR_READ_FILE;
    }
    inoTemp.open(TMP_PATH, ios::out| ios::binary);
    if(!inoTemp.is_open()){
        printf("UNABLE TO CREATE tmp.ino FILE \n");
        return ERROR_READ_FILE;
    }
    //reading the whole file and modifying
    printf("Re-Writing the arduino Sketch: \n");
    string line;
    int lineNumber = 0;
    inoMotherBoard.clear();
    inoMotherBoard.seekg(0, ios::beg);
    while(getline(inoMotherBoard, line)){
        if(lineNumber == PROGRAM_OFFSET_INO){
            inoTemp << "const uint16_t program[256] PROGMEM = {";
            for(int i = 0; i < 256; i++){
                inoTemp << "0x";
                inoTemp << hex << setfill('0') << setw(2) << address[i] << ",";
            }
            inoTemp << "};" << endl;
        }
        else{
            inoTemp << line << endl;
        }
        lineNumber += 1;
    }
    inoMotherBoard.close();
    inoTemp.close();
    //printing the temp back to the original
    inoMotherBoard.open(INO_PATH, ios::out| ios::binary);
    if(!inoMotherBoard.is_open()){
        printf("UNABLE TO LOAD .ino FILE \n");
        return ERROR_READ_FILE;
    }
    inoTemp.open(TMP_PATH, ios::in| ios::binary);
    if(!inoTemp.is_open()){
        printf("UNABLE TO CREATE tmp.ino FILE \n");
        return ERROR_READ_FILE;
    }




    inoTemp.clear();
    inoTemp.seekg(0, ios::beg);
    while(getline(inoTemp, line)){
        inoMotherBoard << line << endl;
    }

    inoMotherBoard.close();
    inoTemp.close();
    printf("Done \n");
}

int sendToIno(){
    //sendToIno
    printf("Sending To RIBO101! \n");
    system(SCRIPT_PATH);
}

int main(int argc, char *argv[])
{

    //open file if exsists
    assembly.open(argv[1], ios::in);
    if(!assembly.is_open()){
        printf("UNABLE TO LOAD FILE \n");
        return ERROR_READ_FILE;
    }
    name = argv[1];

    //cutting the name
    string extensionAssembly = ".asm";
    string extensionRibo = ".ribo";
    name = name.substr(0, name.find(extensionAssembly));
    name = name.append(extensionRibo);

    //create file
    executable.open(name.c_str(), ios::out | ios::binary);
    if(!executable.is_open()){
        printf("UNABLE TO CREATE FILE \n");
        return ERROR_WRITE_FILE;
    }
    decodeFile();
    printToExec();

    printToIno();
    sendToIno();
    return 0;
}
