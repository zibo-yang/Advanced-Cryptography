This TD5 is the most difficult algorithm I have ever implemented. There were several weird errors I encountered and toughly solved:
In order to implement the modular division. I, to begin with, create a new function using loop to calculate fraction number in the modulo since it can avoid the case for mpz_invert doesn't work. But it finally resulted in the phenomenon that Xp=0 and Zp=0. After 3 days of hard analysis and discussing with 3 classmates, I undertood that the meaning of draging off first couples of primes smaller than 10000 will hugely avoid the failure of mpz_invert, which means using mpz_invert should be persisted.

Here are the outcome factors of my challenges,sorry I forgot to print the data of A and initial point before 23:30, so you can see that only my first test have A and initial point, now you can directly see those data after compiling and running my code:
first N with 6:102217--almost 1 minutes
initi x:249668246843

initi z:1007188495495

N:1077608514337

A:559457960446

factor gcd:102217


second N with 10:1019822227 --almost 10 minutes
third N with 13: 105664713542411  ---almost 10 minutes
fourth N with 15: 107311619822233  ---almost 20 minutes
fifth N with 17:10977311619822259  ----almost 40 inutes.
initi x:8607975920191700813885085351061881

initi z:10840422813415783873390124459928522

N:11480793314033099387519554806147053

A:5493838454062784179611303314630975

factor gcd:10977311619822259

sixth N with 20: 1048145865664713542383 almost 2 hours 
initi x:5669333305761018914057596609713703171361

initi z:7986234195821534566206056179646027264014

N:11326240443641012579078572186599317031707

A:9033170259158126695988071812920524027380

factor gcd:1048145865664713542383
seventh N with 25: no result before 23:45 about 1 hours.

usage: 
g++ ecm1.cpp -lgmp -lgmpxx
./a.out factor_number.txt factor_digits.txt
(please save factor number and factor digits in these two files.)

