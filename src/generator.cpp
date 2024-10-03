/**
 * \file generator.cpp implementation of xml generation functions
 * 
 * See LICENSE for copyright information.
 */
#include "generator.hpp"

void xmlpp::generator::element::serialize(std::ostream &os) {
  os << '<' << name;
  if (!attributes.empty()) {
    os << ' ';
    for (const auto& a : attributes) {
      os << a.name << '=' << a.value << '"';
    }
  }
  os << "/>";
}

void xmlpp::generator::composite_element::serialize(std::ostream &os) {
  os << '<' << name;
  if (!attributes.empty()) {
    os << ' ';
    for (const auto& a : attributes) {
      os << a.name << '=' << a.value << '"';
    }
  }

  if (children.empty()) {
    os << " />";
  }
  else {
    for (const auto& e : children) {
      e->serialize(os);
    }
    os << "</" << name << '>';
  }
}

void xmlpp::generator::text::serialize(std::ostream &os) {
   os << value;
}
