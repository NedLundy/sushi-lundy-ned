#include <fstream>
#include <vector>
#include <iostream>
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

std::string Sushi::unquote_and_dup(const std::string &s) {
    std::string result;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            switch (s[++i]) {
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case '\\': result += '\\'; break;
                default: result += s[i]; break;
            }
        } else {
            result += s[i];
        }
    }
    return result;
}

void Sushi::re_parse(int i) {
    if (i <= 0 || i > history.size()) {
        std::cerr << "Error: !" << i << ": event not found" << std::endl;
        return;
    }
    std::string command = history[i - 1];
    std::cout << "Re-executing: " << command << std::endl;
    if (parse_command(command) == 0) {
        store_to_history(command);
    }
}