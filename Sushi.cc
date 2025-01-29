#include <fstream>
#include <vector>
#include <iostream>
#include "Sushi.hh"

const size_t Sushi::MAX_INPUT = 256;
const size_t Sushi::HISTORY_LENGTH = 10;
const std::string Sushi::DEFAULT_PROMPT = "sushi> ";

static std::vector<std::string> command_history;

std::string read_line(std::istream &in)
{
  std::string line;
  if (!std::getline(in, line))
  {
    return "";
}
return line;
}

bool read_config(const char *fname, bool ok_if_missing)
{
  std::ifstream config_file(fname);
  if (!config_file)
  {
    if (!ok_if_missing)
    {
      std::cerr << "Error: Configuration file not found: " << fname << std::endl;
}
  return false;
}

std::string line;
while (std::getline(config_file, line))
{
  if (!line.empty() && line[0] != '#') 
  {
        store_to_history(line);
  }
}
return true;

}

void store_to_history(std::string line)
{
    if (command_history.size() >= Sushi::HISTORY_LENGTH)
    {
        command_history.erase(command_history.begin());
    }
    command_history.push_back(line);
}

void show_history()
{
    for (const auto &cmd : command_history)
    {
        std::cout << cmd << std::endl;
  }
}