#include "functions.h"
#include "yt-dlp.h"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

std::string ytdlp = std::filesystem::absolute("tools/yt-dlp.exe").string();

std::string getFfmpeg(){
    std::string ffmpeg_folder = searchFolder("tools", "ffmpeg");
    std::filesystem::path ffmpeg = std::filesystem::absolute(ffmpeg_folder + "/bin/ffmpeg.exe");
    return ("\"" + ffmpeg.string() + "\"");
}

std::string getVideoTitleFromSearch(const std::string& text){
    std::string cmd = ytdlp + " \"ytsearch1:" + text + "\"" + " --get-title";
    return exec(cmd.c_str());
}


std::string getVideoTitleFromURL(const std::string& text) {
    std::string cmd = ytdlp + " " + text + " --get-title";
    return exec(cmd.c_str());
}


bool checkDownload(const std::string& comando) {
    std::string output;
    char buffer[512];
    FILE* pipe = _popen(comando.c_str(), "r");
    if (!pipe) return false;

    while (fgets(buffer, sizeof(buffer), pipe)) output += buffer;
    _pclose(pipe);
    return (output.find("has already been downloaded") != std::string::npos);
}

int searchAndDownloadVideo(std::string& text, bool autor, bool convert)
{
    std::cout << "> Procurando video...." << "\n";
    std::this_thread::sleep_for(std::chrono::seconds((1 / 10)));
    std::string cmd = ytdlp + " \"ytsearch1:" + text + "\" --extract-audio " + (std::string)"--ffmpeg-location " + getFfmpeg() + " --audio-format mp3 -o \"musicas/%(title)s.%(ext)s\" >nul 2>&1";
    std::cout << "> Baixando...." << "\n";
    if (!checkDownload(cmd.c_str())) {
        std::cout << "> Música baixada com sucesso!" << "\n";
    }
    else std::cout << "[ERRO]: Aconteceu algum erro, Tente novamente." << "\n";
    convertAll(convert);
    
    if (autor) organizarPorAutor("musicas");
    return 0;
}


void downloadMusicUrl(const char* videoUrl, bool autor)
{
    std::cout << "> Procurando video...." << "\n";
    std::this_thread::sleep_for(std::chrono::seconds((1 / 10)));
    std::cout << "> baixando...." << "\n";
    std::string cmd = ytdlp + " --extract-audio " + (std::string)"--ffmpeg-location " + getFfmpeg() + " --audio-format mp3 -o \"musicas/%(title)s.%(ext)s\" \"" + (std::string)videoUrl + "\" >nul 2>&1";

    system(cmd.c_str());
    if (!checkDownload(cmd.c_str()))
    {
        std::cout << "> Música baixada com sucesso!" << "\n";
    }
    else
        std::cout << "[ERRO]: Aconteceu algum erro, Tente novamente." << "\n";
    if(autor) organizarPorAutor("musicas");
}
