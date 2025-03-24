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

        //Mix_Music* currentMusic;
        //observers
        std::vector<std::function<void(int)>> observersIndex;
        std::vector<std::function<void()>> observersState;
        std::vector<std::function<void()>> observersVolume;
        std::vector<std::function<void(int)>> observersDuration;
        static void musicFinishedCallback();
        
        std::thread durationThread;
    
    protected:
        std::vector<std::string> mediaFiles;
        size_t currentIndex;
        std::atomic<int> volume{100};
        std::thread playbackThread;

        std::recursive_mutex stateMutex;

    public:
    
        PlayerController(const std::vector<std::string>& files);
        virtual ~PlayerController();

        void playAudio(const char* filePath);
        void playVideo(const char* filePath);

        std::atomic<bool> playing;
        std::atomic<bool> paused;
        bool manualTransition;
        bool repeat;
        std::atomic<int> currentDuration;
        std::atomic<bool> durationRunning;

        virtual void startDuration();
        virtual void stopDuration(); 
        virtual void resetDuration();
        virtual void stopPlaybackThread();
        virtual void playbackWorker(const std::string& file);
        void setNotificationsEnabled(bool enabled);
        void addObserverIndex(std::function<void(int)> index);
        virtual void notifyObserversIndex();

        void addObserverState(std::function<void()> observer);
        void notifyObserversState(); 

        void addObserverVolume(std::function<void()> observer);
        void notifyObserversVolume();  

        void addObserverDuration(std::function<void(int)> observer);
        void notifyObserversDuration();     

        size_t getCurrentIndex();
        std::vector<std::string> getMediaFiles();
        static std::string currentPlayingFile;

        virtual void play();
        virtual bool isPlaying();

        virtual void pause();
        virtual bool isPause();

        virtual void resume();
        void togglePlayback();

        void toggleRepeat();
        bool isRepeat();

        virtual void stop();
        virtual void playNext();
        virtual void playPrevious();
        virtual void setVolume(int newVolume);
        virtual void increaseVolume(int increment);
        virtual void decreaseVolume(int decrement);

        virtual int getVolume() const;

        int getDuration();

        std::thread& getPlaybackThread() { return playbackThread; }
};

#endif
