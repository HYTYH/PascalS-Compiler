#include <fstream>
#include <iostream>
#include <string>

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Program.h"
#include "llvm/Support/raw_ostream.h"

#include "AST.h"
#include "CodeGenContext.h"
#include "parser.h"
#include "errorHandle.h"

#define OUTPUT_BUFFER_SIZE 300

// extern
extern int yyparse();
extern AST::ProgramNode *astRoot;

// global vars
int parseError = 0;
FILE *file;
bool printAstInfo;

using namespace std;

typedef struct RunOuputMode {
    bool help, ast, ir, irOpt, assemble, error;
} RunOuputMode;

RunOuputMode analysisInpuMode(int argc, char *argv[]);
void outputFileContent(string filename);

int main(int argc, char *argv[]) {

    // argv size check
    RunOuputMode mode = analysisInpuMode(argc, argv);

    if (mode.error) {
        return 1;
    }

    if (mode.help) {
        return 0;
    }

    // create file
    string fname(argv[argc-1], argv[argc-1] + strlen(argv[argc-1]) - 4);
    error_code EC;
    llvm::raw_fd_ostream ir(fname + ".ll", EC, llvm::sys::fs::F_None);

    // frontend
    // construct ast and context
    initErrorHandle();
    if (mode.ast) {
        cout << "----------- AST Construction -----------" << endl;
    }
    
    yyparse();
    fclose(file);

    if (mode.ast) {
        cout << "----------------------------------------" << endl;
    }
    closeErrorHandle();

    if (parseError) {
        return 1;
    }
    
    CodeGenContext context;
    astRoot->CodeGen(context);
    cout << flush;

    // generate IR file
    context.module->print(ir, nullptr);
    ir.close();

    if (mode.ir) {
        cout << "------------- Oringinal IR -------------" << endl;
        string fileName = fname + ".ll";
        outputFileContent(fileName);
        cout << "----------------------------------------" << endl;
    }
    
    // backend
    string errMsg;
    bool executionFailed;

    // optimize IR
    std::string optProgram = "/usr/bin/opt";
    const char *optArgs[] = {"opt","-S", "-dce", "-deadargelim", "-reassociate", (fname + ".ll").c_str(), "-o", (fname + ".ll").c_str(), nullptr};
    llvm::sys::ExecuteAndWait(optProgram, optArgs, nullptr, nullptr, 0, 0, &errMsg, &executionFailed);
    if (executionFailed) {
        std::cout << errMsg;
        return 1;
    }

    if (mode.irOpt) {
        cout << "------------- Optimized IR -------------" << endl;
        string fileName = fname + ".ll";
        outputFileContent(fileName);
        cout << "----------------------------------------" << endl;
    }

    // generate assembly code .s file
    string clangProgram = "/usr/bin/clang";
    const char *clangArgs[] = {"clang", "-S", "-Wno-override-module", (fname + ".ll").c_str(), nullptr};
    llvm::sys::ExecuteAndWait(clangProgram, clangArgs, nullptr, nullptr, 0, 0, &errMsg, &executionFailed);
    if (executionFailed) {
        cout << errMsg << endl;
        return 1;
    }

    if (mode.assemble) {
        cout << "------------ Assemble Code -------------" << endl;
        string fileName = fname + ".s";
        outputFileContent(fileName);
        cout << "----------------------------------------" << endl;
    }

    // generate executable file
    string gccProgram = "/usr/bin/g++";
    const char *gccArgs[] = {"g++", "-o", fname.c_str(), (fname + ".s").c_str(), nullptr};
    llvm::sys::ExecuteAndWait(gccProgram, gccArgs, nullptr, nullptr, 0, 0, &errMsg, &executionFailed);
    if (executionFailed) {
        cout << errMsg << endl;
        return 1;
    }

    return 0;
}

RunOuputMode analysisInpuMode(int argc, char *argv[]) {
    RunOuputMode mode;
    mode.assemble = false; mode.ir = false; mode.irOpt = false;
    mode.error = false; mode.help = false; mode.ast = false;

    if (argc < 2 || (argc == 2 && strcmp(argv[argc-1],"-help") == 0)) {
        mode.help = true;
        cout << "Usage: pascal [options] filename" << endl;
        cout << "Options:" << endl;
        cout << "-T              Output AST Construct to stdout" << endl;
        cout << "-I              Output IR Code to stdout" << endl;
        cout << "-O              Output OPT IR Code to stdout" << endl;
        cout << "-S              Output Assembly Code to stdout" << endl;
        cout << "-A              Output ALL to stdout" << endl;
        return mode;
    }

    for (size_t i = 1; i < argc-1; i++) {
        if(strcmp(argv[i],"-S") == 0) {
            mode.assemble = true;
            continue;
        }
        if(strcmp(argv[i],"-O") == 0) {
            mode.irOpt = true;
            continue;
        }
        if(strcmp(argv[i],"-I") == 0) {
            mode.ir = true;
            continue;
        }
        if(strcmp(argv[i],"-T") == 0) {
            mode.ast = true;
            continue;
        }
        if(strcmp(argv[i],"-A") == 0) {
            mode.ir = true;
            mode.ast = true;
            mode.irOpt = true;
            mode.assemble = true;
            continue;
        }
        cout << "Warning: Optional Input Arg " << argv[i] <<  " Is Not Correct." << endl;
    }

    printAstInfo = mode.ast;

    // arg file name check
    if (strlen(argv[argc-1]) - 4 <= 0) {
        cout << "Error: Input File Filename Extension Is Wrong!" << endl;
        mode.error = true;
        return mode;
    }

    char subname[5];
    subname[4] = '\0';
    strncpy(subname, argv[argc-1] + strlen(argv[argc-1]) - 4, 4);

    if (strcmp(subname,".pas") != 0) {
        cout << "Error: Input File Filename Extension Is Wrong!" << endl;
        mode.error = true;
        return mode;
    }

    file = fopen(argv[argc-1], "r");
    if (file == nullptr) {
        cout << "Error: Not Existed File!" << endl;
        mode.error = true;
        return mode;
    }

    return mode;
}

void outputFileContent(string filename) {
    FILE *outputFile = fopen(filename.c_str(),"r");
    if (!outputFile) {
        cout << "Warning: Can Not Output File!" << endl;
    }
    char tempStr[OUTPUT_BUFFER_SIZE];
    while (fgets(tempStr, OUTPUT_BUFFER_SIZE, outputFile)) {
        cout << tempStr;
    }
    fclose(outputFile);
}