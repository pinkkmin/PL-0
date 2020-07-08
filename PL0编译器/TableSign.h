#ifndef TABLESIGN_H
#define TABLESIGN_H
#include<string>
using namespace std;
typedef struct {
	string type;               // token类型 
	string name;              //  token 名称  
} StrNode;
typedef struct {
	int line ;                 // 出错行
	int type;                 //  出错类型 
    StrNode sign;             //  出错符 
} ErrorSign;
//--------------------------------------------------
class TableSign
{ 
   public:
   	       string   type;
           string   name;       
           bool     valid;      //是否有效(子程序中的过程变量) 
		   int      value;
           int      level;
           int      adress;
        TableSign(){  valid = true;
		}
        TableSign(string tpye_,string name_,int value_,int level_,int adress_);
		TableSign(string type_,string name_,int level_,int adress_);
	   ~TableSign();
	   void SetSign(StrNode sym);
	   void SetVdTrue(){
	        valid  =true;
	   }
	   void SetVdFalse(){
	     valid = false;
	   }
		bool GetValid(){
        return valid;
		 } 
	   void SetType(string type_){
	   	type  = type_;
	   }
	   void SetName(string name_){
	   	name = name_;
	   }
	   void SetValue(int value_){
	     value = value_;
	   }
	   void SetLevel(int level_){
	   	 level = level_;
	   }
	   void SetAdress(int adress_){
	   	adress = adress_;
	   }
	   string GetType(){
	   	return type;
	   }
	   string GetName(){
	   	return name;
	   }
	   int GetValue(){
	   	return value;
	   }
	   int GetLevel(){
	   	return level;
	   }
	   int GetAdress(){
	   	return adress;
	   }
};

#endif
