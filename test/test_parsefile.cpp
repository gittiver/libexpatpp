#include "catch2/catch.hpp"

#include <cstdio>
#include <cstdbool>
#include <cassert>

#include "xmlparser.hpp"

class empty_delegate : public xmlpp::parser::delegate {
  void onStartElement( const XML_Char *fullname,
     const XML_Char **atts) override
  {}

  void onEndElement(  const XML_Char *fullname) override
  {}

  void onCharacterData(const char *pBuf, int len) override {
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

using namespace xmlpp::parser;

TEST_CASE_METHOD(ParseFileFixture,"parse file")
{
  empty_delegate d;

  SECTION("invalid filename") {
    std::string empty;
    REQUIRE(parseFile(empty,d)==result::ERROR_OPEN_FILE);
    REQUIRE(parseFile("file_does_not_exist",d)==result::ERROR_OPEN_FILE);
  }

  SECTION("existing_file_wellformed") {
    empty_delegate d;
    REQUIRE(parseFile("x.xml",d)==result::OK);
  }

  SECTION("existing_file_not_wellformed") {
    empty_delegate d;
    REQUIRE(parseFile("invalid_xml.xml",d)==result::PARSE_ERROR);
  }
}
