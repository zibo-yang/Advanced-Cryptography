In TD3, we produced two file(please make sure your correct installation and environment setting for GMP):

poly1305_gen----generate the authenticator
   usage: g++ poly1305_gen.cpp -lgmp -lgmpxx
          ./a.out msg.txt key.txt 
          
poly1305_check--verify the authenticator
   usage: g++ poly1305_check.cpp -lgmp -lgmpxx
          ./a.out msg.txt key.txt auth.txt

I copied the correct data about message,key and authenticator from RFC 8439 into msg.txt,key.txt and auth.txt.As you can compile and run for my poly1305_check, it will return ACCEPT

In the generating part, I used transfomation bewteen vector<char> and mpz_class, which,as I changed my thought after discussing with three classmates, is very stupid since std::string can serve all the works for vector.
