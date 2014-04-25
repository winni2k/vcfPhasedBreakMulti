### Makefile --- 

## Author: wkretzsch@gmail.com
## Version: $Id: Makefile,v 0.0 2014/04/25 12:33:46 winni Exp $
## Keywords: 
## X-URL: 

CPP := g++
CPPFLAGS := -Wall -std=c++11

all: vcfPhasedBreakMulti

vcfPhasedBreakMulti: vcfPhasedBreakMulti.cpp vcf_parser.hpp
	$(CPP) $(CPPFLAGS) $< -o $@

test: test_vcf_parser
	./$<

test_vcf_parser: test_vcf_parser.cpp vcf_parser.hpp
	$(CPP) $(CPPFLAGS) $< -o $@

clean:
	rm -f test_vcf_parser vcfPhasedBreakMulti *.gch *.o

.PHONY: test all clean

### Makefile ends here
