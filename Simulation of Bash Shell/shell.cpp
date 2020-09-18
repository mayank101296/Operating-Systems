#include<bits/stdc++.h>
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<errno.h>
#include<fcntl.h>

#include<unistd.h>
#include<dirent.h>
#include<experimental/filesystem>
#include<readline/readline.h>
#include<readline/history.h>
using namespace std;

bool check_mode(string s)                           //it checks whether the input mode is valid or not
{
    for(auto i=s.rbegin();i!=s.rend();i++)
    {
        if((*i-'0')>7)                              //mode should not be greater than 7
            return true;
    }
    return false;
}

mode_t compute_permission_bit(string x)             //it computes the permission bit from input string
{
    mode_t mbit[9]={S_IRUSR,S_IWUSR,S_IXUSR,
                    S_IRGRP,S_IWGRP,S_IXGRP,
                    S_IROTH,S_IWOTH,S_IXOTH};
    
    mode_t mbitf=0;
    int flag=0,pos=8;
    for(auto i=x.rbegin();i!=x.rend();i++)
    {
        int t=*i-'0';
        for(int j=0;j<3;j++)            //converting decimal to binary to get actual permission bit
        {
            if(t%2==1)
                mbitf=mbitf|mbit[pos];          //performing OR operation on each set permission bit
            t=t/2;
            pos--;
        }

    }
    return mbitf;           //returning permission bit
}

vector<string> space_handling(string w)             //it removes the spaces(if, any) from the input file name
{
    vector<string> res;
    while(w.length()>0)
    {
        string w1;
        int pos2=0;
        if(w[0]=='"')                               //to handle  " " in file name
        {   
                
            pos2=w.find_first_of('"',1);

            if(pos2==string::npos)
            {
                pos2=w.length();
            }
            w1=w.substr(1,pos2-1);
            pos2++;
            
        }
        else if(w[0]=='\'')                        //to handle ' ' in file name
        {
            pos2=w.find_first_of('\'',1);
            if(pos2==string::npos)
            {
                pos2=w.length();
            }
            w1=w.substr(1,pos2-1);
            pos2++;
        }
        else
        {
            pos2=w.find_first_of(' ');
            
            if(w[pos2-1]=='\\')                     //to handle \\ in file name
            {
                pos2=w.find_first_of(' ',pos2+1);

            }
            if(pos2==string::npos)
            {
                pos2=w.length();
            }
            w1=w.substr(0,pos2);
            int post=0;
            while((post=w1.find('\\'))!=string::npos)       //parsing string for command input
            {
                w1.erase(post,1);
            }
        }
         w.erase(0,pos2+1);
        res.push_back(w1);
    }
           
       return res;     
}

void ls_command(vector<string> v)                   //command to list the files and directories of current directory
{
    char path[100];
    getcwd(path,100);                               //returns path of the current directory
    string path1(path);
    if((*(v.begin())=="ls"||*(v.begin())=="")&&v.size()==1)
    {
    
        DIR *d=opendir(path);                       //opens directory
        if(d==NULL)
        {
            cout<<"ls: cannot access "<<path<<" : No such file or directory"<<endl;
            return;
        }
        for(dirent *i;i=readdir(d);)                    //reading the entries in the directory
        {
            if(i->d_name[0]=='.')
                continue;
            cout<<i->d_name<<endl;                  
        }
        closedir(d);                                //closes directory
        return;
    }
    if(*(v.begin())=="-a"&&v.size()==1)             //case for -a option implementation
    {
        DIR *d=opendir(path);
        if(d==NULL)                 //checking directory is valid or not
        {
            cout<<"ls: cannot access "<<path<<" : No such file or directory"<<endl;
            return;
        }
        for(dirent *i;i=readdir(d);)             //reading the entries in the directory
        {
            
            cout<<i->d_name<<endl;
        }
        closedir(d);
        return;
    }
    int flag=0,flag1=0;
    auto i=v.begin();
    if(v.size()>1)
        flag1=1;
    if((*v.begin())[0]=='-')                    // for additional options
    {
        if(*v.begin()=="-a")                //checking case for option is valid or not
        {
            flag=1;
            if(v.size()>2)
                flag1=1;
            i=v.begin()+1;
        }
        else                                        //validity check for illegal option
        {
            cout<<"ls: invalid option: "<<*v.begin()<<endl;
            return;
        }
        
         
    }
    
    for(;i!=v.end();i++)
    {
        
        DIR *d=opendir(i->c_str());             //opening the directory for scaning its content
        if(d==NULL)
        {
            cout<<"ls: cannot access "<<*i<<" : No such file or directory"<<endl;
            continue;
        }
        if(flag1==1)
        {
            cout<<*i<<":"<<endl;
        }
        for(dirent *i;i=readdir(d);)                //reading the entries from directory
        {
            if(i->d_name[0]=='.'&&flag==0)
                continue;
            cout<<i->d_name<<endl;
        }
        closedir(d);                //closing the directory 
        cout<<endl;

    }
    
}


