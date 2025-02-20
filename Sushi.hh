#pragma once
#include <iostream> 
#include <vector>
#include <string> 

//--------------------------------------------------------------------
// New class
// I/O redirections, as in "foobar < foo > bar"
class Redirection {
private:
  // stdin, stdout-write, stdout-append
  std::string *redir_in, *redir_out1, *redir_out2;
  
public:
  void set_in(std::string *fname)   { redir_in = fname; }
  void set_out1(std::string *fname) { redir_out1 = fname; }
  void set_out2(std::string *fname) { redir_out2 = fname; }
  void set_in(Redirection &redir)   {
    redir_in = redir.redir_out1 ? redir.redir_out1 : redir.redir_out2;
  }
};

// New class
// The program to be executed
class Program {
private:
  std::vector<std::string*> *args; // Arguments, including the program name
  Redirection redir;
  Program *pipe; // The previous program in the pipeline, if any; NULL otherwise

  // Helper methods
  // Converts the args to whatever `execvp` expects
  char* const* vector2array();
  // Frees the memory allocated by vector2array()
  void free_array(char *const argv[]);

public:
  Program(std::vector<std::string*> *args) : args(args) {};
  ~Program();
  void set_pipe(Program *pipe) { this->pipe = pipe; };
  void set_redir(Redirection &redir) { this->redir = redir; };
  std::string progname() { return *args->at(0); }
};

// Old class(es)
//--------------------------------------------------------------------
class Sushi {
private:
  std::vector<std::string> history;
  static const size_t HISTORY_LENGTH;
  static const size_t MAX_INPUT;
  bool exit_flag = false; 

public:
  Sushi() : history() {};
  
  std::string read_line(std::istream &in);
  // DZ: You changed the function signature, why?
  // std::string unquote_and_dup(const std::string &s);
  static std::string *unquote_and_dup(const char *s);
  // DZ: Why did you remove this function?
  static std::string *getenv(const char *name);  
  bool read_config(const char *fname, bool ok_if_missing);
  void store_to_history(const std::string &line);
  void show_history() const;
  void re_parse(int i);
  void set_exit_flag();
  bool get_exit_flag() const;
  // DZ: Wrong signature
  // int parse_command(const std::string &command);
  static int parse_command(const std::string command);
  
  int spawn(Program *exe, bool bg); // New method
  static void prevent_interruption(); // New method
  static void refuse_to_die(int signo); // New method
  static const std::string DEFAULT_PROMPT;
};

#define UNUSED(expr) do {(void)(expr);} while (0)

extern Sushi my_shell;
