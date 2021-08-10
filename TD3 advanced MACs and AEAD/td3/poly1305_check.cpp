#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iterator>
//#include "keccak.h"
#include <vector>
#include <bitset>
#include <algorithm>
#include <sstream>
#include <gmpxx.h>

using namespace std;

//print function
void show(std::vector<char> x){
    printf("now we print");
    for(auto i:x)
      std::cout<<i;
    cout<<endl;
}

//reverse function
void reverse1(vector<char> &k1){
    int n5=(int)k1.size();
    std::reverse(k1.begin(), k1.end());
    for(int i=0;i<(n5/2);++i){
        char k2=k1[2*i];
        k1[2*i]=k1[2*i+1];
        k1[2*i+1]=k2;
    }
}

//erase the useless":"
vector<char> pro_erase(vector<char> m){
    for (auto it = m.begin(); it != m.end(); ) {
        if (*it==':' || *it==' ') {
            it = m.erase(it);
        } else {
            ++it;
        }
    }
    return m;
}

//read message
void binary_read(std::ifstream& fin, std::vector<char>& x, std::vector<char>::size_type n) {
    x.resize(n);
    for(std::vector<char>::size_type i = 0; i < n;++i)
    {
        unsigned char aggr;
        fin.read((char*)&aggr, sizeof(unsigned char));
        x[i]=(char)aggr;
    }
    std::reverse(x.begin(), x.end());
    for(int i=0;i<(n/2);++i){
        char k=x[2*i];
        x[2*i]=x[2*i+1];
        x[2*i+1]=k;
    }
}
//read key
void binary_read1(std::ifstream& fin, std::vector<char>& x, std::vector<char>::size_type n) {
    x.resize(n);
    for(std::vector<char>::size_type i = 0; i < n;++i)
    {
        unsigned char aggr;
        fin.read((char*)&aggr, sizeof(unsigned char));
        x[i]=(char)aggr;
    }
}
//convert hex string into int
mpz_class convert_hex_to_int(std::vector<char> x){
    std::string s1(x.begin(), x.end());
    mpz_class m=mpz_class(s1,16);
    return m;
    cout<<"convert1 int:"<<m<<endl;
    cout<<"convert1 binary:"<<m.get_str(2)<<endl;
}
//convert int into hex string with length bytes
//take care

std::vector<char> convert_int_to_hex_directly(mpz_class m,int length=16){
    //get vector
    string hex1=m.get_str(16);
    std::vector<char> hex2(hex1.begin(),hex1.end());
    show(hex2);
    return hex2;
}
std::vector<char> convert_int_to_hex(mpz_class m,int length=16){
    //get vector
    string hex1=m.get_str(16);
    std::vector<char> hex2(hex1.begin(),hex1.end());
    //pading enough 0
    int n=32-(int)hex2.size();
    std::reverse(hex2.begin(),hex2.end());
    if (n>0){
        for(int i=0;i<n;++i){hex2.push_back('0');}
    }
    std::reverse(hex2.begin(),hex2.end());
    //intercept last vector for length bytes
    int k=(int)hex2.size()-length*2;
    if (k>0){
        std::reverse(hex2.begin(),hex2.end());
        for(int i=0;i<k;++i){hex2.pop_back();}
        std::reverse(hex2.begin(),hex2.end());
    }
    //reverse 
    int n3=(int)hex2.size();
    std::reverse(hex2.begin(), hex2.end());
    for(int i=0;i<(n3/2);++i){
        char k=hex2[2*i];
        hex2[2*i]=hex2[2*i+1];
        hex2[2*i+1]=k;
    }
    //print
    for(auto j:hex2)
      cout<<j;
    return hex2;
}


// this function return the file size

