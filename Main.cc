#include <cstdlib>
#include <iostream>
#include "Sushi.hh"

// Initialize the static constants
const size_t Sushi::MAX_INPUT = 256;
const size_t Sushi::HISTORY_LENGTH = 10;
const std::string Sushi::DEFAULT_PROMPT = "sushi> ";

int main(int argc, char *argv[])
{
  Sushi shell;

  std::cout << Sushi::DEFAULT_PROMPT;
  std::string input = shell.read_line(std::cin);

  if (input.empty()) {
    shell.store_to_history(input);
  }

  shell.show_history();

  return EXIT_SUCCESS;
}
