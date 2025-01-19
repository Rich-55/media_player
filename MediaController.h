#ifndef MEDIA_CONTROLLER_H
#define MEDIA_CONTROLLER_H

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

// A simple audio packet queue
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
    void playOrResume();
    void togglePlayback();
    void toggleRepeat();
    void stop();
    void playNext();
    void playPrevious();
    void setVolume(int newVolume);
    void increaseVolume(int increment);
    void decreaseVolume(int decrement);

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
    Mix_Music* currentMusic;

    static void musicFinishedCallback();
};

#endif
