#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <csignal>
#include "Sushi.hh"

const size_t Sushi::MAX_INPUT = 256;
const size_t Sushi::HISTORY_LENGTH = 10;
const std::string Sushi::DEFAULT_PROMPT = "sushi> ";

std::string Sushi::read_line(std::istream &in) {
    std::string line;
    if (!std::getline(in, line)) {
        return "";
    }
    return line;
}

bool Sushi::read_config(const char *fname, bool ok_if_missing) {
    std::ifstream config_file(fname);
    if (!config_file) {
        if (!ok_if_missing) {
            std::perror("Error: Configuration file not found");
        }
        return ok_if_missing;
    }
    std::string line;
    while (std::getline(config_file, line)) {
        if (!line.empty() && line[0] != '#') {
            if (parse_command(line) == 0) {
                store_to_history(line);
            }
        }
    }
    return true;
}

void Sushi::store_to_history(const std::string &line) {
    if (line.empty()) return;
    if (history.size() >= HISTORY_LENGTH) {
        history.erase(history.begin());
    }
    history.push_back(line);
}

void Sushi::show_history() const {
    for (size_t i = 0; i < history.size(); ++i) {
        std::cout << i + 1 << ": " << history[i] << std::endl;
    }
}

void Sushi::set_exit_flag() {
    exit_flag = true;
}

bool Sushi::get_exit_flag() const {
    return exit_flag;
}

std::string* Sushi::unquote_and_dup(const char *s) {
    if (!s) return nullptr;
    return new std::string(s);
}

void Sushi::re_parse(int i) {
    if (i <= 0 || i > static_cast<int>(history.size())) {
        std::cerr << "Error: !" << i << ": event not found" << std::endl;
        return;
    }
    std::string command = history[i - 1];
    std::cout << "Re-executing: " << command << std::endl;
    if (parse_command(command) == 0) {
        store_to_history(command);
    }
}

int Sushi::spawn(Program *exe, bool bg) {
    if (!exe) return EXIT_FAILURE;
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Error: Failed to fork process" << std::endl;
        return EXIT_FAILURE;
    }
    if (pid == 0) { // Child process
        char* const* argv = exe->vector2array();
        execvp(argv[0], argv);
        std::cerr << "Error: Command execution failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (!bg) {
        waitpid(pid, nullptr, 0);
    }
    return EXIT_SUCCESS;
}

void Sushi::prevent_interruption() {
    struct sigaction sa;
    sa.sa_handler = Sushi::refuse_to_die;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, nullptr);
}

void Sushi::refuse_to_die(int signo) {
    if (signo == SIGINT) {
        std::cout << "\nType exit to exit the shell" << std::endl;
    }
}

char* const* Program::vector2array() {
    
    return nullptr;
}

void Program::free_array(char *const argv[]) {
    
    UNUSED(argv);
}

Program::~Program() {
   
}
