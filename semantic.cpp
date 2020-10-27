#include "semantic.hpp"
extern int yylineno;
extern int inWhileScope ;
SymbolTable symbolTable = SymbolTable() ;

FuncDescr::FuncDescr(string name ,string types) : SymbolDescr(name) , type(types), retType(typeVoid)  ,parametersList(list<TypeName>()){
    if(name == "print")
        parametersList.push_back(typeString);
    else
        parametersList.push_back(typeInt);
}
FuncDescr::FuncDescr(string name , FormalsList* f , TypeName retType) : SymbolDescr(name) , type(string("")) ,retType(retType),parametersList(list<TypeName>()) {

    type+="(";

    if(f!=NULL){
        list<FormalDecl> paramtersListTemp = f->getArgs() ;
        int size = paramtersListTemp.size() ;

        for(const auto e : paramtersListTemp){
            type+= getTypeNameString(e.getType()) ;
            parametersList.push_back(e.getType());
            if(size>1)
                type+="," ;
            --size;

        }
    }

    type+=")->";
    type+=getTypeNameString(retType);
}
vector<string> FuncDescr::getTypesVector(){
    vector<string> parametersVector ;
    for(const auto e : parametersList){
        parametersVector.push_back(getTypeNameString(e)) ;
    }
    return parametersVector;
}

SymbolTable::SymbolTable( ) :tables(list<Table>()) , offsets(stack<int>()){
    tables.push_back(Table());
    offsets.push(0);
    addSymbol(new FuncDescr("print" ,"(STRING)->VOID" ));
    addSymbol(new FuncDescr("printi","(INT)->VOID") );


}
void SymbolTable::enterNewScope(){
    tables.push_back(Table()) ;
    offsets.push(offsets.top()) ;
}
bool SymbolTable::addSymbol(SymbolDescr* sym){
    if(checkIfVarExists(sym->getName())){
        return false ;
    }

    tables.back().push_back(sym);
    if(sym->getOffset()>=0)
        ++(offsets.top());
    return true ;
}

SymbolDescr* SymbolTable::checkIfVarExists(string id){
    for(const auto& table : tables){
        for(const auto& descr : table) {
            if(descr->getName() == id){
                return descr ;
            }
        }
    }
    return NULL ;

}
bool SymbolTable::checkDoubleMain(){
    int i = 0 ;
    for(const auto& table : tables){
        for(const auto& descr : table) {
            if(descr->getName() == "main")
                i++ ;
        }
    }
    if(i>1)
        return true  ;
    return false ;
}
void SymbolTable::endScope(){
    Table table = tables.back() ;
    for(const auto& descr : table) {
        descr->print();
    }
    tables.pop_back() ;
    offsets.pop() ;
}
void SymbolTable::pushArgs(FormalsList* flist ){
    if(!flist) return ;
    list<FormalDecl> args = flist->getArgs() ;
    int offset=-1 ;
    for(const auto e :args ){
        symbolTable.addSymbol(new VarDescr(e.getName() , e.getType() , offset));
        --offset ;

    }
}
void enterNewScope(){
    symbolTable.enterNewScope() ;
}

void functionSymantics(Type* retType ,Id* id ,FormalsList* formalsList){
    //cout<<"linefucked"<<id->getName()<<endl;
    SymbolDescr* funcSign = new FuncDescr(id->getName() , formalsList , retType->getType()) ;
    if(!symbolTable.addSymbol(funcSign)){
        output::errorDef(yylineno, id->getName()) ;
        exit(0) ;
    }
    symbolTable.enterNewScope();
    symbolTable.pushArgs(formalsList) ;
    //
}

void exitScope(){

    output::endScope() ;
    symbolTable.endScope() ;
}
FormalsList* formalListRule(FormalDecl* d , FormalsList* f){
    if(f->addArg(*d))
        return f ;
    output::errorDef(yylineno, d->getName()) ;
    exit(0) ;
    return f;
}
FormalDecl* addFormalDecl(Node* type , Node* id ){
    //{cout<<"mainend: "<<id->getName() <<endl;}
    return new FormalDecl(type->getType() , id->getName() ) ;
}

VarAttribute* addVarDecl(Node*  type , Node*  id){

    SymbolDescr* var = new VarDescr( id->getName() ,type->getType() ,  symbolTable.getOffset()) ;
    if(!symbolTable.addSymbol(var)){
        output::errorDef(yylineno, id->getName()) ;
        exit(0) ;
    }
    return new VarAttribute() ;
}

void callMismatch(){
        output::errorMismatch(yylineno) ;
        exit(0);
};
bool checkVarAssign(TypeName type , ExpAttribute* exp){

    if(type != exp->getType()){

        if(exp->getType()==typeByte && type ==typeInt)
            return true ;
        return false;
    }

    return true ;
}
VarAttribute*  addVarDeclWithInit(Node*  type , Node*  id , Node*  exp){

    SymbolDescr* var = new VarDescr(id->getName(),type->getType()  , symbolTable.getOffset()) ;
    if(!checkVarAssign((type)->getType() , dynamic_cast<ExpAttribute*>(exp))){
        callMismatch();
    }
    if(!symbolTable.addSymbol(var)){
        output::errorDef(yylineno, id->getName()) ;
        exit(0) ;
    }
    return new VarAttribute() ;
}


