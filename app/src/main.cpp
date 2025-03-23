#include<iostream>
#include<binary_loader.h>

int main(int argc, char** argv)
{
  binary_loader loader;
  
  loader.load("../../app/res/file.bin");
  
  std::cout << loader.data() << std::endl;
  std::cin.get();
}