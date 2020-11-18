/**
 * \file xmlparser.hpp contains the xml parser module interface
 *
 * See LICENSE for copyright information.
 */
#ifndef xmlpp_parser_hpp
#define xmlpp_parser_hpp

#include "delegate.hpp"
namespace xmlpp {


/** namespace for SAX2 xml Parser based on expat */
class parser {
public:
  enum class result : uint8_t {
    OK,
    NO_DELEGATE,
    ERROR_OPEN_FILE,
    INVALID_INPUT,
    XML_BUFFER_ERROR,
    READ_ERROR,
    PARSE_ERROR
  };
  enum class status_t {
      ERROR = 0,
      OK = 1,
      SUSPENDED = 2
  };
  enum class error_t {
    NONE,
    NO_MEMORY,
    SYNTAX,
    NO_ELEMENTS,
    INVALID_TOKEN,
    UNCLOSED_TOKEN,
    PARTIAL_CHAR,
    TAG_MISMATCH,
    DUPLICATE_ATTRIBUTE,
    JUNK_AFTER_DOC_ELEMENT,
    PARAM_ENTITY_REF,
    UNDEFINED_ENTITY,
    RECURSIVE_ENTITY_REF,
    ASYNC_ENTITY,
    BAD_CHAR_REF,
    BINARY_ENTITY_REF,
    ATTRIBUTE_EXTERNAL_ENTITY_REF,
    MISPLACED_XML_PI,
    UNKNOWN_ENCODING,
    INCORRECT_ENCODING,
    UNCLOSED_CDATA_SECTION,
    EXTERNAL_ENTITY_HANDLING,
    NOT_STANDALONE,
    UNEXPECTED_STATE,
    ENTITY_DECLARED_IN_PE,
    FEATURE_REQUIRES_XML_DTD,
    CANT_CHANGE_FEATURE_ONCE_PARSING,
    UNBOUND_PREFIX,
    UNDECLARING_PREFIX,
    INCOMPLETE_PE,
    XML_DECL,
    TEXT_DECL,
    PUBLICID,
    SUSPENDED,
    NOT_SUSPENDED,
    ABORTED,
    FINISHED,
    SUSPEND_PE,
    RESERVED_PREFIX_XML,
    RESERVED_PREFIX_XMLNS,
    RESERVED_NAMESPACE_URI,
    INVALID_ARGUMENT
  };

  parser(delegate& delegate);
  virtual ~parser();

  static result parseString(const char*pszString, delegate& delegate);
  static result parseFile(const std::string& filename, delegate& delegate);
  /** get value of xml attribute identifeid by key from attrs
   * @param attrs xml attribute array as array of strings
   * @param key attribute key to search for
   *
   * @return value as pointer to null determined string or nullptr if not found
   */
  static const XML_Char* xmlGetAttrValue(const XML_Char** attrs,
                                  const XML_Char* key);


  status_t parse(const char* buffer, int len, bool isFinal);
  error_t errorcode() const;
  int current_line_number() const ;
  int current_column_number() const ;
private:
  XML_Parser m_parser;
};

/** the parser delegate handles the different parser events */
//result parseFile(const std::string& filename, delegate& pDelegate);

class Attr {
public:
  Attr(const XML_Char** attrs) : attrs_(attrs){};
  std::string getValue(const char* key);
private:
  const XML_Char** attrs_{nullptr};
};


}
#endif // #ifndef xmlpp_parser_hpp
