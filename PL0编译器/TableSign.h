#ifndef TABLESIGN_H
#define TABLESIGN_H
#include<string>
using namespace std;
typedef struct {
	string type;               // token���� 
	string name;              //  token ����  
} StrNode;
typedef struct {
	int line ;                 // ������
	int type;                 //  �������� 
    StrNode sign;             //  ����� 
} ErrorSign;
//--------------------------------------------------
class TableSign
{ 
   public:
   	       string   type;
           string   name;       
           bool     valid;      //�Ƿ���Ч(�ӳ����еĹ��̱���) 
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
