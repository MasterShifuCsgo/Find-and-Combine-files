
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>  // for _getch(); ... press any key funciton
// PURPOSE

/*
 Combine all the found txt files found going down from the partent directory and combine them into one txt file.

*/
namespace fs = std::filesystem;

constexpr const char* Program_Output("root"); //folder
constexpr const char* extension(".md");
constexpr const char* fileName("merged");
const std::string OutputFileName = std::string(fileName) + extension;

int main()
{
  //create the root path
  fs::create_directory(Program_Output);
  fs::path OutputPath{ fs::current_path() / Program_Output / OutputFileName };

  //merge.txt | append mode
  std::ofstream output(OutputPath, std::ios::app);

  //directorys found
  std::vector<fs::path> seenDirectories{fs::current_path()};
  
  //continues when there are no other directories to be seen
  while (!seenDirectories.empty()) {
    fs::path currentFolder = seenDirectories.back();
    seenDirectories.pop_back();
    std::cout << "Directory Changed: " << currentFolder.string() << '\n';

    //start looking for files in the directory
    for (const auto& Entity : fs::directory_iterator(currentFolder)) {
    
      if (Entity.path().filename().string() == OutputFileName) {
        break;
      }

      //cut out symlink files
      if (fs::is_regular_file(Entity.path())) {
        
        //if current file is a txt file, append the contents to the merged.txt file
        if (Entity.path().extension() == extension) {
          
          //open the found .txt file
          std::ifstream target (Entity.path());          

          //append the text from the file into the main text file                    
          std::cout << "File found: " << Entity.path().filename().string() << '\n';
          output << Entity.path().filename() << '\n';
          std::string line;
          while (std::getline(target, line)) {
            output << line;
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

  
  _getch();

  return 0;
}

