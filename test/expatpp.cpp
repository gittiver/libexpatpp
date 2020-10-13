#include <cstdio>
#include <cstdbool>
#include <cassert>

#include "xmlparser.hpp"

class empty_delegate : public xml::parser::delegate {
  void onStartElement( const XML_Char *fullname,
     const XML_Char **atts) override
  {}

  void onEndElement(  const XML_Char *fullname) override
  {}
};

void setup() {
  FILE* x_xml= fopen("x.xml","w");
  if  (!x_xml)
    perror("fopne");
  fputs("<p></p>",x_xml);
  fclose(x_xml);
}

void teardown(){
  remove("x.xml");
}

void test_parseFile_invalid_filename() {
  empty_delegate d;
  assert(xml::parser::parseFile(nullptr,d)==xml::parser::result::ERROR);
  assert(xml::parser::parseFile("file_does_not_exist",d)==xml::parser::result::ERROR);
}

int main() {
  setup();
  test_parseFile_invalid_filename();
  teardown();
  return 0;
}
