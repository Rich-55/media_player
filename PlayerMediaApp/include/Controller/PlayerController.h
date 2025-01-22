#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H
#include "../Model/MediaFileManager.h"
#include "../Model/MediaFile.h"
#include "../utils/ConfigLoader.h"

// A simple audio packet queue
extern std::queue<AVPacket*> audioQueue;
extern std::mutex audioQueueMutex;

// Audio settings
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

        //Mix_Music* currentMusic;
        //observers
        std::vector<std::function<void(int)>> observersIndex;
        std::vector<std::function<void()>> observersState;
        std::vector<std::function<void()>> observersVolume;
        std::vector<std::function<void(int)>> observersDuration;
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
        void setNotificationsEnabled(bool enabled);
        void addObserverIndex(std::function<void(int)> index);
        void notifyObserversIndex();

        void addObserverState(std::function<void()> observer);
        void notifyObserversState(); 

        void addObserverVolume(std::function<void()> observer);
        void notifyObserversVolume();  

        void addObserverDuration(std::function<void(int)> observer);
        void notifyObserversDuration();     

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