void cd_command(string w)                       //command to change the working directory
{
    char p[100];
    if(w=="..")
    {
        
        getcwd(p,100);                          //getting path of the current directory for path  processing
        string s1(p),check;
       
        int x=s1.find_last_of('/');
        
        check=s1.substr(x+1);
       
        getlogin_r(p,100);                  //this function used to get current login user name
        s1=p;
       
        if(check==s1)
        {
                                        //checking for boundary condition 
        }
        else if(chdir(w.c_str()))                       //changing the current directory to parent one
        {
            cout<<"cd: "<<w<<": No such directory"<<endl;
        }

    }
    else if(w.length()>0&&w!="cd")                          //checking the validity of input directory
    {
        
        if(chdir(w.c_str()))                            //changing the current directory to input sub directory
        {
            cout<<"cd: "<<w<<": No such directory"<<endl;
        }
    }
    else
    {
        
    }
    
}

void cat_command(vector<string> v)                      //simulates cat command              
{
    int flag=0,flag1=0;
    auto i=v.begin();
    if((*v.begin())[0]=='-')                //checking for input option is valid or not
    {
        if(*v.begin()=="-n")
        {
            flag=1;
            if(v.size()>2)
                flag1=1;
            i=v.begin()+1;
        }
        else                                    //invalid option output message
        {
            cout<<"cat: invalid option: "<<*v.begin()<<endl;
            return;
        }
        
         
    }
    int count=1;
    for(;i!=v.end();i++)                    
    {
        int sr1=open(i->c_str(),O_RDONLY|O_WRONLY);         //opening file for checking that file exits or not
        if(sr1==-1)
        {
            string err(strerror(errno));
            cout<<"cat: cannot read: "<<*i<<": "<<err<<endl;        //error message for file not exits
            continue;
        }
        close(sr1);
        ifstream file(*i);                  //opening file in read mode
       

        string str;
        while(getline(file,str))            //reading the data line by line
        {
            if(flag==1)                                 
                cout<<"  "<<count<<" ";             //counting the line number
            cout<<str<<endl;

            count++;
        }
        file.close();           //closing the all opened file
    }
    
}


