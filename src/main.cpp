#include <iostream>

#include <StepReader.hpp>
#include <Part.hpp>

int main(int argc, char const *argv[])
{
  Ge::PartBuilder reader("models/cube806020.step");
  //std::cout << reader.succeeded() << std::endl;

  // while (true)
  // {
  //   std::cout << "HI" << std::endl;
  // }

  return 0;
}
