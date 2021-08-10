#include "keccak.h"
#include "math.h"

const int w = 64;
const int l = 6;

void hex_write1(/*std::ofstream& fout*/ const std::vector<bool>& x) {
    std::vector<bool>::size_type n = x.size();
    for(std::vector<bool>::size_type i = 0; i < n;) {
        unsigned char aggr = 0;
        for(unsigned char mask = 1; mask > 0 && i < n; ++i, mask <<= 1)
            if(x.at(i))
                aggr |= mask;
        //fout.write((const char*)&aggr, sizeof(unsigned char));
        int aggr1=(int)aggr;
        std::cout<<std::hex<<aggr1;
    }
}

void show(state &A){
    for(int x=0;x<5;x++){
        for(int y=0;y<5;y++){
            /*for(int z=0;z<w;z++){
                std::cout<<A[x][y][z];
            }*/
            hex_write1(A[x][y]);
            std::cout<<" ";
        }
        std::cout<<"\n"<<std::endl;
    }
}
void show1(std::vector<bool> &S){
    for(int i=0;i<S.size();i++){
        std::cout<<S[i];
    }
    std::cout<<"\n";
}


state theta(state &A){
    //printf("start\n");
    std::vector<std::vector<bool>> C(5,std::vector<bool>(w,false));
    std::vector<std::vector<bool>> D(5,std::vector<bool>(w,false));
    state A0(5,std::vector<std::vector<bool>>(5,std::vector<bool>(w,false)));
    //printf("first\n");    
    for(int x=0;x<5;x++){
        for (int z=0;z<w;z++){
            C[x][z]=A[x][0][z]^A[x][1][z]^A[x][2][z]^A[x][3][z]^A[x][4][z];
        }
    }
    //printf("second\n");
    for(int x=0;x<5;x++){
        for(int z=0;z<w;z++){
            D[x][z]=C[(x-1+5)%5][z]^C[(x+1)%5][(z-1+5)%5];
        }
    }
    //printf("third\n");
    for(int x=0;x<5;x++){
        for(int y=0;y<5;y++){
            for(int z=0;z<w;z++){
                A0[x][y][z]=A[x][y][z]^D[x][z];
            }
        }
    }
    //printf("fourth\n");
    return A0;
}

state rho(state &A){
    state A0(5,std::vector<std::vector<bool>>(5,std::vector<bool>(w,false)));
    for(int z=0;z<w;z++){
        A0[0][0][z]=A[0][0][z];
    }
    int x=1;
    int y=0;
    int temp;
    for(int t=0;t<24;t++){
        int need= ((t+1)*(t+2)/(2*w))+1;
        for(int z=0;z<w;z++){A0[x][y][z]=A[x][y][(z-(t+1)*(t+2)/2+need*w)%w];}
        temp=x;
        x=y;
        y=(2*temp+3*y)%5;
    }
    return A0;
}

state pi(state &A){
    state A0(5,std::vector<std::vector<bool>>(5,std::vector<bool>(w,false)));
    for(int x=0;x<5;x++){
        for(int y=0;y<5;y++){
            for(int z=0;z<w;z++){
                A0[x][y][z]=A[(x+3*y)%5][x][z];
            }
        }
    }
    return A0;   
}

state chi(state &A){
    state A0(5,std::vector<std::vector<bool>>(5,std::vector<bool>(w,false)));
    for(int x=0;x<5;x++){
        for(int y=0;y<5;y++){
            for(int z=0;z<w;z++){
                A0[x][y][z]=A[x][y][z]^(
                    (A[(x+1)%5][y][z]^1)&(A[(x+2)%5][y][z])
                );
            }
        }
    }
    return A0;  
}

bool rc(int t){
    if((t%255)!=0){
        std::bitset<9> R("010000000");
        for(int i=0;i<(t%255);i++){
            R[0]=R[0] xor R[8];
            R[4]=R[4] xor R[8];
            R[5]=R[5] xor R[8];
            R[6]=R[6] xor R[8];
            R>>=1;
        }
        return R[0];
    }
    else{return 1;}
}

