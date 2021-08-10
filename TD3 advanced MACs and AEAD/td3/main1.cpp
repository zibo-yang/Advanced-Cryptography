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

using namespace std;

void show(std::vector<char> x){
    printf("now we print");
    for(auto i:x)
      std::cout<<i;
    std::cout<<"\n";
}

std::vector<char> convert_int_to_hex(mpz_class m,int length=10){
    char op=0;
    string hex1=m.get_str();
    std::vector<char> hex2(hex1.begin(),hex1.end());
    show(hex2);
    int n=10-(int)hex2.size();
    std::reverse(hex2.begin(),hex2.end());
    if (n>0){
        for(int i=0;i<n;++i){hex2.push_back('0');}
    }
    std::reverse(hex2.begin(),hex2.end());
    show(hex2);
    printf("vsdfv:%d",n);
    int k=(int)hex2.size()-length;
    for(int i=0;i<k;++i){hex2.pop_back();}
    for(auto j:hex2)
      cout<<j;
    return hex2;
}

int main(int argc, char **argv){
    string x="1c";
    mpz_class m=mpz_class(x,16);
    cout<<"mpz"<<m.get_str(2)<<endl;
    string x1="ab";
    mpz_class m1=mpz_class(x1,16);
    cout<<"mpz"<<m1.get_str(2)<<endl;
    m &= m1;
    cout<<"mpz"<<m.get_str()<<endl;
    vector<char> a=convert_int_to_hex(m);
    vector<char> a1(a.begin(),a.begin()+1);
    printf("vsdfv");
    show(a1);
    return 0;
}