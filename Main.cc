#include <cstdlib>
#include <iostream>
#include "Sushi.hh"

// Initialize the static constants
const size_t Sushi::MAX_INPUT = 256;
const size_t Sushi::HISTORY_LENGTH = 10;
const std::string Sushi::DEFAULT_PROMPT = "sushi> ";

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    const char *home_dir = std::getenv("HOME");
    if (!home_dir)
    {
        std::cerr << "Error: HOME environment variable not set." << std::endl;
        return EXIT_FAILURE;
    }

    std::string config_path = std::string(home_dir) + "/sushi.conf";
    if (!read_config(config_path.c_str(), true))
    {
        std::cerr << "Error: Failed to read configuration file." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << Sushi::DEFAULT_PROMPT;
    std::string command = read_line(std::cin);
    if (command.empty())
    {
        std::cerr << "Error: Failed to read command." << std::endl;
        return EXIT_FAILURE;
    }

    store_to_history(command);
    show_history();

    return EXIT_SUCCESS;
}
