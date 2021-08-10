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

void show2(vector<mpz_class> state1){
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
        show2(state3);
        chacha_diagonalquarter(state3);
        show2(state3);
    }
    for(int j=0;j<state3.size();++j){
        state3[j]+=init[j];
        state3[j]%=modulo;
    }
    show2(state3);
    //show(init);
}

mpz_class special_change(string m){
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
    show2(result);
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
    show2(state);
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
    cout<<"\nprintf:::"<<read2<<endl;
    read2.erase(std::remove(read2.begin(), read2.end(), ' '), read2.end());
    read2.erase(std::remove(read2.begin(), read2.end(), ':'), read2.end());
    cout<<"\nread:"<<read2<<endl;
    return read2;
}


//here is the poly1305 part
void show(std::vector<char> x){
    //printf("now we print");
    for(auto i:x)
      std::cout<<i;
    cout<<endl;
}
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
        //cout<<"first"<<endl;
        //show(mes);
        std::reverse(mes.begin(), mes.end());
        int n=(int)mes.size();
        for(int i=0;i<(n/2);++i){
            char k=mes[2*i];
            mes[2*i]=mes[2*i+1];
            mes[2*i+1]=k;
        }
        //cout<<"second"<<endl;
        //show(mes);

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
        //cout<<"first"<<endl;
        //show(mes);
        std::reverse(mes.begin(), mes.end());
        int n=(int)mes.size();
        for(int i=0;i<(n/2);++i){
            char k=mes[2*i];
            mes[2*i]=mes[2*i+1];
            mes[2*i+1]=k;
        }
        //cout<<"second"<<endl;
        //show(mes);

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
vector<char> poly1305_mac(vector<char> &key,vector<char> &message){
    std::vector<char> r(key.begin(),key.begin()+32);
    reverse1(r);
    printf("r before\n");
    show(r);
    r=clamp(r);
    printf("r after\n");
    std::vector<char> s(key.begin()+32,key.begin()+64);
    reverse1(s);
    printf("\nr:\n");
    show(r);
    printf("\ns:\n");
    show(s);
    mpz_class a=0;
    mpz_class p=(((mpz_class)1)<<130)-5;
    int l=(int)message.size()/32;
    
    for(int i=0;i<l;++i){
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
    cout<<"\nwe see tag(poly_mac) directly\n"<<endl;
    show(tag);
    return tag;
}

//aead part

string pad16(string x1){
    string x=x1;
    cout<<"\noriginal"<<x<<endl;
    int lack=32-x.length()%32;
    string zero="000000000000000000000000000000000000";
    if(lack!=32){
        x.append(zero,0,lack);
    }
    cout<<"\nend"<<x<<endl;
    return x;
}

string num_to_8_le_bytes(long int a){
    string res;
    for(int i=0;i<8;++i){
        mpz_class tmp=(a>>(8*i))& 0xff;
        string tmps=tmp.get_str(16);
        res.append(tmps);
    }
    cout<<"\nnum_to_8_le_bytes:"<<res<<endl;
    return res;
}
string num_to_8_le_bytes1(long int a){
    mpz_class b=a/2;
    string zero="0000000000000000";
    string b1=b.get_str(16);
    reverse(b1.begin(),b1.end());
    if(b1.length()<16){
        int lack=16-b1.length()%16;
        b1.append(zero,0,lack);
    }
    
    //cout<<"\nvasdv:"<<b1<<endl;
    //reverse(b1.begin(),b1.end());
    for(int i=0;i<b1.length();++i){
        if(i%2==0){
            swap(b1[i],b1[i+1]);
        }
    }
    return b1;
}

vector<char> aead(string &aad,mpz_class &key,string &iv,
string &constant,string &plaintext,string &ciphertext){
    string nonce1;
    nonce1.append(constant);
    nonce1.append(iv);
    auto nonce=special_change(nonce1);
    string otk=poly1305_key_gen(key,nonce);
    mpz_class counter=1;
    ciphertext=chacha_encryption(plaintext,key,counter,nonce);
    cout<<"\naead_ciphertext:"<<ciphertext<<endl;
    string mac_data=pad16(aad);
    mac_data.append(pad16(ciphertext));
    mac_data.append(num_to_8_le_bytes1(aad.length()));
    mac_data.append(num_to_8_le_bytes1(ciphertext.length()));
    cout<<"\nmac_data:"<<mac_data<<endl;
    cout<<"\notk:"<<otk<<endl;
    //renew
    vector<char> otk_c(otk.begin(),otk.end());
    vector<char> mac_data_c(mac_data.begin(),mac_data.end());
    auto tag=poly1305_mac(otk_c,mac_data_c);
    printf("\ntag:");
    show(tag);
    return tag;
    
}




int main(int argc,char** argv){
    
    std::string file_name1=(std::string)argv[1];//key;
    std::string file_name2=(std::string)argv[2];//plaintext;
    std::string file_name3=(std::string)argv[3];//aad;
    std::string file_name4=(std::string)argv[4];//iv;
    std::string file_name5=(std::string)argv[5];//constant;
    int length1=get_file_size_in_byte(file_name1);
    int length2=get_file_size_in_byte(file_name2);
    int length3=get_file_size_in_byte(file_name3);
    int length4=get_file_size_in_byte(file_name4);
    int length5=get_file_size_in_byte(file_name5);
    std::ifstream in1;
    std::ifstream in2;
    std::ifstream in3;
    std::ifstream in4;
    std::ifstream in5;
    in1.open(file_name1);
    in2.open(file_name2);
    in3.open(file_name3);
    in4.open(file_name4);
    in5.open(file_name5);
    auto msg1=special_read(in1,length1);//key;
    auto msg2=special_read(in2,length2);//plaintext
    auto msg3=special_read(in3,length3);//aad
    auto msg4=special_read(in4,length4);//iv
    auto msg5=special_read(in5,length5);//constant

    //generate
    auto key=special_change(msg1);
    string plaintext=msg2;
    string aad=msg3;
    string iv=msg4;
    string constant=msg5;
    string ciphertext;
    

    auto tag=aead(aad,key,iv,constant,plaintext,ciphertext);
    
    /*
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
    string msg=key.get_str(16);
    cout<<"start"<<endl;
    auto nonce=special_change(msg2);
    mpz_class counter=1;
    auto state=chacha_gen(key,counter,nonce);
    show2(state);
    chacha_block(state);
    auto result=chacha_keystream(state);
    auto state5=chacha_keystream_gen(key,counter,nonce,114);
    auto state6=chacha_encryption(msg,key,counter,nonce);
    auto state7=poly1305_key_gen(key,nonce);
    */
    /*
    printf("\n\n\n");
    cout<<"\nmsg1:"<<msg1<<endl;
    vector<char> testkey(msg1.begin(),msg1.end());
    vector<char> testmsg(plaintext.begin(),plaintext.end());
    printf("\ntestkey:");
    show(testkey);
    cout<<"\nmsg1:"<<msg1<<endl;
    auto testtag=poly1305_mac(testkey,testmsg);
    printf("\ntest for tag\n");
    show(testtag);
    */
    return 0;
}