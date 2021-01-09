/**
 * @file   delegate.hpp
 * @author gulliver <gulliver@traumkristalle.net>
 * 
 * @brief  contains the xml parsers delegate interface
 *
 * See LICENSE for copyright information.
 */
#ifndef xmlpp_delegate_hpp
#define xmlpp_delegate_hpp

#include <string>

#include "expat.h"

namespace xmlpp {

/** wrapper class type for expats XML_Error */
class Error  {
public:
  Error (XML_Error error) noexcept;
  std::string to_string() const;
  XML_Error errorcode() const;
private:
  XML_Error error;
};

/** definition of the parser delegate interface.
 * the parser delegate handles the different parser events 
 */
class delegate {
public:
  virtual ~delegate()=default;

  /** 
   * this callback is called after parsing the starting tag of an xml element.
   *
   * @param fullname name of the started xml element 
   * @param atts list of elements attributes as list of key and value strings
   */
  virtual void onStartElement(const XML_Char *fullname,
                              const XML_Char **atts)  = 0;
  /** 
   * callback is called after parsing the end tag of an xml element 
   *
   * @param fullname name of the ended sml element
   */
  virtual void onEndElement(  const XML_Char *fullname)   = 0;
  
  /** 
   * callback is called with parsed character data inside of an xml element.
   *
   * it may called more than once if the text inside of an xml element does 
   * not fit into the parse buffer.
   *
   * @param pBuf pointer to the text data 
   * @param len len of the returned string
   */
  virtual void onCharacterData(const char * pBuf, int len) = 0;

  /** 
   * callback called on parsed xml comment 
   *
   * @param data contains the content of the comment 
   * @todo check if called more than once for one comment
   */
  virtual void onComment( const XML_Char *data)= 0;

  /** 
   * callback will be called if the begin of a PCDATA section was parsed
   *
   */
  virtual void onStartCdataSection()= 0;
  
  /** 
   * callback will be called after the end of a PCDATA section was parsed
   *
   */
  virtual void onEndCdataSection()= 0;

  /** 
   * delegate function is called after the (starting) xml declaration is parsed.
   *
   * @param version the xml version
   * @param encoding the encoding of the following xml content
   * @param standalone 1 for standalone xml file
   */virtual void onXmlDecl( const XML_Char      *version,
                         const XML_Char      *encoding,
                         int standalone)= 0;
  /** 
   * function is called if on parsing a parse error is detected
   *
   * @param line line in xml file or string where the error is detected
   * @param column column in xml file or string where the error is detected
   * @param pos position of detected parse error in bytes from begin of parsing
   * @param error error code for type of parse error (wrapper for XML_Error from expat 
   */
  virtual void onParseError(size_t line,
			    size_t column,
			    size_t pos,
			    Error error) = 0;

  /** 
   * delegate function is called if an xml processing instruction was found
   *
   * @param target TODO
   * @param data TODO
   */
  virtual void onProcessingInstruction(const XML_Char* target,
                                       const XML_Char* data) = 0;

  /** 
   * delegate function is called when an xml namespace is started/ opened
   *
   * @param prefix prefix of the xml namespace 
   * @param uri uri of namespace
   */
  virtual void onStartNamespace(const XML_Char* prefix,
                                const XML_Char* uri) = 0;
  /** 
   * delegate function is called when an xml namespace is ended
   *
   * @param prefix prefix of the xml namespace 
   */
  virtual void onEndNamespace(const XML_Char* prefix) = 0;
  
//@{ dtd AND DOCTYPE RELATED FUNCTIONS

/** 
 * delegate function is called when a DTD (document type definition) xml entry
 * is parsed.
 *
 * @param doctypeName name of the document type definition 
 * @param sysid system Id
 * @param pubid public id
 * @param has_internal_subset  dtd has an internal subset
 */
  virtual void onStartDoctypeDecl(const XML_Char *doctypeName,
                                  const XML_Char *sysid,
                                  const XML_Char *pubid,
                                  int has_internal_subset) = 0;
  /** 
   * delegate function is called if end of DTD is parsed from xml data
   */
  virtual void onEndDoctypeDecl() = 0;

