#include "error.h"
using namespace std;
string SignTable[100] = {} ;        //��ʶ���� 
string ConstTable[100]  ={} ;       //������ 
char strToken[10];                  //��Ź��ɵ��ʷ��ŵ��ַ���
char source[10000]={};              //�����ļ����� 
char ch; 
int Token_i = 0,Search_i=0,SignT_i = 0,ConstT_i = 0;
fstream file("data.txt",ios::in);
fstream outfile("lex.txt",ios::out);
void GetChar() //����һ�ַ�����ch�У�����ָʾ��ǰ��һ�ַ�λ�� 
{   ch = source[Search_i];
    Search_i++;
}
void GetBC()//���ch�Ƿ�Ϊ�հ� �� ����GetChar ֱ��ch����ǿ� 
{
	while(ch==' '||ch=='	')        // �ո����TAB�� 
		           GetChar();
}
void Concat()//��ch������strToken 
{
	strToken[Token_i] = ch;
	Token_i++;
 } 
 void Retract() //������ָʾ���ص�һ���ַ�λ�ã���ch�ÿհ׷� 
{
	Search_i--;
	ch = ' ';
 } 
 bool IsLetter()//�Ƿ�Ϊ��ĸ 
 {
 	if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')) 	return true;    
  return false;
 }
bool IsDight()//�Ƿ�Ϊ���� 
 {
   if(ch>='0'&&ch<='9')   return true;    
  return false;
 }
 bool IsSides()//��� 
{
	for(int i = 0;i<5;i++)
	if(ch==Sides[i]) return true;
	return false;
}
bool IsOperat()// ����� 
{
	for(int i = 0;i<4;i++)
	if(ch==Operat[i]) return true;
	return false;
}
int IsRelOperat()//�ȽϷ� 
{   
	 if(ch=='<'||ch=='>'||ch=='=')      
             return true;
    return false;
}

 int Reserve() //��strToken���ұ����ֱ����� ���ر��� ���߷���-1 
 {
 	for(int i =0;i < 15 ; i++)
	 if(KeyWords[i]==strToken) return i;
	 return -1; 
} 
 int InsertId() //��strToken�еı�ʶ��������ű� �����ط��ű�ָ�� 
 {
 	for(int i =0;i<SignT_i;i++)
	  if(strToken==SignTable[i])  return i;
	SignT_i++;
	SignTable[SignT_i] = strToken;
	return SignT_i;
 }
int InsertConst() ////��strToken�еĳ������볣���� ���س������ָ�� 
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
    if(IsLetter()) //��ĸ��ͷ+����  = ��ʶ������ ������ 
    {  int sign;
      while((IsLetter()||IsDight()))
      {
      	  Concat();
		     GetChar();
	   }
       Retract(); //����һ���ַ� 
       sign = Reserve();
       if(sign ==-1)
       { sign = InsertId(); 
       	 Node.type  = "SIGN";
	   } 
	   else 
         Node.type  = "KEYWORD";
	} 
	else if(IsDight()) //���ֿ�ͷ = ���� 
	{
	  while((IsDight()))
      {
      	  Concat();
		     GetChar();
	   }
	   if(IsLetter()) //���ֿ�ͷ�ı��� ���� 
	   { while((IsLetter()||IsDight()))
         {
      	  Concat();
		     GetChar();
	      }
	   	Retract(); 
       Node.type  = "ILLEGALSIGN";
	   }
	   else //���ǳ��� 
	   {Retract();
	   sign = InsertConst();
       Node.type  = "CONST";}
	} 
	else if(IsOperat()) //����� 
	{ 
	  Concat();
	  Node.type  = "OPERAT";
	}
	else if(IsSides()) // ��� 
	{  Concat(); 
    	 if(ch==':')
    	 {  GetChar();
    	 	if(ch=='=')
    	 	   Concat();
    	else  Retract();	
		 }
	  Node.type  = "SIDE";	
	}
	else if(IsRelOperat()) //�ȽϷ� 
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
	else //������ 
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
    source[lengh] = '$';//������
    cout<<"**********/Դ����/**********"<<endl; 
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
