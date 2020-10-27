

#include "attributes.hpp"
string getTypeNameString(TypeName type){
	switch(type){
		case typeVoid : return  string("VOID");
		case  typeInt  : return string("INT") ;
		case  typeByte : return string("BYTE") ;
		case typeB  : return string("B") ;
		case  typeBool : return string("BOOL") ;
		case typeString :return string("STRING") ;
	}
	return string("") ;
}

int strToInt(char* strOfInt){
    int num ;
    sscanf(strOfInt, "%d" , &num) ;
    return num ;
}
