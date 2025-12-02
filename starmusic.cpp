/*

*/

#include <iostream>
#include <cstdio>
#include <filesystem>  
#include <cstdlib>
#include <string>

#include "curl/curl.h"

#include "devscripts/functions.h"
#include "devscripts/yt-dlp.h"

extern std::string ytdlp;

// organizar
struct Config {
    bool search  = false;
    bool palavra = false;
    bool autor   = false;
    bool convert = true;
};
Config config;

namespace fs = std::filesystem;

int printHelp(char* argv[]) {
    std::cout << "Uso: " << argv[0] << " [opcoes]\n\n";
    std::cout << "opcoes:\n";
    std::cout << std::left; 
    std::cout << "  -h, --help"    << std::setw(23) << " " << "Mostra esta mensagem de ajuda\n";
    std::cout << "  -s, --search"  << std::setw(21) << " " << "Pesquisar video, Ex: " + (std::string)argv[0] + " Love Hurts (1976) -s\n";
    std::cout << "  -c, --convert" << std::setw(20) << " " << "Desativar a conversao (nativa) de todos os arquivos .webm em .mp3\n";
    std::cout << "  -a, --autor" << std::setw(22) << " " << "Organizar pelo autor da musica\n";
    std::cout << "  -p, --palavra" << std::setw(20) << " " << "Organizar por alguma palavra, Ex: " + (std::string)argv[0] + " RDP -p\n";
    return 0;
}

int main(int argc, char* argv[])
{
    if (!std::filesystem::exists("tools"))
    {
        fs::create_directories("tools");
        std::cout << " > Baixando dependências..." << "\n";

        //
        // Install ytdlp
        //
        const std::wstring ytDlpUrl = L"https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp.exe";
        
        std::cout << "[1/4]: Instalando yt-dlp.." << "\n";
        downloadFile(ytDlpUrl, L"tools/yt-dlp.exe");
        
        //
        // Install ffmpeg
        //
        const std::wstring ffmpegZipUrl = L"https://www.gyan.dev/ffmpeg/builds/ffmpeg-release-essentials.zip";

        std::cout << "[2/4]: Instalando ffmpeg [2 / 2].." << "\n";
        downloadFile(ffmpegZipUrl, L"tools/temp.zip");
        extractZip(L"tools/temp.zip", L"tools/");

        //
        // Deletar os temp
        //
        std::cout << "[3/4]: Apagando arquivos temporarios.." << "\n";
        fs::remove("tools/temp.zip");

        std::cout << "[4/4] STARmusic está pronto para ser usado!" << "\n";
    }

    if (!fs::exists("musicas")) fs::create_directory("musicas");
    std::string ffmpeg_folder = searchFolder("tools", "ffmpeg");

    //
    // flags
    //
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <url_do_youtube>\n";
        std::cerr << "\t\t\t--help, -h";
        return 1;
    }
    
    std::string beforeSearch;
    bool search = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") return printHelp(argv);
        if (arg == "--autor" || arg == "-a") config.autor = true;
        if (arg == "--palavra" || arg == "-p") config.palavra = true;
        if (arg == "--search" || arg == "-s") config.search = true;
        if (arg == "--convert" || arg == "-c") config.convert = false;

        if (!beforeSearch.empty()) 
            beforeSearch += " ";
        beforeSearch += arg;
    }

    if(search) return searchAndDownloadVideo(beforeSearch, config.autor, config.convert);
    else
    {
        std::string arg = argv[1];
        if (arg.find("youtu.be") != std::string::npos || arg.find("youtube.com") != std::string::npos)
        {
            downloadMusicUrl(argv[1], config.autor);
        }
        else
        {
            if (config.autor)
            {
                organizarPorAutor("musicas");
                return 0;
            }
            if (config.palavra)
            {
                organizarPorPalavra("musicas", beforeSearch);
                return 0;
            }
            convertAll(config.convert);

            std::cout << "[ ERRO ]: Parece que isso não é um link do youtube." << "\n";
            std::cout << "[ ERRO ]: Use algo como: " << argv[0] << " https://www.youtube.com/watch?v=PIiINg7pNfo" << "\n";
            return 1;
        }
    }
    return convertAll(config.convert);
}

