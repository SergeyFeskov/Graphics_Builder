#ifndef parser_cpp_included
    #define parser_cpp_included
    #include <iostream>
    #include <string>
    #include <cmath>
    #include <cstring>
    #include "parser.h"
    #include "funcs.h"
    using namespace std;

    TFunc Funcs[] = {{"sin", 0, 1, f_sin}, {"cos", 0, 1, f_cos}, {"tg", 0, 1, f_tg}, {"ctg", 0, 1, f_ctg}, 
    {"abs", 0, 1, f_abs}, {"sqrt", 0, 1, f_sqr}};
    const int FuncsAm = 6; 

    TConst Consts[] = {{"π", M_PI}, {"e", M_E}};
    const int ConstsAm = 2;

    TVar *Vars = NULL;
    int VarsAm = 0;

    //const double eps = 1e-10;

    void TVar::inputVar(double v){

        this->Value = v;
    }

    void inputVars(double v)//хз нужна ли
    {
        for (int i = 0; i < VarsAm; i++)
        {
            Vars[i].inputVar(v);
        }
    }

    double max(double num1, double num2)
    {
        if (num1 > num2)
        {
            return num1;
        }
        else
        {
            return num2;
        }
    }

    int TVar::checkVar(char* varName){
        // char *str = (char*)calloc(this->Name.length(), sizeof(char));
        // for(int i = 0; i < this->Name.length(); i++){
        //     str[i]=this->Name[i];
        // }
        // return strcmp(varName, str);
        string name(varName);
        if(name==this->Name){
            return 0;
        }
        else{
            return 1;
        }
    }

