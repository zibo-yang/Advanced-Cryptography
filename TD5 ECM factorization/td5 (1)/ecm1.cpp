#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <bitset>
#include <algorithm>
#include <sstream>
#include <gmpxx.h>
#include <bits/stdc++.h> 

using namespace std;

class coordinate {
    public:
      mpz_class x;
      mpz_class z;
      void dataset(mpz_class x1,mpz_class z1){
          x=x1;
          z=z1;
      }
      void dataprint(){
          cout<<"\ndata is x:"<<x.get_str()<<" z:"<<z.get_str()<<endl;
      }
};

void reve(string &a){
    int l=a.length()/2;
    for(int i=0;i<l;++i){
        char b=a[2*i+1];
        a[2*i+1]=a[2*i];
        a[2*i]=b;
    }
}

mpz_class div(mpz_class n,mpz_class d,mpz_class N){
    mpz_class factor=-1;
    for(mpz_class i=0;i<N;++i){
        if ((d*i-n)%N == 0){
            factor=i;
            break;
        }
    }
    cout<<"n:"<<n.get_str()<<endl;
    cout<<"d:"<<d.get_str()<<endl;
    cout<<"N:"<<N.get_str()<<endl;
    if (factor!=-1){
        cout<<"\nsuccess factor is "<<factor.get_str()<<endl;
    }
    else{
        cout<<"\nno factor found"<<endl;
    }
    return factor;
}

coordinate XADD(coordinate& p,coordinate& q,coordinate& minus,mpz_class modulo){
    //mpz_class modulo=(((mpz_class)1)<<255)-19;
    //mpz_class modulo=1009;
    mpz_class U=((p.x-p.z)*(q.x+q.z))%modulo;
    mpz_class V=((p.x+p.z)*(q.x-q.z))%modulo;
    mpz_class xplus=(minus.z*(U+V)*(U+V))%modulo;
    mpz_class zplus=(minus.x*(U-V)*(U-V))%modulo;
    coordinate plus;
    plus.dataset((xplus+modulo)%modulo,(zplus+modulo)%modulo);
    cout<<"xadd:";
    plus.dataprint();
    return plus;
}
coordinate XDBL(coordinate unit1,mpz_class A,mpz_class modulo){
    //mpz_class modulo=(((mpz_class)1)<<255)-19;
    //mpz_class modulo=1009;    
    mpz_class Q=((unit1.x+unit1.z)*(unit1.x+unit1.z))%modulo;
    mpz_class R=((unit1.x-unit1.z)*(unit1.x-unit1.z))%modulo;
    mpz_class S=(Q-R)%modulo;
    mpz_class xdouble=(Q*R)%modulo;
    mpz_class A1=A+2;
    mpz_class A2=4;
    cout<<"A1"<<A1.get_str()<<endl;

    //A1=div(A1,A2,modulo);
    //cout<<"A1"<<A1.get_str()<<endl;

    mpz_invert(A2.get_mpz_t(),A2.get_mpz_t(),modulo.get_mpz_t());
    A1 = (A1 * A2) % modulo;
    cout<<"Aprocess"<<A1.get_str()<<endl;
    mpz_class zdouble= (S*(R + A1 * S)) % modulo;
    coordinate double1;
    double1.dataset((xdouble+modulo)%modulo,(zdouble+modulo)%modulo);
    cout<<"xdbl:";
    double1.dataprint();
    return double1;
}

coordinate special_add_swap(coordinate &x0,coordinate &x1,int b){
    coordinate x2;
    x2.dataset( (1-b)*x0.x +b*x1.x, (1-b)*x0.z +b*x1.z);
    return x2;
}

void swap(coordinate& output1,coordinate& output2,coordinate input1,coordinate input2,int bits){
    if(bits>1)
      printf("\nplease choose your bits in {0,1} again\n");
    mpz_class bits1=(mpz_class)bits;
    output1=special_add_swap(input1,input2,bits);
    output2=special_add_swap(input1,input2,1-bits);
    if(output1.x==input1.x && output1.z==input1.z){
        printf("\nchange\n");
    }
    else{printf("\nnot change\n");}
}



