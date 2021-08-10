#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include "keccak.h"
#include "utilities.h"
#include <vector>
#include <bitset>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <ctime>

using namespace std;

typedef std::vector<std::vector<std::vector<bool>>> state;

std::vector<bool> random(int N){
    std::vector<bool> c(N*8,true);
    for (int i=0;i<c.size();i++){
        srand((unsigned)time(NULL));
        int k=rand();
        c[i]=k%2;
        std::cout<<"rando"<<k<<std::endl;
    }
    for(auto i:c)
       std::cout<<i;
}

void printp(std::vector<char> a){
    for(auto i:a)
        std::cout<<i;
    std::cout<<"\n";
}

void printb(std::vector<bool> a){
    for(auto i:a)
        std::cout<<i;
    std::cout<<"\n";
}

int getindex(std::vector<bool> a, std::vector<std::vector<bool>> list){
    auto index=find(list.begin(),list.end(),a);
    if (index!=list.end()){
        int i=index-list.begin();
        return i;
    }
    else{return -1;}
}



std::vector<char> conj(std::vector<char> a,std::vector<bool> put){
    int blan=0;
    for(int i=0;i<2*a.size();i++){
        if (a[i]==' '){
            //printf("blank at : %d",i);
            blan++;
            if (blan>0 && put[blan-1]==true){
                std::vector<char>::iterator it;
                it =a.begin();
                a.insert(it+i,' ');
                i++;
            }
        }
    }
    if (blan<put.size())
       printf("the space number should not be smaller than binary size of hash results\n");
    std::cout<<std::endl;
    printf("blank number is %d",blan);
    std::cout<<std::endl;
    /*
    for(auto i:a)
        std::cout<<i;
        */
    return a;
}

std::vector<bool> gconj(std::vector<char> a,std::vector<bool> put){
    auto b=conj(a,put);
    std::vector<bool> c(8*(b.size()-1));
    for(int i=0;i<c.size();i++){
        char d1=b[i/8];
        d1<<=(i%8);
        d1>>=7;
        c[i]=d1;
    }
    //for(auto i:c){
    //    std::cout<<i;
    //}
    return c;
}

std::vector<bool> fconj(std::vector<bool> a,int b,std::vector<char> c){
    auto d=gconj(c,a);
    auto e=shake128(d,b*8);
    
    return e;
}

int get_file_size_in_byte(const std::string file_name) {
  int size_f = 0;
  std::ifstream inf(file_name);

  inf.seekg(0, std::ios::end);
  int file_size = inf.tellg();
  inf.seekg(0, std::ios::beg);
  std::cout << "file size = " << file_size << std::endl;
  return file_size+1 ;
}

