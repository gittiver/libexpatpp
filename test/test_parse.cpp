#include "catch2/catch.hpp"

#include <cstdio>
#include <cstdbool>
#include <cassert>

#include "xmlparser.hpp"
/// TODO make same test for parsefile and parse function

class empty_delegate : public xmlpp::abstract_delegate {
  void onStartElement( const XML_Char *  /* fullname */,
		       const XML_Char ** /* atts*/ ) override
  {}

  void onEndElement(  const XML_Char * /* fullname */ ) override
  {}

  void onCharacterData(const char * /* pBuf*/, int /* len */) override {
  }


};

using xmlpp::parser;

TEST_CASE("parseString")
{
  empty_delegate d;

  SECTION("invalid filename") {
    std::string empty;
    REQUIRE(parser::parseString(nullptr,d)==xmlpp::parser::result::INVALID_INPUT);
  }

  SECTION("parse_wellformed_string") {
    empty_delegate d;
    REQUIRE(parser::parseString("<p></p>",d)==xmlpp::parser::result::OK);
  }

  SECTION("prse_not_wellformed_string") {
    empty_delegate d;
    REQUIRE(parser::parseString("<p></a>",d)==xmlpp::parser::result::PARSE_ERROR);
  }

}
