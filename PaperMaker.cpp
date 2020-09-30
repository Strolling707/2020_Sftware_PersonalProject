#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <io.h>
#include <direct.h>
using namespace std;

void SignIn();
void AskType();
void MakePaper();
void InitUser();
void MainFace();     //生成主界面
void MakeFilePath(); //生成文件路径
bool IsRepetitive(string exercise, string filepath); //判断这道题目是否重复的函数

string g_type;       //当前生成试卷的类型（小学、初中、高中）
int g_opernum;       //每道题目的操作数的个数
int g_oper;         //操作数本身
int g_number;        //试卷内题目的数量(取值范围10~30)
string g_path;       //保存生成文件的路径
string g_checkpath;   //每个账户的查重文件路径
bool g_signflag = 0; //登录标志位，为0则为未登录；为1则为已登录
int g_i;               //当前为第g_i个用户登录

class Teacher
{
    public:
        bool TchJudge(string key);  
        string m_account;
        string m_password;
        string m_type;
};

Teacher TchUser[9];   //声明九个用户

bool Teacher::TchJudge(string key)
{
    string keytmp = m_account + " " + m_password;
    return (key==keytmp); //输入正确为1；输入错误为0
}

void InitUser()
{
    TchUser[0].m_account = "张三1";
    TchUser[0].m_password = "123";
    TchUser[0].m_type = "小学";
    TchUser[1].m_account = "张三2";
    TchUser[1].m_password = "123";
    TchUser[1].m_type = "小学";
    TchUser[2].m_account = "张三3";
    TchUser[2].m_password = "123";
    TchUser[2].m_type = "小学";

    TchUser[3].m_account = "李四1";
    TchUser[3].m_password = "123";
    TchUser[3].m_type = "初中";
    TchUser[4].m_account = "李四2";
    TchUser[4].m_password = "123";
    TchUser[4].m_type = "初中";
    TchUser[5].m_account = "李四3";
    TchUser[5].m_password = "123";
    TchUser[5].m_type = "初中";

    TchUser[6].m_account = "王五1";
    TchUser[6].m_password = "123";
    TchUser[6].m_type = "高中";
    TchUser[7].m_account = "王五2";
    TchUser[7].m_password = "123";
    TchUser[7].m_type = "高中";
    TchUser[8].m_account = "王五3";
    TchUser[8].m_password = "123";
    TchUser[8].m_type = "高中";
}

void SignIn()   //登录函数
{
    cout<<"如果退出程序请输入-1"<<endl;
    string key;
    getline(cin,key);
    cout<<endl;
    if(key == "-1")
    {
        exit(0);
    }
    else
    {  
        for(g_i=0; g_i<10; g_i++)
        {
            if(g_i==9)
            {
                cout<<"如果登录请输入正确的用户名、密码!"<<endl;
                break;
            }

            if(TchUser[g_i].TchJudge(key))
            {
                MainFace();
                cout<<"登录成功！"<<endl;
                g_signflag = 1; //登陆成功后标志位置1，表明已登录
                g_type = TchUser[g_i].m_type; //试卷的类型默认为教师的类型

                while(1)        //登陆成功就可以出题了
                {
                    AskType();
                    MakePaper();
                }  
                break;
            }
        }
    }
}