void cp_command(string w)                   //command to copy files and directories
{
    string src,dest;
    int pos=0,pos1=0,pos2=0,flag=0;

    if(w[0]=='-')
    {
        if(w[1]=='u')                       //checking for additional options
        {
            if(w[2]!=' ')
            {
                cout<<"invalid command : "<<errno<<endl;        //error message for invalid option
                string err(strerror(errno));
              
                return;
            }
            flag=1;
            pos=3;
        }
        else
        {
            cout<<"invalid command :"<<errno<<endl;     //error message for invalid command
            string err(strerror(errno));
            
            return;
        }
        
    }
    pos1=w.find(' ',pos);
    if(pos1==string::npos)                  //checking for operand missing or not
    {
        cout<<"invalid command : missing file operand"<<endl;
        string err(strerror(errno));
        
        return;
    }

    src=w.substr(pos,pos1-pos);             //parsing the source and destination file name for copying
    dest=w.substr(pos1+1);
    
    
    int sr1,sr,des;

    sr1=open(src.c_str(),O_RDONLY|O_WRONLY);            //opening the file to check file exits of not
    sr=open(src.c_str(),O_RDONLY);
    if(sr1==-1)                                         //checking the file name given as input is valid or not
    {
        string err(strerror(errno));
        cout<<"error in opening the "<<src<<" : "<<err<<endl;
        return;   
    }
    string filename;
    pos=src.find_last_of('/');
    if(pos==string::npos)           
    {
        filename=src;

    }
    else
    {
        filename=src.substr(pos);

    }

    //checking destination file path is valid or not by opening it 
    //if not exits than create the file
    des=open(dest.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRGRP|S_IWGRP|S_IRGRP|S_IRUSR|S_IWUSR|S_IROTH|S_IWOTH); 

    //checking for file name already exist so dont create it.    
    if(errno==21)               
    {
        dest=dest+filename;
        des=open(dest.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRGRP|S_IWGRP|S_IRGRP|S_IRUSR|S_IWUSR|S_IROTH|S_IWOTH);
   
    }

    if(flag==1)
    {                       //opening the destination and source file to read data and compair to confirm
                            //that duplicate file is their or not.
        FILE *fp=fopen(dest.c_str(),"r");
        FILE *fp1=fopen(src.c_str(),"r");
        char c,c1;
        if(fp==NULL)
            flag=0;
        else
        {
            while(true)
            {
                if(feof(fp1)||feof(fp))     //checking file is open or some error opening the file
                    break;
                fscanf(fp,"%c",&c);
                fscanf(fp1,"%c",&c1);       //reading the file
                if(c!=c1)
                {
                    flag=0;
                    break;
                }
            }

            fclose(fp);                 //closing the file
            fclose(fp1);
            if(flag==1)
            {
                cout<<"same file exit"<<endl;               //message for same file exot
                return;
            }
        }
        
        
        
    }
    
    if(des==-1)                 //checking for destination file is openable or not
    {
        string err(strerror(errno));
        
        cout<<"error in opening the "<<dest<<" : "<<err<<endl;
        return;   
    }
    int nbyte;
    char inbuf[1000];
    while((nbyte=read(sr,inbuf,1000))>0)            //reading data from source file
    {
        int wbyte=write(des,inbuf,nbyte);
        if(wbyte!=nbyte)                        //writing data in destination file
         {
                cout<<"error in writing file "<<endl;               //if error occur in writing the error message printing 
            return;
         }
    }

    if(nbyte!=0)            //if file reading not complete than showing error
    {  
        cout<<"file reading error"<<endl;
    return;
    }
    close(sr);                  //closing source and destination file
    close(des);
    
    



}


