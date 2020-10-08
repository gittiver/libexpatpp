#include <iostream>

#include <string.h>
#include <cstdio>

#include "xmlparser.hpp"
//#include "litesql/logger.hpp"

using std::string;
using xml::parser;

//using namespace expatpp;

namespace xml {
void XMLParser_xmlSAX2StartElement		(void *ctx,
						 const XML_Char *fullname,
						 const XML_Char **atts)
{
   ((parser*)ctx)->onStartElement(fullname,atts);
}

void XMLParser_xmlSAX2EndElement(void *ctx,const XML_Char *name)
{
   ((parser*)ctx)->onEndElement(name);
}

}

bool parser::parseFile(const std::string& filename)
{
	XML_Parser saxHandler = XML_ParserCreate("UTF-8");
	XML_SetUserData(saxHandler, this);
	XML_SetElementHandler(saxHandler,
		XMLParser_xmlSAX2StartElement,
		XMLParser_xmlSAX2EndElement);

	const size_t BUFF_SIZE = 255;
	FILE* docfd = fopen(filename.c_str(), "r");

	bool success = (docfd != NULL);
	if (!success)
	{
	  //	Logger::error("cant open ", filename);
	}
	else {
		for (;;) {
			int bytes_read;
			void *buff = XML_GetBuffer(saxHandler, BUFF_SIZE);
			/* handle error */
			if (buff == NULL) {
				success = false;
				break;
			}

			bytes_read = fread(buff, 1, BUFF_SIZE, docfd);
			if (bytes_read < 0) {
				/* handle error */
				success = false;
				break;
			}

			if (!XML_ParseBuffer(saxHandler, bytes_read, bytes_read == 0)) {
				/* handle parse error */
				success = false;
				break;
			}

			if (bytes_read == 0)
				break;
		}
		fclose(docfd);
	}

	if (!success)
	{
	  //	Logger::error("error parsing ", filename);
	}
	XML_ParserFree(saxHandler);
	return success;
}

const XML_Char* parser::xmlGetAttrValue(const XML_Char** attrs,const XML_Char* key)
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
