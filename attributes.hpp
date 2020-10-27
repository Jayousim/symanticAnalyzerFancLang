#ifndef __ATTRIBUTES_H
#define __ATTRIBUTES_H

#include <list>
#include <stack>
#include <string>
#include "enums.hpp"
#include "hw3_output.hpp"
#include <stdio.h>

using namespace std ;
int strToInt(char* strOfInt);
class Node{
	public : 
	virtual ~Node() {} ;
    virtual string getName() const { };
    virtual TypeName getType()const {} ;
};
class Type : public  Node{
	
	TypeName name ; 
public:
	Type(TypeName type) : name  (type){ } 
	TypeName  getType()const {return name ; }
    
};
class Id : public Node {
	string name ; 
	public : 
	Id(string name ) : name(name) {} 
	Id(char* name ) : name(name) {} 
	string getName()const {return name ; }
};
class VarAttribute : public  Node {
   	string name ; 
	TypeName type ; 
	public :
    string getName()const {return name ; }
    TypeName  getType()const  {return type;} ;
};
class ExpAttribute : public  Node {
	bool isConst;
	int value;
    TypeName type ; 

	public:
    ExpAttribute(TypeName t) :type(t) {}
    TypeName  getType()const  {return type;} ;
};

class  FormalDecl : public  Node{
    TypeName type ;
    string name ;
public :
    FormalDecl(TypeName type  , string name ) : type(type) , name(name){} ;
    TypeName getType()const {return type;} ;
    string getName()const{return name;}  ;

};
/*class  FuncDecl : public  Node{
	string name ; 
	int params ;
	stack<FormalDecl> args ;
	string returnVal ; 
	public :
	FuncDecl(string name , const stack<FormalDecl>& args  , string returnVal):name(name) , args(args) , returnVal(returnVal) {};
	void setReturn(string r){returnVal = r ; };
	void setArgs(stack<FormalDecl> params){args=params;} ;
};

*/
string getTypeNameString(TypeName) ; 


class StringExp :public  Node{
	string str ; 
	public : 
	StringExp(char* str) : str(str) {} ;
};
class NumberExp: public  Node{
    string numStr ;
    int num;
	public : 
	NumberExp(char* str) : numStr(str) , num(strToInt(str))  {}
	int getNum() {return num ;}
	string getNumString(){return numStr ;}

};



class FormalsList  : public  Node{
	list<FormalDecl> args ;
	public :
	FormalsList():args(list<FormalDecl>()) {};
    FormalsList(FormalDecl d) :args(list<FormalDecl>()){args.push_back(d);}
	FormalsList(FormalsList* f) : args(f->args) { } ;
    bool addArg(const FormalDecl &arg) {
        for(const auto& e :args ){

            if((e.getName())==(arg.getName())) {
               return false ;
            }
       }
        args.push_front(arg);
        return true ;
    };
	list<FormalDecl>  getArgs(){return args;};
	
	
};
class ExpListAtt  : public  Node{
    list<ExpAttribute> exprs ;
public :
    ExpListAtt() : exprs(list<ExpAttribute>()) {} ;
    ExpListAtt(ExpAttribute* e) : exprs(list<ExpAttribute>()) {exprs.push_back(*e);}
    ExpListAtt(ExpListAtt* list ,ExpAttribute* e ) : exprs(list->exprs) { exprs.push_back(*e);}
    list<ExpAttribute> getExpList() {return exprs ; }

};

#define YYSTYPE Node*	// Tell Bison to use STYPE as the stack type


#endif
