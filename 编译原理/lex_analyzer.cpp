#include<bits/stdc++.h>
using namespace std;

string symlist[]={"begin", "end", "integer", "if", "then", "else", "function","read", "write", "10", "11", "=", "<>", "<=", "<", ">=",">", "-", "*", ":=", "(", ")", ";"};
char single_sym[]={'-','*','(',')',';'};
int main()
{
	FILE *infile;
	fstream error;
	fstream outfile;
//	error.open("sample.err",ios::out|ios::in|ios::app);
//	outfile.open("sample.dyd",ios::out|ios::app);
	error.open("sample.err",ios::out|ios::in);
	outfile.open("sample.dyd",ios::out);
	if((infile=fopen("sample.pas","r"))==NULL)
	{
		error<<"Fatal error："<<"无法读取输入文件"<<endl;
		exit(1);
	}
	
	int pos=0,line=1,type_err=0,t_type=0,leftp=0;//位置，行数，错误类型，扫描字符串类型 ,左括号个数 
	bool goback=false;//判断是否需要读取下一字符
	char now;//当前读取字符 
	string temp,gs;//当前读取字符串，匹配输出格式的字符串 
	
	while(true)
	{
		if(goback)
		{
			goback=false;
			pos--;
		}
		else
		{
			now=fgetc(infile);
			pos++;
		}
		temp.clear();
		gs.clear();
		type_err=0;
		t_type=0;
		
		
		if(now==EOF)
		{
			temp="";
			gs.resize(13,' ');
			gs.append("EOF 25");
			outfile<<gs<<endl;
			break;
		}
		
		if(now=='\n'||now=='\r')
		{
			temp="\n";
			line++;
			pos=0;
			gs.resize(12,' ');
			gs.append("EOLN 24");
			outfile<<gs<<endl;
		}
		else if((now=='-')||(now=='*')||(now==';'))
		{
			temp.push_back(now);
		}
		else if((now=='('))
		{
			leftp++;
			temp.push_back(now);
		}
		else if((now==')'))
		{
			if(leftp>0)
			{
				temp.push_back(now);
				leftp--;
			}
			else
			{
				type_err=4;
			}
		}
		else if(now==' ')
		{
			continue;
		}
		else if((now >='a' && now<='z')|| (now>='A' && now<='Z'))
		{
			while((now >='a' && now<='z')|| (now>='A' && now<='Z')||(now>='0' && now<='9'))
			{
				temp.push_back(now);
				pos++;
				now=fgetc(infile);
				if(temp.size()>16)
				{
					type_err=3;
					continue;
				}	
			}
			t_type=1;
			goback=true;
		}
		else if((now>='0' && now<='9'))
		{
			while((now>='0' && now<='9'))
			{
				temp.push_back(now);
				pos++;
				now=fgetc(infile);
				if(temp.size()>16)
				{
					type_err=3;
					continue;
				}
			}
			t_type=2;
			goback=true;
		}
		else if(now==':')
		{
			now=fgetc(infile);
		    pos++;
		    if(now=='=')
		    {
		    	temp=":=";
			}
			else
			{
				if(now!='\0')
				{
					goback=true;
				}
				type_err=1; 
			}
		}
		else if(now=='<')
		{
			now=fgetc(infile);
		    pos++;
		    if(now=='=')
		    {
		    	temp="<=";
			}
			else if(now=='>')
			{
				temp="<>";
			}
			else
			{
				if(now!='\0')
				{
					goback=true;
				} 
			}
		}
		else if(now=='>')
		{
			now=fgetc(infile);
		    pos++;
		    if(now=='=')
		    {
		    	temp=">=";
			}
			else
			{
				if(now!='\0')
				{
					goback=true;
				} 
			}
		}
		else
		{
			type_err=2;
		}
		
		if(type_err!=0)
		{
			if(type_err==1)
			{
				error<<"LINE"<<line<<": "<<" 冒号无法匹配"<<endl; 
				//break;
			}
			else if(type_err==2)
			{
				error<<"LINE"<<line<<": "<<" 出现无法识别的非法字符"<<now<<endl;
				//break;
			}
			else if(type_err==3)
			{
				error<<"LINE"<<line<<": "<<" 标识符"<<temp<<"长度超过16，产生溢出"<<endl;
				//break;
			}
//			else if(type_err==4)
//			{
//				error<<"LINE"<<line<<": "<<" )无法匹配"<<endl;
//				break;
//			}
		}
		
		if(t_type==2)
		{
			int le=temp.size();
			gs.resize((16-le),' ');
			gs.append(temp);
			outfile<<gs<<' '<<11<<endl;
			continue;
		}
		
		int wz=-1;
		for(int i=0;i<23;i++)
		{
			if(symlist[i]==temp)
			{
				wz=i;
				break;
			}
		}
		if(wz!=-1)
		{
			int le=temp.size();
			gs.resize((16-le),' ');
			gs.append(temp);
			outfile<<gs<<' '<<(wz+1)<<endl;
		}
		else if(t_type==1)
		{
			int le=temp.size();
			gs.resize((16-le),' ');
			gs.append(temp);
			outfile<<gs<<' '<<10<<endl;
		}
			
	}
	fclose(infile);
	error.close();
	outfile.close();
	
	return 0;
	
}
