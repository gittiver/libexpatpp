/**
 * @file   state.cpp
 * @author gulliver <gulliver@traumkristalle.net>
 * 
 * @brief  contains classes for stateful parse delegate
 *
 * See LICENSE for copyright information.
 */
#include <iostream>
#include "state.hpp"

using std::cerr;
using std::endl;

using namespace xmlpp;

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
    cerr << "unexpected Element: " << fullname  << "@" << parseStates.top()->tag << endl;
  }
}

void StatefulDelegate::onEndElement(  const XML_Char *fullname)
{
  if (parseStates.top()==&root) {
    cerr << "unexpected Element was closed: " << fullname << endl;
  } else if (parseStates.top()->tag==fullname){
    State* s = parseStates.top();
    if (s->pfEnd) s->pfEnd();
    parseStates.pop();
  } else  {
    cerr << "unexpected Element was closed: " << fullname  << "@" << parseStates.top()->tag << endl;
  }
}

void StatefulDelegate::onCharacterData(const char * pBuf, int len)
{
  if (parseStates.empty()) {
    cerr << "unexpected character data: " << endl;
  } else {
    State* s = parseStates.top();
    if (s->pfText) s->pfText(pBuf,len);
  }
}

