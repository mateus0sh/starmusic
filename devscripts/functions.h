#pragma once

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <filesystem>  
#include <string>

namespace fs = std::filesystem;

std::vector<std::string> split(const std::string& str, char delimitador);
void convertWebmToMp3(const fs::path& webmFile);
bool downloadFile(const std::wstring& url, const std::wstring& pasta);
bool extractZip(const std::wstring& zipPath, const std::wstring& outputDir);
std::string searchFolder(const fs::path& baseDir, const std::string& prefixo);
std::string exec(const char* cmd);
std::string getAuthorFromMp3(const fs::path& file);
int convertAll(bool convert);
int organizarPorAutor(std::string baseDir);
int organizarPorPalavra(const std::string& baseDir, const std::string& palavra);

#endif