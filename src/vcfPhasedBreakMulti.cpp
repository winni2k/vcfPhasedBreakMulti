
#include "vcf_parser.hpp"
#include <algorithm>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
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
      cout << input << "\n";
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
  // print crom line
  cout << input << "\n";

  // parse lines
  vcf_grammar<string::const_iterator> grammar(lineNum);
  while (getline(cin, input)) {
    ++lineNum;

    // don't parse this line if it's biallelic
    size_t startAlt = 0;
    for (unsigned i = 0; i < 4; ++i) {
      startAlt = input.find('\t', startAlt + 1);
    }
    size_t endAlt = input.find('\t', startAlt + 1);
    string alt = input.substr(startAlt + 1, endAlt - startAlt - 1);

    // ',' means this line has more than two alleles
    if (alt.find(',') == string::npos) {
      cout << input << "\n";
      continue;
    }

    // otherwise parse the line and do some splitting!

    // data to hold results of parsing
    // parsedFirstFewCols will hold columns 1-9, but excluding 2, which will be
    // in genomic_pos

    // #CHROM  POS     ID      REF     ALT     QUAL    FILTER  INFO    FORMAT
    // [SAMPLE1 .. SAMPLEN]
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
    vector<string> alts;
    boost::split(alts, firstCols[3], boost::is_any_of(","));
    assert(alts.size() > 1);

    // throw away info information as it is unlikely to still be true
    firstCols[6] = ".";

    // loop to print out each alternate allele's own line
    unsigned altNum = 0;
    for (auto alt : alts) {
      ++altNum;
      // print out first cols
      cout << firstCols[0] << "\t" << genomic_pos << "\t.\t" << firstCols[2]
           << "\t" << alt;
      for (unsigned i = 0; i < 4; ++i)
        cout << "\t" << firstCols[4 + i];

      // print out genotypes
      for (auto gg : genotypes) {
        cout << "\t";
        if (gg.allele1 == altNum)
          cout << 1;
        else
          cout << 0;
        cout << gg.phase;
        if (gg.allele2 == altNum)
          cout << 1;
        else
          cout << 0;
      }
      cout << "\n";
    }
  }

  return 0;
}
