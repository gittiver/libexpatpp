#include <iostream>

#include <string.h>
#include <cstdio>

#include "xmlparser.hpp"
//#include "litesql/logger.hpp"

using std::string;

using std::cout;
using std::endl;

using namespace xmlpp::parser;

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

result xmlpp::parser::parseFile(const char* filename, delegate& pDelegate)
{
  result res = result::READ_ERROR;
  
  if (nullptr == (&pDelegate)) {
    res = result::NO_DELEGATE;
  } else if (filename==nullptr) {
    res = result::ERROR_OPEN_FILE;
  }
  else {

    const size_t BUFF_SIZE = 255;
    FILE* docfd = fopen(filename, "r");

    if (!docfd) {
      res = result::ERROR_OPEN_FILE;
      //	Logger::error("cant open ", filename);
    } else {

      XML_Parser saxHandler = XML_ParserCreate("UTF-8");
      XML_SetUserData(saxHandler, &pDelegate);
      XML_SetElementHandler(saxHandler,
        XMLParser_xmlSAX2StartElement,
        XMLParser_xmlSAX2EndElement);
      XML_SetCharacterDataHandler(saxHandler, XMLParser_OnCharacterData);
      void* buff = XML_GetBuffer(saxHandler, BUFF_SIZE);

      /* handle error */
      if (buff == NULL) {
        res = result::XML_BUFFER_ERROR;
      }

      for (;;) {
        size_t bytes_read;

        bytes_read = fread(buff, 1, BUFF_SIZE, docfd);

        if (!XML_ParseBuffer(saxHandler, static_cast<int>(bytes_read), bytes_read == 0)) {
          /* handle parse error */
          res = result::PARSE_ERROR;
          break;
        }

        if (bytes_read == 0) {
          res = std::feof(docfd) ? result::OK : result::READ_ERROR;
          break;

        }
      }

      XML_ParserFree(saxHandler);
      fclose(docfd);
    }
  }
  return res;
}

const XML_Char* xmlpp::parser::xmlGetAttrValue(const XML_Char** attrs,
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


StatefulDelegate::StatefulDelegate()
{
  parseStates.push(&root);
}
void StatefulDelegate::add_state(State* state) {
  root.substates.push_back(state);
}

void StatefulDelegate::onStartElement( const XML_Char *fullname,
                    const XML_Char **atts)
{
  bool processed = false;

  State* s = parseStates.top();
  for (auto sub : s->substates) {
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
