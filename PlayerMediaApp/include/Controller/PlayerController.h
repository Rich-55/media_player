#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H
#include "../Model/MediaFileManager.h"
#include "../Model/MediaFile.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
class PlayerController {
    private:
        MediaFileManager mediaFileManager;
    public:
        PlayerController(MediaFileManager&);

        void playMusic(std::string);

        // void pauseMusic();
        // void stopMusic();
        // void nextMusic();
        // void previousMusic();
        // void setVolume(int volume);
        // void setMute();
        // void setUnmute();

        // void setRepeat();
};

#endif
