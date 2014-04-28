#include <iostream>
#include "vcf_parser.hpp"
using namespace std;
using namespace GTParser;

int main(int argc, char *argv[]) {
  // test string
  vector<string> test_strings = {
    "20\t60309\tblarg\tG\tT\t100\tPASS\t.\tGT\t1|15\t0|0\t0|0",
    "20\t60828\trs124234\tT\tG\t100\tPASS\t.\tGT\t1|0\t0|0\t0|0",
    "20\t61044\thoinky\tC\tA,ATTTTA\t100\tPASS\t.\tGT\t0|0\t0|0\t0|0",
  };

  unsigned cnt_lines = 0;
  vcf_grammar<string::const_iterator> grammar(cnt_lines);

  for (const auto &line : test_strings) {

    // data to hold results of parsing
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
    string::const_iterator pos = line.cbegin();
    bool success =
        qi::parse(pos, line.cend(), grammar, firstCols[0], genomic_pos,
                  firstCols[1], firstCols[2], firstCols[3], firstCols[4],
                  firstCols[5], firstCols[6], firstCols[7], genotypes);
    if (!success) {
      cerr << "Parse failed" << endl;
      exit(1);
    }

    // output parsing results
    unsigned colNum = 0;
    stringstream ssLine;
    for (auto field : firstCols) {
      if (colNum++ != 0)
        ssLine << "\t";
      ssLine << field;
      if (colNum == 1)
        ssLine << "\t" << genomic_pos;
    }

    //    std::cerr << boost::format("cnt_genotypes = %d\n") % genotypes.size();
    for (const auto &gg : genotypes) {
      ssLine << "\t" << gg.allele1 << gg.phase << gg.allele2;
    }
    if (test_strings[cnt_lines] != ssLine.str()) {
      cerr << "mismatch in line:" << endl << test_strings[cnt_lines] << endl
           << "!=" << endl << ssLine.str() << endl;
      exit(1);
    }

    ++cnt_lines;
  }
}
