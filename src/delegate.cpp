/**
 * \file delegate.cpp contains delegate implementation
 *
 * See LICENSE for copyright information.
 */
#include <iostream>
#include "delegate.hpp"

#include "state.hpp"

using std::string;

using std::cerr;
using std::endl;

namespace xmlpp {

Error::Error (XML_Error _error) noexcept
: error(_error)
{}

std::string Error::to_string() const
{
  return XML_ErrorString(error);
}

XML_Error Error::errorcode() const
{
  return error;
}

abstract_delegate::abstract_delegate() {}

void abstract_delegate::onStartElement(const XML_Char *fullname,
                                       const XML_Char **atts)
{}
void abstract_delegate::onEndElement(	const XML_Char *fullname)
{}

void abstract_delegate::onCharacterData(const char * pBuf, int len)
{}
void abstract_delegate::onProcessingInstruction(const XML_Char* target,
                                                const XML_Char* data)
{}
void abstract_delegate::onUnparsedEntityDecl(const XML_Char* entityName,
                                             const XML_Char* base,
                                             const XML_Char* systemId,
                                             const XML_Char* publicId,
                                             const XML_Char* notationName)
{}

void abstract_delegate::onNotationDecl(const XML_Char* notationName,
                                       const XML_Char* base,
                                       const XML_Char* systemId,
                                       const XML_Char* publicId)
{}
void abstract_delegate::onStartNamespace(const XML_Char* prefix,
                                         const XML_Char* uri)
{}

void abstract_delegate::onEndNamespace(const XML_Char*) {}
void abstract_delegate::onAttlistDecl(const XML_Char *elname,
                                      const XML_Char *attname,
                                      const XML_Char *att_type,
                                      const XML_Char *dflt,
                                      bool            isrequired) {}
void abstract_delegate::onStartCdataSection()
{}
void abstract_delegate::onEndCdataSection()
{}

void abstract_delegate::onStartDoctypeDecl(const XML_Char *doctypeName,
                                           const XML_Char *sysid,
                                           const XML_Char *pubid,
                                           int has_internal_subset)
{}
void abstract_delegate::onEndDoctypeDecl()
{}

void abstract_delegate::onComment( const XML_Char *data)
{}
void abstract_delegate::onElementDecl( const XML_Char *name, XML_Content *model)
{}
void abstract_delegate::onEntityDecl(const XML_Char *entityName,
                                     int is_parameter_entity,
                                     const XML_Char *value,
                                     int value_length,
                                     const XML_Char *base,
                                     const XML_Char *systemId,
                                     const XML_Char *publicId,
                                     const XML_Char *notationName)
{}

void abstract_delegate::onSkippedEntity(const XML_Char *entityName,
                                        int is_parameter_entity)
{}

void abstract_delegate::onXmlDecl( const XML_Char      *version,
                                  const XML_Char      *encoding,
                                  int standalone)
{}

void abstract_delegate::onParseError(size_t line,size_t column, size_t pos, Error error)
{}
  

}
