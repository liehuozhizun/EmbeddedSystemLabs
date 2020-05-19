Hang Yuan, hyuan3, 1564348

This lab was about how to use file functions fopen(), fread() and fseek() with their built-in 
parameters, like "r", "SEEK_END", "SEEK_SET". Learning to read/write to files is very 
important since file and standard input are two main source input in programming so we need to
know how like every program or function on a computer works. Other than that, I also learned 
again how to define and use struct properly.

I read the lab like I always do and tried to understand what I was supposed to do. However,
Try to understand how this lab works is a bit difficult since there is no demo output for use
to imitate. Fortunately, that's not hard to implement. The complexity in that regard was about 
average. It's much more complicate than I think before I started my work. On average, I spent
about 10 hours on this lab.

I simply wrote the code to the best of my ability, but have no idea if it truly works since 
I think it's better to give more information on how to dissect the file into different part
with clear format, for instance:

TAG   CONST    Title   ItemRequired  Description
        --- --------- ------------- -------------
DATA    RPG 0x15 .... 0x1 0x3       0x15 ....
        --- ---- ---- --- ---       ---- ----
BYTE(s)  3    1   21   1   1          1   15
             sz   dt  sz   dt        sz   dt
             
             sz = size
             dt = data
             
Instruction in this format will be better