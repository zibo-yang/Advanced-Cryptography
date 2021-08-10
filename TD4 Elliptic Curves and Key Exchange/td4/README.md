This is the implementation of our Montgomery function and X25519. I have already succeed in all test including RFC7748 and test vectors provided by teacher. 

I personally found two weird points during my understanding and implementation. 
First, the XDBL function is different in RFC7748 description and slides at defining z-coordinate.
Second, the decoding for U is useless here in our implementation.


usage:
g++ test.cpp -lgmp -lgmpxx
./a.out he1.txt he2.txt
here I have already put test vectors into this he1.txt(scalar k) and he2.txt(u-coordinate)