/**
 * \file hello_world.cpp the hello_world example for expatpp library
 * 
 * See LICENSE for copyright information.
 */
#include <iostream>
#include "xmlparser.hpp"
#include <vector>

using std::cout;
using std::endl;

using xmlpp::parser;
using xmlpp::delegate;

class element_delegate : public xmlpp::abstract_delegate {
public:
  std::string fullname;
  std::string characterData;

  void onStartElement( const XML_Char *fullname,
		       const XML_Char **atts) override
  {
    this->fullname = fullname;
  }
 
  void onCharacterData(const char *pBuf, int len) override {
    characterData.append(pBuf,len);
  }
};

int main(int argc,char** argv) {
  // prepare xml input
  const char* xml = "<p>hello world!</p>";

  // use the delegate to extract character data from input string
  element_delegate d;

  // parse xml input using delegate d's callbacks
  xmlpp::parser::result res = parser::parseString(xml,d);

  switch(res) {

  case xmlpp::parser::result::OK:
    // and output the extracted data in case of successful parsing
    cout << d.characterData << endl;
    return EXIT_SUCCESS;

  default:
    cout << "error " << static_cast<int>(res)
	 << " on parsing " << endl;
    return -static_cast<int>(res);

  }
}
