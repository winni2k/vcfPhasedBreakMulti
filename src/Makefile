### Makefile --- 

## Author: wkretzsch@gmail.com
## Version: $Id: Makefile,v 0.0 2014/04/25 12:33:46 winni Exp $
## Keywords: 
## X-URL: 

BIN := ../bin/vcfPhasedBreakMulti
CPP := g++
CPPFLAGS := -Wall -std=c++11

all: CPPFLAGS += -O3
debug: CPPFLAGS += -O1 -g

all: $(BIN)

$(BIN): vcfPhasedBreakMulti.cpp vcf_parser.hpp
	$(CPP) $(CPPFLAGS) $< -o $@

test: test_vcf_parser
	./$<

test_vcf_parser: test_vcf_parser.cpp vcf_parser.hpp
	$(CPP) $(CPPFLAGS) $< -o $@

clean:
	rm -f test_vcf_parser $(BIN) *.gch *.o

.PHONY: test all clean debug

### Makefile ends here
