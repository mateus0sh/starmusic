# STARmusic ![Badge](https://img.shields.io/static/v1?label=cpp&message=Linguagem&color=blue&style=for-the-badge&logo=cpp)
> Bibliotecas Usadas: [FFmpeg](https://github.com/FFmpeg/FFmpeg) [yt-dlp](https://github.com/yt-dlp/yt-dlp)

#### Video de [demonstração](https://youtu.be/OnR5WoHx7vk)

# USO
```bash
Uso: starmusic.exe [opcoes]

opcoes:
  -h, --help                       Mostra esta mensagem de ajuda
  -s, --search                     Pesquisar video, Ex: starmusic.exe Love Hurts (1976) -s
  -c, --convert                    Desativar a conversao (nativa) de todos os arquivos .webm em .mp3
  -a, --autor                      Organizar pelo autor da musica
  -p, --palavra                    Organizar por alguma palavra, Ex: starmusic.exe RDP -p
```

---
> [!NOTE]
> Por enquanto usa apenas o primeiro parometro da divisão do autor e a musica, portanto se o arquivo tiver como:</br>
> - ✅ `Megadeth : Symphony of Destruction`  - Vai Organizar.</br>
> - ✅ `Megadeth - Symphony of Destruction`  - Vai Organizar.</br>
> - ❌ `Symphony of Destruction - Megadeth`  - Vai para a pasta de "Desconhecidos" (ou "Symphony of Destruction").</br></br>
> > Esse sistema está em testes, porém futuramente irei trazer uma atualização da melhora do mesmo, e possívelmente uma inclusão de um Menu GUI.

```python
starmusic.exe -o
# Organiza os arquivos.

starmusic.exe Bosta Rala - Quem Quer? -s
# Pesquisa no Youtube a Musica "Bosta Rala - Quem Quer?" e baixa o primeiro resultado.

starmusic.exe Bosta Rala - Quem Quer? -s -c
# Vai pesquisar e baixar a musica, mas se tiver em .webm não vai converter.

starmusic.exe https://youtu.be/b_2oaZeh7Wo
# Vai baixar musica pelo URL e converter automaticamente.

starmusic.exe https://youtube.com/playlist?list=PLLHyHi7NUwSakx0ONnipN-SoTRWxztStO
# Vai baixar a playlist completa pelo URL e converter automaticamente.
```

