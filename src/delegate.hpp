/**
 * \file delegate.hpp contains the xml parsers delegate interface
 *
 * See LICENSE for copyright information.
 */
#ifndef xmlpp_delegate_hpp
#define xmlpp_delegate_hpp

#include <string>
#include <list>
#include <stack>

#include "expat.h"

namespace xmlpp {
/** the parser delegate handles the different parser events */
class delegate {
public:
  virtual ~delegate()=default;

  virtual void onStartElement(const XML_Char *fullname,
                              const XML_Char **atts)  = 0;
  virtual void onEndElement(  const XML_Char *fullname)   = 0;
  virtual void onCharacterData(const char * pBuf, int len) = 0;
  virtual void onProcessingInstruction(const XML_Char* target,
                                       const XML_Char* data) =0 ;
  virtual void onUnparsedEntityDecl(const XML_Char* entityName,
                                    const XML_Char* base,
                                    const XML_Char* systemId,
                                    const XML_Char* publicId,
                                    const XML_Char* notationName) = 0;
  virtual void onNotationDecl(const XML_Char* notationName,
                              const XML_Char* base,
                              const XML_Char* systemId,
                              const XML_Char* publicId) = 0;
  virtual void onStartNamespace(const XML_Char* prefix,
                                const XML_Char* uri)= 0;
  virtual void onEndNamespace(const XML_Char*) = 0;
  virtual void onAttlistDecl(const XML_Char *elname,
                             const XML_Char *attname,
                             const XML_Char *att_type,
                             const XML_Char *dflt,
                             int             isrequired)= 0;
  virtual void onStartCdataSection()= 0;
  virtual void onEndCdataSection()= 0;
  
  virtual void onStartDoctypeDecl(const XML_Char *doctypeName,
                                  const XML_Char *sysid,
                                  const XML_Char *pubid,
                                  int has_internal_subset)= 0;
  virtual void onEndDoctypeDecl()= 0;

  virtual void onComment( const XML_Char *data)= 0;
  virtual void onElementDecl( const XML_Char *name, XML_Content *model)= 0;
  virtual void onEntityDecl(
                            const XML_Char *entityName,
                            int is_parameter_entity,
                            const XML_Char *value,
                            int value_length,
                            const XML_Char *base,
                            const XML_Char *systemId,
                            const XML_Char *publicId,
                            const XML_Char *notationName)= 0;
  virtual void onSkippedEntity(const XML_Char *entityName,
                               int is_parameter_entity)= 0;
  virtual void onXmlDecl( const XML_Char      *version,
                         const XML_Char      *encoding,
                         int standalone)= 0;
};

/** the parser delegate handles the different parser events */
class abstract_delegate : public delegate {
public:
  abstract_delegate();
  
  void onStartElement(const XML_Char *fullname,
                      const XML_Char **atts) override;
  void onEndElement(	const XML_Char *fullname)    override;
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
  void onStartNamespace(const XML_Char* prefix,
                        const XML_Char* uri) override;
  void onEndNamespace(const XML_Char*) override;
  void onAttlistDecl(const XML_Char *elname,
                     const XML_Char *attname,
                     const XML_Char *att_type,
                     const XML_Char *dflt,
                     int             isrequired) override;
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
};

struct State {
  State(const std::string& _tag,
        std::function<void (const XML_Char **atts)> _pfStart = nullptr,
        std::function<void ()> _pfEnd = nullptr,
        std::function<void (const char *pBuf, int len)> _pfText = nullptr
        ) noexcept
  :  tag{_tag},
  pfStart{_pfStart},
  pfEnd{_pfEnd},
  pfText{_pfText}
  {}

  virtual ~State(){}

  std::string tag;
  void addState(State* s) { substates_.push_back(s); }
  const std::list<State*>& substates() { return substates_; }
  std::function<void (const XML_Char **atts)> pfStart{nullptr};
  std::function<void ()> pfEnd{nullptr};
  std::function<void (const char *pBuf, int len)> pfText{nullptr};

private:
  std::list<State*> substates_;

};

struct RootState: public State {
  State root{"root"};
  std::stack<State*> parseStates;

};

class StatefulDelegate: public abstract_delegate {
public:
  StatefulDelegate();
  void add_state(State* state);
  const State* const current_element() const { return parseStates.top(); }
protected:
  void onStartElement(const XML_Char *fullname, const XML_Char **atts) override;
  void onEndElement(const XML_Char *fullname) override;
  void onCharacterData(const char * pBuf, int len) override;
private:
  State root{"root"};
  std::stack<State*> parseStates;
};

}
#endif // #ifndef xmlpp_delegate_hpp
