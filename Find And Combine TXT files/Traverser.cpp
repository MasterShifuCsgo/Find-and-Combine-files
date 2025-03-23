#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include "Traverser.h"


namespace fs = std::filesystem;

//logs data to the terminal
void log(std::string type, std::string msg) {
  std::cout << type << ": " << msg << std::endl;
}
  Traverser::Traverser() {
    //create the root path
    fs::create_directory(Program_Output);

    startingPath = fs::current_path();
    OutputPath = startingPath / Program_Output / OutputFileName;
    //merge.txt | append mode
    output.open(OutputPath, std::ios::app);
    //directorys found
    seenDirectories.push_back(fs::current_path());
  }
  
  //Splits the merged file into chunks, each of size maximum_file_size
  bool Traverser::split() {


    //if the merged file is too big, check if the user wants to split it.
    auto merged_file_size = fs::file_size(OutputPath);
    if (merged_file_size >= maximum_file_length) {
      char user_choise = 'N'; // default is no.
      std::cout << "\n\n------------CAUTION!------------" << std::endl;
      std::cout << "Merged file is more than maximum file length: " << maximum_file_length << std::endl;
      std::cout << "Would you like to seperate the file into chucks of " << maximum_file_length << "?" << std::endl;

      //ask the user if he wants to split the file into multiple parts
      while (true) {
        std::cout << "\'Y\' or \'N\': " << std::flush;
        std::cin >> user_choise;
        // if user choise is not a char
        if (std::cin.fail()) {
          std::cin.clear(); 
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
          std::cout << "Please Enter a valid choice \'Y\' or \'N\'\n";
          continue;
        }
        //user choise is not a valid choice
        else if (user_choise != 'Y' && user_choise != 'N') {
          std::cout << "Please enter \'Y\' or \'N\'\n";
          continue;
        }
        //clear the input buffer just incase the user inserted "Yawdw". awdw would stay in the input buffer.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        break;
      }

      //if user choise is 'N'
      //skip because it will evaluate to true anyways.

      //if user choice is 'Y'
      if (user_choise == 'Y') {






      }
      
    }
    return true;
  }


  bool Traverser::combine() {

    //searches and combines files into one speficied file.
    //continues when there are no other directories to be seen
    while (!seenDirectories.empty()) {
      
      //change directory
      fs::path currentFolder = seenDirectories.back();
      seenDirectories.pop_back();
      std::cout << "Directory Changed: " << currentFolder.string().substr(startingPath.string().size()) << '\n';

      fs::path startingDir = (startingPath / Program_Output);

      //start looking for files in the directory
      for (const auto& Entity : fs::directory_iterator(currentFolder)) {

        const auto path = Entity.path();
        const auto filename = Entity.path().filename().string();

        // check if the file is the folder stared
        
        if (fs::equivalent(Entity.path(), startingDir)) {
          continue; // skips that folder
        }

        //cut out symlink files
        if (fs::is_regular_file(path)) {

          //if current file has accepted extension, append the contents to the merged.txt file
          if (acceptedExtensions.contains(path.extension().string())) {

            //open the found .txt file
            std::ifstream target(path);

            //append the text from the file into the main text file                    
            std::cout << "File found: " << filename << '\n';
            output << '\'' << filename << '\'' << '\n';
            std::string line;
            while (std::getline(target, line)) {
              output << line << '\n';
            }
            output << "\n\n\n";
            target.close();
          }
        }

        //see if the entity is a directory and if directory is not root directory
        if (fs::is_directory(Entity.path()) && Entity.path() != Program_Output) {
          seenDirectories.push_back(Entity.path());
        }
      }
    }
    output.flush(); // get rid of any leftovers
    output.close(); // close file
    split(); // ask the user if the wants to split the file if the file is too big. size is determined by variable 'maximum_file_length'
    return true;
  }
    

