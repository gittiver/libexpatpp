/**
 * \file xmlparser.cpp implementation of xml parser functions
 *
 * See LICENSE for copyright information.
 */

#include <string.h>
#include "xmlparser.hpp"
#include <expat.h>

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

static void XMLParser_XmlDeclHandler(void * ctx,
                                     const XML_Char *version,
                                     const XML_Char *encoding,
                                     int standalone)
{
  if (ctx) {
    ((delegate*)ctx)->onXmlDecl(version,encoding, standalone);
  }
}

static void XMLParser_EntityDeclHandler(void * ctx,
                                        const XML_Char *entityName,
                                        int is_parameter_entity,
                                        const XML_Char *value,
                                        int value_length,
                                        const XML_Char *base,
                                        const XML_Char *systemId,
                                        const XML_Char *publicId,
                                        const XML_Char *notationName)
{
  if (ctx) {
    ((delegate*)ctx)->onEntityDecl(entityName,
                                   is_parameter_entity,
                                   value, value_length,
                                   base,
                                   systemId,
                                   publicId,
                                   notationName);
  }
}

static void XMLParser_StartCdataSectionHandler(void * ctx)
{
  if (ctx) {
    ((delegate*)ctx)->onStartCdataSection();
  }
}

static void XMLParser_EndCdataSectionHandler(void * ctx)
{
  if (ctx) {
    ((delegate*)ctx)->onEndCdataSection();
  }
}

static void XMLParser_StartNamespaceDeclHandler(void * ctx,
                                                const XML_Char *prefix,
                                                const XML_Char *uri)
{
  if (ctx) {
    ((delegate*)ctx)->onStartNamespace(prefix,uri);
  }
}

static void XMLParser_EndNamespaceDeclHandler(void * ctx,const XML_Char *prefix)
{
  if (ctx) {
    ((delegate*)ctx)->onEndNamespace(prefix);
  }
}

static void XMLParser_ProcessingInstruction(void * ctx,
                                            const XML_Char* target,
                                            const XML_Char* data)
{
  if (ctx) {
    ((delegate*)ctx)->onProcessingInstruction(target,data);
  }
}

static void XMLParser_UnparsedEntityDecl(void * ctx,
                                         const XML_Char* entityName,
                                         const XML_Char* base,
                                         const XML_Char* systemId,
                                         const XML_Char* publicId,
                                         const XML_Char* notationName)
{
  if (ctx) {
    ((delegate*)ctx)->onUnparsedEntityDecl(entityName,
                                           base,
                                           systemId,
                                           publicId,
                                           notationName);
  }
}

static void XMLParser_NotationDecl(void * ctx,const XML_Char* notationName,
                                   const XML_Char* base,
                                   const XML_Char* systemId,
                                   const XML_Char* publicId)
{
  if (ctx) {
    ((delegate*)ctx)->onNotationDecl(notationName,
                                     base,
                                     systemId,
                                     publicId);
  }
}

static void XMLParser_AttlistDecl(void * ctx,const XML_Char *elname,
                                  const XML_Char *attname,
                                  const XML_Char *att_type,
                                  const XML_Char *dflt,
                                  int             isrequired)
{
  if (ctx) {
    ((delegate*)ctx)->onAttlistDecl(elname,
                                    attname,
                                    att_type,
                                    dflt,
                                    isrequired);
  }
}

static void XMLParser_StartDoctypeDecl(void * ctx,const XML_Char *doctypeName,
                                       const XML_Char *sysid,
                                       const XML_Char *pubid,
                                       int has_internal_subset)
{
  if (ctx) {
    ((delegate*)ctx)->onStartDoctypeDecl(doctypeName,
                                         sysid,
                                         pubid,
                                         has_internal_subset);
  }
}

static void XMLParser_EndDoctypeDecl(void * ctx)
{
  if (ctx) {
    ((delegate*)ctx)->onEndDoctypeDecl();
  }
}

