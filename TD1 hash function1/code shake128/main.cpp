#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include "keccak.h"
#include <vector>
#include <bitset>
#include <algorithm>


void hex_write(/*std::ofstream& fout*/ const std::vector<bool>& x) {
    std::vector<bool>::size_type n = x.size();
    /*for(size_t i = 0; i < x.size(); i += w) {
      std::reverse(x.begin() + i, x.begin() + i + w);
    }*/
    for(std::vector<bool>::size_type i = 0; i < n;) {
        unsigned char aggr = 0;
        for(unsigned char mask = 1; mask > 0 && i < n; ++i, mask <<= 1)
            if(x.at(i))
                aggr |= mask;
        //fout.write((const char*)&aggr, sizeof(unsigned char));
        int aggr1=(int)aggr;
        std::cout<<std::hex<<aggr1;
    }
}/*
void hex_write2( const std::vector<bool>& x) {
    std::vector<bool>::size_type n = x.size();
    for(std::vector<bool>::size_type i = 0; i < n;) {
        unsigned char aggr = 0;
        unsigned char mask1= 1;
        mask1<<=(sizeof(unsigned char)-1);
        for(unsigned char mask = mask1; mask > 0 && i < n; ++i, mask >>= 1)
            printf("please type");
            std::cout<<std::hex<<(int)i<<std::endl;
            if(x.at(i))
                aggr |= mask;
        //fout.write((const char*)&aggr, sizeof(unsigned char));
        //int aggr1=(int)aggr;
        std::cout<<std::hex<<(int)aggr;
    }
}*/

void binary_read(std::ifstream& fin, std::vector<bool>& x, std::vector<bool>::size_type n) {
    x.resize(n);
    for(std::vector<bool>::size_type i = 0; i < n;)
    {
        unsigned char aggr;
        fin.read((char*)&aggr, sizeof(unsigned char));
        //std::cout<<"aggr"<<aggr<<std::endl;
        for(unsigned char mask = 1; mask > 0 && i < n; ++i, mask <<= 1){
            x.at(i) = aggr & mask;
            std::cout<<x[i];
        }
    }
}

// this function return the file size
int get_file_size_in_byte(const std::string file_name) {
  int size_f = 0;
  std::ifstream inf(file_name);

  inf.seekg(0, std::ios::end);
  int file_size = inf.tellg();
  inf.seekg(0, std::ios::beg);
  std::cout << "file size = " << file_size << std::endl;
  return file_size * 8;
}
int main(int argc, char *argv[]){
    // input part
	std::cout<<"please"<<std::endl;
    int d=atoi(argv[1]);
    std::string file_name=(std::string)argv[2];
    std::vector<bool>::size_type length=get_file_size_in_byte(file_name);
    std::ifstream in;
    std::vector<bool> input_vector(length);
    std::cout<<"now we are reading the vector"<<std::endl;
    in.open(file_name,std::ios::binary);
    binary_read(in,input_vector,length);
    in.close();

    //processing part
    std::cout<<"now we are hashing the vector"<<std::endl;
    auto output_vector=SHAKE128(input_vector,d);

    // output part
    std::cout<<"now we are printing the vector"<<std::endl;
    hex_write(output_vector);
    std::cout<<"\n";
    //hex_write2(output_vector);

    // test
    /*unsigned char a=1;
    for(int i=0;a>0;a<<=1){
        int a1=(int)a;
        std::cout<<a1<<" ";
        std::cout<<std::hex<<(int)a<<std::endl;
    }
    return 0;*/
}