state iota(state &A, int ir){
    state A0(5,std::vector<std::vector<bool>>(5,std::vector<bool>(w,false)));
    for(int x=0;x<5;x++){
        for(int y=0;y<5;y++){
            for(int z=0;z<w;z++){
                A0[x][y][z]=A[x][y][z];
            }
        }
    }
    std::vector<bool> RC(w,false);
    for(int i=0;i<=l;i++){
        RC[pow(2,i)-1]=rc(i+7*ir);
    }
    for(int z=0;z<w;z++){
        A0[0][0][z]=A0[0][0][z] ^ RC[z];
    }
    return A0;
}

state RND(state &A,int ir){
    printf("theta\n");
    show(A);
    auto theta_A =theta(A);
    printf("rho\n");
    show(A);
    auto rho_A   =rho(theta_A);
    printf("pi\n");
    show(A);
    auto pi_A    =pi(rho_A);
    printf("chi\n");
    show(A);
    auto chi_A   =chi(pi_A);
    printf("iota\n");
    show(A);
    auto iota_A  =iota(chi_A,ir);
    printf("end\n");
    show(A);
    return iota_A;
}

state string_to_state(std::vector<bool> S,int w){
    auto size=S.size();
    if (size==(25*w)){
        state A(5,std::vector<std::vector<bool>>(5,std::vector<bool>(w,false)));
        for(int x=0;x<5;x++){
            for(int y=0;y<5;y++){
                for(int z=0;z<w;z++){
                    A[x][y][z]=S[z+w*(5*y+x)];
                }
            }
        }
        return A;        
    }
    else{printf("please take care of your converted size\n");}
}
std::vector<bool> state_to_string(state &A){
    std::vector<bool> S(25*w,false);
    for(int i=0;i<(25*w);i++){
        int z=i%w;
        int x=((i-z)/w)%5;
        int y=((i-z)/w-x)/5;
        S[i]=A[x][y][z];
    } 
    return S;
}

std::vector<bool> keccak_p(std::vector<bool> S,int b, int nr){
    int w=b/25;
    show1(S);
    auto A= string_to_state(S,w);
    int l=(int)log2(w);
    for(int ir=12+2*l-nr;ir<(12+2l);ir++){
        std::cout<<"rnd"<<std::endl;
        //show(A);
        A=RND(A,ir);
    }
    return state_to_string(A);
}

std::vector<bool> pad101(std::vector<bool> M,int r){
    int m=M.size();
    int j=(m+2)%r;
    j=r-j;
    if(j>r){printf("please take care of j in pad");}
    std::vector<bool> P(j+m+2,false);
    for(int i=0;i<m;i++){
        P[i]=M[i];
    }
    P[m]=1;
    P[j+m+1]=1;
    return P;
}

std::vector<bool> sponge(int b,int r ,int d, std::vector<bool> M){
    auto P=pad101(M,r);
    show1(P);
    std::cout<<P.size()<<std::endl;
    int n=(P.size())/r;
    std::cout<<n<<"\n"<<r<<std::endl;
    int c=b-r;
    std::vector<bool> S(b,false);
    std::vector<bool> pi(r,false);
    std::vector<bool> z(0);
    for(int i=0;i<n;i++){
        for(int j=0;j<r;j++){
            pi[j]=P[i*r+j];
            S[j]=S[j] ^ pi[j];
        }
        S=keccak_p(S,b,24);
        for (std::vector<bool>::const_iterator i = S.begin(); i != S.end(); ++i)
            std::cout << *i ;
        std::cout<<"\n";
        //take care of 24.
    }
    std::vector<bool> z1(0);
    while(z.size()<d){
        z=append(z,copy(S,r));
        S=keccak_p(S,b,24);
    }
    return copy(z,d);
}

std::vector<bool> SHAKE128(std::vector<bool> M,int d){
    std::vector<bool> q(4,true);
    auto M_hash=sponge(1600,1344,d*8,append(M,q));
    return M_hash;
}

std::vector<bool> append(std::vector<bool> a, std::vector<bool> b){
    std::vector<bool> c(a.size()+b.size(),false);
    for(int i=0;i<a.size();i++){
        c[i]=a[i];
    }
    for(int i=a.size();i<(a.size()+b.size());i++){
        c[i]=b[i];
    }
    return c;
}
std::vector<bool> copy(std::vector<bool> a, int n){
    std::vector<bool> b(n,false);
    if(a.size()<n){
        printf("please take care of truncated number n\n");
    }
    else{
        for(int i=0;i<n;i++){
            b[i]=a[i];
        }
    }
    return b;
}




