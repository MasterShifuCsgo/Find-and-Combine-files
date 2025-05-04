#include <iostream>
#include <thread>
#include <chrono>

#include "Traverser.h" 

// PURPOSE
//Combine all the found txt files found going down from the partent directory.

// The program will search for all the files in the current directory and all subdirectories.
int main()
{
  
  Traverser tr;
  if (tr.combine()) {
    std::cout << "\n\nFinished\n\n" << std::endl;
  }

  int seconds = 3;

  for (int i = seconds; i > 0; i--) {
    std::cout << "exiting in " << i << "seconds\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  
  

  return 0;
}

