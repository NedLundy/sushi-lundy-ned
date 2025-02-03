#include <fstream>
#include <vector>
#include <iostream>
#include "Sushi.hh"

const size_t Sushi::MAX_INPUT = 256;
const size_t Sushi::HISTORY_LENGTH = 10;
const std::string Sushi::DEFAULT_PROMPT = "sushi> ";

// DZ: `history` is a lready defined in the Sushi class
//static std::vector<std::string> command_history;

// DZ: Must be a member function
// DZ: Many features are missing
std::string Sushi::read_line(std::istream &in)
{
  std::string line;
  if (!std::getline(in, line))
  {
    return "";
}
return line;
}

// DZ: Must be a member function
bool Sushi::read_config(const char *fname, bool ok_if_missing)
{
  std::ifstream config_file(fname);
  if (!config_file)
  {
    if (!ok_if_missing)
    {
      // DZ: Must use std::perror
      std::cerr << "Error: Configuration file not found: " << fname << std::endl;
}
    // DZ: Wrong, if "ok if missing" then return `true`
  return false;
}

std::string line;
while (std::getline(config_file, line))
{
  // DZ: Wrong checks
  if (!line.empty() && line[0] != '#') // DZ: Why #???
  {
        store_to_history(line);
  }
}
return true;

}

// DZ: Must be a member function
// DZ: `histoey, not `commmand_history`
void Sushi::store_to_history(std::string line)
{
  // DZ: Must check if `line` is empty
  if (history.size() >= Sushi::HISTORY_LENGTH)
    {
      history.erase(history.begin());
    }
    history.push_back(line);
}

// DZ: Must be a member function
void Sushi::show_history()
{
  for (const auto &cmd : history)
    {
      // DZ: Wrong format
        std::cout << cmd << std::endl;
  }
}

void Sushi::set_exit_flag()
{
  // To be implemented
}

bool Sushi::get_exit_flag() const
{
  return false; // To be fixed
}
