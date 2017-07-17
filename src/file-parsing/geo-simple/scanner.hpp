#ifndef SCANNER_H
#define SCANNER_H

#if ! defined (yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer yy_Scanner
#include <FlexLexer.h>
#endif


#undef YY_DECL
#define YY_DECL yy::Parser::symbol_type yy::Scanner::get_next_token()

#include "geo-soft.tab.hh"

namespace yy {
  class Interpreter;

  class Scanner : public yyFlexLexer {
  public:
    Scanner(Interpreter &driver) : m_driver(driver) {}
    virtual ~Scanner() {}
    virtual yy::Parser::symbol_type get_next_token();
  private:
    Interpreter &m_driver;
  };
}

#endif
