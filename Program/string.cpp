/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc3724 RedId: 822936303
 * Vivian Reyes  cssc3713 RedId: 821862269
 * 
 * CS 530 | Professor Lenoard | March 2021
 **/
#include <stdio.h>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
//trying to include regex c++ lib #include <boost/regex.hpp>

using namespace std;

string programName;
/**
 * Non initialized values could run into a runtime error
 * because there is no constructor
 **/
struct ESTABdata{
    string controlSection;
    string instruction;
    string address;
    string length;    
};

/**
 * Object File Line is the Parent Struct of all of the 
 * differnt types of object file liens there can be
 * 
 * each object record posesses different attributes and needs
 * its own struct
 **/
struct ObjectFileLine{
    struct HeaderRecord{
        string controlSectionName;
        unsigned int actualAddress;
        unsigned int length;
    };
    struct DefinitionRecord{

    };
    struct ReferenceRecord{

    };
    struct TextRecord{
        unsigned int relativeAddress;
        string symbolName;
        string instruction;
        vector<string> arguments;
        vector<unsigned int> objectCodes;
    };
    struct ModificationRecord{

    };
    struct EndRecord{

    };
    struct HeaderRecord header;
    struct DefinitionRecord definition;
    struct ReferenceRecord reference;
    struct TextRecord text;
    struct ModificationRecord modification;
    struct EndRecord end;

    unsigned int relativeAddress;   
};

/**
 * This vector holds all of the instructions in a source code file
 * - Each element is an Instruction struct
 **/
vector<ObjectFileLine> listingFile;

/**
 * This Unordered map Data Structure holds the Symbol name as the key
 * and the address as the value
 **/
map<string,ESTABdata> ESTAB;

/**
 * This 2D data structure is used store all of the instructions as
 * tokenized strings. Each vector is an instruction line. Each vector is 
 * stored in the main vector calle `lines` 
 **/
vector<vector<string> > lines;
/**
 * This vector stores all of the lines of the source code as strings
 **/
vector<string> testSourceCode;

// std::regex re(","); NEED to change the char delim to a regex obj for multiple deliminators
vector<string> split(const string str, char delim) {
    vector<string> result;
    istringstream ss(str);
    string token;

    while (getline(ss, token, delim)) {
        if (!token.empty()) {
            result.push_back(token);
        }
    }
    return result;
}

int printInstructions(){
    printf("Instructions Deliminated:\n");
    printf("-------------------------\n");
	for(int i = 0; i < lines.size(); i++){
		vector<string> temp = lines[i];
		for(int j = 0; j < temp.size(); j++){
			cout << temp[j] << ' ';
		}
		printf("\n");
	}
    printf("-------------------------\n\n");
	return 0;
}
 void printSourceCode(){
    printf("Source Code:\n");
    printf("-------------------------\n");
	for(int i = 0; i < testSourceCode.size(); i++){
        cout << testSourceCode[i];
        printf("\n");
	}
    printf("-------------------------\n");
	return;
 }

/*
    Idea on indetifying what is in each column
    - we already have every string in a 2D vector, but not all the indexes and types match up
    - so we are going to compare each line at certain indexes and determine if there is a space or a char 
    - and depending on that we should know what goes where into the struct values, probs with a lot of if statments unfortantely
    - index 8 = labels, index 16/17 = instruction, index 25/26 = arguments, index 51 = address
*/
 void printIndexes(vector<string> sourceCode){
     printf("Testing Indexes\n");
     printf("-------------------------\n");
     string line;
     for(int i = 3; i < 5; i++){
        string line (sourceCode[i].c_str());
        cout << "Char at index 8, labels: " << line.at(8) << endl;
        cout << "Char at index 16, instructions: "<< line.at(16) << endl;
        cout << "Char at index 17, instructions: " << line.at(17) << endl;
        cout << "Char at index 25, arguments: " << line.at(25) << endl;
        cout << "Char at index 26, arguments: " << line.at(26) << endl;
        cout << "Char at index 51, object code: " << line.at(51) << endl;
	}
    printf("-------------------------\n");
    return;
 }

