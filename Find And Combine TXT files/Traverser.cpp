#pragma once

//share code between compilers
#if defined(_DEBUG) || defined(DEBUG)
#define IS_DEBUG_BUILD
#endif

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
        std::ofstream split_file(split_path, std::ios::out); // out should be used when creating a new file.

        // end of the file - start of the file + last file length > maximum 
        // if the amount of lines + last file lines does not go over the maximum file length, keep appending.
        // else close the old split file, create a new split file, reset line number, add 1 to file count and keep appending to the new split

        uint64_t appended_file_lengths = 0;
        uint64_t file_line_amount = 0;
        
        if (!merged_file.is_open()) {
          std::cerr << "\nFailed to open merge.txt file\n";          
          std::abort();
        }

        if (!split_file.is_open()) {
          std::cerr << "\nFailed to create or open split_file\n";
          std::abort();
        }

        for (int i = 0; i < file_ranges.size(); ++i) {
          //check file position vector
          if (file_ranges[i].size() != 2) {
            GlobalFunctions::log("ERROR", "nonvalid file range found.");
            for (auto& n : file_ranges[i]) {
              std::cout << n << ' ';
            }
            std::abort();
          }
          
          file_line_amount = file_ranges[i][1] - file_ranges[i][0];
          if (file_line_amount + appended_file_lengths < maximum_file_length) {
            //add the file lines to the split file. and add the lenth to the appended_file_length
            uint64_t file_start = file_ranges[i][0];
            uint64_t file_end = file_ranges[i][1];
            
            std::string line;
            //skip lines until in the right line
            for (int skip = 0; skip < file_start && std::getline(merged_file, line); ++skip) {
              //std::getline(merged_file, line); // only way to get the line 43 is to go through 42 lines before it.
            }

            //start appending the lines to the split file
            for (uint64_t i = file_start; i < file_end && std::getline(merged_file, line); ++i) {
              split_file << line; // add the line;
            }
            appended_file_lengths += file_line_amount;
          }
          else {
            //add 1 to file_count, create new split file and replace it with the old one, reset the last_file_length variable.
            ++file_count;
            split_file.flush();
            split_file.close();
            split_file.open(separated_dir / (base_filename + std::to_string(file_count)), std::ios::out); // open the new file.
            appended_file_lengths = 0;
          }
        }
      }
    }
    return true;
  }

  bool Traverser::combine() {

    std::vector<std::vector<std::uint64_t>> file_ranges{}; // hold the start and end positions of each file that is appended to the merge.
    std::uint64_t line_number = 0ULL; // holds the line number in the append file
    //open the created file.
    output.open(Output_file_location, std::ios::app);
    if (!output.is_open()) {
      GlobalFunctions::log("Failed to open output file at: " + Output_file_location.string(), "ERROR");
      std::abort();
    }

    std::vector<fs::filesystem_error> errors;

    //searches and combines files into one speficied file.
    //continues when there are no other directories to be seen
    while (!seenDirectories.empty()) {
      
      //change directory
      fs::path currentFolder = seenDirectories.back();
      seenDirectories.pop_back();
      GlobalFunctions::log("Now processing: " + currentFolder.string(), "DEBUG");

      fs::path startingDir = (startingPath / Program_Output);

      

      try {
        //start looking for files in the directory
        for (const auto& Entity : fs::directory_iterator(currentFolder)) {

          const auto filename = Entity.path().filename().string();

          // check if the file is the folder stared

          if (fs::equivalent(Entity.path(), startingDir)) {
            continue; // skips root in starting path folder
          }

          //cut out symlink directories
          if (fs::is_symlink(Entity.path())) {
            continue;
          }


          //cut out symlink files
          if (fs::is_regular_file(Entity.path())) {

            //if current file has accepted extension, append the contents to the merged.txt file
            if (acceptedExtensions.contains(Entity.path().extension().string())) {

              //open the found .txt file
              GlobalFunctions::log("ACTION", std::string("Opening target.txt: " + Entity.path().filename().string()));
              std::ifstream target(Entity.path());
              if (target.is_open()) {
                GlobalFunctions::log("ACTION", std::string("Treating target.txt: " + Entity.path().string()));
                std::vector<uint64_t> file_range = { line_number };
                output << '\'' << filename << '\'' << '\n';
                std::string line;

                //append the text from the file into the main text file
                while (std::getline(target, line)) {
                  ++line_number; // lines from files
                  output << line << '\n';
                }

                output << "\n";
                line_number++; // last '\n'
                file_range.push_back(line_number);

                // adds the file end to the latest file position added.  
                file_ranges.push_back(file_range);

                target.close();
              }
            }
          }


          //see if the entity is a directory and if directory is not root directory
          if (fs::is_directory(Entity.path()) && Entity.path() != Program_Output) {
            GlobalFunctions::log("ACTION", std::string("Found Directory: " + Entity.path().string()));
            seenDirectories.push_back(Entity.path());
          }
        }
      }catch(fs::filesystem_error& e) {        
        errors.push_back(e);
      }
      //output all errors found

     

    }   
    for (auto e : errors) {
      GlobalFunctions::log(std::string(e.what()), "ERROR");
    }
    std::cout << "\n\nAll errors seen: \n";
    output.flush(); // get rid of any leftovers in buffer
    output.close(); 

    

    GlobalFunctions::log("ACTION", "\nAsking user about split.\n");
    split(file_ranges); // ask the user if the wants to split the file if the file is too big. size is determined by variable 'maximum_file_length'
    return true;
  }
    

