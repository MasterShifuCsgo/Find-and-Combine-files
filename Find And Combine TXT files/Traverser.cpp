#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <cstdint> 
#include "Traverser.h"
#include "GlobalFunctions.h"

namespace fs = std::filesystem;

  Traverser::Traverser() {
    //create the root path
    fs::create_directory(Program_Output);
    
    Output_file_location = fs::current_path() / Program_Output / OutputFileName;    
    
    //starting directory
    seenDirectories.push_back(fs::current_path());
  }
  
  //Splits the merged file into chunks, each of size maximum_file_size
  bool Traverser::split(std::vector<std::vector<std::uint64_t>>& file_ranges) {

    //if the merged file is too big, check if the user wants to split it.
    auto merged_file_size = fs::file_size(Output_file_location);
    if (merged_file_size >= maximum_file_length) {
      char user_choise = 'N'; // default is no.
      std::cout << "\n\n------------CAUTION!------------" << std::endl;
      std::cout << "Merged file is more than maximum file length: " << maximum_file_length << std::endl;
      std::cout << "Would you like to seperate the file into chucks of " << maximum_file_length << "?" << std::endl;

      //ask the user if he wants to split the file into multiple parts  
        while (true) {
          if (!GlobalFunctions::readUserInput<char>(user_choise, std::string("Please enter a char"))) {
            std::cout << "\'Y\' or \'N\': " << std::flush;
            continue;
          }          
          if (!GlobalFunctions::isValidChoice<char>(user_choise, { 'Y', 'N' }, std::string("Please enter the right choice"))){
            std::cout << "\'Y\' or \'N\': " << std::flush;
            continue;
          }
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          break; // all tests passed.
        }

      //if user choise is 'N'
      //skip because it will evaluate to true anyways.

      //if users choice is 'Y'
      if (user_choise == 'Y') {

        // since this place is ran once during the program, it wont have to be efficient.

        const fs::path output_dir = startingPath / Program_Output;
        const fs::path separated_dir = output_dir / "separated";
        fs::create_directory(separated_dir);

        const std::string base_filename = "split";
        std::uint64_t line_number = 0;
        std::uint64_t file_count = 0;

        std::ifstream merged_file(Output_file_location);

        // Use a helper to construct full split path
        auto split_path = separated_dir / (base_filename + std::to_string(file_count));
        std::ifstream split_file(split_path);

        // go to the line number given in file_ranges
        // if the amount of lines does not go over the maximum file length keep appending.
        // else close the old split file, create a new split file, reset line number, add 1 to file count and keep appending to the new split




      }
    }
    return true;
  }


  bool Traverser::combine() {

    std::vector<std::vector<std::uint64_t>> files_ranges{}; // hold the start and end positions of each file that is appended to the merge.
    std::uint64_t line_number = 0ULL; // holds the line number in the append file
    //open the created file.
    output.open(Output_file_location, std::ios::app);
    //searches and combines files into one speficied file.
    //continues when there are no other directories to be seen
    while (!seenDirectories.empty()) {
      
      //change directory
      fs::path currentFolder = seenDirectories.back();
      seenDirectories.pop_back();
      
      fs::path startingDir = (startingPath / Program_Output);

      //start looking for files in the directory
      for (const auto& Entity : fs::directory_iterator(currentFolder)) {
       
        const auto filename = Entity.path().filename().string();

        // check if the file is the folder stared
        
        if (fs::equivalent(Entity.path(), startingDir)) {
          continue; // skips that folder
        }

        //cut out symlink files
        if (fs::is_regular_file(Entity.path())) {

          //if current file has accepted extension, append the contents to the merged.txt file
          if (acceptedExtensions.contains(Entity.path().extension().string())) {

            //open the found .txt file
            std::ifstream target(Entity.path());
                        
            files_ranges.push_back({ line_number }); // creates a new instance in the vector where 1 file begins.
            output << '\'' << filename << '\'' << '\n';            
            std::string line;

            //append the text from the file into the main text file
            while (std::getline(target, line)) {
              ++line_number; // lines from files
              output << line << '\n';
            }

            output << "\n";
            line_number++; // last '\n'
            files_ranges.push_back(files_ranges[files_ranges.size()-1]); // adds the file end to the latest file position added.
            target.close();
          }
        }

        //see if the entity is a directory and if directory is not root directory
        if (fs::is_directory(Entity.path()) && Entity.path() != Program_Output) {
          seenDirectories.push_back(Entity.path());
        }
      }
    }
    output.flush(); // get rid of any leftovers in buffer
    output.close(); 
    split(files_ranges); // ask the user if the wants to split the file if the file is too big. size is determined by variable 'maximum_file_length'
    return true;
  }
    

