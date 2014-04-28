### Makefile --- 

## Author: wkretzsch@gmail.com
## Version: $Id: Makefile,v 0.0 2014/04/25 12:33:46 winni Exp $
## Keywords: 
## X-URL: 

CPP := g++
CPPFLAGS := -Wall -std=c++11

all:
	$(MAKE) -C src

test:
	$(MAKE) -C src
	cd t && ./runTests.pl

clean:
	$(MAKE) -C src clean
	$(MAKE) -C t clean

.PHONY: test all clean

### Makefile ends here
