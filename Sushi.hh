#pragma once
#include <iostream>
#include <string> 
#include <deque>
#include <vector> 

class Redirection {
public:
  std::string *redir_in = nullptr;
  std::string *redir_out1 = nullptr;
  std::string *redir_out2 = nullptr;

  Redirection() = default;
  void set_out1(std::string *fname) { redir_out1 = fname; }
  void set_out2(std::string *fname) { redir_out2 = fname; }
  void set_in(std::string *fname)   { redir_in = fname; }
  void set_in(Redirection &redir)   {
    redir_in = redir.redir_out1 ? redir.redir_out1 : redir.redir_out2;
  }
};

class Program {
private:
  std::vector<std::string*> *args; 
  Redirection redir;

public:
  Program *pipe; 
  Program(std::vector<std::string*> *args) : args(args) {};
  ~Program();
  void set_pipe(Program *pipe) { this->pipe = pipe; };
  void set_redir(Redirection &redir) { this->redir = redir; };
  std::string progname() { return *args->at(0); }

  char* const* vector2array();
};

class Sushi {
private:
  std::deque<std::string> history; 
  static const size_t HISTORY_LENGTH = 10;
  static const size_t MAX_INPUT_SIZE = 256;
  bool exit_flag = false; 
  std::string redo;

public:
  Sushi();
  static std::string read_line(std::istream &in);
  static std::string* unquote_and_dup(const char *s); // ✅ made static
  static std::string* getenv(const char *name); 
  static void putenv(const std::string *name, const std::string *value); 

  void assign(const std::string *name, const std::string *value);

  bool read_config(const char *fname, bool ok_if_missing);
  void store_to_history(std::string line);
  void show_history();
  void re_parse(int i); 
  bool re_execute();
  void set_exit_flag(); 
  bool get_exit_flag() const; 
  static int parse_command(const std::string command);
  void mainloop(); 
  int spawn(Program *exe, bool bg);   
  static void prevent_interruption(); 
  static void refuse_to_die(int signo);

  static const std::string DEFAULT_PROMPT;
  static const std::string DEFAULT_CONFIG;
};

#define UNUSED(expr) do {(void)(expr);} while (0)

extern Sushi my_shell;