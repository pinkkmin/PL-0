#include "error.h"
using namespace std;
string SignTable[100] = {} ;        //标识符表 
string ConstTable[100]  ={} ;       //常数表 
char strToken[10];                  //存放构成单词符号的字符串
char source[10000]={};              //缓存文件内容 
char ch; 
int Token_i = 0,Search_i=0,SignT_i = 0,ConstT_i = 0;
fstream file("data.txt",ios::in);
fstream outfile("lex.txt",ios::out);
void GetChar() //将下一字符读到ch中，搜索指示器前移一字符位置 
{   ch = source[Search_i];
    Search_i++;
}
void GetBC()//检查ch是否为空白 是 调用GetChar 直至ch进入非空 
{
	while(ch==' '||ch=='	')        // 空格或者TAB键 
		           GetChar();
}
void Concat()//将ch连接至strToken 
{
	strToken[Token_i] = ch;
	Token_i++;
 } 
 void Retract() //将搜索指示器回调一个字符位置，将ch置空白符 
{
	Search_i--;
	ch = ' ';
 } 
 bool IsLetter()//是否为字母 
 {
 	if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')) 	return true;    
  return false;
 }
bool IsDight()//是否为数字 
 {
   if(ch>='0'&&ch<='9')   return true;    
  return false;
 }
 bool IsSides()//界符 
{
	for(int i = 0;i<5;i++)
	if(ch==Sides[i]) return true;
	return false;
}
bool IsOperat()// 运算符 
{
	for(int i = 0;i<4;i++)
	if(ch==Operat[i]) return true;
	return false;
}
int IsRelOperat()//比较符 
{   
	 if(ch=='<'||ch=='>'||ch=='=')      
             return true;
    return false;
}

 int Reserve() //对strToken查找保留字表，若是 返回编码 否者返回-1 
 {
 	for(int i =0;i < 15 ; i++)
	 if(KeyWords[i]==strToken) return i;
	 return -1; 
} 
 int InsertId() //将strToken中的标识符插入符号表 ，返回符号表指针 
 {
 	for(int i =0;i<SignT_i;i++)
	  if(strToken==SignTable[i])  return i;
	SignT_i++;
	SignTable[SignT_i] = strToken;
	return SignT_i;
 }
int InsertConst() ////将strToken中的常数插入常数表 返回常数表地指针 
{
	for(int i =0;i<ConstT_i;i++)
	   if(strToken==ConstTable[i])  return i;
	ConstT_i++;
	ConstTable[ConstT_i] = strToken;
	return ConstT_i;
 } 
StrNode GetstrToken() 
 {  StrNode Node ;  
    int sign;
   for(int i =0 ;i<10;i++)
   strToken[i] = '\0'; 
    Token_i = 0;
    GetChar();
    GetBC();
    if(IsLetter()) //字母开头+数字  = 标识符或者 保留字 
    {  int sign;
      while((IsLetter()||IsDight()))
      {
      	  Concat();
		     GetChar();
	   }
       Retract(); //回溯一个字符 
       sign = Reserve();
       if(sign ==-1)
       { sign = InsertId(); 
       	 Node.type  = "SIGN";
	   } 
	   else 
         Node.type  = "KEYWORD";
	} 
	else if(IsDight()) //数字开头 = 常数 
	{
	  while((IsDight()))
      {
      	  Concat();
		     GetChar();
	   }
	   if(IsLetter()) //数字开头的变量 错误 
	   { while((IsLetter()||IsDight()))
         {
      	  Concat();
		     GetChar();
	      }
	   	Retract(); 
       Node.type  = "ILLEGALSIGN";
	   }
	   else //就是常数 
	   {Retract();
	   sign = InsertConst();
       Node.type  = "CONST";}
	} 
	else if(IsOperat()) //运算符 
	{ 
	  Concat();
	  Node.type  = "OPERAT";
	}
	else if(IsSides()) // 界符 
	{  Concat(); 
    	 if(ch==':')
    	 {  GetChar();
    	 	if(ch=='=')
    	 	   Concat();
    	else  Retract();	
		 }
	  Node.type  = "SIDE";	
	}
	else if(IsRelOperat()) //比较符 
	 {  Concat(); 
	    if(ch=='<'||ch=='>')
		 {  GetChar();
		    if(ch=='=')	
		        Concat();
		    else  Retract();
		 }
		if(ch=='<')
		{GetChar();
		    if(ch=='>')	
		        Concat();
		    else  Retract();
		} 
        Node.type  = "RELOPERAT";
	 }
	else //其他符 
	{   Concat(); 
	    Node.type  = "UNKNOW";
	 }
	return  Node ;
 }
 
 void Readfile()
 {  char strline[81];
	file.getline(strline,81,'\n');
	strcpy(source,strline);
	strcat(source,"\n");
	while(!file.eof())
	{  file.getline(strline,81,'\n');
    	strcat(source,strline);
	     strcat(source,"\n");
	}
	file.close(); 
 }
void lex(){
    int lengh;
    int line_ = 1,line =  1;
    StrNode Node;
    Readfile();
    lengh = strlen(source);
    source[lengh] = '$';//结束符
    cout<<"**********/源程序/**********"<<endl; 
	cout<<"<"<<line_<<"> ";
    for(int i = 0 ; ; i++)
     {if(source[i]=='$') break;
	  else if(source[i]=='\n') {  line_++; 
	                            if(source[i+1]!='$')
	                                   cout<<source[i]<<"<"<<line_<<"> ";
								else   cout<<endl; 
							 }
	 else  cout<<source[i];
   }
    cout<<"---------------------------------------"<<endl;
   while(source[Search_i]!='$')
   {
   	Node = GetstrToken();
   	if(ch!='\n')
   	 { 
		if(Node.type!="UNKNOW"&&Node.type!="ILLEGALSIGN")
              outfile<<"<"<<Node.type<<","<<strToken<<">$"<<endl;
    	else {    Node.name = strToken;
		               error(0,line,Node); 
		         }
     }
	else   {  
	           line++;
	           outfile<<"<$,$>$"<<endl;
	            }
   }
    outfile.close();	
}
