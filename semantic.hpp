#ifndef  __SEMANTIC_H 
#define  __SEMANTIC_H 


#include "hw3_output.hpp"
#include <iostream> 
#include <stdlib.h>

#include <list>
#include <string>
#include "enums.hpp"
#include "attributes.hpp"
extern int yylineno ;
class SymbolDescr ; 
typedef list<SymbolDescr*> Table ; 

using namespace std;

void exitScope();
void functionSymantics(Type* retType ,Id* id ,FormalsList* formalsList);
FormalDecl* addFormalDecl(Node* type , Node* id );
VarAttribute* addVarDecl(Node* type , Node* id);
VarAttribute*  addVarDeclWithInit(Node* type , Node* id , Node* exp);
bool checkVarAssign(TypeName type , ExpAttribute* exp);
FormalsList* formalListRule(FormalDecl* d , FormalsList* f);
void enterNewScope();
ExpAttribute* idExp(Node* id);
void varAssignSemantics(Node*  Id , Node*  exp);
ExpAttribute* checkFunctionCall(Node* id , ExpListAtt* el);
void enterIfScope(Node* exp);
void closeIFScope();
void enterWhileScope(Node* exp);
void closeWhileScope();
ExpAttribute* checkArithmeticSemantics(Node* exp1 , Node* exp2);
void checkByteRange(NumberExp* num) ;
ExpAttribute* checkLogicSemantics(Node* exp);
ExpAttribute* checkLogicSemantics(Node* exp1 , Node* exp2);
ExpAttribute* checkRelopSemantics(Node* exp1 , Node* exp2);
void checkBreak();
void checkContinue();
void exitGlobalScope();
class SymbolDescr
{   protected: 
    string name ;
	
	public : 
	SymbolDescr(string name): name(name) {} ;
	virtual  ~SymbolDescr() {};
    void printSymbol() {};
    string getName() {return name ; }
    virtual TypeName getType() =0 ;
    virtual int getOffset() =0 ;
	virtual void print() {};
}  ;
class VarDescr : public SymbolDescr{
	int offset ;
	TypeName type ; 
	public : 
	VarDescr( string name  , TypeName type , int offset) :SymbolDescr(name)  ,type(type) ,offset(offset){ } 
	void print(){output::printID(name,offset,getTypeNameString(type));}
    int getOffset(){return offset ; }
    TypeName getType(){return type ;} ;
};
class FuncDescr : public SymbolDescr{
	string type ;
    TypeName retType ;
    list<TypeName> parametersList ;
public :
	FuncDescr(string name  ,  FormalsList* formalsList, TypeName reTtype) ;
    FuncDescr(string name ,string types) ;
	void print(){output::printID(name,0,type);}
    int getOffset(){return -1 ; }
    TypeName getType(){return retType;} ;
    list<TypeName> getParametersList() {return parametersList ;  }
    vector<string> getTypesVector();

};
class SymbolTable{

   list<Table> tables ;
   stack<int> offsets ; 
   int argOffset ; 
   public  : 
   SymbolTable();
   void enterNewScope() ;
   void endScope();
   void exitScope() ;
   bool addSymbol(SymbolDescr* sym) ;
   int getOffset() {return offsets.top() ;};
   void pushArgs(FormalsList* formalsList) ;
   SymbolDescr*  checkIfVarExists(string id);
    bool checkDoubleMain();
};
#endif 