void MakePaper()
{
    cout<<"准备生成"<<g_type<<"数学题目"<<endl;
    cout<<"请输入生成题目的数量（10到30），如果退出登录请输入-1"<<endl;
    cin>>g_number;
    cout<<endl;

    while(g_number<10 || g_number>30)
    {
        if(g_number == -1)
        {
            MainFace();
            cout<<"您已退出登录，请重新登录！"<<endl;
            g_signflag = 0; //未登录状态
            while(!g_signflag)   //如果登录不成功，就需要一直进行登陆操作
            {
                SignIn();
            }
            break;
        }
        else
        {
            cout<<"请重新输入题目数量（10到30）:"<<endl;
            cin>>g_number;
            cout<<endl;
        }
    }

    if(g_number>=10 && g_number<=30)
    {
        ofstream current_file;
        ofstream current_checkfile;
        
        const char *folder = TchUser[g_i].m_account.c_str(); //生成以用户名命名的文件夹
        if(_access(folder,0) == -1)
        {
            _mkdir(folder);
        }
        
        MakeFilePath();
        current_file.open(g_path.c_str());
        current_file<<"当前为"<<g_type<<"试卷，共"<<g_number<<"道题目"<<endl<<endl;

        current_checkfile.open(g_checkpath.c_str(),ios::app); 

        int exercise_number = 1; //当前题号

       if(g_type == "小学")
        {       
            while(g_number != 0)
            {
                stringstream exercise;  //当前生成的题目
                g_opernum = rand()%4+2; //小学题目操作数至少两个，取2到5
                while(g_opernum != 0)   //每个操作数配一个操作符（最后一个除外）,最后合成一个字符串（完整的题目）
                {
                    g_opernum--;         

                    int g_oper = rand()%100+1; //操作数取值范围在1到100
                    exercise<<g_oper;       //把这个操作数加入到题目中

                    if(g_opernum == 0)     //如果之后没有操作数，那么就不必再加入操作符了
                    {
                        break;
                    }

                    int flagtmp = rand()%4; //操作符有四种情况“+、-、*、/”
                    switch (flagtmp)
                    {
                    case 0:
                        exercise<<"+";
                        break;
                    case 1:
                        exercise<<"-";
                        break;
                    case 2:
                        exercise<<"*";
                        break;
                    case 3:
                        exercise<<"/";
                        break;
                    }
                }                          
                
                if(IsRepetitive(exercise.str(),g_checkpath))       //如果有重复题目，那么继续while循环（此时待生成题目数量没减少）
                {
                    continue;
                }
                
                current_file<<exercise_number<<"、"<<exercise.str()<<endl<<endl; //生成题目写入文件
                current_checkfile<<exercise.str()<<endl;             //生成题目写入查重文件           

                exercise_number++; //准备生成下一道题，题号自加1
                g_number--; //当前已生成了一道题目
            }
        }

        if(g_type == "初中")
        {
            while(g_number != 0)
            {
                stringstream exercise;  //当前生成的题目
                bool symbol_flag = 0;   //平方、根号使用情况的标志位，如果为0则为未使用过，为1为使用过
                g_opernum = rand()%5+1; //初中题目的操作数个数，1到5个
                while(g_opernum != 0)
                {
                    g_opernum--;
                    int g_oper = rand()%100+1; //操作数取值范围在1到100
                    exercise<<g_oper;       //把这个操作数加入到题目中

                    int flagtmp1 = rand()%2; //随机给当前操作数加或不加平方、根号操作符
                    switch (flagtmp1)
                    {
                    case 0:                 //不加平方或根号
                        break;
                    case 1:                 //加平方或根号
                        symbol_flag = 1;    //标志位置1，表示已经加过平方或根号了
                        exercise<<"^";
                        int flagtmp2 = rand()%2; //平方或根号选一个
                        switch (flagtmp2)
                        {
                        case 0:                 //平方
                            exercise<<rand()%4+2; //平方数取2到5之间
                            break;
                        case 1:
                            exercise<<"(1/"<<rand()%2+2<<")";//根号数取1/2或1/3
                            break;
                        }
                        break;
                    }

                    if(g_opernum == 0)       //如果这是最后一个操作数
                    {
                        if(!symbol_flag)     //如果还没加过平方或根号
                        {
                            symbol_flag = 1;  //标志位置1，表示已经加过平方或根号了
                            exercise<<"^";
                            int flagtmp4 = rand()%2; //平方或根号选一个
                            switch (flagtmp4)
                            {
                            case 0:                 //平方
                                exercise<<rand()%4+2; //平方数取2到5之间
                                break;
                            case 1:
                                exercise<<"(1/"<<rand()%2+2<<")";//根号数取1/2或1/3
                                break;
                            }
                        }

                        break;               //之后不必再加“+、-、*、/”操作符
                    }
                    int flagtmp3 = rand()%4; //操作符有四种情况“+、-、*、/”
                    switch (flagtmp3)
                    {
                    case 0:
                        exercise<<"+";
                        break;
                    case 1:
                        exercise<<"-";
                        break;
                    case 2:
                        exercise<<"*";
                        break;
                    case 3:
                        exercise<<"/";
                        break;
                    }
                }

                if(IsRepetitive(exercise.str(),g_checkpath))       //如果有重复题目，那么继续while循环（此时待生成题目数量没减少）
                {
                    continue;
                }
                
                current_file<<exercise_number<<"、"<<exercise.str()<<endl<<endl; //生成题目写入文件
                current_checkfile<<exercise.str()<<endl;             //生成题目写入查重文件  

                exercise_number++; //准备生成下一道题，题号自加1
                g_number--; //当前已生成了一道题目
            }               
        }

        if(g_type == "高中")
        {
            while(g_number != 0)
            {
                stringstream exercise; //当前生成题目
                bool symbol_flag = 0; //平方、根号使用情况的标志位，如果为0则为未使用过，为1为使用过
                g_opernum = rand()%5+1;

                while(g_opernum != 0)
                {
                    g_opernum--;
                    int g_oper = rand()%100+1; //操作数取值范围在1到100

                    int flagtmp1 = rand()%2;   //随机判断当前操作数是否要加sin或cos或tan
                    switch (flagtmp1)
                    {
                    case 0:                     //不加三角函数
                        break;
                    case 1:
                        int flagtmp2 = rand()%3; //随机生成sin或者cos或者tan
                        symbol_flag = 1;      //标志位置1，表示已经加了三角函数
                        switch (flagtmp2)
                        {
                        case 0:
                            exercise<<"sin";
                            break;
                        case 1:
                            exercise<<"cos";
                            break;
                        case 2:
                            exercise<<"tan";
                            break;
                        }
                    }

                    if(g_opernum == 0)       //如果这是最后一个操作数
                    {
                        if(!symbol_flag)     //如果还没加过平方或根号
                        {
                            int flagtmp3 = rand()%3; //随机生成sin或者cos或者tan
                            symbol_flag = 1;      //标志位置1，表示已经加了三角函数
                            switch (flagtmp3)
                            {
                            case 0:
                                exercise<<"sin";
                                break;
                            case 1:
                               exercise<<"cos";
                                break;
                            case 2:
                                exercise<<"tan";
                                break;
                            }
                        }
                        exercise<<g_oper;      //加入操作数
                        break;               //之后不必再加“+、-、*、/”操作符
                    }

                    exercise<<g_oper;      //加入操作数

                    int flagtmp4 = rand()%4; //操作符有四种情况“+、-、*、/”
                    switch (flagtmp4)
                    {
                    case 0:
                        exercise<<"+";
                        break;
                    case 1:
                        exercise<<"-";
                        break;
                    case 2:
                        exercise<<"*";
                        break;
                    case 3:
                        exercise<<"/";
                        break;
                    }
                }

                if(IsRepetitive(exercise.str(),g_checkpath))       //如果有重复题目，那么继续while循环（此时待生成题目数量没减少）
                {
                    continue;
                }
                
                current_file<<exercise_number<<"、"<<exercise.str()<<endl<<endl; //生成题目写入文件
                current_checkfile<<exercise.str()<<endl;             //生成题目写入查重文件
                            
                exercise_number++; //准备生成下一道题，题号自加1
                g_number--; //当前已生成了一道题目
            }
        }

        current_file.close();
        current_checkfile.close();
    } 

    MainFace();
    cout<<"题目已生成！文件存放在"<<"\""<<g_path<<"\""<<endl<<"可以继续为您生成题目！"<<endl; 
}
    
