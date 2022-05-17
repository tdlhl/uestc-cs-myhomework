#include<bits/stdc++.h>
using namespace std;

int index=0,line=1;

vector<string> dyd_list;
vector<int> dyd_id;

int begin_code=1,end_code=2,integer_code=3,if_code = 4,then_code = 5;
int else_code = 6,function_code = 7,read_code = 8,write_code = 9;
int identifier_code = 10,constant_code = 11,eoln = 24,eof = 25;
int var_define = 0,var_use = 1,formal_parameter = 1,var_count = 0;
int procedure_count = 0,procedure_level = 0,num_xc=0;

vector<vector<string>> var_table;
vector<vector<string>> pro_table;

string main_procedure_text = "main";
string now_Procedure = main_procedure_text;
string last_Procedure;
fstream error;

void main_procedure();
void branch_procedure();
void advance();
void declare_statement_table();
void declare_statement_tableA();
void declare_statement();
void var_declare(); 
void var(int define_or_use,int is_formal);
string identifier();
void relation_operator();
void condition_expression();
void condition_statement();
void unsigned_integer();
void constant();
void func_all();
void factor();
void itemA();
void item();
void math_expression();
void math_expressionA();
void assign_statement();
void write_statement();
void read_statement();
void exec_statement();
void exec_statement_tableA();
void exec_statement_table();
void func_body();
void parameter();
void func_declare();
int now_more_one(int next);
int now();
string now_word();
string removekg(string hang,int &tid);