mpz_class process(mpz_class aa,mpz_class modulo){
    //mpz_class modulo=(((mpz_class)1)<<255)-19;
    //mpz_class modulo=1009;
    mpz_class a= aa;
    string c=a.get_str(2);
    cout<<"string"<<c<<endl;
    int l=(c.length()-1)/8;
    if (l!=31)
      printf("\nplease take care of the binary size of scalar\n");
    vector<mpz_class> b(l+1,1);
    for(int i=0;i<l+1;i++){
        b[i]=(a>>(8*i)) & 255;
    }
    //cout<<"\nshow1\n";
    //cout<<b[0]<<endl;
    for(auto j:b)
      cout<<j<<endl;
    
    //cout<<"\nshow2\n";
    //cout<<b[0].get_str()<<endl;
    b[0] &= 248;
    //cout<<b[0].get_str()<<endl;
    //cout<<"\nshow3\n";
    b[31] &= 127;
    //cout<<"\nshow4\n";
    b[31] |= 64;
    //cout<<"\nshow5\n";
    
    mpz_class sum=0;
    for(int i=0;i<l+1;++i){
        sum += b[i]<<(8*i);
    }
    return sum;
}

//now we write The Montgomery ladder 
coordinate Montgomery(mpz_class &scalar, coordinate &co,mpz_class &A,mpz_class modulo){
    //scalar=process(scalar,modulo);
    //mpz_class coor=div(co.x,co.z,modulo);
    //mpz_class x_1=coor;
    coordinate coor2;
    coordinate coor3;
    coor2.dataset(1,0);
    coor3=co;
    //coor3.dataset(coor,1);
    coordinate u=coor3;
    printf("possible error1");
    mpz_class swa=0;
    string c1=scalar.get_str(2);
    int bits=c1.length();
    //mpz_class modulo=(((mpz_class)1)<<255)-19;
    //mpz_class modulo=1009;
    mpz_class exp=modulo-2;
    mpz_class foo=0;
    
    //begin processing
    for(int t=0;t<bits;++t){
        mpz_class m_t=(scalar>>(bits-1-t)) & 1;
        cout<<"\nbits:"<<bits<<"\nm_t:"<<m_t<<endl;
        int m_t1=m_t.get_ui();
        if (m_t1==0){
            auto coor4=XDBL(coor2,A,modulo);
            auto coor5=XADD(coor2,coor3,u,modulo);
            coor2=coor4;
            coor3=coor5;
        }
        else {
            auto coor5=XDBL(coor3,A,modulo);
            auto coor4=XADD(coor2,coor3,u,modulo);
            coor2=coor4;
            coor3=coor5;
        }
        //swap(coor2,coor3,XDBL(special_add_swap(coor2,coor3,m_t1),A,modulo),XADD(coor2,coor3,u,modulo),m_t1);
        printf("\nintermediate data\n");
        coor2.dataprint();
        coor3.dataprint();
    }
    mpz_powm(foo.get_mpz_t(),coor2.z.get_mpz_t(),exp.get_mpz_t(),modulo.get_mpz_t());
    foo = (foo*coor2.x)%modulo;
    cout<<"foo: "<<foo.get_str()<<endl;
    string result=foo.get_str(16);
    reve(result);
    printf("vadfvd");
    reverse(result.begin(),result.end());
    cout<<result<<endl;
    return coor2;
}


string special_read(std::ifstream& fin, int length ){
    char* read1=new char [length];
    //for(int i=0;i<length;++i){
    //    fin.read(read1,1);
    //}
    fin.read(read1,length);
    string read2(read1,length);
    return read2;
}

int get_file_size_in_byte(const std::string file_name) {
  int size_f = 0;
  std::ifstream inf(file_name);
  inf.seekg(0, std::ios::end);
  int file_size = inf.tellg();
  inf.seekg(0, std::ios::beg);
  std::cout << "file size = " << file_size << std::endl;
  return file_size;
}


//read all prime smaller than B
vector<mpz_class> primgen(mpz_class B){
    vector<mpz_class> list(B.get_si(),(mpz_class)0);
    for(long p=2;p*p<B.get_si();p++){
        if(list[p]==0){
            for(long i=p*p;i<B.get_si();i+=p){
                list[i]=1;
            }
        }
    }
    vector<mpz_class> plist(1,(mpz_class)2);
    for(mpz_class j=1;j<B;j+=2){
        if (!list[j.get_si()]){
            plist.push_back(j);
        }
    }
    reverse(plist.begin(),plist.end());
    plist.pop_back();
    plist.pop_back();
    plist.push_back(2);
    reverse(plist.begin(),plist.end());
    for(auto k:plist)
        cout<<k<<endl;
    return plist;
}

//randomly generate all the parameters

mpz_class rangen(mpz_class m){
    mpz_class random;
    gmp_randstate_t state;
    gmp_randinit_default(state);
    unsigned long seed= time(NULL);
    gmp_randseed_ui(state,seed);
    mpz_urandomm(random.get_mpz_t(),state,m.get_mpz_t());
    gmp_randclear(state); 
    cout<<"\nrandom is "<<random.get_str()<<endl;
    return random;
}


