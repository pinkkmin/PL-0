#ifndef PCODE_H
#define PCODE_H
#include<bits/stdc++.h>
#define LIT 0          //LIT 0 A ������ȡ��ջ����aΪ����ֵ 
#define LOD 1          //LOD l A ������ȡ��ջ����AΪƫ���� lΪ��� 
#define STO 2          //STO L A ��ջ���������������aΪƫ���� lΪ��� 
#define CAL 3          //CAL L A ���ù��� LΪ��� aΪƫ���� 
#define INT 4          //INT 0 A������ջ��Ϊ���ù��̿���a����Ԫ������ 
#define JMP 5          //JMP 0 A��������ת a 
#define JPC 6          //JPC 0 A Ϊ����ת 
#define OPR 7          //
using namespace std;
class PNode{
	public:
		int f;
		int l;
		int a;
		PNode(){} 
	   ~PNode(){}
	    void SetCode(int F,int L,int A){
	    	f = F;
	    	l = L;
	    	a = A;
		}
		void operator=(const PNode &Pcode)
		{  f = Pcode.f ;
		   a = Pcode.a ;
		   l = Pcode.l ;	
		}
		void printPCode()
		{  
		switch(f){
		    	case 0 : cout<<"LIT ";
				          break;
				case 1 : cout<<"LOD ";
				          break;
				case 2 : cout<<"STO ";
				          break;
			    case 3 : cout<<"CAL ";
				          break;
			    case 4 : cout<<"INT ";
				          break;
				case 5 : cout<<"JMP ";
				          break;
			    case 6 : cout<<"JPC ";
				          break;
				case 7 : cout<<"OPR ";
				          break;		            
		       }
	       cout<<l<<" "<<a<<endl;	
		}
	    void Setf(int f_){
	    	f = f_;
		}
	    void Setl(int l_){
	    	l = l_;
		}
	    void Seta(int a_){
	    	a = a_;
		}
		int Getf(){
			return f;
		}
	   	int Getl(){
	   		return l;
		   }
	   	int Geta(){
	   		return a;
		   } 
		
}; 
PNode Code[200];             //ָ������
int cx = 0;                 // cx-Ŀ������±� 
int b = 0,top = 0,p = 0;     // b����ַָ��  tջ��ָ��  p�����ַָ�� 
int Stack[200];   //���鳤����ʱ��Ϊ200 
int base(int lev)
{ int b1 = b;
  if(lev<0)
  {
  	b1 = top-1;
  }
  while(lev>0)
  {
  	b1 = Stack[b1];
  	lev--;
  }
  return b1;
}
void gen(int f,int l,int a)  //����Ŀ����� 
{  Code[cx].SetCode(f,l,a); 
   cx++;
} 
void OprCode(int a)  // OPRָ��������� 
{  switch(a)
    { 
      case 0 : top = b-1; 
                //cout<<"PRO 0 0 test of  base:"<<b<<endl;
               p  = Stack[b+2];
			    //cout<<"test of end p ------------------------:::"<<p<<endl; 
               b = Stack[b+1] ;
               //cout<<"test of  base:"<<b<<endl;
	           break;                //  OPR 0 0 ���ù��̽���,���ص��õ�,��ջ 
      case 1 : Stack[top-1] = 0-Stack[top-1]; 
	           p++;            
	           break;                //  OPR 0 1 ջ��ȡ��  
      case 2 : Stack[top-2] = Stack[top-1]+Stack[top-2]; 
	           top--; 
			   p++;      
	           break;                //  OPR 0 2 ջ����� 
      case 3 : Stack[top-2] = Stack[top-2] - Stack[top-1]; 
               top--;
               p++;
	           break;               //   OPR 0 3 ջ����� 
      case 4 : Stack[top-2] = Stack[top-1]*Stack[top-2]; 
               top--;
               p++;
	           break;               //   OPR 0 4 ջ�����   
      case 5 : Stack[top-2] = Stack[top-2]/Stack[top-1];
               top--;
               p++;
	           break;               //   OPR 0 5 ջ����� 
      case 6 : Stack[top-1] = Stack[top-1]%2;	 
	           p++;                   
	           break;               //   OPR 0 6 ��ż�ж� 
	  case 7 : 
	           break;
	  case 8 : if(Stack[top-1]==Stack[top-2])    Stack[top-2] = 1;
	           else Stack[top-2] = 0; 
	           top--; 
	           p++;
	           break;               // OPR 0 8  ��ջ����� 
	  case 9 : if(Stack[top-1]!=Stack[top-2])    Stack[top-2] = 1;
	           else Stack[top-2] = 0;
			   top--; 
			   p++;                
	           break;               // OPR 0 9  ��ջ������ 
	  case 10 :if(Stack[top-2]<Stack[top-1])  Stack[top-2] = 1;
	           else Stack[top-2] = 0;
	           top--;
	           p++;
	           break;               // OPR 0 10 ��ջ��С�� 
	  case 11 :if(Stack[top-2]>=Stack[top-1])  Stack[top-2] = 1;
	           else Stack[top-2] = 0;
	           top--;  
			   p++;       
	           break;               // OPR 0  11 ��ջ�����ڵ��� 
      case 12 :if(Stack[top-2]>Stack[top-1])  Stack[top-2] = 1;
	           else Stack[top-2] = 0;
	           top--;
			   p++;  
	           break;               // OPR 0 12  ��ջ������  	
      case 13 :if(Stack[top-2]<=Stack[top-1])  Stack[top-2] = 1;
	           else Stack[top-2] = 0;
	           //cout<<"TOP-2:"<<Stack[top-2]<<endl;
	           //cout<<"TOP-1:"<<Stack[top-1]<<endl;
	           top--;
			   p++; 
	           break;               // OPR 0 13  ��ջ��С�ڵ��� 
      case 14 :cout<<"StackTop:"<<Stack[top-1];
                                  //  cout<<"test value of top:"<<top<<endl; //IS  true
               p++;
	           break;               // OPR 0 14  �������Ļ 
	  case 15 :cout<<endl;  
	        p++;
	           break;               // OPR 0 15  �����Ļ���� 
      case 16 : cout<<"Input the Number:";
	            cin>>Stack[top];
                top++;
                p++;
	           break;              //  OPR 0 16  ��������ջ��
	}
	
}
void Operator(PNode Pcode)    // ָ��ջ������� 
{  switch(Pcode.Getf())
    {    
       case LIT : Stack[top] = Pcode.Geta() ;   // LIT 0 A , A is  CONST 
                  top++; 
                  p++;
	              break;       
       case LOD : Stack[top] = Stack[Pcode.Geta() + base(Pcode.Getl())];
                  top++;                        //  LOD L A 
                  p++;
	              break;                        //  ��ǰ���̻���ַ+ƫ���� 
       case STO : Stack[Pcode.Geta() + base(Pcode.Getl())] = Stack[top-1];
                  //cout<<"STO-Value:"<<Stack[top-1]<<endl;
                  //cout<<"STO:"<<Pcode.Geta() + base(Pcode.Getl())<<endl;
                  //cout<<"top:"<<top<<endl;
                  top--;                        //  STO L A
                  p++;
	              break;                        //  ��ǰ���̻���ַ+ƫ���� 
       case CAL : Stack[top] = base(Pcode.Getl());//��̬��(�������ݻ���ַ)
	              //cout<<"cal_base:"<<top<<endl;               
                  Stack[top+1] = b;               //��̬��(���ù���ǰ�Ļ���ַ) 
                  Stack[top+2] = p+1;              //���ص�ַ 
                  b = top;                      //��ǰ���ù��̻���ַ
				  //cout<<"test cal of base:"<<b<<endl; 
				  p = Pcode.Geta() ;            // ��ڵ�ַ 
	              break; 
       case INT : top = top + Pcode.Geta();     // INT 0 A 
                  p++;
	              break;                        // t = t+a ����a����Ԫ 
       case JMP : p = Pcode.Geta()  ;          // JMP 0 A , A is Adress of PCode 
	              break; 
       case JPC : if(Stack[top-1]==0 )   p = Pcode.Geta();
                  else p++;
	              break;                       // JPC 0 A, False to A(adress of PCode)
       case OPR : OprCode(Pcode.Geta());
	              break; 
	}	
}
void interpreter() //ָ������� 
{  cout<<"ָ�������"<<endl;
  PNode Pcode;
   Pcode = Code[p];
   //Code[p].printPCode(); 
   Operator(Pcode);
	while(p!=0)
	{  
	  Pcode = Code[p];
	 // cout<<"<"<<p<<"> ";
	 // Code[p].printPCode();	
	  Operator(Pcode);
	  //cout<<"test of p:"<<p<<endl;
	}
}
void printPCode()  //��ӡ��֤ �±���� 
{ cout<<"*Ŀ�����*"<<endl;
  for(int i =0;i<cx;i++)
    {cout<<"<"<<i<<"> "; 
	Code[i].printPCode(); }
	
} 
void OutCode()
{ fstream code("pcode.txt",ios::out);
  for(int i =0;i<cx;i++)
  {code<<"<"<<i<<"> ";
   switch(Code[i].f){
		    	case 0 : code<<"LIT ";
				          break;
				case 1 : code<<"LOD ";
				          break;
				case 2 : code<<"STO ";
				          break;
			    case 3 : code<<"CAL ";
				          break;
			    case 4 : code<<"INT ";
				          break;
				case 5 : code<<"JMP ";
				          break;
			    case 6 : code<<"JPC ";
				          break;
				case 7 : code<<"OPR ";
				          break;		            
		       }
	code<<Code[i].Getl()<<" "<<Code[i].Geta()<<endl;		       
}
	code.close();	
}
#endif
