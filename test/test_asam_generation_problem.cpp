#include "catch2/catch_all.hpp"

#include <cstdio>
#include <cstdbool>
#include <cassert>

#include "xmlparser.hpp"
/// TODO make same test for parsefile and parse function

struct empty_delegate : public xmlpp::abstract_delegate {
  void onStartElement( const XML_Char *  fullname,
		       const XML_Char ** /* atts*/ ) override
  {
    //    printf("|%s|\n",fullname);
    elementnames.emplace_back(fullname);
  }

  void onEndElement(  const XML_Char * /* fullname */ ) override
  {}

  void onCharacterData(const char * /* pBuf*/, int /* len */) override {
  }

  std::vector<std::string> elementnames;

};

using xmlpp::parser;

TEST_CASE("parse ASAM mdf v4 fragment")
{
  empty_delegate d;

  const char *SAMPLE ="<HDcomment xmlns=\"http://www.asam.net/mdf/v4\"> <TX>ASAM MDF 4.0 Example file created by ETAS. Contents: 2 simple channel groups containing ints and floats in little endian format. </TX> <time_source>PC timer</time_source> <common_properties> <e name=\"subject\">Example</e> <e name=\"project\">ASAM Example Files</e> <e name=\"department\">ETAS GmbH</e> <e name=\"author\">Tobias Langner</e> </common_properties> </HDcomment>";

  const std::vector<std::string> expected = {
    "http://www.asam.net/mdf/v4:HDcomment",
    "http://www.asam.net/mdf/v4:TX",
    "http://www.asam.net/mdf/v4:time_source",
    "http://www.asam.net/mdf/v4:common_properties",
    "http://www.asam.net/mdf/v4:e",
    "http://www.asam.net/mdf/v4:e",
    "http://www.asam.net/mdf/v4:e",
    "http://www.asam.net/mdf/v4:e",
  };  

  SECTION("parse asam mdf v4") {
    REQUIRE(parser::parseString(SAMPLE,d)==xmlpp::parser::result::OK);
    REQUIRE(expected==d.elementnames);
  }

}

