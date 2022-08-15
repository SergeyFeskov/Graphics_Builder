#ifndef parsenok_h_included
    #define parsenok_h_included
    #include <iostream>
    #include <string>
    #include<cmath>
    #include <cstdlib>
    #include <cstring>
    using namespace std;
    #include <QString>
    #include <stdio.h>
    #include <string.h>
    #include <conio.h>
    // #include <math.h>
    #include "funcs.h"

    typedef enum {NUM_INT, NUM_REAL, LEFT_BRACKET, RIGHT_BRACKET, COMMA, OP_ADDITIVE, OP_MULTIPLICATIVE, OP_DEGREE, FUNC, VAR, CNST} TLexemType;

    class Error{
    private:
        QString error;
    public:
        Error(QString str){
            error = str;
        }
        QString getError(){
            return error;
        }
    };

    class TVar
    {
    private:
        string Name;
        double Value; 
        int amount;
    public:
        TVar ();
        void inputVar(double v);
        int checkVar(char*);
        double getVarValue();
        char* getVarName();
        void copyVarName(char*);
    };

    class TFunc{
    private:
        string Name;
        
    public:
        int isAnyParamsNum;
        int ParamsNum;
        double (*Calc) (double []);
        TFunc(string name, int isAnyParamsNum, 
        int ParamsNum, double (*Calc) (double [])){
            Name = name; 
            this->isAnyParamsNum = isAnyParamsNum;
            this->ParamsNum = ParamsNum;
            this->Calc = Calc; 
        }
        friend int isFunc(char*);
        friend int funcIndex(char* funcName, TFunc *funcs);
    };

    class TConst{
    public:
        string Name;
        double Value;
        TConst(string name, double value){
            Name = name;
            Value = value;
        };
        friend int isConst(char*);
        friend int constIndex(char*, TConst*);
    };

    class TLexem
    {
    public:
        TLexemType LexemType;
        char* Value;
        friend class TreeNode;
        friend TLexem* lexer(char*, int*);
        friend void addToken(TLexem**, int*, int*, char*, TLexemType);
    };

    class TTreeNode : public TLexem{
    public:  
        TTreeNode* Left;
        TTreeNode* Right;
        ~TTreeNode();
        
        friend TTreeNode* parseFuncParams(TLexem*, int, int*);
        friend TTreeNode* parseExpr(TLexem*, int, int*);
        friend TTreeNode* parseFactor(TLexem*, int, int*);
        friend TTreeNode* parseUnaryExpr(TLexem*, int, int*);
        friend TTreeNode* parseDegreeExpr(TLexem*, int, int*);
        friend TTreeNode* parseNumLiteral(TLexem*, int, int*);
        friend TTreeNode* parser(TLexem*, int);
        friend double calcExpr(TTreeNode* tree);
    };
        TLexem* lexer(char*, int*);
        TTreeNode* parseFuncParams(TLexem*, int, int*);
        TTreeNode* parseExpr(TLexem*, int, int*);
        TTreeNode* parseFactor(TLexem*, int, int*);
        TTreeNode* parseUnaryExpr(TLexem*, int, int*);
        TTreeNode* parseDegreeExpr(TLexem*, int, int*);
        TTreeNode* parseNumLiteral(TLexem*, int, int*);
        TTreeNode* parser(TLexem*, int);

    char* toString(char);
    void addToken(TLexem**, int*, int*, char*, TLexemType);
    int skipSpaces(char*, int);
    int isFunc(char*);//friend to TFunc
    int isConst(char*);//friend to TConst

    double calcExpr(TTreeNode* tree);//friend to TTreeNode
    //поиск по массиву TFunc
    int funcIndex(char* funcName);
    //вычисление значения функции, лежит в culcExpr
    void getTuple(TTreeNode* tree, double** params, int* amAdr);

    void inputVars(double V);

#endif
