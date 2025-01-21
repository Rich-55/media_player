#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H
#include "../Model/MediaFileManager.h"
#include "../Model/MediaFile.h"

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <stdexcept>
#include <queue>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/imgutils.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
}

extern std::queue<AVPacket*> audioQueue;
extern std::mutex audioQueueMutex;

extern int64_t audio_out_channel_layout;
extern int audio_out_channels;
extern int audio_out_sample_rate;
extern AVSampleFormat audio_out_sample_fmt;

class PlayerController {
    private:
        std::vector<std::string> mediaFiles;
        size_t currentIndex;
        std::atomic<bool> playing;
        std::atomic<bool> paused;
        std::thread playbackThread;
        std::recursive_mutex stateMutex;
        void playbackWorker(const std::string& file);
        void stopPlaybackThread();
        void playAudio(const char* filePath);
        void playVideo(const char* filePath);
        bool manualTransition;
        bool repeat;
        Mix_Music* currentMusic;
       
        std::vector<std::function<void(int)>> observersIndex;
        std::vector<std::function<void()>> observersState;

        static void musicFinishedCallback();
        
        std::atomic<int> currentDuration;
        std::atomic<bool> durationRunning;
        std::thread durationThread;

        void startDuration();
        void stopDuration();
        void resetDuration();

    public:
        
        PlayerController(const std::vector<std::string>& files);
        ~PlayerController();
        
        void addObserverIndex(std::function<void(int)> index);
        void notifyObserversIndex();

        void addObserverState(std::function<void()> observer);
        void notifyObserversState(); 
        size_t getCurrentIndex();
        std::vector<std::string> getMediaFiles();
        static std::string currentPlayingFile;

        void play();
        bool isPlaying();

        void pause();
        bool isPause();

        void resume();
        void togglePlayback();

        void toggleRepeat();
        bool isRepeat();

        void stop();
        void playNext();
        void playPrevious();
        void setVolume(int newVolume);
        void increaseVolume(int increment);
        void decreaseVolume(int decrement);

        int getVolume();
        
        int getDuration();
};

#endif
