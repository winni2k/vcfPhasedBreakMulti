
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

//
// boost format for debugging
//
#include <boost/format.hpp>

//
//	detailed error handler
//
#include "qi_parse_error_handler.h"
phoenix::function<errorHandlerT> const errorHandler_vcf_lines_handler =
    errorHandlerT("VCF Parser");

//
//	Per genotype data goes in a vector of t_genotype
// 	But boost::spirit doesn't know about your class (no introspection in
// c++)
// 	So we adapt it via BOOST_FUSION_ADAPT_STRUCT
// 	Spirit will then know to write straight to your class
//
// 	N.B. BOOST_FUSION_ADAPT_STRUCT needs to live in global namespace. Change
// at your peril
//
//
#include <boost/fusion/include/adapt_struct.hpp>
#include <array>
struct t_genotype {
  unsigned allele1;
  char phase;
  unsigned allele2;
};

BOOST_FUSION_ADAPT_STRUCT(t_genotype,
                          (unsigned, allele1)(char, phase)(unsigned, allele2))

namespace GTParser {

//
//	grammar using variation of "curiously recurring template pattern"
// 	N.B. Must be defined from what it emits so there must be agreement of
// types
// 			qi::grammar<Iterator, ????? >
// 			vcf_grammar::base_type( ????? )
// 			qi::rule<Iterator, ????? > () > 	vcf_line;
// 	N.B. note that these are all reference types because the values are
// stored...
//
//
template <typename Iterator>
struct vcf_grammar
    : qi::grammar<Iterator, boost::fusion::vector<
                                std::string &, unsigned &, std::string &, std::string &,
                                std::string &, std::string &, std::string &, std::string &,
                                std::string &, std::vector<t_genotype> &>()> {
  vcf_grammar(unsigned &cnt_line_)
      : vcf_grammar::base_type(vcf_line), cnt_line(cnt_line_) {
    using qi::lit;
    using qi::omit;
    using namespace qi::labels;

    // contig is non-whitespace
    tab = lit('\t');
    contig %= +qi::graph;
    pos %= qi::uint_;
    // omit = not saved
    id %= +qi::graph;
    ref %= +qi::graph;
    alt %= +qi::graph;
    qual %= +qi::graph;
    filter %= +qi::graph;
    info %= +qi::graph;
    format %= +qi::graph;

    allele1 %= qi::uint_;
    phase %= qi::char_("|");
    allele2 %= qi::uint_;

    genotype %= allele1 > phase > allele2;
    genotypes %= genotype % "\t";

    //
    // 	We use the ">" expect operator which means no backtracking (return
    // false) once
    // 		the contig is parsed. Failures will throw
    // 	Use ">>" sequence operator otherwise
    //
    vcf_line %= contig > tab > pos > tab > id > tab > ref > tab > alt > tab >
                qual > tab > filter > tab > info > tab > format > tab >
                genotypes;

    //
    // names for error messages
    //
    tab.name("TAB");
    contig.name("contig/chromosome");
    pos.name("position");
    id.name("id");
    ref.name("ref");
    alt.name("alt");
    qual.name("qual");
    filter.name("filter");
    info.name("info");
    format.name("format");
    allele1.name("allele1");
    phase.name("phase");
    allele2.name("allele2");
    genotype.name("genotype");
    genotypes.name("genotypes");

    //
    // Error handler.
    // 		Note sends
    // 		_1 = error_name,
    // 		_2 = beg,
    // 		_3 = end
    // 		_4 = errPos,
    // 		ref to line number (This is a runtime parameter passed in the
    // constructor)
    // 		reference to error message
    // 		flag saying whether an error should be thrown.
    // 		This can be set as a runtime parameter like "cnt_line"
    //
    qi::on_error<qi::fail>(vcf_line, ::errorHandler_vcf_lines_handler(
                                         _1, _2, _3, _4, phoenix::ref(cnt_line),
                                         phoenix::ref(error_msg), true));
  }

  std::string error_msg;

  // reference to a line count define outside this class
  unsigned &cnt_line;

  //
  // emittors of differenc symbols. Note the function declaration syntax: e.g.
  // "unsigned()"
  //
  qi::rule<Iterator, void()> tab;
  qi::rule<Iterator, std::string()> contig;
  qi::rule<Iterator, unsigned()> pos;
  qi::rule<Iterator, std::string()> id;
  qi::rule<Iterator, std::string()> ref;
  qi::rule<Iterator, std::string()> alt;
  qi::rule<Iterator, std::string()> qual;
  qi::rule<Iterator, std::string()> filter;
  qi::rule<Iterator, std::string()> info;
  qi::rule<Iterator, std::string()> format;
  qi::rule<Iterator, unsigned()> allele1;
  qi::rule<Iterator, char()> phase;
  qi::rule<Iterator, unsigned()> allele2;
  qi::rule<Iterator, t_genotype()> genotype;
  qi::rule<Iterator, std::vector<t_genotype>()> genotypes;
  qi::rule<Iterator,
           boost::fusion::vector<std::string &, unsigned &, std::string &, std::string &,
                                 std::string &, std::string &, std::string &, std::string &,
                                 std::string &, std::vector<t_genotype> &>()> vcf_line;
};
};
