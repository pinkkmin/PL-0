#include"lex.h" 
#include"table.h" 
#include"pcode.h"
using namespace std;
fstream infile("lex.txt",ios::in);
StrNode sym,errorSign;  //key ---type--name ----str
string str;
int line = 1;
bool line_flag = true;
int left_paren = 0 , right_paren = 0; //�������ŵĸ��� 
int begin_i = 0,end_i = 0,while_i = 0,do_i = 0,if_i = 0,then_i= 0;
int factorL = 0,factorR = 0;
int readL = 0,readR = 0 ;
int writeL = 0,writeR = 0;
int callL = 0,callR = 0 ;
//--------------------------------------------------------------------
//���ű� 
//����tx���ű�ָ��        
int lev  = 0 , dx = 0,procTx,ProcDx = 0;                 // dx--����ڱ����̻���ַ��ƫ���� ProcDx �����βθ��� 
//-------------------------------------------
void Count();
bool Advance(); 
void Program();
void Block(); //�� 
void Condecl(); //����˵�� 
void Vardecl();//����˵��
void Body();//�������
void Proc();//����ʽ 
void Condition();//���� 
void Statement();//���� 
void factor();//���� 
void term(); //�� 
void expression();//���ʽ
void CopySYM(StrNode &A,StrNode sy);
StrNode RetractSYM();
int SeachKeyword();
void PrintTable(); 
void AddTable(int flag); //��������ű� 
bool IsDefine_Sign();//�Ƿ��Ѷ��� 
bool IsDf_KeyWord(); //�Ƿ���Ϊ�ؼ��� ��ͻ 
int main() {
	lex();  //�ʷ����� 
	Count();
	Advance();
	Program(); 
	ErrorPrint(); //��ӡ���� 
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
	        ErrorPrint(); //��ӡ���� 
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
	         error(1,line,errorSign); //error---ȱʧ�ؼ���--program
	        } 
	   if(sym.type=="SIGN") 
	   	{   enterProc(sym,lev,dx);  // �ȴ�����begin���� 
		   Advance();
     	  }
     	else if(sym.type=="KEYWORD") 
		 { error(3,line,sym); //error----<ID>--ʹ�ùؼ���
			Advance(); 
		   }
		else if(sym.type=="CONST"){ error(17,line,sym);
		 	Advance(); 
		}
	  else error(4,line,errorSign); //error---ȱʧ��ʶ��--SIGN"<<endl; 
	  if(sym.name==";")
	   	              { Advance();
	   	                while(sym.name==";")
		               {  error(15,line,sym);
		     	          Advance();
			             } 
	   	              }
	  else  { errorSign.name = ";"; 
	          error(1,line-1,errorSign); //error--ȱʧ';'"
			  } 
	  Block();
}
//--------------------------------------------------------------
void Block()
{ int preDx  ;                               // ����address
  int JmpCx; 
  int tx0 = tx;
  dx = 3;
  JmpCx = cx;
  gen(JMP,0,0);
  if(sym.name=="const") Condecl();                // ����˵��       
   dx+=ProcDx;
   cout<<"dx:"<<dx<<endl; 
  if(sym.name=="var") 	Vardecl();                // ����˵��
     preDx = dx; 
  if(sym.name=="procedure") { ProcDx  = 0;  
                              Proc();             // if(!Advance()) retur n ;
								  }
  dx  = preDx;
  Code[JmpCx].Seta(cx);                           // ����JMP��ָ����� A 
  Table[procTx].SetAdress(cx);
  ProcDx  = 0; 
  gen(INT,0,dx);                                  // �������ָ��
  Body();
  lev--; 
  gen(OPR,0,0);                                   // ��ջ����  
}
//----------------------------------------------------------------
void Body()/*****/
{   int varCount = dx-3;                          // ����var�ĸ��� 
    int preLevel = lev;                           // ��ǰ�Ĳ�� 
    begin_i++;
	if(sym.name=="begin")  Advance();
	else {     errorSign.name = "begin";
	           error(1,line,errorSign);          //  error-----body----ȱʧbegin
           }
    Statement();                                 //  �������е���                       
	{ while(sym.name!="end")
		{    
		 if(sym.name!=";")                       //  error-----body----ȱʧ; 
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
		       SetLevFalse(lev);   //lev ���� 
		        end_i++;}   
    }
}
//---------------------------------------------------------------------
// procedure <id>��[<id>{,<id>]}��;<block>{;<proc>}
void Proc()
{ int preProc = procTx;
  if(sym.name=="procedure")
	{ Advance();
	  if(sym.type=="SIGN"||sym.type=="KEYWORD")	 
	     { if(IsDfKeyWord(sym.name)) error(3,line,sym);                     //  error---<ID>---ʹ�ùؼ���
		   else{ if(IsLevDfSign(sym.name,lev)) error(5,line,sym);    // error---<ID>---�ѱ�����  
	             else { procTx = tx;
				        enterProc(sym,lev,0); 
	                    lev++;
				       }
	           }
		    Advance();
          }
      else error(4,line,sym);                                     //  error---proc--ȱʧSIGN-ID   
	 if(sym.name=="(")  Advance();
	 else {  errorSign.name ="(";
		        error(1,line,errorSign); }	                      //  error---proc--ȱʧ'('    		
	 if(sym.type=="SIGN"||sym.type=="KEYWORD") {
	    if(IsDfKeyWord(sym.name)) error(3,line,sym);                     //  error---<ID>---ʹ�ùؼ���
		else { if(!IsLevDfSign(sym.name,0)) error(2,line,sym);    //  error---<ID>---δ���� 
		       else {	enterVar(sym,lev,3+ProcDx+1);	          //  3Ϊdl,sl,ra ProcDxΪ��ǰ�βθ���   	  
			             ProcDx++;	}                            
	           }                    //���Ų� lev = 0 
		   Advance();}
     else error(4,line,sym);                                      //  error---proc--ȱʧSIGN-ID 
	 while(sym.name!=")") {                                       //  {,<id>]}
				if(sym.name==",")     Advance();
	  		    else {  errorSign.name =",";                      //  error--ȱʧ',' 
			            error(1,line,errorSign); }   
	  		   if(sym.type=="SIGN"||sym.type=="KEYWORD")	 
	                  { if(IsDfKeyWord(sym.name)) error(3,line,sym);     // error---<ID>---ʹ�ùؼ���
		                else   { if(!IsLevDfSign(sym.name,0))        // error---<ID>---δ����  
	                                       error(2,line,sym);
	                             ProcDx++;
	                             }      
	  		            Advance();
				      }
			   else error(4,line,sym);                            // error---proc--ȱʧSIGN-ID 			   
			   if(sym.name==";"||sym.name=="var"||sym.name=="const"||sym.name=="begin") break;
			  }
	   if(sym.name!=")") { errorSign.name =")";
			                error(1,line,errorSign);               // error---proc--ȱʧ')'
			              } 
	   else Advance();
	   if(sym.name==";")    Advance(); 
	   else { errorSign.name =";";
			   error(1,line,errorSign);                              // error---proc--ȱʧ';'
			  } 
	   Block();
	   procTx = preProc;
	   while(sym.name==";")                                         // {;<proc>}
	  			{ Advance();                                        // �����TEST() 
				  Proc();
				  }			  
	}
}
//-------------------------------------------------------------
//2019-12.31-00.51 �����糿�޸� 
void Statement()
{ StrNode SYMGen;
  int pos,left = 0 ,right = 0;
  if(sym.type=="SIGN")                                           // <id> := <exp>
	{  SYMGen =sym;                                              // ��������genʹ�õı�����  
	  Advance();                                                 // const ????
	  if(sym.name==":=") Advance();
	  else if(sym.name=="="||sym.name==":"){
	          error(6,line,sym);
		         Advance();	}
	  else {  errorSign.name = ":=";                             //  error---Statement----:=
		         error(1,line,errorSign);}
	   expression();
	   pos = position(SYMGen.name,lev);
	  if(!pos) error(2,line,sym);                                //  return0==false error---<ID>---δ���� 
	  else if(Table[pos].GetType()!="VAR") error(2,line,sym);    // �Ǳ��� error����   
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
	   error(9,line,errorSign);                                  //error---Statement----thenȱʧ
	  } 
	    Statement();
		Code[JpcCx].Seta(cx);                                   // ����JPC A 
	    if(sym.name=="else")
	    { int JmpCx = cx;
	      gen(JMP,0,0);  	 
		    Advance();
	    	Statement();
	    	Code[JmpCx].Seta(cx);                                   // ����JMP A 
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
		gen(JMP,0,JmpCx);                                       // ���� 
		Code[JpcCx].Seta(cx);                                   // ����JPC 0 A  
	}
	else if(sym.name=="call")                                    //  call <id>��[<exp>{,<exp>}]��
	{ int parameter; //���� 
	  Advance();
	   if(sym.type=="SIGN") { pos =  position(sym.name,lev); 
			        Advance();}
	   else  error(4,line,sym);                                 // error--Statement---callȱʧSIGN
	   if(sym.name=="(") { Advance();
	                          left++; }	
	   else	  { errorSign.name ="(";
	      	                error(1,line,errorSign); }         //error--Statement---callȱʧ'('	   	 
	    if(sym.name!=")")  {	                                       //�в��� 
		    expression();
	      	 if(Table[pos].GetType()=="PROC"){                // ���ݲ������� 
	      	    parameter = pos+1;
	      	     //cout<<"parameter:"<<lev<<" "<<Table[parameter].GetLevel()<<endl; 
			   gen(STO,lev-Table[parameter].GetLevel(),Table[parameter].GetAdress());  
			   }                                               //��һ�����ǲ���
			   }
	      	while(sym.name==","||sym.name=="("||sym.type=="SIGN")
	      	  	{  Advance();
	      	  	   expression();
	      	  	   if(Table[pos].GetType()=="PROC"){                // ���ݲ������� 
	                       parameter++;
			   gen(STO,lev-Table[parameter].GetLevel(),Table[parameter].GetAdress());  
			   }   
					}
				if(sym.name!=")") 	{errorSign.name =")";
				                      error(1,line,errorSign);}
				else { while(sym.name==")"){ 
					         right++;
					           Advance(); 	  }
					   if(right!=left) error(1,line,errorSign) ;    // error-�������Ų�ƥ�� 
				     } 	                    	                    // cout<<"error--Statement---callȱʧ')'"<<endl;}
	if(!pos) error(2,line,sym);                       // ������errror-<ID>-δ���������� 
	else if(Table[pos].GetType()=="PROC") 
		  gen(CAL,lev-Table[pos].GetLevel(),Table[pos].GetAdress()); //�Ƿ�Ϊadress 
	else  error(2,line,sym);                          // ������error sign���ǹ����� 
	 }
	else if(sym.name=="read")                                       // read (<id>{��<id>})   
	{ Advance();
	  if(sym.name=="(")
	             Advance();
	   else {    errorSign.name ="(";
	             error(1,line,errorSign);}                         // error--Statement---readȱʧ'('               
	  if(sym.type=="SIGN")  { 
	            pos = position(sym.name,lev);                      // �����ʹ�ùؼ��ֺͳ����� 
	            if(!pos) error(2,line,sym);                        // error--<ID>--δ������ 
	            else if(Table[pos].GetType()!="VAR") error(2,line,sym);
				else{  gen(OPR,0,16);                             // ���ɴ���
				       gen(STO,lev-Table[pos].GetLevel(),Table[pos].GetAdress()); 
					 }                                            // ����ջ��,�͵����� 	  
                Advance(); }
	  else  error(4,line,sym);                                     // error--Statement---readȱʧSIGN-ID
	  while(sym.name==","||sym.type=="SIGN")	 
	  	   { Advance();
	  	   	 if(sym.type=="SIGN")  {
				  pos = position(sym.name,lev); 
				  if(!pos) error(2,line,sym);
				  else if(Table[pos].GetType()!="VAR") error(2,line,sym);
				  else{ gen(OPR,0,16);                             // ���ɴ���
				           gen(STO,lev-Table[pos].GetLevel(),Table[pos].GetAdress()); 
					   }      
				  Advance();} 
			  else  error(4,line,sym);
			  if(sym.name==")"||sym.name==";") break;              // �������� 
			}
		if(sym.name==")")  Advance();
		else  { errorSign.name =")"; 
			      error(1,line,errorSign);                        // error--Statement---readȱʧ')'
	            	}	
	 } 
	else if(sym.name=="write")                                    // write (<exp>{,<exp>})
	{ Advance();
	  if(sym.name=="(")
	         Advance();
	  else { errorSign.name ="(";
	              error(1,line,errorSign); }                      // error--Statement---writeȱʧ'('
	  expression();
	  gen(OPR,0,14);
	  while(sym.name==","){      Advance();
	                              expression();
								  gen(OPR,0,14);}
	  gen(OPR,0,15); 
	  if(sym.name==")")  Advance();
	  else {   errorSign.name =")";
	  	         error(1,line,errorSign);            // error--Statement---writeȱʧ')'
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
		  if(IsDfKeyWord(sym.name)) error(3,line,sym);         // error---<ID>---ʹ�ùؼ���
            else{ if(IsLevDfSign(sym.name,lev))            // error---<ID>---�����ѱ�����  
    	                error(5,line,sym);             /*  �޸�error-5 */
	              else    sym_ = sym; } 
	         Advance();          
           }
   else    error(4,line,sym);                           // error--const--ȱʧID
   if(sym.name==":=")  Advance();
   else if(sym.name=="=")  { error(6,line,errorSign);   // error-- =ӦΪ:=   
                                     Advance();   }
   else { errorSign.name=":=";                          // error---const---ȱʧ:=
			       error(1,line,errorSign);  }
   if(sym.type=="CONST")                                // ���� ���� �� 2223
	     	       { strcpy(num,sym.name.c_str()); 
					 value = atoi(num);
					 enterConst(sym_,lev,value,dx);     // CONST enter the SYMTable 
					 Advance();}
    else  error(7,line,sym);                            // error---const--ȱʧ����
}
void Condecl()
{  if(sym.name=="const")  Advance();   
      Condeclaration();
	  while(sym.name==","||sym.type=="SIGN")
	  	 {	if(sym.name==",") Advance();
	  	    else {   errorSign.name =",";               //  error--ȱʧ',' 
			          error(1,line,errorSign); }   
			if(sym.name=="procedure"||sym.name==";"||sym.name=="var")
		     break;	                                    // ��̷��� 
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
		if(IsLevDfSign(sym.name,lev))   error(5,line,sym);   // error---<ID>---�ڵ�ǰ�����ѱ����� 
	       else { enterVar(sym,lev,dx+1);
				  dx++;}                                     // ƫ����  +1 
		   Advance();
	     }
	   else if(sym.type =="KEYWORD"){ 
	           error(3,line,sym);                            // error---<ID>---ʹ�ùؼ���
	           Advance(); }
	   else  error(4,line,errorSign);                        // error--var---ȱʧID
	   
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
				         Advance();                           // error---<ID>---ʹ�ùؼ���      
			     }
			   else  error(4,line,sym);                       // error--ȱʧ<ID>    
	         }
	        else if(sym.type=="SIGN"||sym.type=="KEYWORD")
	        {  errorSign.name = ",";
			   error(1,line,errorSign);                       //  errro---ȱʧ',' 
			 if(IsLevDfSign(sym.name,lev)) error(5,line,sym); //  error---<ID>---�ѱ�����  
	         else { enterVar(sym,lev,dx+1);
				       dx++;} 
	             if(IsDfKeyWord(sym.name))  error(3,line,sym);       //  error---<ID>---ʹ�ùؼ���      
			   Advance();	
			}
			else break;
	       }	
	   if(sym.name!=";") {  errorSign.name=";";
	                          error(1,line-1,errorSign);  //error--ȱʧ';'
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
	                      gen(OPR,0,6);                   // OPR 0 6 �ж���ż 
     	                   expression();
	}
	else {  StrNode symRlOp;
	       if(sym.type=="RELOPERAT")  error(12,line,sym);  // error---ȱʧ---��Ƚ���  
	       else 	expression();
			if(sym.type=="RELOPERAT") symRlOp = sym;                         
		    else  error(10,line,sym);                     // error---condition----ȱʧ�ȽϷ�*/
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
void expression() //���ʽ<exp> ��[+|-]<term>{<aop><term>}  ���ʽ-> ��+|-��
{    StrNode Operat ;
	if(sym.name=="+"||sym.name=="-")  { Operat = sym;
	                                    Advance();}
	term();                                               // ������  
	if(Operat.name=="-") gen(OPR,0,1);                    // opr 0 1 ջ��ȡ��    
	 while(sym.name=="+"||sym.name=="-")
	 { 	Operat = sym; 
	    Advance();
	 	   term();  
	 	if(Operat.name=="+")      gen(OPR,0,2);          //OPR 0 2 ջ����� 
	 	else if(Operat.name=="-") gen(OPR,0,3);          //OPR 0 3 ջ����� 
	 }	
 } 
 void term()  //�� 
{  StrNode Operat;
   factor(); 	
	while(sym.name=="*"||sym.name=="/")
	{   Operat = sym;
		Advance();
		factor();
		if(Operat.name=="*")      gen(OPR,0,4);         //OPR 0 4 ջ����� 
	 	else if(Operat.name=="/") gen(OPR,0,5);	        //OPR 0 5 ջ����� 
	}
}
void factor()
{  
	if(sym.type=="SIGN") 
	{ int pos = position(sym.name,lev);
	  if(!pos)    error(2,line,sym);                     // error---<ID>---δ����  
	  else{ if(Table[pos].GetType()=="VAR") 
	        gen(LOD,lev-Table[pos].GetLevel(),Table[pos].GetAdress());  
			else if(Table[pos].GetType()=="CONST")
		     gen(LIT,0,Table[pos].GetValue()) ;           //LIT 0 A ������ջ�� 
			else error(2,line,sym);                          // �Ǳ����ͳ��� error����      
	      }             
	   Advance();
	    }  
	else if(sym.type=="CONST") { int value;char num[15];
	           strcpy(num,sym.name.c_str()); 
			   value = atoi(num); 
		       gen(LIT,0,value) ;                    //LIT 0 A ������ջ�� 
		         Advance();	} 
		         
	else if(sym.name=="(")
	 { left_paren++;
	 	Advance();
	 	expression(); 
	 if(sym.name==")")
	        {  if(left_paren==0)  error(13,line,sym); //")δ��ƥ��"<<endl; 
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
  CopySYM(sym_now,sym); //���浱ǰsym 
  StrNode sym_next;
  Advance(); //ȡֵ��һ��sym 
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
{   cout<<"-------���ű�----------"<<endl;
   cout<<"����\t"<<"ֵ\t"<<endl;
	for(int i = 1 ;i<ST_i ; i++)
	 cout<<SYMTable[i].type<<"\t"<<SYMTable[i].name<<endl;
}
/*
1.��ʶ��δ���� ���������ʶ��
2.��ʶ�����ظ���������������ͻ  ��var����const���� ���
3.��ʶ��ʹ�ùؼ���   ����ʱ�飺�ؼ��ֱ�  

bool IsDefine_Sign() //�Ƿ����ʶ�� 
{ 	for(int i = 1;i<=ST_i;i++)
	  if(sym.name == SYMTable[i].name)  return true;
return false;
}
bool IsDf_KeyWord()
{  for(int i =0;i <= 14 ; i++)
	 if(KeyWords[i]==sym.name) return true;
	 return false;
} */
