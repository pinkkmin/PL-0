/*������ */
#include<bits/stdc++.h>
#include "TableSign.h"
using namespace std;
queue<ErrorSign> error_;      //  �洢����Ķ��� 
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
{ //��Ӷ��� 
	    ERsign.line = line;
	    ERsign.type = type;
	    ERsign.sign = Node;
	    error_.push(ERsign);
}
void Outerror(ErrorSign a )
{ 
	switch(a.type) 
	{
		case 0 : cout<<"��"<<a.line<<"��δʶ��'"<<a.sign.name<<"'����......."<<endl;break; // δʶ������  �ʷ������� 
		case 1 : cout<<"��"<<a.line<<"��ȱʧ'"<<a.sign.name<<"'����......."<<endl   ;break;//ȱʧ����;
		case 2 : cout<<"��"<<a.line<<"�б�ʶ��'"<<a.sign.name<<"'δ����......."<<endl; break;//δ��������
		case 3 : cout<<"��"<<a.line<<"�б�ʶ������ʹ�ùؼ���....."<<a.sign.name<<"' ."<<endl; break;//��ʶ��ʹ�ùؼ��� 
		case 4 : cout<<"��"<<a.line<<"�� ȱʧID...."<<endl  ; break;//ȱʧ<ID> 
		case 5 : cout<<"��"<<a.line<<"�У�"<<a.sign.name<<"�Ѿ�������...,"<<endl  ; break;//��ʶ���Ѿ����� 
		case 6 : cout<<"��"<<a.line<<"��"<<a.sign.name<<"Ӧ��Ϊ':='....."<<endl  ; break;
		case 7 : cout<<"��"<<a.line<<"��:=ȱʧ����......"<<endl   ;break;//error---const--ȱʧ���� 
		case 8 : cout<<"��"<<a.line<<"�в������﷨�ı�ʶ��......"<<a.sign.name<<endl   ;break;
		case 9 :  cout<<"��"<<a.line<<"��ȱʧ�ؼ���'"<<a.sign.name<<"'......."<<endl   ;break;
		case 10 : cout<<"��"<<a.line<<"��ȱʧ�ȽϷ�'"<<"'......."<<endl;break;
		case 11 : cout<<"��"<<a.line<<"��ȱʧ�ұȽ���'"<<"'......."<<endl;break; 
		case 12 : cout<<"��"<<a.line<<"��ȱʧ�ұȽ���'"<<"'......."<<endl;break; 
		case 13 : cout<<"��"<<a.line<<"�� ����')'δ��ƥ��"<<"......."<<endl;break; 
		case 14 : cout<<"����"<<a.line<<"��beginδ��ƥ���ȱʧ"<<a.line<<"��end......."<<endl;break;
		case 15 : cout<<"��"<<a.line<<"ʹ�ö���� ��;���ֺ�"<<endl;;break; 
		case 16 : cout<<"����"<<a.line<<"��"<<a.sign.name<<"��"<<a.sign.type<<"δ��ƥ���ȱʧ"<<endl;break;
		case 17 :  cout<<"��"<<a.line<<"�б�ʶ��'"<<a.sign.name<<"'����Ϊ����......."<<endl; break;
		default: break;
   }
}
void ErrorPrint()
{  //�ʷ�����������ʾ 
   int errori = 0;
   if(error_.empty())  cout<<"�﷨�����ɹ� , (0)errors......."<<endl;
   else
    while(!error_.empty())
    {   ERsign = error_.front();
	    error_.pop();
	    errori++;
	    cout<<"[error]"<<errori<<" :";
	    Outerror(ERsign);
	}
}
