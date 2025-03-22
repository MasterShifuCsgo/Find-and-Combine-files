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
    std::cout << type << ": " << msg << "\n";
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

  bool Traverser::combine() {

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
        if (Entity.path() == startingDir) {
          continue; // skip that folder
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
    output.flush();

    return true;
  }