static void XMLParser_ElementDecl(void * ctx, const XML_Char *name, XML_Content *model)
{
  if (ctx) {
    ((delegate*)ctx)->onElementDecl(name,model);
  }
}

static void XMLParser_SkippedEntity(void * ctx,const XML_Char *entityName,
                                    int is_parameter_entity)
{
  if (ctx) {
    ((delegate*)ctx)->onSkippedEntity(entityName,is_parameter_entity);
  }
}

parser::parser(delegate& delegate, char namespaceSeparator) {
  m_parser = XML_ParserCreateNS("UTF-8",namespaceSeparator);
  XML_SetUserData(m_parser, &delegate);

  XML_SetElementHandler(m_parser,
                        XMLParser_xmlSAX2StartElement,
                        XMLParser_xmlSAX2EndElement);
  XML_SetCharacterDataHandler(m_parser, XMLParser_OnCharacterData);
  XML_SetCommentHandler(m_parser, XMLParser_CommentHandler);
  XML_SetXmlDeclHandler(m_parser,XMLParser_XmlDeclHandler);
  XML_SetEntityDeclHandler(m_parser, XMLParser_EntityDeclHandler);

  XML_SetCdataSectionHandler(m_parser,
                             XMLParser_StartCdataSectionHandler,
                             XMLParser_EndCdataSectionHandler);
  XML_SetNamespaceDeclHandler(m_parser,
                              XMLParser_StartNamespaceDeclHandler,
                              XMLParser_EndNamespaceDeclHandler);
  XML_SetProcessingInstructionHandler(m_parser,
                                      XMLParser_ProcessingInstruction);
  // TODO OBSOLete replace by XML_EntityDeclHandler
  XML_SetUnparsedEntityDeclHandler(m_parser,XMLParser_UnparsedEntityDecl);
  XML_SetNotationDeclHandler(m_parser,XMLParser_NotationDecl);
  XML_SetAttlistDeclHandler(m_parser, XMLParser_AttlistDecl);
  XML_SetDoctypeDeclHandler(m_parser,
                            XMLParser_StartDoctypeDecl,
                            XMLParser_EndDoctypeDecl);
  XML_SetElementDeclHandler(m_parser,XMLParser_ElementDecl);
  XML_SetSkippedEntityHandler(m_parser,XMLParser_SkippedEntity);
}

parser::~parser()
{
  XML_ParserFree(m_parser);
}

parser::status_t parser::parse(const char* buffer, int len, bool isFinal)
{ return (status_t)XML_Parse(m_parser,buffer, len, isFinal); }

xmlpp::parser::result  parser::parseString(const char* pszString,
					   delegate& delegate)
{
  result res = result::READ_ERROR;

  if (pszString==nullptr) {
    res = result::INVALID_INPUT;
  }
  else if (nullptr == (&delegate)) {
    res = result::NO_DELEGATE;
  } else {

    parser p(delegate);

    const char* pBuf = pszString;
    size_t len = strlen(pszString);
    if (p.parse(pBuf,static_cast<int>(len), true)==status_t::ERROR) {
      /* handle parse error */
      res = result::PARSE_ERROR;
      delegate.onParseError(XML_GetCurrentLineNumber(p.m_parser),
                            XML_GetCurrentColumnNumber(p.m_parser),
                            XML_GetCurrentByteIndex(p.m_parser),
                            xmlpp::Error(XML_GetErrorCode(p.m_parser)));
    } else {
      res = result::OK;
    }
  }
  return res;
}

xmlpp::parser::result parser::parseFile(const std::string& filename,
					delegate& delegate) {
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
          delegate.onParseError(XML_GetCurrentLineNumber(p.m_parser),
                                XML_GetCurrentColumnNumber(p.m_parser),
                                XML_GetCurrentByteIndex(p.m_parser),
                                Error(XML_GetErrorCode(p.m_parser)));
          break;
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
