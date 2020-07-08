/*错误处理 */
#include<bits/stdc++.h>
#include "TableSign.h"
using namespace std;
queue<ErrorSign> error_;      //  存储出错的队列 
ErrorSign ERsign;
string KeyWords[16] = {"program" , "var" , "const" , "procedure" ,
 "begin"   , "end" ,  "if"   , "else" ,"while",
 "then"    , "do"  , "call"  , "read" , "write","odd"
};
char Sides[6] = {',',';','(',')',':'};
char Operat[5] = {'+','-','*','/'};
string RelOperat[8] = {"<","<=",">",">=","=","<>"}; 
//-----------------------------------------------------
void error(int type,int line,StrNode Node)
{ //添加队列 
	    ERsign.line = line;
	    ERsign.type = type;
	    ERsign.sign = Node;
	    error_.push(ERsign);
}
void Outerror(ErrorSign a )
{ 
	switch(a.type) 
	{
		case 0 : cout<<"第"<<a.line<<"行未识别'"<<a.sign.name<<"'符号......."<<endl;break; // 未识别类型  词法分析中 
		case 1 : cout<<"第"<<a.line<<"行缺失'"<<a.sign.name<<"'符号......."<<endl   ;break;//缺失类型;
		case 2 : cout<<"第"<<a.line<<"行标识符'"<<a.sign.name<<"'未定义......."<<endl; break;//未定义类型
		case 3 : cout<<"第"<<a.line<<"行标识符定义使用关键字....."<<a.sign.name<<"' ."<<endl; break;//标识符使用关键字 
		case 4 : cout<<"第"<<a.line<<"行 缺失ID...."<<endl  ; break;//缺失<ID> 
		case 5 : cout<<"第"<<a.line<<"行；"<<a.sign.name<<"已经被定义...,"<<endl  ; break;//标识符已经定义 
		case 6 : cout<<"第"<<a.line<<"行"<<a.sign.name<<"应该为':='....."<<endl  ; break;
		case 7 : cout<<"第"<<a.line<<"行:=缺失常数......"<<endl   ;break;//error---const--缺失常数 
		case 8 : cout<<"第"<<a.line<<"行不符合语法的标识符......"<<a.sign.name<<endl   ;break;
		case 9 :  cout<<"第"<<a.line<<"行缺失关键字'"<<a.sign.name<<"'......."<<endl   ;break;
		case 10 : cout<<"第"<<a.line<<"行缺失比较符'"<<"'......."<<endl;break;
		case 11 : cout<<"第"<<a.line<<"行缺失右比较项'"<<"'......."<<endl;break; 
		case 12 : cout<<"第"<<a.line<<"行缺失右比较项'"<<"'......."<<endl;break; 
		case 13 : cout<<"第"<<a.line<<"行 括号')'未被匹配"<<"......."<<endl;break; 
		case 14 : cout<<"存在"<<a.line<<"个begin未被匹配或缺失"<<a.line<<"个end......."<<endl;break;
		case 15 : cout<<"第"<<a.line<<"使用多余的 ’;’分号"<<endl;;break; 
		case 16 : cout<<"存在"<<a.line<<"对"<<a.sign.name<<"和"<<a.sign.type<<"未被匹配或缺失"<<endl;break;
		case 17 :  cout<<"第"<<a.line<<"行标识符'"<<a.sign.name<<"'定义为常数......."<<endl; break;
		default: break;
   }
}
void ErrorPrint()
{  //词法分析出错显示 
   int errori = 0;
   if(error_.empty())  cout<<"语法分析成功 , (0)errors......."<<endl;
   else
    while(!error_.empty())
    {   ERsign = error_.front();
	    error_.pop();
	    errori++;
	    cout<<"[error]"<<errori<<" :";
	    Outerror(ERsign);
	}
}