void main_procedure()
{
	//<程序>→<分程序>
	branch_procedure();
	return;
}
void branch_procedure()
{
	//<分程序>→begin <说明语句表>；<执行语句表> end
	if(now()==begin_code)
	{
		advance();
		declare_statement_table();
		if(now()==23)
		{
			advance();
			exec_statement_table();
			if(now()==end_code)
			{
				advance();
			}
			else
			{
				error<<"LINE"<<line<<": "<<"缺少符号错 缺少end"<<endl;
			}
		}
		else
		{
			error<<"LINE"<<line<<": "<<"缺少符号错 缺少;"<<endl;	
		} 
	}
	else
	{
		error<<"LINE"<<line<<": "<<"缺少符号错 缺少begin"<<endl;
	}
	return;
}
void advance()
{
	int h1=now();
	index++;
}
void declare_statement_table()
{
//     <说明语句表>→<说明语句><说明语句表A>
//    <说明语句表A>→;<说明语句><说明语句表A>│空
	declare_statement();
	declare_statement_tableA();
}
void declare_statement_tableA()
{
//	#<说明语句表A>→;<说明语句><说明语句表A>│空
	if(now()==23&&now_more_one((index+1))==integer_code)
	{
		advance();
		declare_statement();
		declare_statement_tableA();
	}
	else
	{
		return;
	}
}
void declare_statement()
{
	//<说明语句>→<变量说明>│<函数说明>
	if(now()==integer_code)
	{
		if(now_more_one(index+1)==7)
		{
			func_declare();
		}
		else
		{
			var_declare();
		}
	}
	else
	{
		//cout<<now()<<endl;
		error<<"LINE"<<line<<": "<<"说明语句错 缺少integer"<<endl;
	}
}
void var_declare()
{
	//<变量说明>→integer <变量>
	if(now()==3)
	{
		advance();
		var(var_define,0);
	}
	else
	{
		error<<"LINE"<<line<<": "<<"变量声明错 缺少integer"<<endl;
	}
}
void var(int define_or_use,int is_formal)
{
	//<变量>→<标识符>
	vector<string> temp;
	string word; 
	//分类：变量是定义还是使用 
	if(define_or_use==var_define)
	{
		word=identifier();
		bool flag=true;
		for(int i=0;i<var_table.size();i++)
		{
			if(word==var_table[i][0])
			{
				flag=false;
				if(var_table[i][2]=="1"&&num_xc>0)
				{
					num_xc--;
				}
				else
				{
					error<<"LINE"<<line<<": "<<"变量"<<word<<"重定义"<<endl;
					break;
				}			
			}
		} 
		if(flag)
		{
			temp.push_back(word);
			temp.push_back(now_Procedure);
			temp.push_back(to_string(is_formal));
			temp.push_back("integer");
			temp.push_back(to_string(procedure_level));
			temp.push_back(to_string(var_count));
			var_count++;
			var_table.push_back(temp);
			
			//更新过程表中的fadr，ladr 
			for(int i=0;i<pro_table.size();i++)
			{
				if(pro_table[i][0]==now_Procedure)
				{
					if(pro_table[i][3]=="-1")
					{
						pro_table[i][3]=to_string(var_count-1);
					}
					if(pro_table[i][4]<to_string(var_count-1))
					{
						pro_table[i][4]=to_string(var_count-1);
					}
					
				}
			}
		}
	}
	else if(define_or_use==var_use)
	{
		word=identifier();
		bool use_word=false;
		for(int i=0;i<var_table.size();i++)
		{
			if(word==var_table[i][0])
			{
				use_word=true;
			}
		} 
		for(int j=0;j<pro_table.size();j++)
		{
			if(word==pro_table[j][0])
			{
				use_word=true;
			}
		}
		if(!use_word)
		{
			error<<"LINE"<<line<<": "<<"符号"<<word<<"无定义"<<endl;
		}
	}
} 
string identifier()
{
	//<标识符>→<字母>│<标识符><字母>│ <标识符><数字>
	string temp_word;
	if(now()==identifier_code)
	{
		temp_word=now_word();
		advance();
		return temp_word;
	}
	else
	{
		error<<"LINE"<<line<<": "<<"无法匹配标识符"<<endl;
	}
}
void relation_operator()
{
	//<关系运算符> →<│<=│>│>=│=│<>
	int cht=now();
	if(cht==15||cht==14||cht==17||cht==16||cht==12||cht==13)
	{
		advance();
	}
	else
	{
		error<<"LINE"<<line<<": "<<"无法匹配关系运算符"<<endl;
	}
}
void condition_expression()
{
	//<条件表达式>→<算术表达式><关系运算符><算术表达式>
	math_expression();
    relation_operator();
    math_expression();
}
void condition_statement()
{
	//<条件语句>→if<条件表达式>then<执行语句>else <执行语句>
	if(now()==4)
	{
		advance();
		condition_expression();
		if(now()==5)
		{
			advance();
			exec_statement();
			if(now()==6)
			{
				advance();
				exec_statement();
			} 
			else
			{
				error<<"LINE"<<line<<": "<<"缺少符号错，缺少else"<<endl;
			}
		}
		else
		{
			error<<"LINE"<<line<<": "<<"缺少符号错，缺少then"<<endl;
		}
	} 
	else
	{
		error<<"LINE"<<line<<": "<<"缺少符号错，缺少if"<<endl;	
	}
}
void unsigned_integer()
{
	if(now()==11)
	{
		advance();
	}
	else
	{
		error<<"LINE"<<line<<": "<<"常数匹配非数字"<<endl;
	}
}
void constant()
{
	//<常数>→<无符号整数>
	unsigned_integer();
}
void func_all()
{
	//<函数调用>→<标识符>(<算数表达式>)
	identifier();
	if(now()==21)
	{
		advance();
		math_expression();
		if(now()==22)
		{
			advance();
		}
		else
		{
			error<<"LINE"<<line<<": "<<"缺少符号错,缺少)"<<endl;
		}	
	} 
	else
	{
		error<<"LINE"<<line<<": "<<"缺少符号错,缺少("<<endl;
	}
}
void factor()
{
	//<因子>→<变量>│<常数>│<函数调用>
	if(now()==11)
	{
		constant();
	}
	else if(now_more_one(index+1) ==21)
	{
		func_all();
		
	}
	else
	{
		var(var_use,0);
	}
}
void itemA()
{
	//<项A>→*<因子><项A>│空
	if(now()==19)
	{
		advance();
		factor();
		itemA();
	}
	else
	{
		return;
	}
}
void item()
{
//    <项>→<因子><项A>
//    <项A>→*<因子><项A>│空
	factor();
	itemA();
}
void math_expressionA()
{
	//<算术表达式A>→-<项><算术表达式A>|空
	if(now()==18)
	{
		advance();
		item();
		math_expressionA();
	} 
	else
	{
		return;
	}
}
void math_expression()
{
//    <算术表达式>→<项><算术表达式A>
//    <算术表达式A>→-<项><算术表达式A>|空
	item();
	math_expressionA();
}
void assign_statement()
{
	//<赋值语句>→<变量>:=<算术表达式>
	var(var_use,0);
	if(now()==20)
	{
		advance();
		math_expression();
	}
	else
	{
		error<<"LINE"<<line<<": "<<"无法识别赋值语句"<<endl;
	}
}
void write_statement()
{
	//<写语句>→write(<变量>)
	if(now()==9)
	{
		advance();
		if(now()==21)
		{
			advance();
			var(var_use,0);
			if(now()==22)
			{
				advance();
			}
			else
			{
				error<<"LINE"<<line<<": "<<"缺少符号错,缺少)"<<endl;
			}
		}
		else
		{
			error<<"LINE"<<line<<": "<<"缺少符号错,缺少("<<endl;
		}
	}
	else
	{
		error<<"LINE"<<line<<": "<<"无法识别write语句"<<endl;
	}
}
void read_statement()
{
	//<读语句>→read(<变量>)
	if(now()==8)
	{
		advance();
		if(now()==21)
		{
			advance();
			var(var_use,0);
			if(now()==22)
			{
				advance();
			}
			else
			{
				error<<"LINE"<<line<<": "<<"缺少符号错,缺少)"<<endl;
			}
		}
		else
		{
			error<<"LINE"<<line<<": "<<"缺少符号错,缺少("<<endl;
		}
	}
	else
	{
		error<<"LINE"<<line<<": "<<"无法识别read语句"<<endl;
	}
}
void exec_statement()
{
	//<执行语句>→<读语句>│<写语句>│<赋值语句>│<条件语句>
	int state=now();
	if(state==read_code)
	{
		read_statement();
	} 
	else if(state==write_code)
	{
		write_statement();
	}
	else if(state==if_code)
	{
		condition_statement();
	}
	else if(state==identifier_code)
	{
		assign_statement();
	}
	else
	{
		error<<"LINE"<<line<<": "<<"fatal error 执行语句出错，无法进一步分析"<<endl;
		//exit(1);
	}
}
void exec_statement_tableA()
{
	//<执行语句表A>→;<执行语句><执行语句表A>│空
	int check=now_more_one(index+1);
	if(now()==23&&(check==8||check==9||check==4||check==10))
	{
		advance();
		exec_statement();
		exec_statement_tableA();
	}
	else
	{
		return;
	}
}
void exec_statement_table()
{
//    # <执行语句表>→<执行语句><执行语句表A>
//    # <执行语句表A>→;<执行语句><执行语句表A>│空
	exec_statement();
	exec_statement_tableA();
	return;
}
void func_body()
{
	//<函数体>→begin <说明语句表>；<执行语句表> end
	if(now()==begin_code)
	{
		advance();
		declare_statement_table();
		if(now()==23)
		{
			advance();
			exec_statement_table();
			if(now()==end_code)
			{
				advance();
			}
			else
			{
				error<<"LINE"<<line<<": "<<"缺少符号错,缺少end"<<endl;
			}
		}
		else
		{
			error<<"LINE"<<line<<": "<<"缺少符号错,缺少;"<<endl;
		}
	}
	else
	{
		error<<"LINE"<<line<<": "<<"缺少符号错,缺少begin"<<endl;
	}
}
void parameter()
{
	//<参数>→<变量>
	var(var_define,formal_parameter);
	num_xc++;
	return;
}
void func_declare()
{
	//<函数说明>→integer function <标识符>（<参数>）；<函数体>
	procedure_level++;
	bool flag;
	vector<string> temp_p;
	last_Procedure = now_Procedure;
	if(now()==integer_code)
	{
		advance();
		if(now()==function_code)
		{
			advance();
			now_Procedure = identifier();
			flag=true;
			for(int i=0;i<pro_table.size();i++)
			{
				if(now_Procedure==pro_table[i][0])
				{
					flag=false;
				}
			}
			if(flag)
			{
				temp_p.push_back(now_Procedure);
				temp_p.push_back("integer");
				temp_p.push_back(to_string(procedure_level));
				temp_p.push_back("-1");
				temp_p.push_back("-1");
				pro_table.push_back(temp_p);
			}
			if(now()==21)
			{//左括号 
				advance();
				parameter();
				if(now()==22)
				{
					advance();
					if(now()==23)
					{
						advance();
						func_body();
					}
					else
					{
						error<<"LINE"<<line<<": "<<"缺少符号错,函数说明缺少;"<<endl;
					}
				}
				else
				{
					error<<"LINE"<<line<<": "<<"缺少符号错,缺少)"<<endl;
				} 
			}
			else
			{
				error<<"LINE"<<line<<": "<<"缺少符号错,缺少("<<endl;
			}
		}
		else
		{
			error<<"LINE"<<line<<": "<<"缺少符号错,缺少function"<<endl;
		}
	}
	else
	{
		error<<"LINE"<<line<<": "<<"缺少符号错,缺少integer"<<endl;
	}
	now_Procedure = last_Procedure;
	procedure_level--;
}
int now_more_one(int next)
{
	if(dyd_id[next]==eoln)
	{
		return now_more_one(next+1);
	}
	else
	{
		return dyd_id[next];
	}
}
int now()
{//返回当前种别号 
	if(dyd_id[index]==eoln)
	{
		line++;
		index++;
		return now();
	}
	else
	{
		return dyd_id[index];
	}
}
string now_word()
{//返回当前单词符号 
	if(dyd_id[index]==eoln)
	{
		line++;
		index++;
		return now_word();
	}
	else
	{
		return dyd_list[index];
	}
}