void mkdir_command(string w)                        //command to create directories
{
    char gt1[100];
    if(w!="")
    {   
        
        
        int pos_per=w.find_last_of("-");            //checking for command is with option or without option
        
        mode_t mbit=S_IRWXU | S_IRGRP|S_IXGRP | S_IROTH | S_IXOTH;          //declaring default mode bit
        if(pos_per!=string::npos)   
        {
            string str=w.substr(pos_per+2,w.find(' ',pos_per)-pos_per-2);
            if(str=="")                 //checking any option is missing or not 
            {
                
                cout<<"missing operand" <<endl;
                return;
            }
            
            if(check_mode(str))                 //checking mode is valid or not
            {
                cout<<"mkdir: invalid mode ‘"<<str<<"’"<<endl;
                return;
            }
            mbit=compute_permission_bit(str);       //computing the permission bit form input string 
            
            
        }
        
        int len=w.length(),pos1=0,pos2=0;
        string w1;          
        const char *c;
        while(w.length()>0)                     //parsing the string in loop 
        {
            if(w[0]=='"')
            {   
                    
                pos2=w.find_first_of('"',1);            //checking for "" in file name

                if(pos2==string::npos)
                {
                    pos2=w.length();
                }
                w1=w.substr(1,pos2-1);
                c=w1.c_str();
                string temp(c);
               
                pos2++;
                
            }
            else if(w[0]=='\'')                  //checking for '' in file name
            {
                pos2=w.find_first_of('\'',1);
                if(pos2==string::npos)
                {           
                    pos2=w.length();
                }
                w1=w.substr(1,pos2-1);              //removing the string to get required path
                c=w1.c_str();
                string temp(c);
                
                pos2++;
            }
            else
            {
                pos2=w.find_first_of(' ');              //checking for space is in file name
                
                if(w[pos2-1]=='\\')
                {
                    pos2=w.find_first_of(' ',pos2+1);

                }
                if(pos2==string::npos)
                {
                    pos2=w.length();
                }
                w1=w.substr(0,pos2);                    //extracting required string form input command string
                if(w1.find("\\\\")!=string::npos)
                {
                    w1="";
                }                   
                int post=0;                                             //checking for \\ in file name 
                while((post=w1.find('\\'))!=string::npos)
                {
                    w1.erase(post,1);               //erasing the \\ character
                }

                c=w1.c_str();                   //required path is generated under space in file name
                string temp(c);
                
            }
            w.erase(pos1,pos2+1);               //extracting required string form input command string
            
            if(c[0]=='-')                   
            {
                if(c[1]=='m')               //checking for input option is valid or not.
                    continue;
                else
                {
                    cout<<"mkdir: Invalid option: "<<c<<endl;
                    return;
                }
                
            }
                
            if(mkdir(c,mbit))                       //make a directory with given permission 
            { 
                string err(strerror(errno));
                cout<<"mkdir: cannot create directory ‘"<<c<<"’: "<<err<<endl;
            }
            
            
        }
        
    
        
    }
    else
    {                                   //showing error for argument missing
        cout<<"missing argument"<<endl;
    }
            
}

bool compare_function(string &s1,string &s2)                //utility function to compare 2 input strings
{
    
    
    if(s1.length()<=0||s2.length()<=0)
        return true;
    for(long long int i=0,j=0;i<s1.length()&&j<s2.length();++i,++j)
    {
                                                //checking for each condition for comparing two string
        if((s1[i])==s2[j])
            continue;
        if(((s1[i]>=32&&s1[i]<=47)||(s1[i]>=58&&s1[i]<=64)||(s1[i]>=91&&s1[i]<=96)||(s1[i]>=173&&s1[i]<=177))&&((s2[j]>=32&&s2[j]<=47)||(s2[j]>=58&&s2[j]<=64)||(s2[j]>=91&&s2[j]<=96)||(s2[j]>=173&&s2[j]<=177)))
        return s1[i]>s2[j]?true:false;
        if(((s1[i]>=32&&s1[i]<=47)||(s1[i]>=58&&s1[i]<=64)||(s1[i]>=91&&s1[i]<=96)||(s1[i]>=173&&s1[i]<=177))&&((s2[j]>='0'&&s2[j]<='9')||(s2[j]>='A'&&s2[j]<='Z')||(s2[j]>='a'&&s2[j]<='z')))
            return true;
        if(((s2[j]>=32&&s2[j]<=47)||(s2[j]>=58&&s2[j]<=64)||(s2[j]>=91&&s2[j]<=96)||(s2[j]>=173&&s2[j]<=177))&&((s1[i]>='0'&&s1[i]<='9')||(s1[i]>='A'&&s1[i]<='Z')||(s1[i]>='a'&&s1[i]<='z')))
            return false;
        if(s1[i]>='0'&&s1[i]<='9'&&s2[j]>='0'&&s2[j]<='9')
            return s1[i]>s2[j]?true:false;

        if(((s1[i]>='A'&&s1[i]<='Z')||(s1[i]>='a'&&s1[i]<='z'))&&((s2[j]>='A'&&s2[j]<='Z')||(s2[j]>='a'&&s2[j]<='z')))
        {
            if(s1[i]>='A'&&s1[i]<='Z'&&s2[j]>='A'&&s2[j]<='Z')
                return s1[i]>s2[j]?true:false;
            if(s1[i]>='a'&&s1[i]<='z'&&s2[j]>='a'&&s2[j]<='z')
                return (s1[i])>(s2[j])?true:false;
            
            if(s1[i]>='A'&&s1[i]<='Z'&&s2[j]>='a'&&s2[j]<='z')
                return (s1[i]+32)>s2[j]?true:false;
            if(s1[i]>='a'&&s1[i]<='z'&&s2[j]>='A'&&s2[j]<='Z')
                return (s1[i]-32)>=s2[j]?true:false;
        }

         if(s1[i]>='0'&&s1[i]<='9'&&((s2[j]>='A'&&s2[j]<='Z')||(s2[j]>='a'&&s2[j]<='z')))
             return true;
        if(s2[j]>='0'&&s2[j]<='9'&&((s1[i]>='A'&&s1[i]<='Z')||(s1[i]>='a'&&s1[i]<='z')))
             return false;
      

        
       return true;
    }

    return true;
}


