#include <iostream>
#include <thread>
#include <chrono>
#include "Traverser.h" // Handles file traversal + combination

int main() {
  if (Traverser().combine())
    std::cout << "\n\nFinished\n\n";

  for (int i = 3; i > 0; --i) {
    std::cout << "Exiting in " << i << " seconds\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}