void printESTAB(){
    /**
     * Order contents in map by address
     * Write to file
     * Call a check proper address function before
     * this to throw an error
     **/
    printf("\nESTAB:\n------------------------\n");
    map<string, ESTABdata>::iterator it;
    for(it = ESTAB.begin(); it != ESTAB.end(); it++){
        if(it->first == "")
            continue;
        else if (it->second.instruction == ""){
            cout << it->first
                 << "       "
                 << setw(5)
                 << setfill('0')
                 << " "
                 << it->second.address
                 << setw(3)
                 << it->second.length
                 << endl;
        }
        else if(it->second.instruction != ""){
            cout << " "
                 << setw(10)
                 << setfill(' ')
                 << it->first
                 << " "
                 << setw(4)
                 << setfill('0')
                 << it->second.address
                 << endl;
        }
    }
    printf("\n------------------------\n\n");
    return;
}
int count;
void generateESTAB(vector<string> vec, string instruction){

    ESTABdata data;

    if(vec.size() < 3){
        cout << "Contents of vect:" << vec[0] << endl;
    } else {
        cout << "Contents of vect:" 
             << vec[1]
             << setw(3)
             << " "
             << setfill(' ')
             <<  vec[2] << endl;
    }
    if(vec.size() == 0 || vec[0] == ".")
        return;
    else if(vec.size() < 3) 
        return;
    else if(vec[2] == "START"){
        programName = vec[1];
        data.controlSection = programName;
        unsigned int address;
        istringstream converter(vec[0].c_str());
        converter >> hex >> address;
        data.address = atoi(vec[0].c_str());
        data.length = atoi(vec[2].c_str());
        ESTAB[programName] = data;
    }
    else if(vec[1] == "EXTDEF" || vec[1] == "EXTREF"){
        cout << "COUNT: " << count << endl;
        string arg = vec[2];
        cout << "Argument: " << arg << endl;
        data.controlSection = programName;
        data.address = atoi(vec[0].c_str());
        data.instruction = arg;
        ESTAB[arg] = data;
    }
    else if(vec[2] == "WORD" || vec[2] == "RESW"){
        string arg = vec[1];
        if(ESTAB.count(arg)){
            data.controlSection = programName;
            data.address = atoi(vec[0].c_str());
            data.instruction = arg;
            ESTAB[arg] = data;
        }
    }
    else if(vec[2] == "=C'EOF'"){
        unsigned int val = ESTAB[programName].length;
        unsigned int length;
        istringstream converter(vec[0].c_str());
        converter >> hex >> length;
        ESTAB[programName].length = length - val;
    }
    count++;
    return;
}



void generateHeaderRecord(vector<string> sourceCode, vector<vector<string> > tokenized){
    
}

void generateDefinitionRecord(vector<string> sourceCode, vector<vector<string> > tokenized){
    
}

//we generate one line of the text record
//should return new index
//created 3/10 by viv
int generateTextRecord(int index, vector<string> sourceCode, vector<vector<string> > tokenized){
    ObjectFileLine data;
    //will need some sort of counter to make sure we are not going over size IE
    int counter;

    //store rel address
    unsigned int relAddress = atoi((tokenized.at(index).at(0).c_str()));
    data.text.relativeAddress = relAddress;

    //what will our loop condition be?
    //counter < IE (hex))
    while(true)
    {
        vector<string> instruct = tokenized.at(index);
        //if line has a symbol name then index 1 = symnbol name and index 2 = instruction
        //
        string line = sourceCode[index];
        if(line.at(8) != ' ')
        {
            data.text.symbolName = tokenized[index][1];
            data.text.instruction = tokenized[index][2];
            data.text.arguments.push_back(tokenized[index][3]);
            //not always the case for this need to check bounds
            if(instruct.size() > 3){
                stringstream temp(tokenized[index][4]);
                unsigned int x = 0;
                temp >> x;
                data.text.objectCodes.push_back(x);
            }
        }
            
        else{
            data.text.instruction = tokenized[index][1]; //if no second column instruction is at index 1 of instruct vector
            data.text.arguments.push_back(tokenized[index][2]);
            //check for bounds
            if(instruct.size() > 3){
                stringstream temp(tokenized[index][3]);
                unsigned int x = 0;
                temp >> x;
                data.text.objectCodes.push_back(x);
            }
        }
        //also need to be filling the TextRecord struct of every loop
        //increment index 
        index++;
    }
    return index;   
}

void generateModificationRecord(vector<string> sourceCode, vector<vector<string> > tokenized){

}

void generateEndRecord(vector<string> sourceCode, vector<vector<string> > tokenized){

}


ObjectFileLine instructionParse(vector<string> sourceCode, vector<vector<string> > tokenized){
    struct ObjectFileLine structData;

    for(int i = 4; i < sourceCode.size(); i++){

        if(tokenized.at(i).at(0) == "END"){
            generateEndRecord(sourceCode, tokenized);
            break;
        }
        //generateTextRecord(index, sourceCode, tokenized);
    }
        
    return structData;    
}

int readFileESTAB(const char* input){
    ifstream file(input);
    string line;
    vector<string> temp;
    count = 0;
    if (file.is_open()) {
        while (getline(file, line)) {
            temp = split(line, ' ');
            lines.push_back(temp);
            testSourceCode.push_back(line);
            generateESTAB(temp,line);
            //ESTAB = sortESTAB(ESTAB);
        }
    }
    printESTAB();
    file.close();
    return 0;
}

void readFileObjectFile(const char* input){
    map<string,ObjectFileLine> instructionMap;  

    ifstream file(input);
    string line;
    vector<vector<string> > tokenized;
    vector<string> sourceCode;
    if (file.is_open()) {
        while (getline(file, line)) {
            vector<string> temp;
            temp = split(line, ' ');
            tokenized.push_back(temp);
            sourceCode.push_back(line);
        }
    }
    
    instructionParse(sourceCode, tokenized);
    //Call the generate Object file function here
    printIndexes(sourceCode);
    file.close();
    return;
}

int main(int argc, char *argv[]){
    printf("The size of argc is: %d\n",argc);
    if(argc < 2){
        cout << "Error: Please specify a listing file to parse" << endl;
        return 0;
    } else if(argc > 4){
        cout << "Error: Please provide 1-4 listing files to parse" << endl;
        return 0;
    }

    for(int i = 1; i < argc; i++){
        readFileESTAB(argv[i]);
        //readFileObjectFile(argv[i]);
    }
    printInstructions();
    printSourceCode();

    return 0;
}