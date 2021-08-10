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

coordinate XADD(coordinate& p,coordinate& q,coordinate& minus){
    mpz_class modulo=(((mpz_class)1)<<255)-19;
    //mpz_class modulo=1009;
    mpz_class U=((p.x-p.z)*(q.x+q.z))%modulo;
    mpz_class V=((p.x+p.z)*(q.x-q.z))%modulo;
    mpz_class xplus=(minus.z*(U+V)*(U+V))%modulo;
    mpz_class zplus=(minus.x*(U-V)*(U-V))%modulo;
    coordinate plus;
    plus.dataset(xplus,zplus);
    cout<<"xadd:";
    plus.dataprint();
    return plus;
}
coordinate XDBL(coordinate unit1,mpz_class A){
    mpz_class modulo=(((mpz_class)1)<<255)-19;
    //mpz_class modulo=1009;    
    mpz_class Q=((unit1.x+unit1.z)*(unit1.x+unit1.z))%modulo;
    mpz_class R=((unit1.x-unit1.z)*(unit1.x-unit1.z))%modulo;
    mpz_class S=(Q-R)%modulo;
    mpz_class xdouble=(Q*R)%modulo;
    mpz_class A1=A+2;
    mpz_class A2=4;
    mpz_invert(A2.get_mpz_t(),A2.get_mpz_t(),modulo.get_mpz_t());
    A1 = (A1 * A2) % modulo;
    //cout<<"Aprocess"<<A1.get_str()<<endl;
    mpz_class zdouble= (S*(R + A1 * S)) % modulo;
    coordinate double1;
    double1.dataset(xdouble,zdouble);
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



mpz_class process(mpz_class aa){
    mpz_class modulo=(((mpz_class)1)<<255)-19;
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
    cout<<"\nshow1\n";
    //cout<<b[0]<<endl;
    for(auto j:b){
      cout<<j<<endl;
	}
    cout<<"\nshow2\n";
    cout<<b[0].get_str()<<endl;
    b[0] &= 248;
    cout<<b[0].get_str()<<endl;
    cout<<"\nshow3\n";
    b[31] &= 127;
    cout<<"\nshow4\n";
    b[31] |= 64;
    cout<<"\nshow5\n";
    
    mpz_class sum=0;
    for(int i=0;i<l+1;++i){
        sum += b[i]<<(8*i);
    }
    return sum;
}

//now we write The Montgomery ladder 
mpz_class Montgomery(mpz_class &scalar, mpz_class &coor,mpz_class &A){
    scalar=process(scalar);
    mpz_class x_1=coor;
    coordinate coor2;
    coordinate coor3;
    coor2.dataset(1,0);
    coor3.dataset(coor,1);
    coordinate u=coor3;
    printf("possible error1");
    mpz_class swa=0;
    string c1=scalar.get_str(2);
    int bits=c1.length();
    mpz_class modulo=(((mpz_class)1)<<255)-19;
    //mpz_class modulo=1009;
    mpz_class exp=modulo-2;
    mpz_class foo=0;
    
    //begin processing
    for(int t=0;t<bits;++t){
        mpz_class m_t=(scalar>>(bits-1-t)) & 1;
        cout<<"\nbits:"<<bits<<"\nm_t:"<<m_t<<endl;
        int m_t1=m_t.get_ui();
        swap(coor2,coor3,XDBL(special_add_swap(coor2,coor3,m_t1),A),XADD(coor2,coor3,u),m_t1);
        printf("\nintermediate data\n");
        coor2.dataprint();
        coor3.dataprint();
    }
    mpz_powm(foo.get_mpz_t(),coor2.z.get_mpz_t(),exp.get_mpz_t(),modulo.get_mpz_t());
    foo = (foo*coor2.x)%modulo;
    return foo;
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
void reve(string &a){
    int l=a.length()/2;
    for(int i=0;i<l;++i){
        char b=a[2*i+1];
        a[2*i+1]=a[2*i];
        a[2*i]=b;
    }
}

int main(int argc,char** argv){
    std::string file_name1=(std::string)argv[1];
    int length1=get_file_size_in_byte(file_name1);
    std::ifstream in1;
    in1.open(file_name1);
    auto msg1=special_read(in1,length1);
    reve(msg1);
    reverse(msg1.begin(),msg1.end());
    mpz_class key=mpz_class(msg1,16);
    key=process(key);
    cout<<"\nmsg1:"<<msg1<<endl;
    mpz_class uco;
	if(argc == 3){
        std::string file_name2=(std::string)argv[2];
        int length2=get_file_size_in_byte(file_name2);
		std::ifstream in2;
		in2.open(file_name2);
		auto msg2=special_read(in2,length2);
		reve(msg2);
		reverse(msg2.begin(),msg2.end());
		uco=mpz_class(msg2,16);
		cout<<"\nmsg2:"<<msg2<<endl;
	}else{
		uco = 9;
	}
	
    cout<<"\nprint key and uco\n\n"<<endl;
    cout<<"key:"<<key.get_str()<<endl;
    cout<<"uco:"<<uco.get_str()<<endl;

    //test 
    /*
    coordinate data1;
    coordinate data2;
    coordinate data3;
    coordinate data4;
    data1.dataset(3,2);
    data2.dataset(7,5);
    data3.dataset(13,11);
    //XADD(data4,data1,data2,data3);
    data4=XDBL(data3,(mpz_class)1);
    data4.dataprint();
    //data4=swap(data2,1);
    printf("vsdfv");
    data4.dataprint();
    mpz_class pro1=257;
    mpz_class pro2=4;
    //printf("\ntest2:%d\n",(int)process(pro1)):
    cout<<"test2"<<process(pro1)<<endl;
    cout<<"test3"<<Montgomery(pro1,pro2,pro2)<<endl;
    cout<<pro1;
    data4=special_add_swap(data2,data3,0);
    cout<<"test4";
    data4.dataprint();
    cout<<"test5";
    swap(data1,data4,data2,data3,1);
    data1.dataprint();
    cout<<"test6";
    auto foo1= Montgomery(pro1,pro2,pro2);
    cout<<foo1<<endl;
    */
    coordinate pro1;
    pro1.dataset(2,1);
    mpz_class A=486662;
    mpz_class b=9;
    mpz_class b1=7;
    auto pro2=Montgomery(key,uco,A);
    printf("result:");
    //pro2.dataprint();
    //mpz_invert(b1.get_mpz_t(),b.get_mpz_t(),A.get_mpz_t());
    //cout<<pro2.get_str(16)<<endl;
    string result=pro2.get_str(16);
    reve(result);
    //printf("vadfvd");
    reverse(result.begin(),result.end());
    cout<<result<<endl;
    return 0;
}
