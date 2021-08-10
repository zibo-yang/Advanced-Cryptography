#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <bitset>
#include <cmath>
#include <unordered_map>
#include <sstream>


// representation of the state as 3d vector
typedef std::vector<std::vector<std::vector<bool>>> state;
typedef std::bitset<8> byte;
typedef std::bitset<4> half_byte;

state theta(state &A);
state rho(state &A);
state pi(state &A);
state chi(state &A);
bool rc(int t);
state iota(state &A, int ir);
state RND(state &A,int ir);
state string_to_state(std::vector<bool> S,int w);
std::vector<bool> state_to_string(state &A);
std::vector<bool> keccak_p(std::vector<bool> S,int b, int nr);
std::vector<bool> pad101(std::vector<bool> M,int r);
std::vector<bool> sponge(int b,int r ,int d, std::vector<bool> M);
std::vector<bool> SHAKE128(std::vector<bool> M,int d);
std::vector<bool> append(std::vector<bool> a, std::vector<bool> b);
std::vector<bool> copy(std::vector<bool> a, int n);
