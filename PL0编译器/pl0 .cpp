#include"lex.h" 
#include"table.h" 
#include"pcode.h"
using namespace std;
fstream infile("lex.txt",ios::in);
StrNode sym,errorSign;  //key ---type--name ----str
string str;
int line = 1;
bool line_flag = true;
int left_paren = 0 , right_paren = 0; //左右括号的个数 
int begin_i = 0,end_i = 0,while_i = 0,do_i = 0,if_i = 0,then_i= 0;
int factorL = 0,factorR = 0;
int readL = 0,readR = 0 ;
int writeL = 0,writeR = 0;
int callL = 0,callR = 0 ;
//--------------------------------------------------------------------
//符号表 
//包含tx符号表指针        
int lev  = 0 , dx = 0,procTx,ProcDx = 0;                 // dx--相对于本过程基地址的偏移量 ProcDx 过程形参个数 
//-------------------------------------------
void Count();
bool Advance(); 
void Program();
void Block(); //块 
void Condecl(); //常量说明 
void Vardecl();//变量说明
void Body();//复合语句
void Proc();//产生式 
void Condition();//条件 
void Statement();//句型 
void factor();//因子 
void term(); //项 
void expression();//表达式
void CopySYM(StrNode &A,StrNode sy);
StrNode RetractSYM();
int SeachKeyword();
void PrintTable(); 
void AddTable(int flag); //添加至符号表 
bool IsDefine_Sign();//是否已定义 
bool IsDf_KeyWord(); //是否定义为关键字 冲突 
int main() {
	lex();  //词法分析 
	Count();
	Advance();
	Program(); 
	ErrorPrint(); //打印错误 
	//printPCode();
	OutTable();
	OutCode();
	interpreter();	
	return 0;
}
//------------------------------------------------------------
bool Advance(){ 
	int found_1,found_2;
	 getline(infile,str);
	 if(!infile.eof()) 
    {found_1 = str.find(',',0);
    sym.type  = str.substr(1,found_1-1);
    if(sym.type=="$") { 
    	line++;
    	Advance();
	} 
	else if(sym.type=="UNKNOW") {
	                                 	Advance();	
                                    }
	else
    {  
	 found_2 = str.find('$',found_1); 
	  sym.name = str.substr(found_1+1,found_2-2-found_1);
	  //cout<<"  "<<sym.type<<"  "<<sym.name<<endl; 
      return true;
    }
	}
	/*else {  if(begin_i>end_i&&sym.name!="end") error(14,begin_i-end_i,sym);
	        ErrorPrint(); //打印错误 
	          printPCode();
	        if(if_i!=then_i){  errorSign.name = "if";
	                           errorSign.type  = "then";
	                        if(if_i>then_i) line = if_i-then_i;
	                        else line = then_i-if_i;
			                    error(16,line,errorSign);
			                       }
	        if(while_i!=do_i) {  errorSign.name = "while";
	                             errorSign.type = "do";
	                            if(while_i>do_i) line  = while_i-do_i;
	                            else line = do_i- while_i;
			                      error(16,line,errorSign);}
	       // PrintTable();
         }*/
    return false;
    
}
//--------------------------------------------------------
void Count() //
{  fstream file("lex.txt",ios::in);
   int found_1,found_2;
   StrNode sy;
   string s;
	getline(file,s);
	while(!file.eof()) 
    {found_1 = s.find(',',0);
    sy.type  = s.substr(1,found_1-1);
    if(sy.type!="$"&&sy.type!="UNKNOW") 
    {  
	 found_2 = s.find('$',found_1); 
	  sy.name = s.substr(found_1+1,found_2-2-found_1);
      if(sy.name=="if") if_i++;
      else if(sy.name=="then") then_i++;
      else if(sy.name=="while") while_i++;
      else if(sy.name=="do") do_i++;
    }
	 getline(file,s);
}
}
//-------------------------------------------------------------
void Program()
{ 
  if(sym.name=="program") 
	  Advance();
	else { errorSign.name= "program";
	         error(1,line,errorSign); //error---缺失关键字--program
	        } 
	   if(sym.type=="SIGN") 
	   	{   enterProc(sym,lev,dx);  // 等待进入begin回填 
		   Advance();
     	  }
     	else if(sym.type=="KEYWORD") 
		 { error(3,line,sym); //error----<ID>--使用关键字
			Advance(); 
		   }
		else if(sym.type=="CONST"){ error(17,line,sym);
		 	Advance(); 
		}
	  else error(4,line,errorSign); //error---缺失标识符--SIGN"<<endl; 
	  if(sym.name==";")
	   	              { Advance();
	   	                while(sym.name==";")
		               {  error(15,line,sym);
		     	          Advance();
			             } 
	   	              }
	  else  { errorSign.name = ";"; 
	          error(1,line-1,errorSign); //error--缺失';'"
			  } 
	  Block();
}
//--------------------------------------------------------------
void Block()
{ int preDx  ;                               // 保留address
  int JmpCx; 
  int tx0 = tx;
  dx = 3;
  JmpCx = cx;
  gen(JMP,0,0);
  if(sym.name=="const") Condecl();                // 常量说明       
   dx+=ProcDx;
   cout<<"dx:"<<dx<<endl; 
  if(sym.name=="var") 	Vardecl();                // 变量说明
     preDx = dx; 
  if(sym.name=="procedure") { ProcDx  = 0;  
                              Proc();             // if(!Advance()) retur n ;
								  }
  dx  = preDx;
  Code[JmpCx].Seta(cx);                           // 回填JMP的指令入口 A 
  Table[procTx].SetAdress(cx);
  ProcDx  = 0; 
  gen(INT,0,dx);                                  // 生成入口指令
  Body();
  lev--; 
  gen(OPR,0,0);                                   // 退栈返回  
}
//----------------------------------------------------------------
void Body()/*****/
{   int varCount = dx-3;                          // 变量var的个数 
    int preLevel = lev;                           // 当前的层差 
    begin_i++;
	if(sym.name=="begin")  Advance();
	else {     errorSign.name = "begin";
	           error(1,line,errorSign);          //  error-----body----缺失begin
           }
    Statement();                                 //  待定进行到这                       
	{ while(sym.name!="end")
		{    
		 if(sym.name!=";")                       //  error-----body----缺失; 
		             { errorSign.name = ";";
					   error(1,line-1,errorSign);
					   }
		 
		  else { Advance();
		          while(sym.name==";")
		           {  error(15,line,sym);
		            	Advance();
			        } 
		        if(sym.name=="end")  break;       
			     }
		Statement(); 	     
		} 
		if(sym.name!="end") { errorSign.name = "end";
					            error(1,line-1,errorSign);}
		else { Advance();
		       SetLevFalse(lev);   //lev 待定 
		        end_i++;}   
    }
}
//---------------------------------------------------------------------
// procedure <id>（[<id>{,<id>]}）;<block>{;<proc>}
void Proc()
{ int preProc = procTx;
  if(sym.name=="procedure")
	{ Advance();
	  if(sym.type=="SIGN"||sym.type=="KEYWORD")	 
	     { if(IsDfKeyWord(sym.name)) error(3,line,sym);                     //  error---<ID>---使用关键字
		   else{ if(IsLevDfSign(sym.name,lev)) error(5,line,sym);    // error---<ID>---已被定义  
	             else { procTx = tx;
				        enterProc(sym,lev,0); 
	                    lev++;
				       }
	           }
		    Advance();
          }
      else error(4,line,sym);                                     //  error---proc--缺失SIGN-ID   
	 if(sym.name=="(")  Advance();
	 else {  errorSign.name ="(";
		        error(1,line,errorSign); }	                      //  error---proc--缺失'('    		
	 if(sym.type=="SIGN"||sym.type=="KEYWORD") {
	    if(IsDfKeyWord(sym.name)) error(3,line,sym);                     //  error---<ID>---使用关键字
		else { if(!IsLevDfSign(sym.name,0)) error(2,line,sym);    //  error---<ID>---未定义 
		       else {	enterVar(sym,lev,3+ProcDx+1);	          //  3为dl,sl,ra ProcDx为当前形参个数   	  
			             ProcDx++;	}                            
	           }                    //待排查 lev = 0 
		   Advance();}
     else error(4,line,sym);                                      //  error---proc--缺失SIGN-ID 
	 while(sym.name!=")") {                                       //  {,<id>]}
				if(sym.name==",")     Advance();
	  		    else {  errorSign.name =",";                      //  error--缺失',' 
			            error(1,line,errorSign); }   
	  		   if(sym.type=="SIGN"||sym.type=="KEYWORD")	 
	                  { if(IsDfKeyWord(sym.name)) error(3,line,sym);     // error---<ID>---使用关键字
		                else   { if(!IsLevDfSign(sym.name,0))        // error---<ID>---未定义  
	                                       error(2,line,sym);
	                             ProcDx++;
	                             }      
	  		            Advance();
				      }
			   else error(4,line,sym);                            // error---proc--缺失SIGN-ID 			   
			   if(sym.name==";"||sym.name=="var"||sym.name=="const"||sym.name=="begin") break;
			  }
	   if(sym.name!=")") { errorSign.name =")";
			                error(1,line,errorSign);               // error---proc--缺失')'
			              } 
	   else Advance();
	   if(sym.name==";")    Advance(); 
	   else { errorSign.name =";";
			   error(1,line,errorSign);                              // error---proc--缺失';'
			  } 
	   Block();
	   procTx = preProc;
	   while(sym.name==";")                                         // {;<proc>}
	  			{ Advance();                                        // 可添加TEST() 
				  Proc();
				  }			  
	}
}
//-------------------------------------------------------------
//2019-12.31-00.51 明日早晨修改 
void Statement()
{ StrNode SYMGen;
  int pos,left = 0 ,right = 0;
  if(sym.type=="SIGN")                                           // <id> := <exp>
	{  SYMGen =sym;                                              // 保留生成gen使用的变量名  
	  Advance();                                                 // const ????
	  if(sym.name==":=") Advance();
	  else if(sym.name=="="||sym.name==":"){
	          error(6,line,sym);
		         Advance();	}
	  else {  errorSign.name = ":=";                             //  error---Statement----:=
		         error(1,line,errorSign);}
	   expression();
	   pos = position(SYMGen.name,lev);
	  if(!pos) error(2,line,sym);                                //  return0==false error---<ID>---未定义 
	  else if(Table[pos].GetType()!="VAR") error(2,line,sym);    // 非变量 error补充   
	  else gen(STO,lev-Table[pos].GetLevel(),Table[pos].GetAdress()) ;
	  //test-Output:cout<<Table[pos].name <<"-lev:"<<Table[pos].GetLevel()<<endl; 
	}
	else if(sym.name=="if")                                      // if <lexp> then <statement>[else <statement>]
	{ Advance();
	  Condition();
	  int JpcCx ;	
	  if(sym.name=="then") {Advance();
	                        JpcCx = cx;
                            gen(JPC,0,0);}
	  else { errorSign.name = "then";
	   error(9,line,errorSign);                                  //error---Statement----then缺失
	  } 
	    Statement();
		Code[JpcCx].Seta(cx);                                   // 回填JPC A 
	    if(sym.name=="else")
	    { int JmpCx = cx;
	      gen(JMP,0,0);  	 
		    Advance();
	    	Statement();
	    	Code[JmpCx].Seta(cx);                                   // 回填JMP A 
			}
	}
	else if(sym.name=="while")                                    //  while <lexp> do <statement>
	{ int JmpCx = cx,JpcCx;
	   Advance();
	   Condition();
		if(sym.name=="do"){ JpcCx = cx;
		                    gen(JPC,0,0);
		                    Advance();}
		else {  errorSign.name = "do"; 
	             error(9,line,errorSign);}
		Statement();
		gen(JMP,0,JmpCx);                                       // 回跳 
		Code[JpcCx].Seta(cx);                                   // 回填JPC 0 A  
	}
	else if(sym.name=="call")                                    //  call <id>（[<exp>{,<exp>}]）
	{ int parameter; //参数 
	  Advance();
	   if(sym.type=="SIGN") { pos =  position(sym.name,lev); 
			        Advance();}
	   else  error(4,line,sym);                                 // error--Statement---call缺失SIGN
	   if(sym.name=="(") { Advance();
	                          left++; }	
	   else	  { errorSign.name ="(";
	      	                error(1,line,errorSign); }         //error--Statement---call缺失'('	   	 
	    if(sym.name!=")")  {	                                       //有参数 
		    expression();
	      	 if(Table[pos].GetType()=="PROC"){                // 传递参数处理 
	      	    parameter = pos+1;
	      	     //cout<<"parameter:"<<lev<<" "<<Table[parameter].GetLevel()<<endl; 
			   gen(STO,lev-Table[parameter].GetLevel(),Table[parameter].GetAdress());  
			   }                                               //下一个就是参数
			   }
	      	while(sym.name==","||sym.name=="("||sym.type=="SIGN")
	      	  	{  Advance();
	      	  	   expression();
	      	  	   if(Table[pos].GetType()=="PROC"){                // 传递参数处理 
	                       parameter++;
			   gen(STO,lev-Table[parameter].GetLevel(),Table[parameter].GetAdress());  
			   }   
					}
				if(sym.name!=")") 	{errorSign.name =")";
				                      error(1,line,errorSign);}
				else { while(sym.name==")"){ 
					         right++;
					           Advance(); 	  }
					   if(right!=left) error(1,line,errorSign) ;    // error-左右括号不匹配 
				     } 	                    	                    // cout<<"error--Statement---call缺失')'"<<endl;}
	if(!pos) error(2,line,sym);                       // 待补充errror-<ID>-未过程名定义 
	else if(Table[pos].GetType()=="PROC") 
		  gen(CAL,lev-Table[pos].GetLevel(),Table[pos].GetAdress()); //是否为adress 
	else  error(2,line,sym);                          // 待补充error sign不是过程名 
	 }
	else if(sym.name=="read")                                       // read (<id>{，<id>})   
	{ Advance();
	  if(sym.name=="(")
	             Advance();
	   else {    errorSign.name ="(";
	             error(1,line,errorSign);}                         // error--Statement---read缺失'('               
	  if(sym.type=="SIGN")  { 
	            pos = position(sym.name,lev);                      // 可添加使用关键字和常量等 
	            if(!pos) error(2,line,sym);                        // error--<ID>--未被定义 
	            else if(Table[pos].GetType()!="VAR") error(2,line,sym);
				else{  gen(OPR,0,16);                             // 生成代码
				       gen(STO,lev-Table[pos].GetLevel(),Table[pos].GetAdress()); 
					 }                                            // 读入栈顶,送到变量 	  
                Advance(); }
	  else  error(4,line,sym);                                     // error--Statement---read缺失SIGN-ID
	  while(sym.name==","||sym.type=="SIGN")	 
	  	   { Advance();
	  	   	 if(sym.type=="SIGN")  {
				  pos = position(sym.name,lev); 
				  if(!pos) error(2,line,sym);
				  else if(Table[pos].GetType()!="VAR") error(2,line,sym);
				  else{ gen(OPR,0,16);                             // 生成代码
				           gen(STO,lev-Table[pos].GetLevel(),Table[pos].GetAdress()); 
					   }      
				  Advance();} 
			  else  error(4,line,sym);
			  if(sym.name==")"||sym.name==";") break;              // 结束符号 
			}
		if(sym.name==")")  Advance();
		else  { errorSign.name =")"; 
			      error(1,line,errorSign);                        // error--Statement---read缺失')'
	            	}	
	 } 
	else if(sym.name=="write")                                    // write (<exp>{,<exp>})
	{ Advance();
	  if(sym.name=="(")
	         Advance();
	  else { errorSign.name ="(";
	              error(1,line,errorSign); }                      // error--Statement---write缺失'('
	  expression();
	  gen(OPR,0,14);
	  while(sym.name==","){      Advance();
	                              expression();
								  gen(OPR,0,14);}
	  gen(OPR,0,15); 
	  if(sym.name==")")  Advance();
	  else {   errorSign.name =")";
	  	         error(1,line,errorSign);            // error--Statement---write缺失')'
	        }
	 }
	else if(sym.name=="do"||sym.name=="then") return;
	else Body(); //<body>
}