//хз нужна ли
    int varIndex(char* varName)
    {
        for (int i = 0; i < VarsAm; i++)
        {
            if (Vars[i].checkVar(varName)==0)
            {
                return i;
            }
        }
        return 1;
    }

 

    int funcIndex(char* funcName, TFunc *Funcs)
    {
        string f(funcName);
        for (int i = 0; i < FuncsAm; i++)
        {
            if (f==Funcs[i].Name)
            {
                return i;
            }
        }
        return 1;
    }

    int constIndex(char* constName, TConst *Consts)
    {
        string name(constName);
        for (int i = 0; i < ConstsAm; i++)
        {
            if (constName==Consts[i].Name)
            {
                return i;
            }
        }
        return 1;

    }



    double TVar::getVarValue(){
        return this->Value;
    }

    double calcExpr(TTreeNode* tree)
    {
        switch (tree->LexemType)
        {
        case NUM_INT:
        case NUM_REAL:
            return atof(tree->Value);
            break;

        case CNST:
            return Consts[constIndex(tree->Value, Consts)].Value;
            break;

        case VAR:
            return Vars[varIndex(tree->Value)].getVarValue();

        case OP_ADDITIVE:
            if (tree->Right == NULL)
            {
                if (tree->Value[0] == '-')
                {
                    return - calcExpr(tree->Left);
                }
                else
                {
                    return + calcExpr(tree->Left);
                }
            }
            else
            {
                if (tree->Value[0] == '-')
                {
                    return calcExpr(tree->Left) - calcExpr(tree->Right);
                }
                else
                {
                    return calcExpr(tree->Left) + calcExpr(tree->Right);
                }
            }
            break;

        case OP_MULTIPLICATIVE:
            if (tree->Value[0] == '*')
            {
                return calcExpr(tree->Left)*calcExpr(tree->Right);
            }
            else
            {
                double numer = calcExpr(tree->Left);
                double denumer = calcExpr(tree->Right);
                return numer/denumer;


            }
            break;
        
        case OP_DEGREE:
            return pow(calcExpr(tree->Left), calcExpr(tree->Right));
            break;

        case FUNC:
            int ind = funcIndex(tree->Value, Funcs);
            double* params = NULL;
            int paramsAm = 0;
            getTuple(tree->Left, &params, &paramsAm);
            if (Funcs[ind].isAnyParamsNum)
            {
                Funcs[ind].ParamsNum = paramsAm;
                return Funcs[ind].Calc(params);
            }
            else if (paramsAm == Funcs[ind].ParamsNum)
            {
                return Funcs[ind].Calc(params);
            }
            else
            {
                cout<<"Error: unexpected func's number of parameters: "<<
                Funcs[ind].ParamsNum;
                exit(EXIT_FAILURE);
            }
            break;

          cout<<"Error: unexpected lexem type in calculations.";
          exit(EXIT_FAILURE);
          break;
        }
        return 501;
    }

    TTreeNode::~TTreeNode(){
        delete Left;
        delete Right;
    }

    void getTuple(TTreeNode* tree, double** paramsAdr, int* amAdr)
    {
        if (tree->LexemType != COMMA)
        {

            (*amAdr)++;
            *paramsAdr = (double*)realloc(*paramsAdr, (*amAdr)*sizeof(double));
            (*paramsAdr)[(*amAdr)-1] = calcExpr(tree);
        }
        else
        {
            getTuple(tree->Left, paramsAdr, amAdr);
            (*amAdr)++;
            *paramsAdr = (double*)realloc(*paramsAdr, (*amAdr)*sizeof(double));
            (*paramsAdr)[(*amAdr)-1] = calcExpr(tree->Right);
        }
    }

    TTreeNode* parseExpr(TLexem*, int, int*);

  TTreeNode* parseFuncParams(TLexem* lexemsArr, int lexemsAm, int* posAdr)
    {
        TTreeNode* funcParamsAdr = parseExpr(lexemsArr, lexemsAm, posAdr);
        
        while (*posAdr < lexemsAm && lexemsArr[*posAdr].LexemType != RIGHT_BRACKET)
        {
            if (lexemsArr[*posAdr].LexemType == COMMA)
            {
                TTreeNode* cortegeNodeAdr = (TTreeNode*)malloc(sizeof(TTreeNode));
                cortegeNodeAdr->Left = funcParamsAdr;
                cortegeNodeAdr->LexemType = lexemsArr[*posAdr].LexemType;
                cortegeNodeAdr->Value = lexemsArr[*posAdr].Value;
                (*posAdr)++;
                cortegeNodeAdr->Right = parseExpr(lexemsArr, lexemsAm, posAdr);
                funcParamsAdr = cortegeNodeAdr;
            }
        }
        if (lexemsArr[*posAdr].LexemType == RIGHT_BRACKET)
        {
            return funcParamsAdr;
        }
        else
        {
            throw Error("Error: Right bracket not found.");
        }
    }


 TTreeNode* parseNumLiteral(TLexem* lexemsArr, int lexemsAm, int* posAdr)
    {
        if (*posAdr > lexemsAm)
        {
            throw Error("Error: Number expected, but end of token's list found.");
        }
        
        if (lexemsArr[*posAdr].LexemType == NUM_INT || 
            lexemsArr[*posAdr].LexemType == NUM_REAL || 
            lexemsArr[*posAdr].LexemType == VAR || 
            lexemsArr[*posAdr].LexemType == CNST)
        {
            TTreeNode* numberNodeAdr = (TTreeNode*)malloc(sizeof(TTreeNode));
            numberNodeAdr->Left = NULL;
            numberNodeAdr->LexemType = lexemsArr[*posAdr].LexemType;
            numberNodeAdr->Value = lexemsArr[*posAdr].Value;
            numberNodeAdr->Right = NULL;
            (*posAdr)++;
            return numberNodeAdr;
        }

        if (lexemsArr[*posAdr].LexemType == FUNC)
        //TODO: добавить проверку на отсутствие открыв. скобки
        {
            TTreeNode* funcNodeAdr = (TTreeNode*)malloc(sizeof(TTreeNode));
            funcNodeAdr->LexemType = lexemsArr[*posAdr].LexemType;
            funcNodeAdr->Value = lexemsArr[*posAdr].Value;
            (*posAdr) += 2;
            funcNodeAdr->Left = parseFuncParams(lexemsArr, lexemsAm, posAdr);
            if (*posAdr >= lexemsAm || lexemsArr[*posAdr].LexemType != RIGHT_BRACKET)
            {
                throw Error("Error: Right bracket not found!");

            }
            else
            {
                (*posAdr)++;
            }
            funcNodeAdr->Right = NULL;
            return funcNodeAdr;
        }
        

        if (lexemsArr[*posAdr].LexemType == LEFT_BRACKET)
        {
            (*posAdr)++;
            TTreeNode* exprNodeAdr = parseExpr(lexemsArr, lexemsAm, posAdr);
            if (*posAdr >= lexemsAm || lexemsArr[*posAdr].LexemType != RIGHT_BRACKET)
            {

                throw Error("Error: Right bracket not found.");
            }
            else
            {
                (*posAdr)++;
            }
            return exprNodeAdr;
        }
        else
        {
            throw Error("Error: unknown token in parser.");

        }
    }


    TTreeNode* parseDegreeExpr(TLexem* lexemsArr, int lexemsAm, int* posAdr)
    {
        TTreeNode* degreeExprAdr = parseNumLiteral(lexemsArr, lexemsAm, posAdr);
        while (*posAdr < lexemsAm)
        {
            if (lexemsArr[*posAdr].LexemType == OP_DEGREE)
            {
                TTreeNode* degreeNodeAdr = (TTreeNode*)malloc(sizeof(TTreeNode));
                degreeNodeAdr->Left = degreeExprAdr;
                degreeNodeAdr->LexemType = lexemsArr[*posAdr].LexemType;
                degreeNodeAdr->Value = lexemsArr[*posAdr].Value;
                (*posAdr)++;
                degreeNodeAdr->Right = parseNumLiteral(lexemsArr, lexemsAm, posAdr);
                degreeExprAdr = degreeNodeAdr;
            }
            else
            {
                return degreeExprAdr;
            }
        }
        return degreeExprAdr;
    }


