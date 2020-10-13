#include <iostream>

#include <string.h>
#include <cstdio>

#include "xmlparser.hpp"
//#include "litesql/logger.hpp"

using std::string;

using namespace xml::parser;

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

result xml::parser::parseFile(const char* filename, delegate& pDelegate)
{
  if (nullptr == (&pDelegate)) {
    return result::NO_DELEGATE;
  };
  result res=result::ERROR;

  XML_Parser saxHandler = XML_ParserCreate("UTF-8");
  XML_SetUserData(saxHandler, &pDelegate);
  XML_SetElementHandler(saxHandler,
			XMLParser_xmlSAX2StartElement,
			XMLParser_xmlSAX2EndElement);

  const size_t BUFF_SIZE = 255;
  FILE* docfd = fopen(filename, "r");
  
  if (!docfd) {
    res = result::ERROR;
    //	Logger::error("cant open ", filename);
  } else {
    for (;;) {
      int bytes_read;
      void *buff = XML_GetBuffer(saxHandler, BUFF_SIZE);
      /* handle error */
      if (buff == NULL) {
	res = result::ERROR;
	break;
      }
      
      bytes_read = fread(buff, 1, BUFF_SIZE, docfd);
      if (bytes_read < 0) {
	/* handle error */
	res = result::ERROR;
	break;
      }

      if (!XML_ParseBuffer(saxHandler, bytes_read, bytes_read == 0)) {
	/* handle parse error */
	res = result::ERROR;
	break;
      }

      if (bytes_read == 0)
	break;
    }
    fclose(docfd);
  }

  XML_ParserFree(saxHandler);
  return res;
}

const XML_Char* xml::parser::xmlGetAttrValue(const XML_Char** attrs,
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

