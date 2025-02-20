#include <cstdlib>
#include <iostream>
#include "Sushi.hh"

Sushi my_shell;

int main(int argc, char *argv[]) {
    UNUSED(argc);
    UNUSED(argv);

  // New function call
  Sushi::prevent_interruption();
  
    const char *home_dir = std::getenv("HOME");
    if (!home_dir) {
        std::cerr << "Error: HOME environment variable not set." << std::endl;
        return EXIT_FAILURE;
    }

    std::string config_path = std::string(home_dir) + "/sushi.conf";
    my_shell.read_config(config_path.c_str(), true);

    while (!my_shell.get_exit_flag()) {
        std::cout << Sushi::DEFAULT_PROMPT;  
        std::string command = my_shell.read_line(std::cin);

        if (command.empty()) {
            continue;
        }

        if (my_shell.parse_command(command) == 0) {
            my_shell.store_to_history(command);
        }
    }
    return EXIT_SUCCESS;
}
