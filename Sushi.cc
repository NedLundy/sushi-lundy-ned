#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cstdio>
#include <csignal>
#include <cassert>
#include <sys/wait.h>
#include <cstdlib>     
#include "Sushi.hh"
#include <cstring>


const std::string Sushi::DEFAULT_PROMPT = "sushi> ";
const std::string Sushi::DEFAULT_CONFIG = "sushi.conf";

std::string Sushi::read_line(std::istream &in)
{
  std::string line;
  if(!std::getline(in, line)) {
    if(!in.eof()) { 
      std::perror("getline");
    }
    return "";
  }
  if(std::all_of(line.begin(), line.end(), isspace)) {
    return "";
  }
  if(line.size() > MAX_INPUT_SIZE) {
    line.resize(MAX_INPUT_SIZE);
    std::cerr << "Line too long, truncated." << std::endl;
  }
  return line; 
}

bool Sushi::read_config(const char *fname, bool ok_if_missing)
{
  std::ifstream config_file(fname);
  if (!config_file) {
    if (!ok_if_missing) {
      std::perror(fname);
      return false;
    }
    return true;
  }

  while(!config_file.eof()) {
    std::string line = read_line(config_file);
    if(!parse_command(line)) {
      store_to_history(line);
    }
  }
  
  return true; 
}

void Sushi::store_to_history(std::string line)
{
  if (line.empty()) return;
  while (history.size() >= HISTORY_LENGTH) {
    history.pop_front();
  }
  history.emplace_back(line);
}

void Sushi::show_history() 
{
  int index = 1;
  if (history.size() == HISTORY_LENGTH) {
    history.pop_front();
  }
  for (const auto &cmd: history) {
    std::cout << std::setw(5) << index++ << "  " << cmd << std::endl;
  }
  std::cout << std::setw(5) << index++ << "  " << "history" << std::endl;
}

void Sushi::set_exit_flag() { exit_flag = true; }

bool Sushi::get_exit_flag() const { return exit_flag; }

int Sushi::spawn(Program *exe, bool bg) {
    pid_t pid = fork();

    if (pid == -1) {
        std::perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        // Child process
        char* const* args = exe->vector2array();
        assert(args);
        execvp(args[0], args);
        std::perror(args[0]);
        _exit(EXIT_FAILURE);
    }

    // Parent
    int status = 0;
    if (!bg) {
        if (waitpid(pid, &status, 0) != pid) {
            std::perror("waitpid");
            status = 1;
        }

        
        std::string code = std::to_string(WEXITSTATUS(status));
        setenv("?", code.c_str(), 1);
    } else {
        
        setenv("?", "0", 1);
    }

    return EXIT_SUCCESS;
};

void Sushi::prevent_interruption() {
  struct sigaction sa;
  sa.sa_handler = refuse_to_die;
  sa.sa_flags = SA_RESTART;
  if(sigaction(SIGINT, &sa, nullptr) != 0) {
    std::perror("sigaction");
    std::exit(EXIT_FAILURE);
  };
};

void Sushi::refuse_to_die(int signo) {
  UNUSED(signo);
  std::cerr << "Type exit to exit the shell" << '\n';
};

void Sushi::mainloop() {
  while (!get_exit_flag()) {
    std::string* ps1 = Sushi::getenv("PS1");
    std::cout << (ps1 && !ps1->empty() ? *ps1 : DEFAULT_PROMPT);
    delete ps1;

    std::string command = read_line(std::cin);
    if (command.empty()) continue;

    if (parse_command(command) == 0) {
      store_to_history(command);
    }
  }
};

void Sushi::re_parse(int i) {
    if (i <= 0 || i > static_cast<int>(history.size())) {
      std::cerr << "Error: !" << i << ": event not found" << std::endl;
      return;
    }
    redo = history[i - 1];
  };  


bool Sushi::re_execute() {
  if (redo.empty()) return false;
  if (parse_command(redo) == 0) {
    store_to_history(redo);
    redo.clear();
    return true;
  }
  redo.clear();
  return false;
};

char* const* Program::vector2array() {
  assert(args);
  size_t size = args->size();
  char** array = new char*[size + 1];
  for (size_t i = 0; i < size; ++i) {
    assert((*args)[i]);
    array[i] = const_cast<char*>((*args)[i]->c_str());
  }
  array[size] = nullptr;
  return array;
};

Program::~Program() {

};

std::string* Sushi::getenv(const char* name) {
  const char* val = std::getenv(name);
  if (!val) return new std::string("");
  return new std::string(val);
};

void Sushi::assign(const std::string* name, const std::string* value) {
  setenv(name->c_str(), value->c_str(), 1);
  delete name;
  delete value;
};
void Sushi::putenv(const std::string *name, const std::string *value) {
    std::string env = *name + "=" + *value;
    ::putenv(strdup(env.c_str()));  
    delete name;
    delete value;
}

