#pragma once

#ifndef YTDLP_H
#define YTDLP_H

#include <string>

std::string getVideoTitleFromSearch(const std::string& text);
bool checkDownload(const std::string& comando);
void downloadMusicUrl(const char* videoUrl, bool autor);
int searchAndDownloadVideo(std::string& text, bool autor, bool convert);

#endif