int main(int argc, char *argv[]){
    srand((unsigned)time(NULL));
    //c[i]=rand()%2;
    std::cout<<"rando"<<rand()<<std::endl;
    srand((unsigned)time(NULL));
    //c[i]=rand()%2;
    std::cout<<"rando"<<rand()<<std::endl;
    std::string file_name=(std::string)argv[1];
    int N=atoi(argv[2]);
    int M=atoi(argv[3]);
    std::ifstream in(file_name);
    std::vector<bool>::size_type length=get_file_size_in_byte(file_name);
    //std::cout<<"length"<<(int)length<<std::end;
    char a[length];
    std::vector<char> b(length);
    printf("avfdfavs\n");
    random(1);
    //in.getline(b,20);
    printf("avfdfavs\n");
    in.getline(a,length);
    for(int i=0;i<length;i++){
        b[i]=a[i];
        std::cout<<b[i];
    }
    //std::cout<<"a:"<<a[5]<<std::endl;

    /*std::vector<bool> c(5,true);
    printf("gconj");
    auto b1=gconj(b,c);*/

    //polard method :now we iterate f to find collisions;
    /*
    int s=1;
    std::vector<bool> init(N*8,false);
    auto t=fconj(init,N,b);
    auto h=fconj(t,N,b);
    while(t!=h){
        s++;
        t=fconj(t,N,b);
        auto h1=fconj(h,N,b);
        h=fconj(h1,N,b);
    }
    
    auto t1=t;
    auto t2=init;
    auto t01=fconj(t1,N,b);
    auto t02=fconj(t2,N,b);
    while(t01!=t02){
        t1=t01;
        t2=t02;
        t01=fconj(t1,N,b);
        t02=fconj(t2,N,b);
    }
    auto t11=conj(b,t1);
    auto t12=conj(b,t2);
    std::cout<<"first message"<<std::endl;
    for(auto i:t11)
        std::cout<<i;
    printf("hash first message with %d bytes\n",N);
    for(auto i:t01)
        std::cout<<i;
    std::cout<<"second message"<<std::endl;
    for(auto i:t12)
        std::cout<<i;
    printf("hash second message with %d bytes\n",N);
    for(auto i:t02)
        std::cout<<i;
    */

    //test2
    std::vector<bool> test1(2,true);
    test1[0]=false;
    std::vector<std::vector<bool>> test2(2,std::vector<bool>(2,false));
    std::cout<<"index"<<getindex(test1,test2);
    
    //brutal parallel collision
    //std::vector<bool> coll1;
    //std::vector<bool> coll2;
    //std::vector<std::vector<bool>> iterdata(M, std::vector<bool>(N,false));
    state colldata(1, std::vector<std::vector<bool>>(M, std::vector<bool>(N*8, false)));
    // here randomlize the init vector
    for(int i=0;i<N*8;i++){
        colldata[0][1][i]=true;
    }
    for(int i=0;i<N*8;i++){
        if(i==1)
            colldata[0][2][i]=true;
    }
    for(int i=0;i<N*8;i++){
        if(i==2)
            colldata[0][3][i]=true;
    }
    for(int i=0;i<N*8;i++){
        if(i==3)
            colldata[0][4][i]=true;
    }
    for(int i=0;i<N*8;i++){
        if(i==4)
            colldata[0][5][i]=true;
    }
    for(int i=0;i<N*8;i++){
        if(i==5)
            colldata[0][6][i]=true;
    }
    for(int i=0;i<N*8;i++){
        if(i==6)
            colldata[0][7][i]=true;
    }
    for(int i=0;i<N*8;i++){
        if(i==7)
            colldata[0][8][i]=true;
    }


    
    //iteration for M thread
    for(int i=0;;i++){
        auto iterate1=colldata[i];
        for(int j=0;j<M;j++){
            cout<<"before"<<endl;
            printb(iterate1[j]);
            iterate1[j]=fconj(iterate1[j],N,b);
            cout<<"after"<<endl;
            printb(iterate1[j]);
            for(int k=1;k<colldata.size();k++){
                int kdata=getindex(iterate1[j],colldata[k]);
                printf("we find %d\n",kdata);
                if (kdata!=-1){
                    auto coll1=colldata[k-1][kdata];
                    auto coll2=colldata[i][j];
                    auto coll11=conj(b,coll1);
                    auto coll12=conj(b,coll2);
                    printp(coll11);
                    printp(coll12);
                    printf("1we end at %d\n",i);
                    return 0;
                }
                else{printf("nothing");}
            }
        }
        for(int j=0;j<M;j++){
            int kdata=getindex(iterate1[j],iterate1);
            if (kdata!=-1 && kdata!=j){
                auto coll1=colldata[i][kdata];
                auto coll2=colldata[i][j];
                auto coll11=conj(b,coll1);
                auto coll12=conj(b,coll2);
                printp(coll11);
                printp(coll12);
                printf("1we end at %d\n",i);
                return 0;
            }
        }
        colldata.push_back(iterate1);
        printf("\nno collsion in %d iteration\n",i);
    }
    
    return 0;
}