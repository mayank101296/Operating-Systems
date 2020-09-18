#include<bits/stdc++.h>
#include<iostream>
#include<vector>
#include<fstream>
using namespace std;

//function for finding average of array elements
float findAverages(vector<int> arr){
    float sum = 0;
    for(int i=0; i<arr.size(); i++){
        sum += arr[i];
    }
    sum = sum / arr.size();
return sum;
}

//find waiting time for each process
float findWaitingTime(vector<int> AT,vector<int> BT, vector<int> CT){
    int len = AT.size();
    vector<int> waitTime;
    for(int i=0; i<len; i++){
        waitTime.push_back(CT[i]-AT[i]-BT[i]);   //waiting time = completion time - arrival time - brust time
    }
    cout<<"\nAverage waiting time = "<<findAverages(waitTime)<<endl;
    return findAverages(waitTime); //find average of all waiting time
}

//find turn around time for each process
float findTurnaroundTime(vector<int> AT,vector<int> BT, vector<int> CT){
    int len = AT.size();
    vector<int> turnAroundTime;
    for(int i=0; i<len; i++){
        turnAroundTime.push_back(CT[i]-AT[i]); //trunaround time = completion time - arrival time
    }
    cout<<"Average turn around time = "<<findAverages(turnAroundTime)<<endl; 
return findAverages(turnAroundTime); //find average of all turnaround time
}

//Draw gantt chart for particular type of scheduling
void drawGantChart(vector<int> AT,vector<int> BT, vector<int> CT, vector<int> sequence){
    int len = sequence.size();
    for(int i=0; i<len; i++){
        if(i==0){
            cout<<"| P"<<sequence[i]<<" |";    
        }else{
            if(sequence[i]!=sequence[i-1])
                cout<<"P"<<sequence[i]<<" |";
        }
    }
    cout<<endl;
}

//Sortest Job first Scheduling strategy(No preemption allowed)
vector<float> SJF_scheduling(vector<int> id,vector<int> AT,vector<int> BT){
    //variable declaration
    int numComplete = 0, total = id.size(),start = 0,done[total] = {0},currentTime = 0;
    int completeTime[id.size()];
    vector<int> sequence;

    //loop until all processes completed
    while(numComplete<total){
        //find process whose arrival time is less/equal than current time and select one having minimum brust
        int count = 0;
        vector<int> candidate;

        //find all the candidates for run
        for(int i=0; i<total; i++){
            if(done[i]==0 && currentTime>=AT[i]){
                candidate.push_back(i);
            }
        }
        if(candidate.size()!=0){  //if no candidate arrive at that moment 
            //select one whose BT is minimum
            int x = BT[candidate[0]],c=0,index=candidate[0];
            for(int j=1; j<candidate.size(); j++){
                if(x>BT[candidate[j]]){
                    index = candidate[j];
                    x = BT[candidate[j]];
                }
            }
            currentTime += BT[index];  //updating current time
            done[index] = 1; //set done array for index
            numComplete++;  //increment number of completed processes
            completeTime[index] = currentTime;  //store completion time
            sequence.push_back(index+1);
            cout<<"P"<<index+1<<" completed at time= "<<currentTime<<endl;
        }else{
            //select one whose arrival time is minimum
            int min = pow(2,31)-1,index=0;
            vector<int> v1;
            for(int j=0; j<total; j++){
                if(done[j]==0){
                    if(min>AT[j]){
                        min = AT[j];
                        index = j;
                        v1.empty();
                        v1.push_back(j);
                    }else if(min==AT[j]){
                        v1.push_back(j);
                    }
                }
            }
            if(v1.size()==1){  //only one candidate whose value is minimum
                currentTime += BT[v1[0]];  //updating current time
                done[v1[0]] = 1;
                numComplete++;
                sequence.push_back(v1[0]+1);
                completeTime[v1[0]] = currentTime;
                cout<<"P"<<v1[0]+1<<" completed at time= "<<currentTime<<endl;
            }else if(v1.size()>1){ //if more than one candidate 
                int x = BT[v1[0]],index1=v1[0];
                for(int j=1; j<v1.size(); j++){
                    if(x>BT[v1[j]]){
                        index1 = v1[j];
                        x = BT[v1[j]];
                    }
                }
                currentTime += BT[index1];  //updating current time
                done[index1] = 1;
                numComplete++;
                sequence.push_back(index1+1);
                completeTime[index1] = currentTime;
                cout<<"P"<<index1+1<<" completed at time= "<<currentTime<<endl;
            }
        }
    }
    vector<int> CT;
    vector<float> averages;
    for(int i=0; i<id.size(); i++){
        CT.push_back(completeTime[i]);
    }
    averages.push_back(findWaitingTime(AT,BT,CT));
    averages.push_back(findTurnaroundTime(AT,BT,CT));
    drawGantChart(AT,BT,CT,sequence);
return averages;
}