  /** 
   * delegate function is called if an DTD element declaration is parsed 
   * from xml data. 
   *
   * for details of the model see expat library documentation
   * @param name name of the declared element
   * @param model the content model for the declared element
   */
  virtual void onElementDecl( const XML_Char *name, XML_Content *model) = 0;

  /** 
   * delegate function is called if an element attribute list declaration 
   * is parsed. 
   *
   * @param elname name of the declared element this attribute list refers to 
   * @param attname name of the declared attribute
   * @param att_type type of the declared attribute
   * @param dflt  
   * @param isrequired true if attribute is required, false if optional
   */
  virtual void onAttlistDecl(const XML_Char *elname,
                             const XML_Char *attname,
                             const XML_Char *att_type,
                             const XML_Char *dflt,
                             bool            isrequired) = 0;

  /** 
   * delegate function is called if an entity declaration was parsed  
   *
   * @param entityName name of the declared entity
   * @param is_parameter_entity 
   * @param value 
   * @param value_length 
   * @param base 
   * @param systemId 
   * @param publicId 
   * @param notationName 
   */virtual void onEntityDecl(const XML_Char *entityName,
                            int is_parameter_entity,
                            const XML_Char *value,
                            int value_length,
                            const XML_Char *base,
                            const XML_Char *systemId,
                            const XML_Char *publicId,
                            const XML_Char *notationName) = 0;
  /** 
   * delegate function is called if an notation declaration 
   * was parsed 
   *
   * @param notationName name of the declared notation
   * @param base 
   * @param systemId 
   * @param publicId 
   */virtual void onNotationDecl(const XML_Char* notationName,
                              const XML_Char* base,
                              const XML_Char* systemId,
                              const XML_Char* publicId) = 0;
  
  virtual void onUnparsedEntityDecl(const XML_Char* entityName,
                                    const XML_Char* base,
                                    const XML_Char* systemId,
                                    const XML_Char* publicId,
                                    const XML_Char* notationName) = 0;
  virtual void onSkippedEntity(const XML_Char *entityName,
                               int is_parameter_entity) = 0;
//@}
};

/** default base class for delegates.
 only needed methods needs to be overridden, all interface maethods have an empt default implementation */
class abstract_delegate : public delegate {
public:
  abstract_delegate();
  
  void onStartElement(const XML_Char *fullname, const XML_Char **atts) override;
  void onEndElement(	const XML_Char *fullname) override;
  void onCharacterData(const char * pBuf, int len) override;
  void onProcessingInstruction(const XML_Char* target,
                               const XML_Char* data) override;
  void onUnparsedEntityDecl(const XML_Char* entityName,
			    const XML_Char* base,
                            const XML_Char* systemId,
                            const XML_Char* publicId,
                            const XML_Char* notationName) override;
  void onNotationDecl(const XML_Char* notationName,
                      const XML_Char* base,
                      const XML_Char* systemId,
                      const XML_Char* publicId) override;
  void onStartNamespace(const XML_Char* prefix, const XML_Char* uri) override;
  void onEndNamespace(const XML_Char*) override;
  void onAttlistDecl(const XML_Char *elname,
                     const XML_Char *attname,
                     const XML_Char *att_type,
                     const XML_Char *dflt,
                     bool            isrequired) override;
  void onStartCdataSection() override;
  void onEndCdataSection() override;
  
  void onStartDoctypeDecl(const XML_Char *doctypeName,
                          const XML_Char *sysid,
                          const XML_Char *pubid,
                          int has_internal_subset) override;
  void onEndDoctypeDecl() override;

  void onComment( const XML_Char *data) override;
  void onElementDecl( const XML_Char *name, XML_Content *model) override;
  void onEntityDecl(const XML_Char *entityName,
                    int is_parameter_entity,
                    const XML_Char *value,
                    int value_length,
                    const XML_Char *base,
                    const XML_Char *systemId,
                    const XML_Char *publicId,
                    const XML_Char *notationName) override;
  void onSkippedEntity(const XML_Char *entityName,
                       int is_parameter_entity) override;
  void onXmlDecl( const XML_Char      *version,
                 const XML_Char      *encoding,
                 int standalone) override;
  void onParseError(size_t line,size_t column, size_t pos, Error error) override;
};
}
#endif // #ifndef xmlpp_delegate_hpp
