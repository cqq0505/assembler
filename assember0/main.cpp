#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

const char *insR1[4]={"add","and","or","xor"};//add/and/or/xor rd,rs,rt
const char *insR1Op[4]={"000000000001","000001000001","000001000010","000001000100"};//the op of the add/and/or/xor
const char *insR2[3]={"sra","srl","sll"};//sll/srl/sra  rd,rt,shift
const char *insR2Op[3]={"000010000001","000010000010","000010000011"};//the op of the sra/srl/sll
const char *insI1[4]={"addi","andi","ori","xori"};//andi/ori/xori  rt,rs,imm
const char *insI1Op[4]={"000101","001001","001010","001100"};//
const char *insI2[2]={"load","store"};//load/store  rt,offset(rs)
const char *insI2Op[2]={"001101","001110"};
const char *insI3[2]={"beq","bne"};//beq/bne  rs,rt,label
const char *insI3Op[2]={"001111","010000"};

void convert(int m,int n,ostream&os)
{
    int i=0;
    int num[m];
    int sign =(n>=0)? 1: -1;
    n = n *sign;
    while(n)
    {
        num[i]=n%2;
        n = n/2;
        i++;
    }

     if(sign ==-1)
     {
         for(i=i;i<m;i++)
         num[i]=1;//装二进制的数组置0，确保输出位数

     }
     else
     {
        for(i=i;i<m;i++)
         num[i]=0;//装二进制的数组置0，确保输出位数

     }

     for(i=m-1;i>=0;i--)
    {
        //printf("%d",num[i]);
        cout<<num[i];
        os<<num[i];
    }

}

int getReg(char* arg)
{
    int reg=0;
    char *p= arg;
    while (*p == ' ') p++;
    if (*p == 'r')
    {
        p++;
    }
    else return  -1;

    while(*p>='0'&& *p <= '9')
    {
        reg =10 * reg + (*p -'0');
        p++;
    }

    if (*p !='\0') return -1;
    else return reg;
}

