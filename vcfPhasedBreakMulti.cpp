
#include "vcf_parser.hpp"
#include <algorithm>
#include <iostream>
using namespace std;
using namespace GTParser;

int main() {

  // expect vcf formatted file as input
  string input;

  // pass on header
  unsigned lineNum(0);
  while (getline(cin, input)) {
    ++lineNum;
    if (input.size() < 2)
      continue;
    if (input[0] == '#' && input[1] == '#') {
      cout << input << "\t";
    } else if (input[0] == '#' && input[1] == 'C')
      break;
    else {
      cerr << "Unexpected line in header at line " << lineNum << endl;
      cerr << "Unexpected line: " << input << endl;
      exit(1);
    }
  }

  // count number of columns in chrom line
  ++lineNum;
  size_t numCols = std::count(input.begin(), input.end(), '\t') + 1;
  if (numCols < 7) {
    cerr << "Too few columns in VCF CHROM line (less than 7 columns)" << endl;
    cerr << "Broken line: " << input << endl;
    exit(1);
  }

  // parse lines
  vcf_grammar<string::const_iterator> grammar(lineNum);
  while (getline(cin, input)) {
    ++lineNum;

    

    
    // data to hold results of parsing
    // parsedFirstFewCols will hold columns 1-9, but excluding 2, which will be
    // in genomic_pos

    vector<string> firstCols(8);
    unsigned genomic_pos;

    vector<t_genotype> genotypes;

    //
    // pos will point to where parsing stops.
    // Should == line.cend() if grammar is intended to consume whole line
    // Can check for true as well as "success"
    // Note that the destination of the parse are in a variadic list of
    // arguments (contig, genomic_pos etc.)
    // This list must be <= 9 (google SPIRIT_ARGUMENTS_LIMIT)
    //
    string::const_iterator pos = input.cbegin();
    bool success =
        qi::parse(pos, input.cend(), grammar, firstCols[0], genomic_pos,
                  firstCols[1], firstCols[2], firstCols[3], firstCols[4],
                  firstCols[5], firstCols[6], firstCols[7], genotypes);
    if (!success) {
      cerr << "Parse failed" << endl;
      exit(1);
    }

    vector<

  }

  return 0;
}