void varAssignSemantics(Node*  id , Node*  exp){
    SymbolDescr* sym = symbolTable.checkIfVarExists(id->getName());
    if(!sym){
        output::errorUndef(yylineno,id->getName()) ;
        exit(0) ;
      // call undefined Error
    }
    if(!checkVarAssign(sym->getType(), dynamic_cast<ExpAttribute*>(exp))){
        callMismatch();
    }

}

ExpAttribute* idExp(Node* id){
    SymbolDescr* sym = symbolTable.checkIfVarExists(id->getName()) ;

    if(!sym){
        output::errorUndef(yylineno,id->getName()) ;
        exit(0) ;
        // call undefined Error
    }
    //cout<<id->getName()<<getTypeNameString(sym->getType())<<endl;
    return new ExpAttribute(sym->getType());
}

ExpAttribute* checkFunctionCall(Node* id , ExpListAtt* el){

    SymbolDescr* sym = symbolTable.checkIfVarExists(id->getName()) ;
    if(!sym){

        output::errorUndefFunc(yylineno , id->getName());
        exit(0) ;
        // call undefined Error
    }

    FuncDescr* funSym = dynamic_cast<FuncDescr*>(sym) ;
    if(!funSym){

        output::errorUndefFunc(yylineno , id->getName());
        exit(0) ;
    }
    list<TypeName> parametersList = funSym->getParametersList() ;
    list<ExpAttribute> expList = list<ExpAttribute>() ;
    if(el)
        expList = el->getExpList() ;

    vector<string> tv = funSym->getTypesVector() ;
    for(const auto & t: parametersList){
        ExpAttribute exp=  expList.back() ;
        if((expList.size()==0) ||(!checkVarAssign(t, &exp))){
            output::errorPrototypeMismatch(yylineno , id->getName() , tv) ;
            exit(0) ;
        }

         expList.pop_back();
    }
    return new ExpAttribute(sym->getType());
}
void checkBoolExp(Node* exp){
    if(exp->getType()!=typeBool){
        callMismatch();
    }
}

void enterIfScope(Node* exp){
    checkBoolExp(exp) ;
    enterNewScope();
}
void closeIFScope(){
    exitScope() ;
}
void enterWhileScope(Node* exp){
    checkBoolExp(exp) ;
    enterNewScope();
    inWhileScope++;
}

void closeWhileScope(){
    exitScope() ;
    inWhileScope-- ;
}
TypeName getBiggerRange(TypeName t1 , TypeName t2){
    if(t1 == typeInt|| t2==typeInt )
        return typeInt;
    if(t1 == typeByte|| t2==typeByte )
        return typeByte;
    return typeBool ;
}
void checkNumeric(TypeName t1 , TypeName t2){
    if(t1 == typeVoid ||t1 == typeString   )
        callMismatch() ;
    if(t2 == typeVoid ||t2 == typeString )
        callMismatch() ;
}
ExpAttribute* checkArithmeticSemantics(Node* exp1 , Node* exp2){
    checkNumeric(exp1->getType() ,exp2->getType() ) ;
    TypeName resultType = getBiggerRange(exp1->getType() ,exp2->getType() );
    return new ExpAttribute(resultType);
}
void checkByteRange(NumberExp* num){
    if(num->getNum() >255){
        output::errorByteTooLarge(yylineno , num->getNumString());
        exit(0) ;
    }
}
ExpAttribute* checkLogicSemantics(Node* exp){
    if(exp->getType()!=typeBool){
        callMismatch() ;
        return NULL ;
    }
    return new ExpAttribute(typeBool);
}
ExpAttribute* checkLogicSemantics(Node* exp1 , Node* exp2){
    if(exp1->getType()!=typeBool ||exp2->getType()!=typeBool){
        callMismatch() ;
        return NULL ;
    }
    return new ExpAttribute(typeBool);
}
ExpAttribute* checkRelopSemantics(Node* exp1 , Node* exp2){

    checkNumeric(exp1->getType() ,exp2->getType() ) ;
    return new ExpAttribute(typeBool);
}

void checkBreak(){
    if(inWhileScope)
        return ;
    output::errorUnexpectedBreak(yylineno) ;
    exit(0);
}
void checkContinue(){
    if(inWhileScope)
        return ;
    output::errorUnexpectedContinue(yylineno);
    exit(0);
}
void exitGlobalScope(){

    SymbolDescr* sym = symbolTable.checkIfVarExists("main") ;
    FuncDescr* funSym = dynamic_cast<FuncDescr*>(sym) ;
    if(!funSym){
        output::errorMainMissing() ;
        exit(0);
    }
    if((((funSym->getParametersList()).size())!=0) || ((funSym->getType()) !=typeVoid)){
        output::errorMainMissing() ;
        exit(0);
    }



    exitScope() ;
}