vector<float> SRTF_scheduling(vector<int> id,vector<int> AT,vector<int> BTime){
    vector<int> BT = BTime; //because brust time changes in each iteration
    int currentTime = 0,total = id.size(),done[total] = {0},numComplete=0,completeTime[total];
    vector<int> sequence;

    while(numComplete<total){
        //find process whose arrival time is less/equal than current time and select one having minimum brust
        int count = 0;
        vector<int> candidate;
        for(int i=0; i<total; i++){
            if(done[i]==0 && currentTime>=AT[i]){
                candidate.push_back(i);
            }
        }
        if(candidate.size()!=0){
            //select one whose BT is minimum
            int x = BT[candidate[0]],c=0,index=candidate[0];
            for(int j=1; j<candidate.size(); j++){
                if(x>BT[candidate[j]]){
                    index = candidate[j];
                    x = BT[candidate[j]];
                }
            }
            currentTime++;  //updating current time
            sequence.push_back(index+1);
            BT[index]--;
            if(BT[index]==0){
                done[index] = 1;
                numComplete++;
                completeTime[index] = currentTime;
                sequence.push_back(index+1);
                cout<<"P"<<index+1<<" completed at time= "<<currentTime<<endl;
            }
        }else{
            //select one whose arrival time is minimum
            int min = pow(2,31)-1,index=0;
            vector<int> v1;
            for(int j=0; j<total; j++){
                if(done[j]==0){
                    if(min>AT[j]){
                        min = AT[j];
                        index = j;
                        v1.empty();
                        v1.push_back(j);
                    }else if(min==AT[j]){
                        v1.push_back(j);
                    }
                }
            }
            if(v1.size()==1){  //only one candidate whose value is minimum
                currentTime++;  //updating current time
                BT[v1[0]]--;
                sequence.push_back(v1[0]);
                if(BT[v1[0]]==0){
                    done[v1[0]] = 1;
                    numComplete++;
                    sequence.push_back(v1[0]+1);
                    completeTime[v1[0]] = currentTime;
                    cout<<v1[0]+1<<"Process completed at time= "<<currentTime<<endl;
                }
            }else if(v1.size()>1){ //if more than one candidate 
                int x = BT[v1[0]],index1=v1[0];
                for(int j=1; j<v1.size(); j++){
                    if(x>BT[v1[j]]){
                        index1 = v1[j];
                        x = BT[v1[j]];
                    }
                }
                currentTime++;  //updating current time
                BT[index1]--;
                sequence.push_back(index1+1);
                if(BT[index1]==0){
                    done[index1] = 1;
                    numComplete++;
                    sequence.push_back(index1+1);
                    completeTime[index1] = currentTime;
                    cout<<"P"<<index1+1<<" completed at time= "<<currentTime<<endl;
                }
            }
        }
    }
    vector<int> CT;
    vector<float> averages;
    for(int i=0; i<id.size(); i++){
        CT.push_back(completeTime[i]);
    }

    averages.push_back(findWaitingTime(AT,BTime,CT));
    averages.push_back(findTurnaroundTime(AT,BTime,CT));
    drawGantChart(AT,BT,CT,sequence);
return averages;
}

int main(){
    vector<int> id,AT,BT;
    string str;
    int i=0;
    //taking input from file and storing in vector
    ifstream file("pro1_input1.txt");
    while(getline(file,str)){
        if(i!=0){
            stringstream s(str); 
            string word;
            int j=0;  
            while (s >> word){
                if(j==0){ //process id
                    id.push_back(atoi(word.substr(1).c_str()));
                }else if(j==1){ //brust time
                    BT.push_back(atoi(word.c_str()));
                }else{ //arrival time
                    AT.push_back(atoi(word.c_str()));
                }
                j++;
            } 
        }
        i++;
    }
    file.close();
    // for(int i=0; i<id.size(); i++){
    //     std::cout<<id[i]<<" "<<AT[i]<<" "<<BT[i]<<endl;
    // }
    vector<float> s1,s2;
    cout<<"\t*********************SJF************************************"<<endl;
    s1 = SJF_scheduling(id,AT,BT);
    cout<<"\n\t*********************SRTF************************************"<<endl;
    s2 = SRTF_scheduling(id,AT,BT);
    cout<<"\n**************Improvement of performance of preemptive over non-preemptive SJF scheduling****************\n";
    cout<<"\nIn terms of average waiting time = "<<((s1[0]-s2[0])/s1[0])*100<<"%"<<endl;
    cout<<"In terms of turnaround time = "<<((s1[1]-s2[1])/s2[1])*100<<"%"<<endl;
    return 0;
}