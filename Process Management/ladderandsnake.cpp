#include<bits/stdc++.h>
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<fcntl.h>

#include<unistd.h>

using namespace std;
int *status,status_child=0,parent_status=1,parent_id=getpid(),flag=0;

void signalhandler(int sig)         //signal handler fuction 
{
    if(sig==SIGCONT)
    {
        if(getpid()==parent_id)
        {
            parent_status=1;
        }
        else
        {
            status_child=1;
        }
        
       
    }
    else 
    {
        
    }
   if(sig==0)
    {
        //parent_status=2;
    }
     if(sig==12)
    {
        status_child=1;
    }

    if(sig==10)
    {
        status_child=100;
    }
    
    
}

int main(int n,char **args)
{

    if(n<4)                             //command line argument checking
    {
          cout<<"missing command line arguments"<<endl;
          exit(1);
    }  
    int players=0,ngrid=0;

    string s1(args[1]),s2(args[2]),file(args[3]);
    try
    {
        ngrid=stoi(s1);         //checking argument is valid or not using try & catch 
        players=stoi(s2);
    }
    catch(invalid_argument x)
    {
        cout<<"invalid argument :not an integer "<<x.what()<<endl;
        exit(0);
    }

    int sr1=open(file.c_str(),O_RDONLY|O_WRONLY);       //opening file 
    if(sr1==-1)                                           //checking wheather file is valid or not
    {               
        string err(strerror(errno));
        cout<<"error: cannot read: "<<file<<": "<<err<<endl;
        exit(0);
    }
    close(sr1);

    FILE *fp=fopen(file.c_str(),"r");           //opening file to read data
    if(fp==NULL)
    {
        cout<<"error in opening the file"<<endl;
        exit(1);
    }
    char c,d;
    int st=0,end=0;
    string w;
    unordered_map<int,pair<int,char>> m;
    while(true)                         //reading content of the file 
    {
        if(feof(fp))
            break;
        
        
        fscanf(fp,"%c",&c);
        fscanf(fp,"%d",&st);
        fscanf(fp,"%d",&end);
        fscanf(fp,"%c",&d);
        if(c!='L'&&c!='S'&&c!='l'&&c!='s')
        {
            cout<<"invalid file content :"<<c<<" "<<st<<" "<<end<<"\n no L/S argument speacified\n";
            exit(EXIT_FAILURE);
        }
        if(c=='L'&&st>end)
        {
            cout<<"invalid file content :"<<c<<" "<<st<<" "<<end<<"\n In case of ladder START should be less than END\n";
            exit(EXIT_FAILURE);
        }
        if(c=='S'&&st<end)
        {
            cout<<"invalid file content :"<<c<<" "<<st<<" "<<end<<"\n In case of snake START should be greater than END\n";
            exit(EXIT_FAILURE);
        }
        m.insert(make_pair(st,make_pair(end,c)));
       
    }

    int p0[players][2],p1[2],pid[n],nbytes;
    char inbuf[1000];
    int i=0;
    /*for(int i=0;i<players;i++)
    {*/
        if(pipe(p1)<0)//read-p0[0]  write -p1[1]    //pipes created
        {
            cout<<"problem in creating pipe"<<endl;
            exit(EXIT_FAILURE);
        }
    //}
    
    signal(12,signalhandler);           //registering signal to its handler function
    signal(10,signalhandler);
    
    status=(int*)calloc(players,sizeof(int));
    int ar[2]={0};
    for(int i=0;i<players;i++)
    {
        if((pid[i]=fork())==0)              //child process created for each player
        {
             srand((unsigned)(time(0)+getpid()));           //random function seed
            while(true)
            {
               
                if(status_child==1)
                {
                    
                    string st;
                    ar[0]=((rand()+time(0))%6)+1;           //generating random value
                   
                    /*close(p0[i][0]);
                    int x=write(p0[i][1],ar,1);*/
                    close(p1[0]);
                    int x=write(p1[1],ar,1);                         //writing in pipes
                    cout<<"process: "<<i+1<<" write: "<<ar[0]<<" pid: "<<getpid()<<" write status: "<<x<<endl;
                   
                    status_child=0;                                   //updating process status.
                    //kill(parent_id,12);
                    
                }
                else if(status_child==0)
                {
                    //cout<<"child process is waiting "<<i<<endl;
                    //swait(NULL);
                }
                else if(status_child==100)
                {
                    
                    break;
                }
                

                
            }
            cout<<"player "<<i+1<<" leave the game"<<endl;
            close(p1[1]);                                       //closing write hand of the pipe
            exit(0);                                            //players leaving the game
        }
    }
    srand((unsigned)(time(0)+getpid()));
    int itr=rand()%players;                                     //selecting 1st player randomly
    int pos[players]={0};   
    string st1;
    
    while(true)
    {
        int ar2[2]={0};
        char *inbuf1;
        if(parent_status==1||parent_status==2)                  //checking status of parent process
        {
            close(p1[1]);
           // close(p0[itr][1]);
            kill(pid[itr],12);                                    //signaling to player for generate random no. and write in pipe
            cout<<"player "<<itr+1<<" throw dice "<<endl;
            sleep(1);                                               //waiting for child to complete its writing
            while((nbytes=read(p1[0],ar2,2))<=0);                       //reading data from pipe
           
            cout<<"process: "<<itr+1<<" read: "<<ar2[0]<<" #bytes read: "<<nbytes<<endl;
            //ar2[0]=(ar2[0]%6)+1;
           
            if(ar2[0]<=0)
            {
                ar2[0]=1;
            }
            int x=ar2[0];
            if(nbytes!=0)
                pos[itr]+=x;                    //incrementing the position of player according to no. generated
            auto it=m.find(pos[itr]);
            if(it!=m.end())
            {
                if(it->second.second=='L')                      //checking for player to get any ladder 
                {
                    cout<<"player "<<itr+1<<" got ladder"<<endl;
                    pos[itr]=it->second.first;

                    
                }
                else if(it->second.second=='S')                     //checking for player to get any snake
                {
                    cout<<"player "<<itr+1<<" bitten by snake"<<endl;
                    pos[itr]=it->second.first;

                    if(pos[itr]<0)
                    {
                        pos[itr]=0;
                    }
                }
            }
            if(pos[itr]>ngrid)
                    pos[itr]=ngrid;
            
            for(int i=0;i<players;i++)                                      //printing the current position of all players
            {
                cout<<"player "<<i+1<<" is at position : "<<pos[i]<<" pid= "<<pid[i]<<endl;
            } 
            if(pos[itr]>=ngrid)                                                 //checking for winning of player on its current position
            {
                cout<<"player "<<itr+1<<" win the game"<<endl;
                cout<<endl<<"--------------------"<<endl;
                break;
            } 
            
            cout<<"---------\n\n";  
            if(x!=6)
                itr=(itr+1)%players;                    //increamenting the count for next player to throw dice
        }
        else
        {
            //cout<<"main process waiting :"<<endl;
            
            
        }

       
        
        
        
        
    }
    flag=1;
    for(int i=0;i<players;i++)
    {
        
        kill(pid[i],10);            //signal to child process form terminate itself as the game is over
        sleep(1);
       
    }
    
    cout<<"game over"<<endl;


    return 0;           //end
}