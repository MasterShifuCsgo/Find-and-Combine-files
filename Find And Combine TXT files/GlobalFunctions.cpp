#include "GlobalFunctions.h"
#include <iostream>
#include <vector>
#include <limits>
#include <string>


template<typename T>
bool GlobalFunctions::readUserInput(T& user_choice, const std::string message_when_failed) {
  std::cin >> user_choice;

  if (std::cin.fail()) {
    std::cin.clear(); // Reset error state
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input

    std::cout << "\nInvalid Choice!\n";
    std::cout << message_when_failed << '\n';
    return false;
  }

  return true;
}

template<typename T>
bool GlobalFunctions::isValidChoice(const T& user_choice, const std::vector<T> valid_choices, const std::string message_when_failed) {
  for (const T& choice : valid_choices) {
    if (user_choice == choice) {
      return true;
    }
  }

  std::cout << message_when_failed << '\n';
  return false;
}
