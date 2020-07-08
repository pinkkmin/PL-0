#include "table.h"
string KeyWord[16] = {"program" , "var" , "const" , "procedure" ,
 "begin"   , "end" ,  "if"   , "else" ,"while",
 "then"    , "do"  , "call"  , "read" , "write","odd"
};
void table::enterConst(StrNode sym,int lev,int val,int adr )//添加常量 
{   Table[tx].SetSign(sym); 
    Table[tx].SetAdress(adr) ;
    Table[tx].SetLevel(lev);
	Table[tx].SetValue(val) ; 
	tx++;
}
void table::enterVar(StrNode sym,int lev,int adr )  //添加变量 
{   Table[tx].SetSign(sym); 
    Table[tx].SetAdress(adr) ;
    Table[tx].SetLevel(lev);
	tx++;
}
void table::enterProc(StrNode sym,int lev,int adr ) //添加过程名 
{   Table[tx].SetSign(sym); 
    Table[tx].SetAdress(adr);
    Table[tx].SetLevel(lev);
	tx++;
}
int table::position(string name){
     for(int i = 1;i<=tx;i++)
	  if(name == Table[i].GetName())  return i;
	return 0;
} 
bool IsDfKeyWord(string name)
{  for(int i =0;i <= 14 ; i++)
	 if(KeyWord[i]==name) return true;
	 return false;
} 
table::~table()
{
}