int main()
{
    int line=0;
    int i=0;
    char command[20],arg1[20],arg2[20],arg3[20];
    char address[100];
    int rd,rs=0,rt;
    char everyline[100];

    fstream in("input.txt",ios::out|ios::in|ios::trunc);
    ofstream out("output.txt");

    if(!in)
    {
        cerr<<"input file can not be found"<<endl;
        exit(0);
    }

    while(cin.getline(everyline,100))//写入指令
        in<<everyline<<endl;

    in.clear();
    in.seekg(0);


    if(!out)
    {
        cerr<<"output file can not be found"<<endl;
        exit(0);
    }

    while(!in.eof()&& in.peek()!=EOF)//排除最后一行干扰
    {
        line++;
        cout<<"line "<<line<<": ";
        in.getline(command,20,' ');

        if(!strcmp(command,"jump"))//jump  target
        {

             in.getline(address,100);
                int target =0;
                char *p=address;
                while(*p==' ') p++;

                while(*p>='0'&& *p<='9')
                {
                    target=10*target+(*p -'0');
                    p++;
                }

            cout<<"010010";
            out<<"010010";
            convert(26,target,out);
            cout<<endl;
            out<<endl;
        }

        else if(!(strcmp(command,"and")&&strcmp(command,"add")&&strcmp(command,"or")&&strcmp(command,"xor")
                  ))
        {
        for(i=0;i<4;i++)//add/and/or/xor rd,rs,rt
        {
            if(!strcmp(command,insR1[i]))
            {
                in.getline(arg1,20,',');
                rd=getReg(arg1);
                if(rd == -1)
                {
                    cerr<<"error at line "<<line<<" about rd"<<endl;
                    in.close();
                    out.close();
                    exit(0);

                 }

                 in.getline(arg2,20,',');
                 rs=getReg(arg2);
                 if(rs == -1)
                {
                   cerr<<"error at line :"<<line<<" about rs"<<endl;
                   in.close();
                   //out.close();
                   exit(0);

                 }

                 in.getline(arg3,20);
                 rt=getReg(arg3);
                  if(rt == -1)
                {
                    cerr<<"error at line :"<<line<<" about rt"<<endl;
                    in.close();
                    //out.close();
                    exit(0);

                }
              cout<<insR1Op[i];
              out<<insR1Op[i];
              convert(5,0,out);
              convert(5,rd,out);
              convert(5,rs,out);
              convert(5,rt,out);
              cout<<endl;
              out<<endl;

            }

        }


        }
        else if(!(strcmp(command,"andi")&&strcmp(command,"addi")&&strcmp(command,"ori")&&strcmp(command,"xori")
                  ))
        {
        for(i=0;i<4;i++)//addi/andi/ori/xori  rt,rs,imm
        {
            if(!strcmp(command,insI1[i]))
            {

                in.getline(arg3,20,',');
                 rt=getReg(arg3);
                  if(rt == -1)
                {
                    cerr<<"error at line :"<<line<<" about rt"<<endl;
                    in.close();
                    //out.close();
                    exit(0);

                }

                in.getline(arg2,20,',');
                 rs=getReg(arg2);
                 if(rs == -1)
                {
                   cerr<<"error at line :"<<line<<" about rs"<<endl;
                   in.close();
                   //out.close();
                   exit(0);

                 }

                in.getline(arg1,20);
                 int offset=0;
                char *p=arg1;
                while(*p==' ') p++;

                while(*p>='0'&& *p<='9')
                {
                    offset=10*offset+(*p -'0');
                    p++;
                }

                cout<<insI1Op[i];
                out<<insI1Op[i];
                convert(16,offset,out);
                convert(5,rs,out);
                convert(5,rt,out);
                cout<<endl;
                out<<endl;

            }
        }
        }


        else if(!(strcmp(command,"sra")&&strcmp(command,"srl")&&strcmp(command,"sll")))
        {

        for(i=0;i<3;i++)//sll/srl/sra  rd,rt,shift
        {
            if(!strcmp(command,insR2[i]))
            {
                in.getline(arg1,20,',');
                rd=getReg(arg1);
                if(rd == -1)
                {
                    cerr<<"error at line "<<line<<" about rd"<<endl;
                    in.close();
                    out.close();
                    exit(0);
                }

                 in.getline(arg3,20,',');
                 rt=getReg(arg3);
                  if(rt == -1)
                {
                    cerr<<"error at line :"<<line<<" about rt"<<endl;
                    in.close();
                    out.close();
                    exit(0);
                }

                in.getline(arg2,20);
                int shift=0;
                char *p=arg2;
                while(*p==' ') p++;

                while(*p>='0'&& *p<='9')
                {
                    shift=10*shift+(*p -'0');
                    p++;
                }

                cout<<insR2Op[i];
                out<<insR2Op[i];
                convert(5,shift,out);
                convert(5,rd,out);
                convert(5,0,out);//rs
                convert(5,rt,out);
                cout<<endl;
                out<<endl;
            }
        }
        }

        else if(!(strcmp(command,"load")&&strcmp(command,"store")))
        {
        for(i=0;i<2;i++)//load/store rt,offset(rs)
        {
            if(!strcmp(command,insI2[i]))
            {

                in.getline(arg3,20,',');
                rt=getReg(arg3);
                if(rt == -1)
                {
                    cerr<<"error at line "<<line<<" about rt"<<endl;
                    in.close();
                    out.close();
                    exit(0);

                }

                in.getline(arg2,20);
                int offset=0;
                int sign=1;

                char *p =arg2;//offset(rs)

                while (*p ==' ') p++;

                if(*p =='-')
                {
                    p++;
                    sign= -1;

                }

                //offset
                while(*p>='0'&&*p<='9')
                {
                    offset =10 *offset + (*p -'0');
                    p++;
                }

                offset=sign*offset;

                while (*p ==' ') p++;//skip ' '
                if(*p =='(') p++;//skip '('
                else
                {
                    cout<<"error at line"<<line<<endl;
                    in.close();
                    out.close();
                    exit(0);
                }

                //rs
                rs=0;
                if(*p == 'r') p++;
                 while(*p>='0'&&*p<='9')
                {
                    rs =10 *rs + (*p -'0');
                    p++;
                }

                 if(rs == -1)
                {
                    cerr<<"error at line "<<line<<" about rs"<<endl;
                    //in.close();
                    //exit(0);

                }

                while (*p ==' ') p++;//skip ' '
                if(*p != ')')
                {
                    cerr<<"error at line "<<line<<endl;

                }

                cout<<insI2Op[i];
                out<<insI2Op[i];
                convert(16,offset,out);
                convert(5,rs,out);
                convert(5,rt,out);
                cout<<endl;
                out<<endl;
            }
        }

        }

        else if(!(strcmp(command,"beq")&&strcmp(command,"bne")))
        {

        for(i=0;i<2;i++)//beq/bne  rs,rt,label
        {
            if(!strcmp(command,insI3[i]))
            {
                 in.getline(arg2,20,',');
                 rs=getReg(arg2);
                 if(rs == -1)
                {
                   cerr<<"error at line "<<line<<" about rs"<<endl;
                   in.close();
                   out.close();
                   exit(0);

                 }

                 in.getline(arg3,20,',');
                 rt=getReg(arg3);
                  if(rt == -1)
                {
                    cerr<<"error at line "<<line<<" about rt"<<endl;
                    in.close();
                    out.close();
                    exit(0);

                }

                in.getline(arg1,20);
                int offset,label=0;
                char *p=arg1;
                while(*p== ' ') p++;
                while(*p>='0'&&*p<='9')
                {
                    label =10 *label + (*p -'0');
                    p++;
                }

                offset = label /4 - line;// label= pc+4+ offset<<2;

                cout<<insI3Op[i];
                out<<insI3Op[i];
                convert(16,offset,out);
                convert(5,rs,out);
                convert(5,rt,out);
                cout<<endl;
                out<<endl;

            }
        }
        }

        else
        {
            in.getline(address,100);
            cout<<"invalid instruction about "<<command<<address<<endl;
            out<<"invalid instruction about "<<command<<address<<endl;
        }

    }

    in.close();
    out.close();
    return 0;
}
