#pragma once

#include <string>
#include <vector>

namespace GlobalFunctions {

#if defined(_DEBUG) || defined(DEBUG)
  inline void log(const std::string& message = "", const std::string& type = "") {
    if (type == "") {
      std::cout << message << std::endl;
    }
    else {
      std::cout << type << ':' << message << std::endl;
    }
  }
#else
  inline void log(const std::string& message = "", const std::string& type = "") {}
#endif

  // Attempts to read input into `user_choice`. 
  // If the input stream is in a failed state (e.g., invalid type),
  // prints `message_when_failed`, clears the error, and returns false.
  // Otherwise, returns true.
  template<typename T>
  bool readUserInput(T& user_choice, const std::string message_when_failed) {
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

  // Validates if `user_choice` is in the `valid_choices` list.
  // If not, prints `message_when_failed` and returns false.
  // Otherwise, returns true.
  template<typename T>
  bool isValidChoice(const T& user_choice, const std::vector<T>& valid_choices, const std::string message_when_failed) {
    for (const T& choice : valid_choices) {
      if (user_choice == choice) {
        return true;
      }
    }

    std::cout << message_when_failed << '\n';
    return false;
  };
 


}
