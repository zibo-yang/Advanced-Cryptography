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
#include <gmp.h>
#include <math.h>
//#include <string.hpp>
//#include <boost>

using namespace std;

mpz_class leftroll(mpz_class a,int r){
    mpz_class ar=(mpz_class)1<<(32-r);
    //cout<<"ar:"<<ar.get_str(16)<<endl;
    mpz_class modulo=(mpz_class)1<<32;
    mpz_class af=a/ar;
    //cout<<"af:"<<af.get_str(16)<<endl;
    mpz_class a1=(a<<r)%modulo;
    //cout<<"a1:"<<a1.get_str(16)<<endl;
    a1+=af;
    return a1;
}

void show(vector<mpz_class> state1){
    printf("\nshow:\n");
    int l=state1.size()/4;
    for(int i=0;i<l;++i){
        cout<<" "<<state1[4*i].get_str(16);
        cout<<" "<<state1[4*i+1].get_str(16);
        cout<<" "<<state1[4*i+2].get_str(16);
        cout<<" "<<state1[4*i+3].get_str(16)<<endl;
    }
}

void chacha_quarter (mpz_class &a,mpz_class &b,mpz_class &c,mpz_class &d){
    mpz_class modulo=((mpz_class)1)<<32;
    a=(a+b)%modulo;
    d^=a;
    d=leftroll(d,16);
    
    c=(c+d)%modulo;
    b^=c;
    b=leftroll(b,12);
    
    a=(a+b)%modulo;
    d^=a;
    d=leftroll(d,8);
    
    c=(c+d)%modulo;
    b^=c;
    b=leftroll(b,7);
}
void chacha_columnquarter(vector<mpz_class> &state2){
    chacha_quarter(state2[0],state2[4],state2[8],state2[12]);
    chacha_quarter(state2[1],state2[5],state2[9],state2[13]);
    chacha_quarter(state2[2],state2[6],state2[10],state2[14]);
    chacha_quarter(state2[3],state2[7],state2[11],state2[15]);
}
void chacha_diagonalquarter(vector<mpz_class> &state2){
    chacha_quarter(state2[0],state2[5],state2[10],state2[15]);
    chacha_quarter(state2[1],state2[6],state2[11],state2[12]);
    chacha_quarter(state2[2],state2[7],state2[8],state2[13]);
    chacha_quarter(state2[3],state2[4],state2[9],state2[14]);
}


vector<mpz_class> chacha_gen(mpz_class key,mpz_class counter,mpz_class nonce){
    vector<mpz_class> state;
    mpz_class keyc;
    state.push_back(0x61707865);
    state.push_back(0x3320646e);
    state.push_back(0x79622d32);
    state.push_back(0x6b206574);
    for(int i=0;i<8;++i){
        mpz_class roll1=(mpz_class)1<<(32*i);
        mpz_class roll2=(mpz_class)1<<(32);
        keyc=key/roll1;
        //cout<<"kec:"<<keyc.get_str(16)<<endl;
        keyc=keyc%roll2;
        //cout<<"kec:"<<keyc.get_str(16)<<endl;
        state.push_back(keyc);
    }
    state.push_back(counter);
    for(int j=0;j<3;++j){
        mpz_class roll1=(mpz_class)1<<(32*j);
        mpz_class roll2=(mpz_class)1<<(32);
        keyc=nonce/roll1;
        keyc=keyc%roll2;
        state.push_back(keyc);
    }
    return state;
}

void chacha_block(vector<mpz_class> &state3){
    vector<mpz_class> init=state3;
    mpz_class modulo=(mpz_class)1<<32;
    for(int i=0;i<10;++i){
        chacha_columnquarter(state3);
        show(state3);
        chacha_diagonalquarter(state3);
        show(state3);
    }
    for(int j=0;j<state3.size();++j){
        state3[j]+=init[j];
        state3[j]%=modulo;
    }
    show(state3);
    //show(init);
}

mpz_class special_change(string &m){
    //cout<<"\nm:"<<m<<endl;
    string zero="00000000";
    reverse(m.begin(),m.end());
    if (m.length()<8){
        //cout<<"\nm:"<<m<<endl;
        int lack=8-m.length();
        //cout<<"lack:"<<lack<<endl;
        m.append(zero,0,lack);
        //cout<<"\nm:"<<m<<endl;
    }


    //cout<<"\nm:"<<m<<endl;
    for(int i=0;i<m.length();++i){
        if (i%2==0){
            std::swap(m[i],m[i+1]);
        }
    }
    //cout<<"\nm:"<<m<<endl;
    mpz_class m1=mpz_class(m,16);
    return m1;
}
string special_change2(vector<mpz_class> m){
    string m1;
    for(auto i:m){
        string tmp=i.get_str(16);
        string zero="00000000";
        if (tmp.length()<8){
            //cout<<"\nm:"<<m<<endl;
            int lack=8-tmp.length();
            //cout<<"lack:"<<lack<<endl;
            tmp.append(zero,0,lack);
            //cout<<"\nm:"<<m<<endl;
        }
        reverse(tmp.begin(),tmp.end());
        for(int j=0;j<tmp.length();++j){
            if(j%2==0){
                swap(tmp[j],tmp[j+1]);
            }
        }
        //reverse(tmp.begin(),tmp.end());
        //mpz_class tmp1=mpz_class(tmp,16);
        //m1.push_back(tmp1);
        m1.append(tmp);
    }
    return m1;
}
string chacha_keystream(vector<mpz_class> &state4){
    vector<mpz_class> result;
    for(auto i:state4){
        string tmp=i.get_str(16);
        //cout<<"\nstr:"<<tmp<<endl;
        result.push_back(special_change(tmp));
    }
    show(result);
    string zero1="00000000";
    string result2;
    for(auto i1:result){
        string i=i1.get_str(16);
        if(i.length()<8){
            int lack1=8-i.length();
            reverse(i.begin(),i.end());
            i.append(zero1,0,lack1);
            reverse(i.begin(),i.end());
        }
        result2.append(i);
    }
    cout<<"\nkeystream:"<<result2<<endl;
    return result2;
}

