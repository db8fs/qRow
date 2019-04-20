#ifndef PARSING_H
#define PARSING_H

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/bind.hpp>
#include <boost/spirit/include/classic_actor.hpp>
#include <boost/spirit/include/qi_lazy.hpp>

//using namespace boost::spirit;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

using qi::int_;
using qi::lit;
using qi::double_;
using qi::lexeme;
using ascii::char_;
using qi::space;
using qi::string;
using qi::int_parser;
using qi::eps;
using boost::phoenix::ref;

using GrammarString = std::string;
using Iterator = GrammarString::const_iterator;

template<typename T> using LexerSymbols = qi::symbols<char, T>;
template<typename T> using GrammarRuleData = qi::rule<Iterator, T, ascii::space_type>;

using GrammarRule = qi::rule<Iterator, ascii::space_type>;
using Grammar = qi::grammar<Iterator, ascii::space_type>;



#endif // PARSING_H
