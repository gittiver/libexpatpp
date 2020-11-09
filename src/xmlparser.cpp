/**
 * \file implementation of xml parser functions
 *
 * See LICENSE for copyright information.
 */

#include <string.h>
#include "xmlparser.hpp"

using std::string;

using xmlpp::parser;
using xmlpp::delegate;
using xmlpp::Attr;

static void XMLParser_xmlSAX2StartElement		(void *ctx,
                                             const XML_Char *fullname,
                                             const XML_Char **atts)
{
  if (ctx) {
    ((delegate*)ctx)->onStartElement(fullname,atts);
  }
}

static void XMLParser_xmlSAX2EndElement(void *ctx,const XML_Char *name)
{
  if (ctx) {
    ((delegate*)ctx)->onEndElement(name);
  }
}

static void XMLParser_OnCharacterData(void * ctx, const char * pBuf, int len)
{
  if (ctx) {
    ((delegate*)ctx)->onCharacterData(pBuf,len);
  }
}

static void XMLParser_CommentHandler(void * ctx, const XML_Char *data)
{
  if (ctx) {
    ((delegate*)ctx)->onComment(data);
  }
}


parser::parser(delegate& delegate) {
  m_parser = XML_ParserCreate("UTF-8");
  XML_SetUserData(m_parser, &delegate);
  XML_SetElementHandler(m_parser,
    XMLParser_xmlSAX2StartElement,
    XMLParser_xmlSAX2EndElement);
  XML_SetCharacterDataHandler(m_parser, XMLParser_OnCharacterData);
  XML_SetCommentHandler(m_parser, XMLParser_CommentHandler);

}

parser::~parser()
{
  XML_ParserFree(m_parser);
}

parser::status_t parser::parse(const char* buffer, int len, bool isFinal)
{ return (status_t)XML_Parse(m_parser,buffer, len, isFinal); }

xmlpp::parser::result  parser::parseString(const char* pszString, delegate& delegate)
{
  result res = result::READ_ERROR;

  if (nullptr == (&delegate)) {
    res = result::NO_DELEGATE;
  } else {

    parser p(delegate);

    const char* pBuf = pszString;
    int len = strlen(pszString);
    if (p.parse(pBuf,len, true)==status_t::ERROR) {
      /* handle parse error */
      res = result::PARSE_ERROR;
      /// TODO utilize these methods to get useful errorstring
      //          XML_GetCurrentLineNumber(p),
      //          XML_ErrorString(XML_GetErrorCode(p)));          break;
    } else {
      res = result::OK;
    }
  }
  return res;
}

xmlpp::parser::result parser::parseFile(const std::string& filename, delegate& delegate) {
  result res = result::READ_ERROR;

  if (nullptr == (&delegate)) {
    res = result::NO_DELEGATE;
  } else {

    const size_t BUFF_SIZE = 255;
    FILE* docfd = fopen(filename.c_str(), "r");

    if (!docfd) {
      res = result::ERROR_OPEN_FILE;
      //  Logger::error("cant open ", filename);
    } else {
      parser p(delegate);

      char buff[BUFF_SIZE];
      for (;;) {
        size_t bytes_read;

        bytes_read = fread(buff, 1, BUFF_SIZE, docfd);

        if (p.parse(buff, static_cast<int>(bytes_read), bytes_read == 0)==status_t::ERROR) {
          /* handle parse error */
          res = result::PARSE_ERROR;
          break;
/// TODO utilize these methods to get useful errorstring
//          XML_GetCurrentLineNumber(p),
//          XML_ErrorString(XML_GetErrorCode(p)));          break;
        }

        if (bytes_read == 0) {
          res = std::feof(docfd) ? result::OK : result::READ_ERROR;
          break;

        }
      }

      fclose(docfd);
    }
  }
  return res;
}
parser::error_t parser::errorcode() const
{ return (error_t)XML_GetErrorCode(m_parser); }

int parser::current_line_number() const
{ return XML_GetCurrentLineNumber(m_parser); }
int parser::current_column_number() const
{ return XML_GetCurrentColumnNumber(m_parser); }

const XML_Char* parser::xmlGetAttrValue(const XML_Char** attrs,
                                             const XML_Char* key)
{
  if (attrs!=NULL)
  {
    for (size_t i = 0; attrs[i]!=NULL;i+=2)
    {
      if (!strcmp(attrs[i],key))
      {
        return attrs[i+1];
      }
    }
  }
  return NULL;
}

std::string Attr::getValue(const char* key)
{
  const XML_Char* value = parser::xmlGetAttrValue(this->attrs_,key);
  return value==nullptr ? "" : std::string(value);
}


