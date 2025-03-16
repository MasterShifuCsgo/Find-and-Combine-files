#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

constexpr const char* Program_Output("root"); //folder
constexpr const char* extension(".txt");
constexpr const char* fileName("merged");

class Traverser {

  const std::string OutputFileName = std::string(fileName) + extension;

  fs::path OutputPath;
  fs::path startingPath;
  std::ofstream output;
  std::vector<fs::path> seenDirectories;
  
  std::unordered_set<std::string> acceptedExtensions{
    ".txt",
    ".md"
  };

public:
  Traverser();
  bool combine();

};


