#if !defined(QI_PARSE_ERROR_HANDLER_H)
#define QI_PARSE_ERROR_HANDLER_H
#include <string>
#include <sstream>
#include <stdexcept>
#include <exception>
#include <boost/spirit/include/qi.hpp>


// anonymous namespace for common function
namespace
{
	template <typename Iter>
	std::string parsing_error_to_msg(const std::string& error_name, Iter beg, Iter end, Iter errPos, boost::spirit::qi::info const &what, unsigned curr_line_number)
	{
		Iter search_end = end;
		if (search_end - errPos > 1000)
			search_end = errPos + 1000;
		Iter line_end = errPos;
		while (line_end != search_end && *line_end != '\n')
			++line_end;

		std::string line = std::string(beg, line_end);
		std::string line_from_error = std::string(errPos, line_end);
		std::string line_to_error = std::string(beg, errPos);
		std::string line_to_error_spaces;
		for (unsigned i = 0; i < line_to_error.length(); ++i)
			line_to_error_spaces += (line_to_error[i] == '\t') ? '\t' : ' ';
		std::stringstream msg;
		msg
				<< "Parsing " << error_name << "\n"
				<< "Expecting '"
				<< what
				<< "' here:\n'" << line_from_error << "'"
				<< "\n" 
				<< "line #"   << curr_line_number
				<< ", column #" << line_to_error.length() + 1
				<< " ("  << line_from_error.length() << " from the end)\n"
				<< "[" << line << "]\n "
				<< line_to_error_spaces << "^\n"
				<< std::endl;
		return msg.str();

	}
}

//!_______________________________________________________________________________________
//!     
//!		errorHandlerT
//!			exception throwing parsing error handler
//!     
//!		\author lg (4/13/2011)
//!_______________________________________________________________________________________
struct errorHandlerT  
{
    //      4 input paramters, returns void
    template <typename, typename, typename, typename, typename, typename, typename>
    struct result { typedef void type; };
    errorHandlerT(const std::string& error_name_):error_name(error_name_){}
    std::string error_name;
    
    template <typename Iter>
    void operator()(Iter beg, Iter end, Iter errPos, boost::spirit::qi::info const &what, 
					unsigned curr_line_number, std::string& error_msg, bool throw_on_error) const
    {
		error_msg = parsing_error_to_msg(error_name, beg, end, errPos, what, curr_line_number);
		if (throw_on_error)
			throw std::runtime_error(error_msg);
	}
};



#endif	// QI_PARSE_ERROR_HANDLER_H