string removekg(string hang,int &tid)
{
	string temp;
//	char num,all=0;
	int i=0;
	while(i<hang.size()&&hang[i]==' ')
	{
		i++;
	}
	while(i<hang.size()&&hang[i]!=' ')
	{
		temp.push_back(hang[i]);
		i++;
	}
	i++;
	string rest(hang,i);
	int num=atoi(rest.c_str());
	tid=num;
	return temp;
}

int main()
{	
	ifstream infile;
	fstream outdys;
	fstream outvar;
	fstream outpro;
	infile.open("sample.dyd",ios::in);
//	error.open("sample.err",ios::out|ios::in|ios::app);
//	outdys.open("sample.dys",ios::out|ios::app);
//	outvar.open("sample.var",ios::out|ios::app);
//	outpro.open("sample.pro",ios::out|ios::app);
	
	error.open("sample.err",ios::out|ios::in);
	outdys.open("sample.dys",ios::out);
	outvar.open("sample.var",ios::out);
	outpro.open("sample.pro",ios::out);
	
	string hang,whang;
	int tid;
	if(infile)
	{
		while(getline(infile,hang))
		{
			outdys<<hang<<endl;
			whang=removekg(hang,tid);
			dyd_list.push_back(whang);
			dyd_id.push_back(tid);
		}
	}
	else
	{
		error<<"fatal error: 无法读取dyd文件"<<endl;
		return -1;
	}
	
//	for(int i=0;i<dyd_list.size();i++)
//	{
//		cout<<dyd_list[i]<<" "<<dyd_id[i]<<endl;
//	}

	main_procedure();
	//cout<<"var_table.size()"<<var_table.size()<<endl; 
	//cout<<"pro_table.size()"<<pro_table.size()<<endl; 
	for(int i=0;i<var_table.size();i++)
	{
		for(int j=0;j<var_table[i].size();j++)
		{
			outvar<<var_table[i][j]<<' ';
		}
		outvar<<endl;
	}
	for(int i=0;i<pro_table.size();i++)
	{
		for(int j=0;j<pro_table[i].size();j++)
		{
			outpro<<pro_table[i][j]<<' ';
		}
		outpro<<endl;
	}
	
	if(now()!=25)
	{
		error<<"LINE"<<line<<": "<<"语法分析完成，但仍有未规约语句"<<endl;
	}
	
	infile.close();
	error.close();
	outvar.close();
	outpro.close();
	return 0;
}
