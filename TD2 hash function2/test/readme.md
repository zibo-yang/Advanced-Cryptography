The code in main.cpp implements 2 different ways to find collision and establish the injective function "gconj"(same feature as the g_m in the article).

command:
$ make
$ ./shake128 test.txt <number of bytes N> <number of parallel thread M(suggest 9)>


Here is the description about my implementation for "gconj":

when provided vector<bool> a translated binaryly by hash results, I generate the bijection between a[i] and whether the ith space will be increased.For example: the modification of message "I have a funny toy."after vector (0101) is:
"I have  a funny  toy."
       ^
       |
"I have a funny toy."

obviously it is an injective function.

Before the Tuesday, I implement the Floyd cycle-finding algorithm(I have commented all this algorithm in my main.cpp). Based on my experiment,the algorithm perform badly since:
1. it cannot even offer 2 bytes collision in time
2. weird segmentation when,based on my modificaion approach explained before, the number of space is "not enough" comparing with N bytes, which means only the large volume of text with respect to N bytes can avoid segmentation.

Finally I decided to implement to parallel computing. Since time is not enough for me to familiarize the MPI, I only used the stupid way to implement. Based on my experiment:
1. I got 1 bytes collisions only in a few seconds with 2 parallel threads.
2. 2 bytes collisions took almost 1 minute with 4 parallel threads.
3. 3 bytes collisions took more than 10 minutes with 4 parallel threads.
4. never get 4 bytes collisions after 1 hour computing with 8 parallel threads.
I think 3 is my best result because my mathematical calculation indicates there needs at least 32 thread to possibly get 4 bytes collision and it should be about 10 bytes as the limit if I utilize MPI to program the real parallel computing.

when you check the hash.please take care of two things.

1. please take care of EOF when you read our file. There is no EOF in our hashing.
2. if you want to use the hash function shake128 in my folder, please set shake128(a,N*8) rather than shake128(a,N).
 
