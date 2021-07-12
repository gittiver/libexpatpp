/**
 * @file   state.hpp
 * @author gulliver <gulliver@traumkristalle.net>
 * 
 * @brief  contains classes for stateful parse delegate
 *
 * See LICENSE for copyright information.
 */
#ifndef xmlpp_state_hpp
#define xmlpp_state_hpp

#include <list>
#include <stack>
#include <functional>

#include "delegate.hpp"

namespace xmlpp {

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
  State* addState(const std::string& tagname)
  {
    State* s = new State(tagname);
    substates_.push_back(s);
    return s;
  }
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
#endif // #ifndef xmlpp_state_hpp