TTreeNode* parseUnaryExpr(TLexem* lexemsArr, int lexemsAm, int* posAdr)
    {
        if (*posAdr > lexemsAm)
        {
            throw Error("Error: unary expression not found.");

        }
        
        if (lexemsArr[*posAdr].LexemType == OP_ADDITIVE)
        {
            if (lexemsArr[*posAdr].Value[0] == '-')
            {
                TTreeNode* unaryNodeAdr = (TTreeNode*)malloc(sizeof(TTreeNode));
                unaryNodeAdr->LexemType = lexemsArr[*posAdr].LexemType;
                unaryNodeAdr->Value = lexemsArr[*posAdr].Value;
                (*posAdr)++;
                unaryNodeAdr->Left = parseDegreeExpr(lexemsArr, lexemsAm, posAdr);
                unaryNodeAdr->Right = NULL;
                return unaryNodeAdr;
            }
            else
            {
                (*posAdr)++;
                return parseDegreeExpr(lexemsArr, lexemsAm, posAdr);
            }
        }
        else
        {
            return parseDegreeExpr(lexemsArr, lexemsAm, posAdr);
        }
    }


      TTreeNode* parseFactor(TLexem* lexemsArr, int lexemsAm, int* posAdr)
    {
        TTreeNode* factorAdr = parseUnaryExpr(lexemsArr, lexemsAm, posAdr);
        
        while (*posAdr < lexemsAm)
        {
            if (lexemsArr[*posAdr].LexemType == OP_MULTIPLICATIVE)
            {
                TTreeNode* mulNodeAdr = (TTreeNode*)malloc(sizeof(TTreeNode));
                mulNodeAdr->Left = factorAdr;
                mulNodeAdr->LexemType = lexemsArr[*posAdr].LexemType;
                mulNodeAdr->Value = lexemsArr[*posAdr].Value;
                (*posAdr)++;
                mulNodeAdr->Right = parseUnaryExpr(lexemsArr, lexemsAm, posAdr);
                factorAdr = mulNodeAdr;
            }
            else
            {
                return factorAdr;
            }
        }   
        return factorAdr;
    }


    TTreeNode* parseExpr(TLexem* lexemsArr, int lexemsAm, int* posAdr)
    {
        TTreeNode* exprAdr = parseFactor(lexemsArr, lexemsAm, posAdr);
        
        while (*posAdr < lexemsAm)
        {
            if (lexemsArr[*posAdr].LexemType == OP_ADDITIVE)
            {
                TTreeNode* sumNodeAdr = (TTreeNode*)malloc(sizeof(TTreeNode));
                sumNodeAdr->Left = exprAdr;
                sumNodeAdr->LexemType = lexemsArr[*posAdr].LexemType;
                sumNodeAdr->Value = lexemsArr[*posAdr].Value;
                (*posAdr)++;
                sumNodeAdr->Right = parseFactor(lexemsArr, lexemsAm, posAdr);
                exprAdr = sumNodeAdr;
            }
            else if (lexemsArr[*posAdr].LexemType == RIGHT_BRACKET)
            {
                return exprAdr;
            }
            else if (lexemsArr[*posAdr].LexemType == COMMA)
            {
                return exprAdr;
            }
            else
            {
                throw Error("Error: binary operator not found.");

            }
        }
        return exprAdr;
    }

  
    

   
  
    TTreeNode* parser(TLexem* lexemsArr, int lexemsAm)
    {
        int pos = 0;
        return parseExpr(lexemsArr, lexemsAm, &pos);
    }

    void addToken(TLexem** lexemsArrAdr, int* arrSizeAdr, int* lexemsAmAdr, char* token, TLexemType tokenType)
    {
        (*lexemsAmAdr)++;
        if (*arrSizeAdr < *lexemsAmAdr)
        {
            *arrSizeAdr += 1000;
            *lexemsArrAdr = (TLexem*)realloc(*lexemsArrAdr, (*arrSizeAdr)*sizeof(TLexem));
        }
        (*lexemsArrAdr)[*lexemsAmAdr-1].LexemType = tokenType;
        (*lexemsArrAdr)[*lexemsAmAdr-1].Value = token;
    }

    char* toString(char character)
    {
        char* str = (char*)calloc(2, sizeof(char));
        str[1] = '\0';
        str[0] = character;
        return str;
    }

    int isSpace(char character)
    {
        return (character == ' ') || (character == '\t') || (character == '\r') || (character == '\n');
    }

    int skipSpaces(char* expr, int pos)
    {
        while (pos < strlen(expr) && isSpace(expr[pos]) )
        {
        pos++;
        }
        return (pos < strlen(expr)) ? pos : -1;
    }

    TVar::TVar(){}

    void TVar::copyVarName(char* newName){
        string nName(newName);
        this->Name=nName;
    }

    TLexem* lexer(char* expr, int* lexemsAmAdr)
    {
        VarsAm = 0;
        int pos = 0;
        int arrSize = 100;
        int lexemsAm = 0;
        TLexem* lexemsArr = (TLexem*)calloc(arrSize, sizeof(TLexem));
        
        int varsSize = 100;
        Vars = (TVar*)calloc(varsSize, sizeof(TVar));
        while (expr[pos] != '\0')
        {
            pos = skipSpaces(expr, pos);
            int pointPassed = 0;
            int symbolsAm = 0;
            int strSize = 10;
            char* litStr = NULL;
            switch (expr[pos])
            {
            case ',':
                addToken(&lexemsArr, &arrSize, &lexemsAm, toString(','), COMMA);
                pos++;
                break;

            case '(':
                addToken(&lexemsArr, &arrSize, &lexemsAm, toString('('), LEFT_BRACKET);
                pos++;
                break;

            case ')':
                addToken(&lexemsArr, &arrSize, &lexemsAm, toString(')'), RIGHT_BRACKET);
                pos++;
                break;
            
            case '+':
                addToken(&lexemsArr, &arrSize, &lexemsAm, toString('+'), OP_ADDITIVE);
                pos++;
                break;

            case '-':
                addToken(&lexemsArr, &arrSize, &lexemsAm, toString('-'), OP_ADDITIVE);
                pos++;
                break;

            case '*':
                addToken(&lexemsArr, &arrSize, &lexemsAm, toString('*'), OP_MULTIPLICATIVE);
                pos++;
                break;

            case '/':
                addToken(&lexemsArr, &arrSize, &lexemsAm, toString('/'), OP_MULTIPLICATIVE);
                pos++;
                break;

            case '^':
                addToken(&lexemsArr, &arrSize, &lexemsAm, toString('^'), OP_DEGREE);
                pos++;
                break;
            
            case '0':
                if (expr[pos+1] == '.')
                {
                    symbolsAm += 2;
                    litStr = (char*)calloc(strSize, sizeof(char));
                    litStr[0] = '0';
                    litStr[1] = '.';
                    pos += 2;
                    pointPassed = 1;
                }
                else
                {
                    throw Error("Error: Unknown symbol in expression.");
                }
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (!pointPassed)
                {
                    litStr = (char*)calloc(strSize, sizeof(char));
                    do
                    {
                        symbolsAm++;
                        if (symbolsAm > strSize-1)
                        {
                            strSize += 10;
                            litStr = (char*)realloc(litStr, strSize*sizeof(char));
                        }
                        litStr[symbolsAm-1] = expr[pos];
                        pointPassed = (expr[pos] == '.'); 
                        pos++;
                    } while (((expr[pos] >= '0' && expr[pos] <= '9') || expr[pos] == '.') && !pointPassed);
                }
                if (pointPassed)
                {
                    if (expr[pos] >= '0' && expr[pos] <= '9')
                    {
                        do
                        {
                            symbolsAm++;
                            if (symbolsAm > strSize-1)
                            {
                                strSize += 10;
                                litStr = (char*)realloc(litStr, strSize*sizeof(char));
                            }
                            litStr[symbolsAm-1] = expr[pos];
                            pos++;
                        } while (expr[pos] >= '0' && expr[pos] <= '9');
                    }
                    else
                    {
                        throw Error("Error: incorrect real number representation.");
                    }
                }
                litStr = (char*)realloc(litStr, (symbolsAm+1)*sizeof(char));
                litStr[symbolsAm] = '\0';
                if (pointPassed)
                {
                    addToken(&lexemsArr, &arrSize, &lexemsAm, litStr, NUM_REAL);
                }
                else
                {
                    addToken(&lexemsArr, &arrSize, &lexemsAm, litStr, NUM_INT);
                }
                break;
            default:

                if (expr[pos] >= 'a' && expr[pos] <= 'z')
                {
                    litStr = (char*)calloc(strSize, sizeof(char));
                    do
                    {
                        symbolsAm++;
                        if (symbolsAm > strSize-1)
                        {
                            strSize += 10;
                            litStr = (char*)realloc(litStr, strSize*sizeof(char));
                        }
                        litStr[symbolsAm-1] = expr[pos];
                        pos++;

                    } while ((expr[pos] >= 'a' && expr[pos] <= 'z') || (expr[pos] >= '0' && expr[pos] <= '9'));
                    if (isFunc(litStr))
                    {
                        addToken(&lexemsArr, &arrSize, &lexemsAm, litStr, FUNC);
                    }
                    else if (isConst(litStr))
                    {
                        addToken(&lexemsArr, &arrSize, &lexemsAm, litStr, CNST);
                    }
                    else
                    {
                        VarsAm++;
                        if (VarsAm > varsSize-1)
                        {
                            varsSize += 100;
                            Vars = (TVar*)realloc(Vars, varsSize*sizeof(TVar));
                        }
                        Vars[VarsAm-1].copyVarName(litStr);
                        addToken(&lexemsArr, &arrSize, &lexemsAm, litStr, VAR);
                    }
                }
                else
                {
                    throw Error("Error: unknown token.");
                }
                break;
            }
        }
        Vars = (TVar*)realloc(Vars, VarsAm*sizeof(TVar));
        lexemsArr = (TLexem*)realloc(lexemsArr, lexemsAm*sizeof(TLexem));
        *lexemsAmAdr = lexemsAm;

        return lexemsArr;
    }

    int isFunc(char* w)
    {
        string word(w);

        for (int i = 0; i < FuncsAm; i++)
        {
            if (word==Funcs[i].Name)
            {
                return 1;
            }
        }
        return 0;
    }

    int isConst(char* w)
    {
        string word(w);
        for (int i = 0; i < ConstsAm; i++)
        {
            if (word==Consts[i].Name)
            {
                return 1;
            }
        }
        return 0;
    }
#endif
