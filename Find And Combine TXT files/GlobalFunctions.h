#pragma once

#include <string>
#include <vector>

namespace GlobalFunctions {

  // Attempts to read input into `user_choice`. 
  // If the input stream is in a failed state (e.g., invalid type),
  // prints `message_when_failed`, clears the error, and returns false.
  // Otherwise, returns true.
  template<typename T>
  bool readUserInput(T& user_choice, std::string message_when_failed);

  // Validates if `user_choice` is in the `valid_choices` list.
  // If not, prints `message_when_failed` and returns false.
  // Otherwise, returns true.
  template<typename T>
  bool isValidChoice(T& user_choice, std::vector<T> valid_choices, std::string message_when_failed);
 
}
