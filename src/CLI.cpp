//
// Created by Craig on 2/11/2021.
//

#include <stack>
#include <string.h>
#include <iostream>
#include <string>
#include <sstream>
#include "CLI.h"
#include "Tiff.h"


bool CLI::isValidNum(char* num){
    char* check= num;
    bool decUsed= false;
    int i= 0;

    if(num==NULL){
        return true;
    }

    while(*check){
        if(!isdigit(check[0]) && !((check[0] == '+' || check[0] == '-') && i==0)){
            if(check[0] == '.' && !decUsed){
                decUsed= true;
            }else{
                return false;
            }
        }
        check++;
        i++;
    }
    return true;
}

double CLI::CLI_FloatPar(char param[]){
    return atof(param);
}

void CLI::CLI_Move(char params[]){
    char* p1= strtok(NULL, " ,");
    char* p2= strtok(NULL, " ,");
    char* p3= strtok(NULL, " ,");

    if(!(isValidNum(p1) && isValidNum(p2) && isValidNum(p3))){
        std::cout << "expecting numeric parameters\n";
    }else{
        double d1= CLI_FloatPar(p1);
        double d2= CLI_FloatPar(p2);
        double d3= CLI_FloatPar(p3);

        std::cout << "Move " << d1 << "," << d2 << "," << d3 << "\n";
    }
}

void CLI::CLI_Draw(char params[]){
    char* p1= strtok(NULL, " ,");
    char* p2= strtok(NULL, " ,");
    char* p3= strtok(NULL, " ,");

    if(!(isValidNum(p1) && isValidNum(p2) && isValidNum(p3))){
        std::cout << "expecting numeric parameters\n";
    }else{
        double d1= CLI_FloatPar(p1);
        double d2= CLI_FloatPar(p2);
        double d3= CLI_FloatPar(p3);

        std::cout << "Draw " << d1 << "," << d2 << "," << d3 << "\n";
    }
}

void CLI::CLI_Color(char params[]){
    char* p1= strtok(NULL, " ,");
    char* p2= strtok(NULL, " ,");
    char* p3= strtok(NULL, " ,");


    if(!(isValidNum(p1) && isValidNum(p2) && isValidNum(p3))){
        std::cout << "expecting numeric parameters\n";
    }else{
        double d1= CLI_FloatPar(p1);
        double d2= CLI_FloatPar(p2);
        double d3= CLI_FloatPar(p3);

        std::cout << "Color " << d1 << "," << d2 << "," << d3 << "\n";
    }
}




//add file to stack
void CLI::CLI_Read(char* fileName, std::stack<char*> fileStack){
    std::cout << "Now reading " << fileName << "\n";

    if(fileStack.size()>5){
        std::cout << "Too many files being read\n";
        fileStack.pop();
        return;
    }

    std::ifstream newFile;
    newFile.open(fileName);

    if(newFile) {
        std::string line;
        while (std::getline(newFile, line))
        {
            std::istringstream iss(line);
            char* linePointer = &line[0];
            CLI_ProcessLine(linePointer, fileStack);
        }
    }else{
        std::cout << "File cannot be read\n";
        fileStack.pop();
        return;
    }

    newFile.close();
    fileStack.pop();
}

void CLI::CLI_ProcessLine(char* currLine, std::stack<char*> fileStack){
    char* command= strtok(currLine, " ,");

    if (command==NULL) {
        std::cout << "No command\n";
        return;
    }
    for(size_t i=0; i< sizeof(command); i++) {
        command[i] = tolower(command[i]);
    }
    if (strcmp(command, "move") == 0) {
        CLI_Move(currLine);
    } else if (strcmp(command, "draw") == 0) {
        CLI_Draw(currLine);
    } else if (strcmp(command, "color") == 0) {
        CLI_Color(currLine);
    } else if (strcmp(command, "read") == 0) {
        char* newFileName= strtok(NULL, " ,");
        if(newFileName==NULL){
            std::cout << "Null Filename\n";
            return;
        }
        if(fileStack.size()>0){
            if(strcmp(fileStack.top(), newFileName) == 0){
                std::cout << "Cannot call read on the file that is being read\n";
                return;
            }
        }
        fileStack.push(newFileName);
        CLI_Read(newFileName, fileStack);
    } else if(strcmp(command, "tiffread") == 0){
        char* newFileName= strtok(NULL, " ,");
        if(newFileName==NULL){
            std::cout << "Null Filename\n";
            return;
        }
        Tiff::tiffRead(newFileName);
    }else if(strcmp(command, "tiffstat") == 0){
        char* newFileName= strtok(NULL, " ,");
        if(newFileName==NULL){
            std::cout << "Null Filename\n";
            return;
        }
        Tiff::tiffStat(newFileName);
    }else if(strcmp(command, "tiffwrite") == 0){
        char* newFileName= strtok(NULL, " ,");
        if(newFileName==NULL){
            std::cout << "Null Filename\n";
            return;
        }
        auto x0= (size_t)(strtok(NULL, " ,"));
        auto y0= (size_t)(strtok(NULL, " ,"));
        auto xc= (size_t)(strtok(NULL, " ,"));
        auto yc= (size_t)(strtok(NULL, " ,"));
        Tiff::tiffWrite(newFileName, x0, y0, xc, yc);
    }else{
        std::cout << "invalid command\n";
    }
}