string chacha_keystream_gen(mpz_class &key,mpz_class &counter,mpz_class &nonce,int msgl){
    string result3;
    for(int l=0;l<=floor(msgl/64);++l){
        auto newstate=chacha_gen(key,counter+l,nonce);
        chacha_block(newstate);
        auto str=chacha_keystream(newstate);
        if (l==floor(msgl/64)){
            result3.append(str,0,2*(msgl%64));
        }
        else{result3.append(str);}
    }
    cout<<"keystream_gen:"<<result3<<endl;
    return result3;
}

string chacha_encryption(string msg,
mpz_class &key,mpz_class &counter,mpz_class &nonce){
    string msg_encrypted;
    int msgl=msg.length()/2;
    auto keystream_gen=chacha_keystream_gen(key,counter,nonce,msgl);
    mpz_class key1=mpz_class(keystream_gen,16);
    mpz_class msg1=mpz_class(msg,16);
    cout<<"\nchangebefore:"<<msg1.get_str(16)<<endl;
    cout<<"\noperationkey:"<<key1.get_str(16)<<endl;
    msg1^=key1;
    cout<<"\nchangeafter:"<<msg1.get_str(16)<<endl;
    msg_encrypted=msg1.get_str(16);
    if(msg_encrypted.length()<msgl){
        int lack=msgl-msg_encrypted.length();
        string zero="00000000000000";
        msg_encrypted.append(zero,0,lack);
    }
    cout<<"\nplaintext:"<<msg<<endl;
    cout<<"\nkey:"<<keystream_gen<<endl;
    cout<<"ciphertext:"<<msg_encrypted<<endl;
    return msg_encrypted;
}

string poly1305_key_gen(mpz_class &key, mpz_class &nonce){
    mpz_class counter=0;
    auto state=chacha_gen(key,counter,nonce);
    show(state);
    chacha_block(state);
    vector<mpz_class> state1(state.begin(),state.begin()+8);
    auto state2=special_change2(state1);
    cout<<"\npoly_key_gen:"<<state2<<endl;
    //show(state2);
    return state2;
}



string special_read(std::ifstream& fin, int length ){
    char* read1=new char [length];
    //for(int i=0;i<length;++i){
    //    fin.read(read1,1);
    //}
    fin.read(read1,length);
    string read2(read1,length);
    read2.erase(std::remove(read2.begin(), read2.end(), ' '), read2.end());
    read2.erase(std::remove(read2.begin(), read2.end(), ':'), read2.end());
    cout<<"\nread:"<<read2<<endl;
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


int main(int argc,char** argv){
    
    std::string file_name1=(std::string)argv[1];
    std::string file_name2=(std::string)argv[2];
    std::string file_name3=(std::string)argv[3];

    int length1=get_file_size_in_byte(file_name1);
    int length2=get_file_size_in_byte(file_name2);
    int length3=get_file_size_in_byte(file_name3);

    std::ifstream in1;
    std::ifstream in2;
    std::ifstream in3;
    in1.open(file_name1);
    in2.open(file_name2);
    in3.open(file_name3);

    auto msg1=special_read(in1,length1);
    auto msg2=special_read(in2,length2);
    auto msg=special_read(in3,length3);

    
    mpz_class a = 0x11111111;
    mpz_class b = 0x01020304;
    mpz_class c = 0x9b8d6f43;
    mpz_class d = 0x01234567;
    
    chacha_quarter(a,b,c,d);
    cout<<"\na:"<<a.get_str(16)<<endl;
    cout<<"\nb:"<<b.get_str(16)<<endl;
    cout<<"\nc:"<<c.get_str(16)<<endl;
    cout<<"\nd:"<<d.get_str(16)<<endl;
    cout<<"start"<<endl;
    auto key=special_change(msg1);
    //string msg=key.get_str(16);
    cout<<"start"<<endl;
    auto nonce=special_change(msg2);
    mpz_class counter=1;
    auto state=chacha_gen(key,counter,nonce);
    show(state);
    chacha_block(state);
    auto result=chacha_keystream(state);
    auto state5=chacha_keystream_gen(key,counter,nonce,114);
    auto state6=chacha_encryption(msg,key,counter,nonce);
    //auto state7=poly1305_key_gen(key,nonce);
    return 0;
}