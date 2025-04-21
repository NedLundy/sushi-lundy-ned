#include <iostream>
#include <fstream>
#include <cstring> 
#include <iomanip>
#include <csignal>
#include <cstdlib>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "Sushi.hh"

const std::string Sushi::DEFAULT_PROMPT = "sushi> ";
const std::string Sushi::DEFAULT_CONFIG = "sushi.conf";

Sushi::Sushi() {
  prevent_interruption();
  const char *home = std::getenv("HOME");
  if (home) {
    std::string config_path = std::string(home) + "/" + DEFAULT_CONFIG;
    read_config(config_path.c_str(), true);
  }
}

std::string Sushi::read_line(std::istream &in) {
  std::string line;
  if (!std::getline(in, line) || line.find_first_not_of(" \t\n") == std::string::npos) return "";
  if (line.size() > MAX_INPUT_SIZE) {
    line.resize(MAX_INPUT_SIZE);
    std::cerr << "Line too long, truncated.\n";
  }
  return line;
}

bool Sushi::read_config(const char *fname, bool ok_if_missing) {
  std::ifstream file(fname);
  if (!file) return ok_if_missing;
  std::string line;
  while (std::getline(file, line)) {
    if (!parse_command(line)) store_to_history(line);
  }
  return true;
}

void Sushi::store_to_history(std::string line) {
  if (line.empty()) return;
  if (history.size() >= HISTORY_LENGTH) history.pop_front();
  history.push_back(line);
}

void Sushi::show_history() {
  int i = 1;
  for (const auto& cmd : history) std::cout << std::setw(5) << i++ << "  " << cmd << '\n';
  std::cout << std::setw(5) << i << "  history" << '\n';
}

void Sushi::set_exit_flag() { exit_flag = true; }

bool Sushi::get_exit_flag() const { return exit_flag; }

void Sushi::prevent_interruption() {
  struct sigaction sa;
  sa.sa_handler = refuse_to_die;
  sa.sa_flags = SA_RESTART;
  sigaction(SIGINT, &sa, nullptr);
}

void Sushi::refuse_to_die(int signo) {
  UNUSED(signo);
  std::cerr << "Type exit to exit the shell\n";
}

void Sushi::mainloop() {
  while (true) {
    if (get_exit_flag()) break;

    const char *ps1 = std::getenv("PS1");
    std::cout << (ps1 ? ps1 : DEFAULT_PROMPT);
    std::cout.flush();

    std::string cmd = read_line(std::cin);

    if (!parse_command(cmd) && !re_execute()) {
      store_to_history(cmd);
    }

    if (get_exit_flag()) break;
  }
}

int Sushi::spawn(Program *exe, bool bg) {
  std::vector<pid_t> children;
  int in_fd = STDIN_FILENO;
  int pipefd[2];

  Program* current = exe;

  while (current) {
    bool has_pipe = current->pipe != nullptr;
    if (has_pipe && pipe(pipefd) < 0) {
      std::perror("pipe");
      return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
      std::perror("fork");
      return EXIT_FAILURE;
    }

    if (pid == 0) {
      // Child
      if (in_fd != STDIN_FILENO) {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
      }

      if (has_pipe) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
      }

      char* const* args = current->vector2array();
      execvp(args[0], args);
      std::perror(args[0]);
      _exit(EXIT_FAILURE);
    }

    // Parent
    children.push_back(pid);

    if (in_fd != STDIN_FILENO)
      close(in_fd);

    if (has_pipe) {
      close(pipefd[1]);
      in_fd = pipefd[0];
    }

    current = current->pipe;
  }

  if (!bg) {
    for (pid_t pid : children) {
      // DZ: status must be collected and stored in $? 
      waitpid(pid, nullptr, 0);
    }
  }

  return EXIT_SUCCESS;
}

// Two new methods to implement
void Sushi::pwd()
{
  std::cerr << "pwd: not implemented yet" << std::endl;
}

void Sushi::cd(std::string *s)
{
  std::cerr << "cd(" << *s << "): not implemented yet" << std::endl;
}

char* const* Program::vector2array() {
  assert(args);
  size_t n = args->size();
  char** arr = new char*[n + 1];
  for (size_t i = 0; i < n; ++i) arr[i] = const_cast<char*>((*args)[i]->c_str());
  arr[n] = nullptr;
  return arr;
}

Program::~Program() {}