void paragen(mpz_class &A,coordinate &k,mpz_class &N){
    mpz_class sita;
    while(1){
        while(1){
            sita = rangen(N);
            if (sita > 4)
                break;
        }
        cout<<"\nnow sita is"<<sita.get_str()<<endl;
        mpz_class u = (sita * sita - 5)%N;
        mpz_class v = (4 * sita)%N;
        mpz_class u1=517;
        mpz_class v1=152;
        //cout<<"\nnow u is"<<u.get_str()<<endl;
        //cout<<"\nnow v is"<<v.get_str()<<endl;
        mpz_class kx;
        mpz_class kz;
        mpz_powm_ui(kx.get_mpz_t(),u.get_mpz_t(),3,N.get_mpz_t());
        mpz_powm_ui(kz.get_mpz_t(),v.get_mpz_t(),3,N.get_mpz_t());
        k.dataset(kx,kz);
        k.dataprint();
        //calculate A
        mpz_class vu=v-u;
        cout<<"\nnow vu is "<<vu.get_str()<<endl;
        mpz_powm_ui(vu.get_mpz_t(),vu.get_mpz_t(),3,N.get_mpz_t());
        cout<<"\nnow vu is "<<vu.get_str()<<endl;    
        mpz_class numerator= (vu * (3*u+v))%N;
        cout<<"\nnow numerator is "<<numerator.get_str()<<endl;
        mpz_powm_ui(u.get_mpz_t(),u.get_mpz_t(),3,N.get_mpz_t());
        mpz_class dominator= (4*u*v)%N;
        //cout<<"\nnow dominator is "<<dominator.get_str()<<endl;
        mpz_invert(dominator.get_mpz_t(),dominator.get_mpz_t(),N.get_mpz_t());
        if (dominator==0){continue;}
        cout<<"\nnow dominator is "<<dominator.get_str()<<endl;
        //A=div(numerator,dominator,N);
        A=(numerator*dominator)%N;
        //A=89;
        cout<<"\nnow A is "<<A.get_str()<<endl;
        if (A!=-1){
            A = (A+2*N-2)%N;
            cout<<"\nnow A is "<<A.get_str()<<endl;
            break;
        }       
    }

}

mpz_class Bgen(mpz_class N){
    string l=N.get_str(2);
    int length=l.length()/2;
    cout<<"l:"<<length<<endl;
    mpz_class B;
    if (length<=40){
        B=3*(10e6);
    }
    else if(40<length<=45){
        B=11*(10e6);
    }
    else if(45<length<=50){
        B=43*(10e6);
    }
    else if(50<length<=55){
        B=110*(10e6);
    }
    else if(55<length<=60){
        B=260*(10e6);
    }
    else{
        B=850*(10e6);
    }
    //cout<<"\nB:"<<B.get_str()<<endl;
    return B;
}

//ECMtrivial algorithm
mpz_class ECMtrivial(mpz_class N,mpz_class pow){
    //auto B=Bgen(N);
    //mpz_class B;
    //mpz_class Bt=10;
    //mpz_pow_ui(B.get_mpz_t(),Bt.get_mpz_t(),6);
    mpz_class B=((mpz_class)1)<<(pow.get_ui());
    //mpz_class B=64;
    auto primevector=primgen(B);
    /*
    vector<mpz_class> primevector;
    primevector.push_back(64);
    primevector.push_back(81);
    primevector.push_back(25);
    primevector.push_back(7);
    primevector.push_back(11);
    primevector.push_back(13);
    */
    while(1){
        //auto B=Bgen(N);
        
        mpz_class A;
        coordinate init;
        //init.dataset(7,1);
        paragen(A,init,N);
        coordinate initi=init;
        cout<<"\nB:"<<B.get_str()<<endl;
        cout<<"\nA:"<<A.get_str()<<endl;
        init.dataprint();
        cout<<"\nvasdvs\n"<<endl;
        mpz_class first=gcd((A*A)-4,N);
        cout<<"\nvasdvs\n"<<endl;
        
        if (first!=1 && first!=N){
            cout<<"\niniti x:"<<initi.x.get_str()<<endl;
            cout<<"\niniti z:"<<initi.z.get_str()<<endl;
            cout<<"\nN:"<<N.get_str()<<endl;
            cout<<"\nA:"<<A.get_str()<<endl;
            cout<<"\nfactor gcd:"<<first.get_str()<<endl;
            return first;
        }
        
        
        cout<<"\nvasdvs\n"<<endl;
    
        //cout<<"prime"<<primevector[2].get_str()<<primevector[3].get_str()<<endl;
        //for(auto q:primevector){cout<<q<<" ";}
        
        for(auto p:primevector){
            double logp=log(p.get_ui());
            double logB=log(B.get_ui());
            mpz_class pfactor;
            mpz_pow_ui(pfactor.get_mpz_t(),p.get_mpz_t(),floor(logB/logp));
            cout<<"\niniti x:"<<initi.x.get_str()<<endl;
            cout<<"\niniti z:"<<initi.z.get_str()<<endl;
            cout<<"\nN:"<<N.get_str()<<endl;
            cout<<"\nA:"<<A.get_str()<<endl;
            init=Montgomery(pfactor,init,A,N);
            if (init.x==0 && init.z==0){
                cout<<"\nwe fail at P:"<<p.get_str()<<endl;
                continue;
            }
            //if (p==2){return 0;}
        }
        mpz_class g=gcd(init.z,N);
        if (g!=1 && g!=0 && g!=N){
            cout<<"\niniti x:"<<initi.x.get_str()<<endl;
            cout<<"\niniti z:"<<initi.z.get_str()<<endl;
            cout<<"\nN:"<<N.get_str()<<endl;
            cout<<"\nA:"<<A.get_str()<<endl;
            cout<<"\nfactor gcd:"<<g.get_str()<<endl;
            return g;
        }
    }
}