//----------------------------------------------------------
void Condeclaration()
{ int value; char num[15];
  StrNode sym_;  
  if(sym.type =="SIGN"||sym.type =="KEWWORD"){ 
		  if(IsDfKeyWord(sym.name)) error(3,line,sym);         // error---<ID>---使用关键字
            else{ if(IsLevDfSign(sym.name,lev))            // error---<ID>---当层已被定义  
    	                error(5,line,sym);             /*  修改error-5 */
	              else    sym_ = sym; } 
	         Advance();          
           }
   else    error(4,line,sym);                           // error--const--缺失ID
   if(sym.name==":=")  Advance();
   else if(sym.name=="=")  { error(6,line,errorSign);   // error-- =应为:=   
                                     Advance();   }
   else { errorSign.name=":=";                          // error---const---缺失:=
			       error(1,line,errorSign);  }
   if(sym.type=="CONST")                                // 常数 数字 如 2223
	     	       { strcpy(num,sym.name.c_str()); 
					 value = atoi(num);
					 enterConst(sym_,lev,value,dx);     // CONST enter the SYMTable 
					 Advance();}
    else  error(7,line,sym);                            // error---const--缺失常数
}
void Condecl()
{  if(sym.name=="const")  Advance();   
      Condeclaration();
	  while(sym.name==","||sym.type=="SIGN")
	  	 {	if(sym.name==",") Advance();
	  	    else {   errorSign.name =",";               //  error--缺失',' 
			          error(1,line,errorSign); }   
			if(sym.name=="procedure"||sym.name==";"||sym.name=="var")
		     break;	                                    // 后继符号 
		     Condeclaration();
	    }
	 if(sym.name!=";") { errorSign.name=";";
	                   	   error(1,line-1,errorSign);
	                   }
	 else {Advance();
	       while(sym.name==";")
		     {  error(15,line,sym);
		     	Advance();
			   } }
}
//---------------------------------------------------------------------------
void Vardecl()
{ if(sym.name=="var")
	{	  Advance();
	    if(sym.type =="SIGN")
	    {  
		if(IsLevDfSign(sym.name,lev))   error(5,line,sym);   // error---<ID>---在当前过程已被定义 
	       else { enterVar(sym,lev,dx+1);
				  dx++;}                                     // 偏移量  +1 
		   Advance();
	     }
	   else if(sym.type =="KEYWORD"){ 
	           error(3,line,sym);                            // error---<ID>---使用关键字
	           Advance(); }
	   else  error(4,line,errorSign);                        // error--var---缺失ID
	   
	    while(sym.name!=";")
	      {  if(sym.name==",")
	         { Advance();
	           if(sym.type=="SIGN") {
			        if(IsLevDfSign(sym.name,lev))   error(5,line,sym);  
	                 else { enterVar(sym,lev,dx+1);
				             dx++;} 
			        Advance();
			    } 
	           else if(sym.type=="KEYWORD"){                  // if(sym.type=="var")  error(3,line,sym);
		                error(3,line,sym);                    // else 
				         Advance();                           // error---<ID>---使用关键字      
			     }
			   else  error(4,line,sym);                       // error--缺失<ID>    
	         }
	        else if(sym.type=="SIGN"||sym.type=="KEYWORD")
	        {  errorSign.name = ",";
			   error(1,line,errorSign);                       //  errro---缺失',' 
			 if(IsLevDfSign(sym.name,lev)) error(5,line,sym); //  error---<ID>---已被定义  
	         else { enterVar(sym,lev,dx+1);
				       dx++;} 
	             if(IsDfKeyWord(sym.name))  error(3,line,sym);       //  error---<ID>---使用关键字      
			   Advance();	
			}
			else break;
	       }	
	   if(sym.name!=";") {  errorSign.name=";";
	                          error(1,line-1,errorSign);  //error--缺失';'
                           }
        else  {Advance();
		        while(sym.name==";")
		        {  error(15,line,sym);
		         	Advance();
			      } 
			  }
    }
}
//--------------------------------------------------------------------
void Condition()  
{  // <exp> <lop> <exp>|odd <exp> 
	if(sym.name=="odd") { Advance();
	                      gen(OPR,0,6);                   // OPR 0 6 判断奇偶 
     	                   expression();
	}
	else {  StrNode symRlOp;
	       if(sym.type=="RELOPERAT")  error(12,line,sym);  // error---缺失---左比较项  
	       else 	expression();
			if(sym.type=="RELOPERAT") symRlOp = sym;                         
		    else  error(10,line,sym);                     // error---condition----缺失比较符*/
		     Advance();
	       expression(); 
	        if(symRlOp.name=="<")          gen(OPR,0,10);
			  else if (symRlOp.name=="<=") gen(OPR,0,13);
			  else if (symRlOp.name==">")  gen(OPR,0,12);
			  else if (symRlOp.name==">=") gen(OPR,0,11);
			  else if (symRlOp.name=="=")  gen(OPR,0,8);
			  else if (symRlOp.name=="<>") gen(OPR,0,9); 
    	}
}
//--------------------------------------------------------------------------------
void expression() //表达式<exp> →[+|-]<term>{<aop><term>}  表达式-> 项+|-项
{    StrNode Operat ;
	if(sym.name=="+"||sym.name=="-")  { Operat = sym;
	                                    Advance();}
	term();                                               // 进入项  
	if(Operat.name=="-") gen(OPR,0,1);                    // opr 0 1 栈顶取反    
	 while(sym.name=="+"||sym.name=="-")
	 { 	Operat = sym; 
	    Advance();
	 	   term();  
	 	if(Operat.name=="+")      gen(OPR,0,2);          //OPR 0 2 栈顶相加 
	 	else if(Operat.name=="-") gen(OPR,0,3);          //OPR 0 3 栈顶相减 
	 }	
 } 
 void term()  //项 
{  StrNode Operat;
   factor(); 	
	while(sym.name=="*"||sym.name=="/")
	{   Operat = sym;
		Advance();
		factor();
		if(Operat.name=="*")      gen(OPR,0,4);         //OPR 0 4 栈顶相乘 
	 	else if(Operat.name=="/") gen(OPR,0,5);	        //OPR 0 5 栈顶相除 
	}
}
void factor()
{  
	if(sym.type=="SIGN") 
	{ int pos = position(sym.name,lev);
	  if(!pos)    error(2,line,sym);                     // error---<ID>---未定义  
	  else{ if(Table[pos].GetType()=="VAR") 
	        gen(LOD,lev-Table[pos].GetLevel(),Table[pos].GetAdress());  
			else if(Table[pos].GetType()=="CONST")
		     gen(LIT,0,Table[pos].GetValue()) ;           //LIT 0 A 常量置栈顶 
			else error(2,line,sym);                          // 非变量和常量 error补充      
	      }             
	   Advance();
	    }  
	else if(sym.type=="CONST") { int value;char num[15];
	           strcpy(num,sym.name.c_str()); 
			   value = atoi(num); 
		       gen(LIT,0,value) ;                    //LIT 0 A 常数置栈顶 
		         Advance();	} 
		         
	else if(sym.name=="(")
	 { left_paren++;
	 	Advance();
	 	expression(); 
	 if(sym.name==")")
	        {  if(left_paren==0)  error(13,line,sym); //")未被匹配"<<endl; 
	          	else  {left_paren--;
	 	     	        Advance();
		              }
	        } 
	}
}
//-------------------------------------------------------------------------------
void CopySYM(StrNode &A,StrNode sy)
{
	A.type = sy.type;
	A.name = sy.name ;
}

