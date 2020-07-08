#ifndef TABLE_H
#define TABLE_H
#include "TableSign.h"
using namespace std;
TableSign Table[200];
int tx = 1;     //符号表下标 
string KeyWord[16] = {"program" , "var" , "const" , "procedure" ,
 "begin"   , "end" ,  "if"   , "else" ,"while",
 "then"    , "do"  , "call"  , "read" , "write","odd"
};
void OutTable()
{fstream tablefile("table.txt",ios::out);
  tablefile<<"***SIGN-TABLE***"<<endl;
  tablefile<<"num\t"<<setw(8);
  tablefile<<"name \t"<<"type\t"<<"level\t"
           <<"value\t"<<"address"<<endl; 
 for(int i = 1;i<tx;i++)
 { tablefile<<"<"<<i<<"> ";
   tablefile.right;
   tablefile.width(7); 
   tablefile<<Table[i].GetName();
   tablefile.left;
   tablefile.width(7); 
   tablefile<<Table[i].GetType();
   tablefile.left;
   tablefile.width(7); 
   tablefile<<Table[i].GetLevel();
   tablefile.left;
   tablefile.width(7); 
   tablefile<<Table[i].GetValue();
   tablefile.left;
   tablefile.width(7); 
   tablefile<<Table[i].GetAdress()<<endl;
 }
tablefile.close();	
}
void enterConst(StrNode sym,int lev,int val,int adr )//添加常量 
{   Table[tx].SetName(sym.name); 
    Table[tx].SetType("CONST");
    Table[tx].SetAdress(adr) ;
    Table[tx].SetLevel(lev);
	Table[tx].SetValue(val) ; 
	tx++;
}
void enterVar(StrNode sym,int lev,int adr )  //添加变量 
{   Table[tx].SetName(sym.name); 
    Table[tx].SetType("VAR"); 
    Table[tx].SetAdress(adr) ;
    Table[tx].SetLevel(lev);
	tx++;
}
void enterProc(StrNode sym,int lev,int adr ) //添加过程名 
{   Table[tx].SetName(sym.name); 
    Table[tx].SetType("PROC"); 
    Table[tx].SetAdress(adr);
    Table[tx].SetLevel(lev);
	tx++;
}
int position(string name,int lev)                    // 查找是否定义过该标识符/并且有效
{ while(lev>=0)
 {for(int i = 1;i<=tx;i++)
	  if(name==Table[i].GetName()&&Table[i].GetLevel()==lev) return i;
	  lev--;
}
	return 0;
} 
int SetLevFalse(int lev)
{ for(int i =0;i<=tx;i++)
	if(Table[i].GetLevel()==lev) 
	{ if(Table[i].GetType()=="CONST"||Table[i].GetType()=="VAR")
	    Table[i].SetVdFalse();
    }
}
bool IsLevDfSign(string name,int lev) //当前层是否定义该标识符 
{ 	for(int i = 1;i<=tx;i++)
	  if(name==Table[i].name&&Table[i].GetValid()&&lev==Table[i].GetLevel()) { 
	  return true;}
	  //在当前过程已经被定义 
return false;
}
bool IsDfKeyWord(string name)
{  for(int i =0;i <= 14 ; i++)
	 if(KeyWord[i]==name) return true;
	 return false;
} 
#endif