void ECMBASIC(mpz_class &N,vector<mpz_class> &pfl){
    auto plist2=primgen(10000);
    for(int i=0;i<plist2.size();i++){
        mpz_class pm=plist2[i];
        //cout<<"pm:\n"<<pm.get_str()<<endl;
        //cout<<"plist:\n"<<plist2[i].get_str()<<endl;
        if(N%pm==0){
            N=N/pm;
            pfl.push_back(pm);
            i--;
        }
        
    }
    //for(auto i:pfl){cout<<i;}
}


int main(int argc,char** argv){
    std::string file_name1=(std::string)argv[1];
    std::string file_name2=(std::string)argv[2];
    int length1=get_file_size_in_byte(file_name1);
    int length2=get_file_size_in_byte(file_name2);
    std::ifstream in1;
    std::ifstream in2;
    in1.open(file_name1);
    in2.open(file_name2);
    auto msg1=special_read(in1,length1);
    auto msg2=special_read(in2,length2);
    //reve(msg1);
    //reve(msg2);
    //reverse(msg1.begin(),msg1.end());
    //reverse(msg2.begin(),msg2.end());
    mpz_class key=mpz_class(msg1,10);
    mpz_class modulo=(((mpz_class)1)<<255)-19;
    //key=process(key,modulo);
    mpz_class uco=mpz_class(msg2,10);
    cout<<"\nmsg1:"<<msg1<<endl;
    cout<<"\nmsg2:"<<msg2<<endl;
    cout<<"\nprint key and uco\n\n"<<endl;
    cout<<"key:"<<key.get_str()<<endl;
    cout<<"uco:"<<uco.get_str()<<endl;

    /*
    coordinate pro1;
    pro1.dataset(2,1);
    mpz_class A=486662;
    mpz_class b=9;
    mpz_class b1=7;
    coordinate test;
    test.dataset(uco,1);
    auto pro2=Montgomery(key,test,A,modulo);
    printf("result:");
    pro2.dataprint();
    //mpz_invert(b1.get_mpz_t(),b.get_mpz_t(),A.get_mpz_t());
    //cout<<pro2.get_str(16)<<endl;
    */

    //special test
    mpz_class modulot=1009;
    mpz_class At=682;
    coordinate testt;
    testt.dataset(7,1);
    mpz_class keyt=947;
    auto prot=Montgomery(keyt,testt,At,modulot);
    cout<<"\nspecial test"<<endl;
    prot.dataprint();
    

    //auto p3=primgen(100);
    //auto p4=rangen(100);
    
    //mpz_class A;
    coordinate coor;
    mpz_class N=key;
    //paragen(A,coor,N);
    //auto B=Bgen(3000000);
    //auto p2=ECMtrivial(N);
    //auto plist1=primgen(N);
    //cout<<"p2:"<<p2.get_str()<<endl;
    
    vector<mpz_class> pfl;
    ECMBASIC(N,pfl);
    cout<<"bsdfb"<<endl;
    for(auto i:pfl){cout<<"dfvdsf"<<i<<endl;}
    cout<<"bsdfb"<<N.get_str()<<endl;
    auto p2=ECMtrivial(N,uco);
    cout<<"p2:"<<p2.get_str()<<endl;


    return 0;
}