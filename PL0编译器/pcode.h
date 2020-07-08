#ifndef PCODE_H
#define PCODE_H
#include<bits/stdc++.h>
#define LIT 0          //LIT 0 A 将常量取到栈顶，a为常数值 
#define LOD 1          //LOD l A 将变量取到栈顶，A为偏移量 l为层差 
#define STO 2          //STO L A 将栈顶内容送入变量，a为偏移量 l为层差 
#define CAL 3          //CAL L A 调用过程 L为层差 a为偏移量 
#define INT 4          //INT 0 A在运行栈中为调用过程开辟a个单元数据区 
#define JMP 5          //JMP 0 A无条件跳转 a 
#define JPC 6          //JPC 0 A 为真跳转 
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
PNode Code[200];             //指令数组
int cx = 0;                 // cx-目标代码下标 
int b = 0,top = 0,p = 0;     // b基地址指针  t栈顶指针  p程序地址指针 
int Stack[200];   //数组长度暂时定为200 
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
void gen(int f,int l,int a)  //生成目标代码 
{  Code[cx].SetCode(f,l,a); 
   cx++;
} 
void OprCode(int a)  // OPR指令运算操作 
{  switch(a)
    { 
      case 0 : top = b-1; 
                //cout<<"PRO 0 0 test of  base:"<<b<<endl;
               p  = Stack[b+2];
			    //cout<<"test of end p ------------------------:::"<<p<<endl; 
               b = Stack[b+1] ;
               //cout<<"test of  base:"<<b<<endl;
	           break;                //  OPR 0 0 调用过程结束,返回调用点,退栈 
      case 1 : Stack[top-1] = 0-Stack[top-1]; 
	           p++;            
	           break;                //  OPR 0 1 栈顶取反  
      case 2 : Stack[top-2] = Stack[top-1]+Stack[top-2]; 
	           top--; 
			   p++;      
	           break;                //  OPR 0 2 栈顶相加 
      case 3 : Stack[top-2] = Stack[top-2] - Stack[top-1]; 
               top--;
               p++;
	           break;               //   OPR 0 3 栈顶相减 
      case 4 : Stack[top-2] = Stack[top-1]*Stack[top-2]; 
               top--;
               p++;
	           break;               //   OPR 0 4 栈顶相乘   
      case 5 : Stack[top-2] = Stack[top-2]/Stack[top-1];
               top--;
               p++;
	           break;               //   OPR 0 5 栈顶相除 
      case 6 : Stack[top-1] = Stack[top-1]%2;	 
	           p++;                   
	           break;               //   OPR 0 6 奇偶判断 
	  case 7 : 
	           break;
	  case 8 : if(Stack[top-1]==Stack[top-2])    Stack[top-2] = 1;
	           else Stack[top-2] = 0; 
	           top--; 
	           p++;
	           break;               // OPR 0 8  判栈顶相等 
	  case 9 : if(Stack[top-1]!=Stack[top-2])    Stack[top-2] = 1;
	           else Stack[top-2] = 0;
			   top--; 
			   p++;                
	           break;               // OPR 0 9  判栈顶不等 
	  case 10 :if(Stack[top-2]<Stack[top-1])  Stack[top-2] = 1;
	           else Stack[top-2] = 0;
	           top--;
	           p++;
	           break;               // OPR 0 10 判栈顶小于 
	  case 11 :if(Stack[top-2]>=Stack[top-1])  Stack[top-2] = 1;
	           else Stack[top-2] = 0;
	           top--;  
			   p++;       
	           break;               // OPR 0  11 判栈顶大于等于 
      case 12 :if(Stack[top-2]>Stack[top-1])  Stack[top-2] = 1;
	           else Stack[top-2] = 0;
	           top--;
			   p++;  
	           break;               // OPR 0 12  判栈顶大于  	
      case 13 :if(Stack[top-2]<=Stack[top-1])  Stack[top-2] = 1;
	           else Stack[top-2] = 0;
	           //cout<<"TOP-2:"<<Stack[top-2]<<endl;
	           //cout<<"TOP-1:"<<Stack[top-1]<<endl;
	           top--;
			   p++; 
	           break;               // OPR 0 13  判栈顶小于等于 
      case 14 :cout<<"StackTop:"<<Stack[top-1];
                                  //  cout<<"test value of top:"<<top<<endl; //IS  true
               p++;
	           break;               // OPR 0 14  输出到屏幕 
	  case 15 :cout<<endl;  
	        p++;
	           break;               // OPR 0 15  输出屏幕换行 
      case 16 : cout<<"Input the Number:";
	            cin>>Stack[top];
                top++;
                p++;
	           break;              //  OPR 0 16  输入置于栈顶
	}
	
}
void Operator(PNode Pcode)    // 指令栈运算操作 
{  switch(Pcode.Getf())
    {    
       case LIT : Stack[top] = Pcode.Geta() ;   // LIT 0 A , A is  CONST 
                  top++; 
                  p++;
	              break;       
       case LOD : Stack[top] = Stack[Pcode.Geta() + base(Pcode.Getl())];
                  top++;                        //  LOD L A 
                  p++;
	              break;                        //  当前过程基地址+偏移量 
       case STO : Stack[Pcode.Geta() + base(Pcode.Getl())] = Stack[top-1];
                  //cout<<"STO-Value:"<<Stack[top-1]<<endl;
                  //cout<<"STO:"<<Pcode.Geta() + base(Pcode.Getl())<<endl;
                  //cout<<"top:"<<top<<endl;
                  top--;                        //  STO L A
                  p++;
	              break;                        //  当前过程基地址+偏移量 
       case CAL : Stack[top] = base(Pcode.Getl());//静态链(最新数据基地址)
	              //cout<<"cal_base:"<<top<<endl;               
                  Stack[top+1] = b;               //动态链(调用过程前的基地址) 
                  Stack[top+2] = p+1;              //返回地址 
                  b = top;                      //当前调用过程基地址
				  //cout<<"test cal of base:"<<b<<endl; 
				  p = Pcode.Geta() ;            // 入口地址 
	              break; 
       case INT : top = top + Pcode.Geta();     // INT 0 A 
                  p++;
	              break;                        // t = t+a 开辟a个单元 
       case JMP : p = Pcode.Geta()  ;          // JMP 0 A , A is Adress of PCode 
	              break; 
       case JPC : if(Stack[top-1]==0 )   p = Pcode.Geta();
                  else p++;
	              break;                       // JPC 0 A, False to A(adress of PCode)
       case OPR : OprCode(Pcode.Geta());
	              break; 
	}	
}
void interpreter() //指令解释器 
{  cout<<"指令解释器"<<endl;
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
void printPCode()  //打印验证 下标待调 
{ cout<<"*目标代码*"<<endl;
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
