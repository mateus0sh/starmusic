#include "functions.h"
#include <urlmon.h>
#include <sstream>
#include <algorithm>  // std::remove
#pragma comment(lib, "urlmon.lib")

bool downloadFile(const std::wstring& url, const std::wstring& pasta) {
    HRESULT hr = URLDownloadToFileW(NULL, url.c_str(), pasta.c_str(), 0, NULL);
    return SUCCEEDED(hr);
}



bool extractZip(const std::wstring& zipPath, const std::wstring& outputDir) {
    std::wstring command = L"powershell -Command \"Expand-Archive -Force '" + zipPath + L"' '" + outputDir + L"'\"";
    int result = _wsystem(command.c_str());
    return result == 0;
}

std::string searchFolder(const fs::path& baseDir, const std::string& prefixo) {
    for (const auto& entry : fs::directory_iterator(baseDir)) {
        if (entry.is_directory()) {
            std::string nome = entry.path().filename().string();
            if (nome.rfind(prefixo, 0) == 0) return entry.path().string();
        }
    }
    return "";
}

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = _popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (fgets(buffer, sizeof buffer, pipe) != NULL)
        result += buffer;
    _pclose(pipe);
    return result;
}


void convertWebmToMp3(const fs::path& webmFile) {
    fs::path mp3File = webmFile;
    mp3File.replace_extension(".mp3");
    std::string ffmpeg_folder = searchFolder("tools", "ffmpeg");
    std::string command = ffmpeg_folder + "\\bin\\ffmpeg.exe -i \"" + webmFile.string() + "\" -vn -ar 44100 -ac 2 -b:a 192k \"" + mp3File.string() + "\" -y >nul 2>&1";
    std::cout << "[..] Convertendo: " << webmFile.string() << " -> " << mp3File.filename() << "\n";

    int result = system(command.c_str());
    if (result == 0) {
        std::cout << "[OK] Conversao concluída: " << mp3File.filename() << "\n";
        fs::remove(webmFile);
    }
    else {
        std::cerr << "[ERRO] Falha ao converter: " << webmFile.filename() << "\n";
    }
}

int convertAll(bool convert)
{
    if (convert)
    {
        std::cout << "[ ! ]: Verificando Arquivos.." << "\n";
        fs::path musicDir = "musicas";

        for (const auto& entry : fs::directory_iterator(musicDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".webm") {
                convertWebmToMp3(entry.path());
            }
        }
        std::cout << "[ ! ]: Verificando Arquivos.. OK" << "\n";

        return 1;
    }
}

std::string getffprobe()
{
    std::string ffmpeg_folder = searchFolder("tools", "ffmpeg");
    std::filesystem::path ffmpeg = std::filesystem::absolute(ffmpeg_folder + "/bin/ffprobe.exe");
    return ffmpeg.string();
}

std::vector<std::string> split(const std::string& str, char delimitador) {
    std::vector<std::string> partes;
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, delimitador)) partes.push_back(item);
    return partes;
}

std::string getAuthorFromMp3(const fs::path& file) {
    std::string cmd = getffprobe() + " -v quiet -show_entries format_tags=artist "
        "-of default=noprint_wrappers=1:nokey=1 \"" + file.string() + "\"";

    FILE* pipe = _popen(cmd.c_str(), "r");

    char buffer[256];
    std::string author;
    if (fgets(buffer, sizeof(buffer), pipe)) {
        author = buffer;
        while (!author.empty() && (author.back() == '\n' || author.back() == '\r'))
            author.pop_back();
    }

    _pclose(pipe);

    if (author.empty() || !pipe)
    {
        std::string texto = file.string();
        texto.erase(std::remove(texto.begin(), texto.end(), ' '), texto.end());
        if (texto.find("-") != std::string::npos) return split(texto, '-')[0];
        else if (texto.find(":") != std::string::npos) return split(texto, ':')[0];
    }
    return "Desconhecido";
}


int organizarPorAutor(std::string baseDir) {

    fs::path pasta = fs::absolute(baseDir);
    if (!fs::exists(pasta)) {
        std::cerr << "Diretório não existe!\n";
        return 1;
    }

    int count = 0;
    for (const auto& entry : fs::directory_iterator(pasta)) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().extension() != ".mp3") continue;

        std::string autor = getAuthorFromMp3(entry.path());
        fs::path destino = pasta / autor;
        if (!fs::exists(destino)) fs::create_directory(destino);

        std::error_code ec;
        fs::rename(entry.path(), (destino / entry.path().filename()), ec);

        if (ec) {
            std::cerr << "[ERRO] Falha ao mover: " << ec.message() << "\n";
        }
        else {
            std::cout << "[MOVENDO] " << entry.path().filename() << " -> " << destino << "\n";
            count++;
        }
    }
    std::cout << "Arquivos movidos: " << count << "\n";
}

int organizarPorPalavra(const std::string& baseDir, const std::string& palavra) {

    fs::path pasta = fs::absolute(baseDir);
    if (!fs::exists(pasta)) {
        std::cerr << "Diretório não existe!\n";
        return 1;
    }

    fs::path destino = pasta / palavra;
    if (!fs::exists(destino)) fs::create_directory(destino);

    int count = 0;

    for (const auto& entry : fs::directory_iterator(pasta)) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().extension() != ".mp3") continue;

        std::string nome = entry.path().filename().string();
        if (nome.find(palavra) == std::string::npos) continue;

        std::error_code ec;
        fs::rename(entry.path(), destino / entry.path().filename(), ec);

        if (ec) std::cerr << "[ERRO] Falha ao mover: " << ec.message() << "\n";
        else {
            std::cout << "[MOVENDO] " << nome << " -> " << destino << "\n";
            count++;
        }
    }

    std::cout << "Arquivos movidos: " << count << "\n";
    return 0;
}
