SHAKE128 implementation

usage: 1. run make command in your terminal.
       2. ./shake128 <truncated number=32> <filename>

This implementation is not successful to get the correct solution. The below are main diffculies I encountered:

1. understanding the different endian system in different implemenation.

2. whether using slicing function in implementation of 5 functions became our main topic of discussion for almost 3 days. We finally were inclined to believe that whether using it or not will not cause the failure.

3. the binary_read function in the main.cpp is one of the only two functions I copy from github:https://github.com/CHoudrouge4/shake128/blob/master/README.md.(another one is get_file_size_in_byte function). I personally think this function deserves my most doubt and confusion to cause my failure. Because, as you can see the printing at the very beginning. the number and string we read is absolutely wrong. If you get the chance to restart, I will choose to write the reading part by myself.

4. the lack of data in the process increases the difficulty to recover the right codes if there are tiny but fatal errors. 

Anyway, I personally still love this implementation, which makes me improve a lot.