void AskType()
{
    cout<<"当前试卷类型为："<<g_type<<endl;
    cout<<"如果需要切换类型，请输入“切换为XX”；如果不需要，请输入“否”；如果退出登录请输入“-1”！"<<endl;
    string str;
    cin>>str;
    MainFace();
    if(str != "-1" && str != "否" && str != "切换为小学" && str != "切换为初中" && str != "切换为高中")
    {
        cout<<"请输入正确的语句！"<<endl;
        AskType();
    }
    else if(str == "-1")
    {
        cout<<"您已退出登录，请重新登录！"<<endl;
        g_signflag = 0; //未登录状态
        while(!g_signflag)   //如果登录不成功，就需要一直进行登陆操作
        {
            SignIn();
        }
    }
    else if(str != "否")
    {
        cout<<"请再次确定您想要切换之后的试卷类型！（“小学”或“初中”或“高中”）（如果退出请输入-1）"<<endl;
        cin>>str;
        MainFace();
        if(str == "-1")
        {
            cout<<"您已退出登录，请重新登录！"<<endl;
            g_signflag = 0; //未登录状态
            while(!g_signflag)   //如果登录不成功，就需要一直进行登陆操作
            {
                SignIn();
            }
        }
        while(str != "小学" && str != "初中" && str != "高中")
        {
            cout<<"请输入正确的语句！（“小学”或“初中”或“高中”）（如果退出请输入-1）"<<endl;
            cin>>str;
            MainFace();
        }
        g_type = str; //切换试卷类型
    }
}

void MakeFilePath()   //生成文件路径
{
    g_path = "";                //路径清空
    g_checkpath = "";
    struct tm *current_time;
    time_t lt;
    char str[80];
    lt=time(NULL);
    current_time=localtime(&lt);
    strftime(str,100,"%Y-%m-%d-%H-%M-%S.txt",current_time);
    g_path = TchUser[g_i].m_account + "/" + str;
    g_checkpath = TchUser[g_i].m_account + "/HistoryExercise.txt"; 
}

bool IsRepetitive(string exercise, string filepath)      //如果遇到重复的题目，就返回true
{
    fstream file;
    file.open(filepath.c_str());
    string file_exercise;
    while(getline(file,file_exercise))
    {
        if(file_exercise == exercise)
        {
            file.close();
            return true;                        
        }
    }
    file.close();
    return false;
}

void MainFace()
{
    system("cls");
    cout<<endl;
    cout<<"*****************************************"<<endl;
    cout<<"*                                       *"<<endl;
    cout<<"*              试卷生成系统             *"<<endl;
    cout<<"*                                       *"<<endl;
    cout<<"*****************************************"<<endl; 
    cout<<endl;   
}

int main()
{
    InitUser();
    MainFace();
    cout<<"如果登录请输入用户名和密码（之间用空格隔开）:"<<endl;

    while(!g_signflag)   //如果登录不成功，就需要一直进行登陆操作
    {
        SignIn();
    }
    system("pause");
    return 0;
}
