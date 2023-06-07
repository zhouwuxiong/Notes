#include <iostream>
#include <fstream>
#include <sstream>
int main()
{
  std::stringstream file;
  std::cout << "Default buffer size: " << file.rdbuf()->in_avail() << std::endl;

  return 0;
}