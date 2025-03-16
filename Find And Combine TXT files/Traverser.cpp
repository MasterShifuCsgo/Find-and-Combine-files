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

  bool Traverser::combine() {

    //continues when there are no other directories to be seen
    while (!seenDirectories.empty()) {
      
      //change directory
      fs::path currentFolder = seenDirectories.back();
      seenDirectories.pop_back();
      std::cout << "Directory Changed: " << currentFolder.string().substr(startingPath.string().size()) << '\n';

      //start looking for files in the directory
      for (const auto& Entity : fs::directory_iterator(currentFolder)) {

        // check if the file is the merged file itself.
        if (Entity.path().filename().string() == OutputFileName) {
          break;
        }

        //cut out symlink files
        if (fs::is_regular_file(Entity.path())) {

          //if current file has accepted extension, append the contents to the merged.txt file
          if (acceptedExtensions.contains(Entity.path().extension().string())) {

            //open the found .txt file
            std::ifstream target(Entity.path());

            //append the text from the file into the main text file                    
            std::cout << "File found: " << Entity.path().filename().string() << '\n';
            output << Entity.path().filename() << '\n';
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

    return true;
  }

