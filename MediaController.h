#ifndef MEDIACONTROLLER_H
#define MEDIACONTROLLER_H

#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

#include <mutex>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
// #include <libswresample/swresample.h>

class MediaController {
public:
    MediaController(const std::vector<std::string>& files);
    ~MediaController();

    void play();
    void pause();
    void resume();
    void stop();
    void playNext();
    void playPrevious();

private:
    std::vector<std::string> mediaFiles;
    size_t currentIndex;
    std::atomic<bool> playing;
    std::atomic<bool> paused;
    std::thread playbackThread;
    std::recursive_mutex stateMutex; // Allow recursive locking

    void playbackWorker(const std::string& file);
    void stopPlaybackThread();
    void playAudio(const char* filePath);
    void playVideo(const char* filePath);
};


#endif // MEDIACONTROLLER_H
