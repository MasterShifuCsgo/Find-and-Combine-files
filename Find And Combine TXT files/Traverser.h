#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <cstdint>

namespace fs = std::filesystem;

constexpr const char* Program_Output("root"); //folder
constexpr const char* extension(".txt");
constexpr const char* fileName("merged");
constexpr const uint32_t maximum_file_length = 500'000;

class Traverser {

  const std::string OutputFileName = std::string(fileName) + extension;

  fs::path OutputPath;
  fs::path startingPath;
  std::ofstream output;
  std::vector<fs::path> seenDirectories;
  
  std::unordered_set<std::string> acceptedExtensions2{
    ".txt",
    ".md"
  };

  std::unordered_set<std::string> acceptedExtensions{
    ".txt",   // Plain text file
    ".md",    // Markdown file
    ".pdf",   // Portable Document Format
    ".doc",   // Microsoft Word document
    ".docx",  // Microsoft Word document (XML-based)
    ".rtf",   // Rich Text Format
    ".odt",   // OpenDocument Text Document
    ".html",  // HTML file
    ".htm",   // HTML file (alternative extension)
    ".csv",   // Comma-separated values
    ".xml",   // XML file
    ".json",  // JSON file
    ".yaml",  // YAML file
    ".yml",   // YAML file (alternative extension)
    ".epub",  // EPUB eBook file
    ".mobi",  // MOBI eBook file
    ".tex",   // LaTeX source file
    ".rst",   // reStructuredText file
    ".log",   // Log file
    ".asc",   // ASCII-armored file (usually GPG, PGP, or plain text)

    // Programming files
    ".cpp",   // C++ source code file
    ".h",     // C/C++ header file
    ".hpp",   // C++ header file (alternative extension)
    ".c",     // C source code file
    ".cs",    // C# source code file
    ".java",  // Java source code file
    ".js",    // JavaScript file
    ".py",    // Python source code file
    ".go",    // Go source code file
    ".php",   // PHP source code file
    ".rb",    // Ruby source code file
    ".swift", // Swift source code file
    ".rs",    // Rust source code file
    ".ts",    // TypeScript file
    ".scala", // Scala source code file
    ".pl"     // Perl script
  };


public:
  Traverser();
  bool combine();
  bool split();

};


