#include "TableSign.h"		
TableSign::TableSign(string tpye_,string name_,int value_,int level_,int adress_)
{    type   = tpye_;
     name   = name_;
     value  = value_;
     level  = level_;
     adress = adress_; 
}
TableSign::TableSign(string type_,string name_,int level_,int adress_)
{    type   = type_;
     name   = name_;
     level  = level_;
     adress = adress_; 	
}
void TableSign::SetSign(StrNode sym)
{  type = sym.type;
   name = sym.name;
}
TableSign::~TableSign()
{
}