StrNode RetractSYM()
{ StrNode sym_now; 
  CopySYM(sym_now,sym); //保存当前sym 
  StrNode sym_next;
  Advance(); //取值下一个sym 
  CopySYM(sym_next,sym);  
  return sym_next;	
} 

int SeachKeyword()
{  fstream Seachfile;
  string s;
  StrNode PrtWord;
  int found_1,found_2;
  getline(Seachfile,str);
  found_1 = s.find(',',0);
  PrtWord.type  = s.substr(1,found_1-1);
	while(!Seachfile.eof()) 
    {
    if(PrtWord.type!="$"&&sym.type!="UNKNOW")  
      {  found_2 = str.find('$',found_1); 
	       PrtWord.name = str.substr(found_1+1,found_2-2-found_1); 
     	}
     	if(PrtWord.name=="do") return 1;
     	else if(PrtWord.name=="then") return 2;
     	 getline(Seachfile,s);
         found_1 = s.find(',',0);
          PrtWord.type  = s.substr(1,found_1-1);
    }
    return 0;
}
/*void PrintTable()
{   cout<<"-------符号表----------"<<endl;
   cout<<"类型\t"<<"值\t"<<endl;
	for(int i = 1 ;i<ST_i ; i++)
	 cout<<SYMTable[i].type<<"\t"<<SYMTable[i].name<<endl;
}
/*
1.标识符未声明 查表：遇到标识符
2.标识符号重复声明或者声明冲突  ：var或者const进入 查表
3.标识符使用关键字   声明时查：关键字表  

bool IsDefine_Sign() //是否定义标识符 
{ 	for(int i = 1;i<=ST_i;i++)
	  if(sym.name == SYMTable[i].name)  return true;
return false;
}
bool IsDf_KeyWord()
{  for(int i =0;i <= 14 ; i++)
	 if(KeyWords[i]==sym.name) return true;
	 return false;
} */
