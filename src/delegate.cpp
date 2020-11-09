/**
 * \file delegate.cpp contains delegate implementation
 *
 * See LICENSE for copyright information.
 */
#include <iostream>
#include "delegate.hpp"

using std::string;

using std::cout;
using std::endl;

namespace xmlpp {

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
                                     int             isrequired) {}
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

StatefulDelegate::StatefulDelegate()
{
  parseStates.push(&root);
}
void StatefulDelegate::add_state(State* state) {
  root.addState(state);
}

void StatefulDelegate::onStartElement( const XML_Char *fullname,
                    const XML_Char **atts)
{
  bool processed = false;

  State* s = parseStates.top();
  for (auto sub : s->substates()) {
    if (sub->tag==fullname) {
      parseStates.push(sub);
      if (sub->pfStart) sub->pfStart(atts);
      processed = true;
      break;
    }
  };
  if (!processed)
  {
    cout << "unexpected Element: " << fullname  << "@" << parseStates.top()->tag << endl;
  }
}

void StatefulDelegate::onEndElement(  const XML_Char *fullname)
{
  if (parseStates.top()==&root) {
    cout << "unexpected Element: " << fullname << endl;
  } else if (parseStates.top()->tag==fullname){
    State* s = parseStates.top();
    if (s->pfEnd) s->pfEnd();
    parseStates.pop();
  } else  {
    cout << "unexpected Element: " << fullname  << "@" << parseStates.top()->tag << endl;
  }
}

void StatefulDelegate::onCharacterData(const char * pBuf, int len)
{
  if (parseStates.empty()) {
    cout << "unexpected chracter data: " << endl;
  } else {
    State* s = parseStates.top();
    if (s->pfText) s->pfText(pBuf,len);
  }
}

}