void sort_command(vector<string> &v)                    //replicates sort command
{
    multiset<string> s;
    vector<string> v1;
    int flag=0;
    
    for(auto i=v.begin();i!=v.end();i++)            
    {
        if(*i=="-r")                //checking for option validity
        {    
            flag=1;
            continue;
            
        }
        int sr1=open(i->c_str(),O_RDONLY|O_WRONLY);             //opening the file for checking its existence
        if(sr1==-1)
        {
            string err(strerror(errno));
            cout<<"sort: cannot read: "<<*i<<": "<<err<<endl;           //error reporting
            return;  
        }
        close(sr1);
        ifstream file(*i);          //opening the file under reading mode


        string str="";
        while(getline(file,str))            //getting line from file to compare for sorting
        {
           
            v1.push_back(str);
        }

        

        

    }
    sort(v1.begin(),v1.end());              //sorting the file lines 
    
    if(flag==1)
    {
        for(auto i=v1.rbegin();i!=v1.rend();i++)            //output the sorted lines of file
        {
            cout<<*i<<endl;
        }
    }
    else
    {
       for(auto i=v1.begin();i!=v1.end();i++)
        {
            cout<<*i<<endl;
        } 
    }
}


void grep_command(vector<string> &v)                //command for pattern matching
{
    auto i=v.begin();
    string search=*i;
    i++;
    int flag=0,flag1=0;
    if(search=="-n")                                // for additional options
    {
        flag=1;
        search=*i;
        i++;

        if(v.size()>3)                  //checkingption  for option is valid or not
            flag1=1;
    }
    else
    {
        if(v.size()>2)
            flag1=1;
    }
    
    if(search[0]=='"'||search[0]=='\'')             //checking for "" and '' in search pattern
    {
        search=search.substr(1,search.length()-2);
        
    }
    for(;i!=v.end();i++)
    {
        int count=0;
        int sr1=open(i->c_str(),O_RDONLY|O_WRONLY);         //opening the file for checking its existence
        if(sr1==-1)
        {
            string err(strerror(errno));
            cout<<"grep: cannot read: "<<*i<<": "<<err<<endl;           //reporting the error msg if file not exist
            continue;   
        }
        close(sr1);
        ifstream file(*i);          //opening the file under read mode
        string str;
        while(getline(file,str))        //     reading file line by line for pattern search
        {
            count++;
           if(str.find(search)!=string::npos)               //find pattern in each line
           {
               if(flag1==0)
               {
                   if(flag==1)
                cout<<count<<":"<<str<<endl;            //maintaining a count for option -n
                else
                    cout<<str<<endl;
               }
               else
               {
                   if(flag==1)                      //display the command output
                    cout<<*i<<":"<<count<<":"<<str<<endl;
                    else
                        cout<<*i<<":"<<str<<endl;
               }
               
               
                
           }
        }

    }

}

string print_path()                     //basic function to print path name
{
    char gt[100];
    getlogin_r(gt,100);
    string path2(gt),path3;
    //cout<<path2<<"@";
    path3=path2+"@";                //building the path
    gethostname(gt,100);
    string path1(gt);
    //cout<<path1<<":~";
    path3=path3+path1+":~"; 
    getcwd(gt,100);                     //function used to get current working directory
    string path(gt);
    
    //cout<<path.substr(path.find(path2)+path2.length())<<"$ ";
    path3=path3+path.substr(path.find(path2)+path2.length())+"$";           //build the path in required.
    return path3;
}

