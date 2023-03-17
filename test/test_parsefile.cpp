#include "catch2/catch_all.hpp"

#include <cstdio>
#include <cstdbool>
#include <cassert>

#include "xmlparser.hpp"
/// TODO make same test for parsefile and parse function

class empty_delegate : public xmlpp::abstract_delegate {
  void onStartElement( const XML_Char *,
		       const XML_Char **) override
  {}

  void onEndElement(  const XML_Char *) override
  {}

  void onCharacterData(const char *, int ) override {
  }


};

class ParseFileFixture {
public:  
ParseFileFixture() {
  FILE* x_xml= fopen("x.xml","w");
  if  (!x_xml)
    perror("fopne");
  fputs("<p></p>",x_xml);
  fclose(x_xml);

  FILE* invalid_xml= fopen("invalid_xml.xml","w");
  if  (!invalid_xml)
    perror("fopen");
  fputs("<p></a>",invalid_xml);
  fclose(x_xml);
}

~ParseFileFixture(){
  remove("x.xml");
  remove("invalid_xml.xml");
}
};

using xmlpp::parser;

TEST_CASE_METHOD(ParseFileFixture,"parse file")
{
  
  SECTION("invalid filename") {
    std::string empty;
    empty_delegate d;
    REQUIRE(parser::parseFile(empty,d)==xmlpp::parser::result::ERROR_OPEN_FILE);
    REQUIRE(parser::parseFile("file_does_not_exist",d)==xmlpp::parser::result::ERROR_OPEN_FILE);
  }

  SECTION("existing_file_wellformed") {
    empty_delegate d;
    REQUIRE(parser::parseFile("x.xml",d)==xmlpp::parser::result::OK);
  }

  SECTION("existing_file_not_wellformed") {
    empty_delegate d;
    REQUIRE(parser::parseFile("invalid_xml.xml",d)==xmlpp::parser::result::PARSE_ERROR);
  }
}
