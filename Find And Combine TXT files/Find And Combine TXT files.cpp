
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>  // for _getch(); ... press any key funciton
#include <unordered_set>

#include "Traverser.h" 

// PURPOSE
//Combine all the found txt files found going down from the partent directory.


int main()
{
  
  Traverser tr;
  if (tr.combine()) {
    std::cout << "\n\nFinished\n\n" << std::endl;
  }

  


  _getch();

  return 0;
}

