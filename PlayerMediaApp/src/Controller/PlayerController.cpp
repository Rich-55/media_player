#include "../../include/Controller/PlayerController.h"

PlayerController::PlayerController(MediaFileManager& mediaFileManager) : mediaFileManager(mediaFileManager) {}

void PlayerController::playMusic(std::string mediaFileName) {
     std::shared_ptr<MediaFile> mediaFile = mediaFileManager.getMediaFile(mediaFileName);
    if (!mediaFile) {
        std::cerr << "Media file not found: " << mediaFileName << std::endl;
        return;
    }

    // Khởi tạo SDL và SDL_Mixer
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Failed to initialize SDL_Mixer: " << Mix_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    // Tải tệp MP3
    Mix_Music* music = Mix_LoadMUS(mediaFile->getPath().c_str());
    if (!music) {
        std::cerr << "Failed to load music file: " << Mix_GetError() << std::endl;
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    // Phát nhạc
    if (Mix_PlayMusic(music, 1) == -1) { // `1` là số lần lặp lại, `-1` để phát liên tục
        std::cerr << "Failed to play music: " << Mix_GetError() << std::endl;
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    std::cin.get(); // Đợi người dùng nhấn Enter để dừng phát nhạc

    // Giải phóng tài nguyên
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();

}