vector<string> parse_command(string w)              //basic function to parse the input commands
{
    vector<string> v;
    stringstream st(w);
    string w1;              
    while(getline(st,w1,' '))               //breaking the input command from space         
    {
        v.push_back(w1);
    }
    return v;
}



int main()
{
    
    string s;
    cout << "\033[2J\033[1;1H";
    cout<<"\n\n\n**********Terminal************"<<endl;                 //main terminal
    
    
    char *s1;
   
    while(true)
    {
        
        
        
        s1=readline(print_path().c_str());                  //readline function to simulate the up down key functionality
        if(s1&&*s1)
            add_history(s1);                            //add_history function for maintaing command history
        s=s1;
        free(s1);
        //getline(cin,s);
       
        stringstream st(s);                 
        string w;
        getline(st,w,' ');
        if(w=="exit"||w=="EXIT")                //checking   command type if it is 'exit'command or not
            exit(0);
        else if(w=="ls")                        //check for ls command
          {
             
                getline(st,w);
                
                vector<string> arg;                 //parsing and initial command formate check
                if(w=="")
                    w="ls";

                arg=parse_command(w);

                ls_command(arg);                //user defined function for ls command functionality

          }
        else if(w=="cd")                 //check for cd command
        {
            
            getline(st,w);
            
           cd_command(w);                    //user defined function for cd command functionality
        }  
        else if(w=="cat")                       //check for cat command
        {
            int count =0;
            getline(st,w);
            if(w==""||w=="cat")                  //parsing and initial command formate check
            {
                string input;
                while(true)
                {
                    getline(cin,input);
                    
                    cout<<input<<endl;
                    if(count==20)
                        break;
                }
                continue;
            }

            vector<string> arg=space_handling(w);       //space handling in input file name
            
            cat_command(arg);                //user defined function for cat command functionality
        }
        else if(w=="mkdir")                 //check for mkdir command
        {  
           
            getline(st,w);
            if(w==""||w=="mkdir")                //parsing and initial command formate check
            {
                cout<<"invalid command : missing arguments"<<endl;
                continue;
            }
            mkdir_command(w);           //user defined function for cp command functionality
        }
        else if(w=="cp")                //check for mkdir command
          {  
             getline(st,w);
            if(w==""||w=="cp")               //parsing and initial command formate check
            {
                cout<<"invalid command : missing arguments"<<endl;
                continue;
            }
            cp_command(w);              //user defined function for cp command functionality
          }
        else if(w=="sort")              //check for sort command
          {  
              
              getline(st,w);
            if(w==""||w=="sort")               //parsing and initial command formate check
            {
                cout<<"invalid command : missing file operand"<<endl;
                continue;
            }
            vector<string> args=parse_command(w);

            sort_command(args);         //user defined function for sort command functionality
          }
        else if(w=="grep")                  //check for grep command
         {   

            
            getline(st,w);
            if(w==""||w=="grep")             //parsing and initial command formate check
            {
                cout<<"invalid command : missing arguments"<<endl;
                continue;
            }
            vector<string> args=parse_command(w);           //parsing the command into strings

            if((*args.begin())[0]=='-')
            {
                if(*args.begin()!="-n")                 //checking for validity of option
                {
                    cout<<"invalid option :"<<*args.begin()<<endl;
                    continue;
                }

                if(args.size()<3)                   //checking for missiong arguments
                {
                    cout<<"invalid command :missing operand "<<endl;
                    continue;
                }
            }
            else
            {
                if(args.size()<2)                   //checking for missing operand in without option case
                {
                    cout<<"invalid command :missing operand "<<endl;
                    continue;
                }
            }
            
            grep_command(args);                     //user defined function for grep command functionality

         }
        else
             cout<<"not a command"<<endl;           //if any mismatch in command
       
        


    
    
    }



return 0;

}