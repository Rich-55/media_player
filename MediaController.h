#ifndef MEDIA_CONTROLLER_H
#define MEDIA_CONTROLLER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <stdexcept>
#include <queue>


/* added libs for getting duration */
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>
#include <taglib/mpegfile.h>
#include <taglib/mp4file.h>

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

// Audio settings
extern int64_t audio_out_channel_layout;
extern int audio_out_channels;
extern int audio_out_sample_rate;
extern AVSampleFormat audio_out_sample_fmt;

class MediaController {
public:
    MediaController(const std::vector<std::string>& files);
    ~MediaController();

    void play();
    void pause();
    void resume();
    bool repeat;
    void togglePlayback();
    void toggleRepeat();
    void stop();
    void playNext();
    void playPrevious();
    void setVolume(int newVolume);
    void increaseVolume(int increment);
    void decreaseVolume(int decrement);


    /* added for getting duration */

    int getPlayingDuration();
    int calculateDuration(const std::string&);

    // Add an observer to be notified when currentIndex changes
    void addObserver(const std::function<void()>& observer) {
        observers.push_back(observer);
    }

    // Notify all observers
    void notifyObservers() {
        for (const auto& observer : observers) {
            observer();
        }
    }

    // Set the currently playing index
    void setCurrentPlayingIndex(size_t index) {
        if (currentPlayingIndex != index && index < mediaFiles.size()) {
            currentPlayingIndex = index;
            notifyObservers();
        }
    }



private:
    std::vector<std::string> mediaFiles;
    size_t currentIndex;
    std::atomic<bool> playing;
    std::atomic<bool> paused;
    std::thread playbackThread;
    std::recursive_mutex stateMutex;

    void playbackWorker(const std::string&);
    void stopPlaybackThread();
    void playAudio(const char*);
    void playVideo(const char*);
    bool manualTransition;
    // static std::atomic<int> i;
    Mix_Music* currentMusic;

    static void musicFinishedCallback();


    /* added this part */
    size_t currentPlayingIndex  = -1;
    std::unordered_map<std::string, int> durationCache; // Cache for file durations
    std::vector<std::function<void()>> observers; // Observer callbacks

};

#endif