int get_file_size_in_byte(const std::string file_name) {
  int size_f = 0;
  std::ifstream inf(file_name);

  inf.seekg(0, std::ios::end);
  int file_size = inf.tellg();
  inf.seekg(0, std::ios::beg);
  std::cout << "file size = " << file_size << std::endl;
  return file_size;
}
// clamp r
std::vector<char> clamp(std::vector<char> r){
    //string r1="cf";
    std::string r1="0ffffffc0ffffffc0ffffffc0fffffff";
    mpz_class m1=mpz_class(r1,16);
    string R(r.begin(),r.end());
    mpz_class m2=mpz_class(R,16);
    cout<<"binary-m1\n"<<m1.get_str(2)<<endl; 
    cout<<"binary-m2\n"<<m2.get_str(2)<<endl;
    m2 &= m1;
    cout<<"binary-m3\n"<<m2.get_str(2)<<endl;
    return convert_int_to_hex_directly(m2);
}
//intercept message
vector<char> intercept(int position,vector<char> me){
    if ((int)me.size()<32*(position+1)){
        vector<char> mes(me.begin()+32*position,me.end());
        cout<<"first"<<endl;
        show(mes);
        std::reverse(mes.begin(), mes.end());
        int n=(int)mes.size();
        for(int i=0;i<(n/2);++i){
            char k=mes[2*i];
            mes[2*i]=mes[2*i+1];
            mes[2*i+1]=k;
        }
        cout<<"second"<<endl;
        show(mes);

        //pading
        std::reverse(mes.begin(),mes.end());
        mes.push_back('1');
        mes.push_back('0');
        if (34>(int)mes.size()){
            while(1){
                mes.push_back('0');
                if (34==(int)mes.size())
                break;
            }
        }
        std::reverse(mes.begin(),mes.end());
        cout<<"third"<<endl;
        show(mes);
        return mes;
    }
    else{
        vector<char> mes(me.begin()+32*position,me.begin()+32*(position+1));
        cout<<"first"<<endl;
        show(mes);
        std::reverse(mes.begin(), mes.end());
        int n=(int)mes.size();
        for(int i=0;i<(n/2);++i){
            char k=mes[2*i];
            mes[2*i]=mes[2*i+1];
            mes[2*i+1]=k;
        }
        cout<<"second"<<endl;
        show(mes);

        //pading
        std::reverse(mes.begin(),mes.end());
        mes.push_back('1');
        mes.push_back('0');
        if (34>(int)mes.size()){
            while(1){
                mes.push_back('0');
                if (34==(int)mes.size())
                break;
            }
        }
        std::reverse(mes.begin(),mes.end());
        cout<<"third"<<endl;
        show(mes);
        return mes;
    }
}



int main(int argc, char *argv[]){
    
    std::string file_name1=(std::string)argv[1];
    std::string file_name2=(std::string)argv[2];
    std::string file_name3=(std::string)argv[3];
    std::vector<char>::size_type length1=get_file_size_in_byte(file_name1);
    std::vector<char>::size_type length2=get_file_size_in_byte(file_name2);
    std::vector<char>::size_type length3=get_file_size_in_byte(file_name3);    
    std::ifstream in1;
    std::ifstream in2;
    std::ifstream in3;
    std::vector<char> message1(length1);
    std::vector<char> key1(length2);
    std::vector<char> authenticator1(length3);
    std::cout<<"now we are reading the vector"<<std::endl;
    in1.open(file_name1);
    in2.open(file_name2);
    in3.open(file_name3);
    binary_read1(in1,message1,length1);
    binary_read1(in2,key1,length2);
    binary_read1(in3,authenticator1,length3);
    auto message=pro_erase(message1);
    auto key=pro_erase(key1);
    auto authenticator=pro_erase(authenticator1);
    in1.close();
    in2.close();

    printf("\nmessage\n");
    show(message);
    printf("\nkey\n");
    show(key);

    //generate r and s
    // take care. it need further test
    //poly1305 algorithm
    
    std::vector<char> r(key.begin(),key.begin()+32);
    reverse1(r);
    printf("r before\n");
    show(r);
    r=clamp(r);
    printf("r after\n");
    std::vector<char> s(key.begin()+32,key.begin()+64);
    reverse1(s);
    show(r);
    show(s);
    mpz_class a=0;
    mpz_class p=(((mpz_class)1)<<130)-5;
    int l=(int)message.size()/32;
    
    for(int i=0;i<l+1;++i){
        mpz_class n1=convert_hex_to_int(intercept(i,message));
        a += n1;
        a = (convert_hex_to_int(r) * a) % p;
        cout<<"\nwe see a directly\n"<<endl;
        show(convert_int_to_hex_directly(a));
    }
    
    a += convert_hex_to_int(s);
    cout<<"\nwe see a finally\n"<<endl;
    show(convert_int_to_hex_directly(a));
    auto tag= convert_int_to_hex(a,16);
    cout<<"\nwe see tag directly\n"<<endl;
    show(tag);

    printf("\nlet's check\n");
    int res=0;
    if(authenticator.size()!=tag.size()){
        printf("\ncheck failure since incompatible length\n");
        res=1;
    }
    for(int i=0;i<(int)authenticator.size();++i){
        if (authenticator[i]!=tag[i]){
          printf("\ncheck failure since your wrong %d element in tag\n",i);
          res=1;
        }
    }
    if (!res){
        printf("ACCEPT");
    }
    else{printf("REJECT");}
    
    return 0;
}