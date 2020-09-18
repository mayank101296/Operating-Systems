#include<bits/stdc++.h>
using namespace std;

void LRU_pageReplacement(vector<int> requests,int frames){

    //variable declaration
    int curr_alloc = 0,count_pageFault = 0;
    int total_requsest = requests.size();   //total number of requests
    stack<int> allocated;  //array for frames which are allocated


    for(int i=0; i<total_requsest; i++){  //loop for each requests
        //if number of frames allocated is less than total then simply allocate farme to that request
        cout<<"Requestd Page = "<<requests[curr_alloc]<<"\t";
        bool found = false;      //flag for checking that element is present in the stack or not
        if(curr_alloc<frames){
            allocated.push(requests[curr_alloc]);
            curr_alloc++;
            count_pageFault++;
        }else{ 
            //find the request inside the stack
            stack<int> temp_stack;    //creating a temporary stack
            while(!allocated.empty()){          //run loop until stack is empty
                if(allocated.top() == requests[curr_alloc]){    //if stack top contain the element
                    found = true;              //set flag true
                    allocated.pop();            //remove that element form the stack
                    break;
                }else{
                    temp_stack.push(allocated.top());   //push the element into the temporary stack
                    allocated.pop();        //pop the element from the allocated stack
                }
            }
            //now put back all the elements back into the allocated stack
            if(found == false){            //if element is not found in stack then there is page fault so remove least recently used element form the stack
                count_pageFault++;         //increment page fault count
                temp_stack.pop();           
            }
            while(!temp_stack.empty()){          //put all the element form temporary stack to allocatd stack
                allocated.push(temp_stack.top());
                temp_stack.pop();
            }
            allocated.push(requests[curr_alloc]);  //put current requested element back into the stack
            curr_alloc++;   //increment the page request number
        }
        //printing the current status of frame allocation
        vector<int> show;
        while(!allocated.empty()){
            show.push_back(allocated.top());
            allocated.pop();
        }
        cout<<"[ ";
        for(int j=show.size()-1; j>=0; j--){
            allocated.push(show[j]);
            cout<<show[j]<<" ";
        }
        int current = show.size();
        while(current < frames){
            cout<<"_ ";
            current++;
        }
        cout<<"]";
        if(found == false){
            cout<<" *";
        }
        cout<<endl;
    }
    cout<<"Page fault ratio in case of LRU algorithm = "<<(float)count_pageFault/total_requsest<<endl;
}

void OPR_pageReplacement(vector<int> requests,int frames){
    //variable declaration
    int curr_alloc = 0,count_pageFault = 0;
    int total_requsest = requests.size();
    vector<int> allocated;

    for(int i=0; i<total_requsest; i++){  //loop for each requests
        //if number of frames allocated is less than total then simply allocate farme to that request
        cout<<"Requestd Page = "<<requests[curr_alloc]<<"\t";
        bool found = false;
        if(curr_alloc<frames){
            allocated.push_back(requests[curr_alloc]);
            curr_alloc++;
            count_pageFault++;
        }else{ 
            vector<int>::iterator it = find(allocated.begin(),allocated.end(),requests[curr_alloc]); //find the request inside the allocated vector
            if(it!=allocated.end()){ //frame is already present
                curr_alloc++;
                found = true;
            }else{
                //select frame from allocation vector which is least recently used
                vector<int> arr,arr1;
                vector<int>::iterator it1;
                int flag = 0;
                for(int j=0; j<allocated.size(); j++){  //loop for each allocated frames
                    it1 = find(requests.begin()+curr_alloc,requests.end(),allocated[j]);  //find inside future requests
                    if(it1!=requests.end() && flag!=1){  //if have future reference than put its index into the array
                        arr.push_back(it1-requests.begin());
                    }else if(it1==requests.end()){  //if no future reference than insert into another vector arr1
                        arr1.push_back(allocated[j]);
                        flag=1;
                    }
                }
                
                if(flag!=1){  //find the frame whose future reference is most late
                    int max = 0, index = 0;
                    for(int k=0; k<arr.size(); k++){
                        if(max<arr[k]){
                            max = arr[k];
                            index = k;
                        }
                    }
                    //replace and insert new frame
                    it1 = find(allocated.begin(),allocated.end(),requests[index]);
                    if(it1!=allocated.end()){
                        allocated.insert(it1,requests[curr_alloc]);
                        allocated.erase(it1+1);
                    }
                }else{
                    if(arr1.size()==1){  //if only one frame is candidate for replacement
                        it1 = find(allocated.begin(),allocated.end(),arr1[0]);  //find it
                        allocated.insert(it1,requests[curr_alloc]);  //insert new frame
                        allocated.erase(it1+1);  //remove old frame from allocated array
                    }else{  //if more than one frame is candidate
                        //replace the frame based on first come first serve
                        vector<int> temp;
                        for(int l=0; l<arr1.size(); l++){
                            for(int k=curr_alloc-1; k>=0; k--){
                                if(requests[k]==arr1[l]){
                                    temp.push_back(k);
                                    break;
                                }
                            }
                        }
                        int min = temp[0],ind = 0;
                        for(int m=0; m<temp.size(); m++){
                            if(min>temp[m]){
                                min = temp[m];
                                ind = m;
                            }
                        }
                        //now find element at index m into the allocation
                        it1 = find(allocated.begin(),allocated.end(),requests[min]);
                        allocated.insert(it1,requests[curr_alloc]);
                        allocated.erase(it1+1);
                    }
                }
                curr_alloc++;
                count_pageFault++;
            }
        }
        cout<<"[ ";
        for(int i=0; i<allocated.size(); i++){
            cout<<allocated[i]<<" ";
        }
        int current = allocated.size();
        while(current < frames){
            cout<<"_ ";
            current++;
        }
        cout<<"]";
        if(found == false){
            cout<<" *";
        }
        cout<<endl;
    }
    cout<<"Page fault ratio in case of optimal algorithm = "<<(float)count_pageFault/total_requsest<<endl;
}



int main(){
    //reading the input from the file
    ifstream file("pro2_input.txt");
    string str;
    vector<int> pageRequest;
    int i=0, numOfFrames;
    while(getline(file,str)){
        stringstream s(str);
        string word;
        int j=0;
        while(s >> word){
            if(i==0){
                if(j>2){
                    pageRequest.push_back(atoi(word.substr(0,1).c_str()));
                }
                j++;
            }else if(i==1){
                if(j>2){
                    numOfFrames = atoi(word.c_str());
                }
                j++;
            }else{
                cout<<"number of lines should be 2";
            }
        }
        i++;
    }
    //printing the inputs from the file
    // for(int i=0; i<pageRequest.size(); i++){
    //     cout<<pageRequest[i]<<" ";
    // }
    // cout<<"Number of frames="<<numOfFrames;
    cout<<"*************Least Resently Used***************************\n";
    LRU_pageReplacement(pageRequest,numOfFrames);
    cout<<"*************Optimal Page Replacement**********************\n";
    OPR_pageReplacement(pageRequest,numOfFrames);
    return 0;
}