#include <cstring>
#include <cassert>
#include <iostream>
#include <cstdlib>
#include "Sushi.hh"

//---------------------------------------------------------------
// Implement the function
std::string* Sushi::unquote_and_dup(const char* s) {
  assert(s);
  std::string result;
  result.reserve(std::strlen(s));

  while (*s) {
    if (*s == '\\') {
      ++s;
      if (!*s) {
        result += '\\';
        break;
      }

      switch (*s) {
        case 'a':  result += '\a'; break;
        case 'b':  result += '\b'; break;
        case 'e':  result += '\x1B'; break;
        case 'f':  result += '\f'; break;
        case 'n':  result += '\n'; break;
        case 'r':  result += '\r'; break;
        case 't':  result += '\t'; break;
        case 'v':  result += '\v'; break;
        case '\\': result += '\\'; break;
        case '\'': result += '\''; break;
        case '"':  result += '"';  break;
        default:
          result += '\\';
          result += *s;
          break;
      }
      ++s;
    } else {
      result += *s++;
    }
  }

  return new std::string(std::move(result));
}

//---------------------------------------------------------------
// Implement the function
std::string* Sushi::getenv(const char* s) {
  const char* value = std::getenv(s);
  return new std::string(value ? value : "");
}

//---------------------------------------------------------------
// Implement the function
void Sushi::putenv(const std::string* name, const std::string* value) {
  std::string var = *name + "=" + *value;
  ::putenv(strdup(var.c_str())); // strdup allocates memory owned by the environment
  // DO NOT free 'var' after putenv; it's now managed by environment
  delete name;
  delete value;
}

//---------------------------------------------------------------
// Implement the function
void Sushi::re_parse(int i) {
  if (i <= 0 || static_cast<size_t>(i) > history.size()) {
    std::cerr << "!" << i << ": event not found" << std::endl;
  } else {
    redo = history[i - 1];
  }
}

//---------------------------------------------------------------
// Implement the function
bool Sushi::re_execute() {
  if (!redo.empty()) {
    if (!parse_command(redo)) {
      store_to_history(redo);
    }
    redo.clear();
    return true;
  }
  return false;
}

//---------------------------------------------------------------
// Do not modify this function
void yyerror(const char* s) {
  std::cerr << "Parse error: " << s << std::endl;
}
