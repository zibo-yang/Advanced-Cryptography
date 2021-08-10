In this TD8, we succeed in implementing chacha20 and aead algorithm. To run the program,please follow the instruction below:

1. if you want to check chacha20:
g++ chacha20.cpp -lgmp -lgmpxx
./a.out key1.txt nonce.txt msg1.txt
(here I have already saved all the test input data from RFC 8439)

2. if you want to check aead:
g++ aead.cpp -lgmp -lgmpxx
./a.out key.txt plaintext.txt aad.txt iv.txt constant.txt
(here I have already saved all the test input data from RFC 8439)

all the output should be consistent with RFC 8439