#ifndef xmlpp_parser_hpp
#define xmlpp_parser_hpp

#include <string>
// TODO: fix in expat cmake files
#undef _MSC_EXTENSIONS
#include "expat.h"

namespace xmlpp {


/** nabstract base class for SAX2 Parser based on expat */
namespace parser {
  enum class result : uint8_t {
    OK,
    NO_DELEGATE,
    ERROR_OPEN_FILE,
    XML_BUFFER_ERROR,
    READ_ERROR,
    PARSE_ERROR
  };
  class delegate {
  public:
   virtual void onStartElement(	const XML_Char *fullname,
                               const XML_Char **atts)	= 0;
    virtual void onEndElement(	const XML_Char *fullname)   = 0;
    virtual void onCharacterData(const char * pBuf, int len) = 0;

  };

  result parseFile(const char* filename, delegate& pDelegate);
   
  const XML_Char* xmlGetAttrValue(const XML_Char** attrs,
					  const XML_Char* key);
}
}
#endif // #ifndef xmlpp_parser_hpp
