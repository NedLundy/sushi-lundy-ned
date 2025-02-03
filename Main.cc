#include <cstdlib>
#include <iostream>
#include "Sushi.hh"

Sushi my_shell; // New global var
// Initialize the static constants
// DZ: Duplicates
//const size_t Sushi::MAX_INPUT = 256;
//const size_t Sushi::HISTORY_LENGTH = 10;
//const std::string Sushi::DEFAULT_PROMPT = "sushi> ";

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    const char *home_dir = std::getenv("HOME");
    // DZ: No need to exit because "ok if missing"
    if (!home_dir)
    {
        std::cerr << "Error: HOME environment variable not set." << std::endl;
        return EXIT_FAILURE;
    }

    std::string config_path = std::string(home_dir) + "/sushi.conf";
    if (!my_shell.read_config(config_path.c_str(), true))
    {
      // DZ: Error displayed in the wrong function
      // std::cerr << "Error: Failed to read configuration file." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << Sushi::DEFAULT_PROMPT;
    std::string command = my_shell.read_line(std::cin);
    if (command.empty())
    {
      // DZ: Not necessarily an error
        std::cerr << "Error: Failed to read command." << std::endl;
        return EXIT_FAILURE;
    }

    my_shell.store_to_history(command);
    my_shell.show_history();

    return EXIT_SUCCESS;
}
