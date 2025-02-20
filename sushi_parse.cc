#include "Sushi.hh"

std::string *Sushi::unquote_and_dup(const char* s)
{
  return new std::string(s); // Must be changed
}

// DZ: If you implement the function in another file, you must at least
// DZ: remove the skeeleton
/*
void Sushi::re_parse(int i) {
  UNUSED(i);
// Must be implemented
}
*/

//---------------------------------------------------------------
// Do not modify this function YET
std::string *Sushi::getenv(const char* s) 
{
  return new std::string(s); // Must be changed - eventually
}

//---------------------------------------------------------------
// Do not modify this function
void yyerror(const char* s) {
  std::cerr << "Parse error: " << s << std::endl;
}

