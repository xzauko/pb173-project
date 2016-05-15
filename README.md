# pb173-project
This project aims to provide a fixedpoint arithmetic library with arbitrary base.
Currently the maximal base is limited by 

The Makefile provided does not build any useful program. 
The only reasonable target is `test`, which builds and runs various tests verifying the functionality of `include/fixedpoint.h`.
Test for constructors assumes the following about integral types: 
* short is at least 16 bits
* int and long are at least 32 bits
* long long is at least 64 bits

`include` - the place of the single fixedpoint.h header file

`tests` - contains tests for fixedpoint.h

`docs` - contains Doxyfile for generating documentation (html and manpages formats)


This project uses [Catch framework](https://github.com/philsquared/Catch) for tests
