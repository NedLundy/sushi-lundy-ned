#include <cstdlib>
#include <iostream>
#include "Sushi.hh"

Sushi my_shell;

int main(int argc, char *argv[]) {
    Sushi::prevent_interruption();

    const char *home_dir = std::getenv("HOME");
    if (home_dir) {
        std::string config_path = std::string(home_dir) + "/" + Sushi::DEFAULT_CONFIG;
        if (!my_shell.read_config(config_path.c_str(), true)) {
            std::cerr << "Warning: failed to read config file\n";
        }
    }

    // Handle script execution
    for (int i = 1; i < argc; ++i) {
        if (!my_shell.read_config(argv[i], false)) {
            std::cerr << "Failed to read script file: " << argv[i] << "\n";
            return EXIT_FAILURE; 
        }
    }

    // Only enter mainloop if shell isn't supposed to exit
    if (!my_shell.get_exit_flag()) {
        my_shell.mainloop();
    }

    return EXIT_SUCCESS;
};

