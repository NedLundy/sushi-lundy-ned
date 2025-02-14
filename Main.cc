#pragma once
#include <iostream> 
#include <vector>
#include <string> 

class Sushi {
private:
  std::vector<std::string> history;
  static const size_t HISTORY_LENGTH;
  static const size_t MAX_INPUT;
  bool exit_flag = false; 

public:
  Sushi() : history() {};
  
  std::string read_line(std::istream &in);
  std::string unquote_and_dup(const std::string &s); 
  bool read_config(const char *fname, bool ok_if_missing);
  void store_to_history(const std::string &line);
  void show_history() const;
  void re_parse(int i);
  void set_exit_flag();
  bool get_exit_flag() const;
  int parse_command(const std::string &command);

  static const std::string DEFAULT_PROMPT;
};

#define UNUSED(expr) do {(void)(expr);} while (0)

extern Sushi my_shell;