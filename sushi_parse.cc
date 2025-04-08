#include "Sushi.hh"

std::string *Sushi::unquote_and_dup(const char* s)
{
  // DZ: The implementation is still missing
  std::cerr << "Fix me!\n";
  return new std::string(s); // Must be changed
}

//---------------------------------------------------------------
// Do not modify this function
void yyerror(const char* s) {
  std::cerr << "Parse error: " << s << std::endl;
}
