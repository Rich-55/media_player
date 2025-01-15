#include "MediaController.h"
#include <iostream>
#include <thread>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
    #include <libavutil/imgutils.h>
    #include <libswresample/swresample.h>
}

MediaController::MediaController(const std::vector<std::string>& files)
    : mediaFiles(files), currentIndex(0), playing(false), paused(false) {}

MediaController::~MediaController() {
    stopPlaybackThread();
}

void MediaController::play() {
    if (mediaFiles.empty()) {
        std::cerr << "No media files to play.\n";
        return;
    }

    stopPlaybackThread(); // Stop any existing playback thread

    const std::string& file = mediaFiles[currentIndex];
    std::cout << "Playing: " << file << "\n";

    playbackThread = std::thread(&MediaController::playbackWorker, this, file);
}

void MediaController::pause() {
    std::unique_lock<std::recursive_mutex> lock(stateMutex);
    if (!playing || paused) {
        std::cerr << "Cannot pause. No media is playing or already paused.\n";
        return;
    }
    paused = true;
    Mix_PauseMusic();
    std::cout << "Playback paused.\n";
}

void MediaController::resume() {
    std::unique_lock<std::recursive_mutex> lock(stateMutex);
    if (!playing || !paused) {
        std::cerr << "Cannot resume. No media is paused.\n";
        return;
    }
    paused = false;
    Mix_ResumeMusic();
    std::cout << "Playback resumed.\n";
}

void MediaController::stop() {
    stopPlaybackThread();
    std::cout << "Playback stopped.\n";
}

void MediaController::playNext() {
    stop();
    {
        std::unique_lock<std::recursive_mutex> lock(stateMutex);
        currentIndex = (currentIndex + 1) % mediaFiles.size();
    }
    play();
}

void MediaController::playPrevious() {
    stop();
    {
        std::unique_lock<std::recursive_mutex> lock(stateMutex);
        currentIndex = (currentIndex == 0) ? mediaFiles.size() - 1 : currentIndex - 1;
    }
    play();
}

void MediaController::stopPlaybackThread() {
    if (playbackThread.joinable()) {
        {
            std::unique_lock<std::recursive_mutex> lock(stateMutex);
            playing = false;
        }
        playbackThread.join();
    }
}

void MediaController::playbackWorker(const std::string& file) {
    try {
        {
            std::unique_lock<std::recursive_mutex> lock(stateMutex);
            playing = true;
            paused = false;
        }

        if (file.ends_with(".mp3")) {
            playAudio(file.c_str());
        } else if (file.ends_with(".mp4")) {
            playVideo(file.c_str());
        } else {
            std::cerr << "Unsupported file format: " << file << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in playbackWorker: " << e.what() << "\n";
    }

    {
        std::unique_lock<std::recursive_mutex> lock(stateMutex);
        playing = false;
    }
}

void MediaController::playAudio(const char* filePath) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        throw std::runtime_error("Failed to initialize SDL_mixer: " + std::string(Mix_GetError()));
    }

    Mix_Music* music = Mix_LoadMUS(filePath);
    if (!music) {
        Mix_CloseAudio();
        throw std::runtime_error("Failed to load audio file: " + std::string(Mix_GetError()));
    }

    Mix_PlayMusic(music, 1);

    while (Mix_PlayingMusic()) {
        {
            std::unique_lock<std::recursive_mutex> lock(stateMutex);
            if (!playing) {
                Mix_HaltMusic();
                break;
            }
            if (paused) {
                SDL_Delay(100);
                continue;
            }
        }
        SDL_Delay(100);
    }

    Mix_FreeMusic(music);
    Mix_CloseAudio();
}



void MediaController::playVideo(const char* filePath) {
    avformat_network_init();
    AVFormatContext* formatContext = nullptr;

    if (avformat_open_input(&formatContext, filePath, nullptr, nullptr) != 0) {
        std::cerr << "Failed to open video file: " << filePath << "\n";
        return;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        avformat_close_input(&formatContext);
        std::cerr << "Failed to retrieve stream info.\n";
        return;
    }

    int videoStreamIndex = -1;
    for (unsigned i = 0; i < formatContext->nb_streams; ++i) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        avformat_close_input(&formatContext);
        std::cerr << "No video stream found.\n";
        return;
    }

    AVCodecParameters* codecParams = formatContext->streams[videoStreamIndex]->codecpar;
    AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);
    if (!codec) {
        avformat_close_input(&formatContext);
        std::cerr << "Unsupported codec.\n";
        return;
    }

    AVCodecContext* codecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecContext, codecParams);
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        std::cerr << "Failed to open codec.\n";
        return;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        throw std::runtime_error("Failed to initialize SDL: " + std::string(SDL_GetError()));
    }

    SDL_Window* window = SDL_CreateWindow("Video Player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          codecContext->width, codecContext->height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING,
                                             codecContext->width, codecContext->height);

    SwsContext* swsContext = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt,
                                            codecContext->width, codecContext->height, AV_PIX_FMT_YUV420P,
                                            SWS_BILINEAR, nullptr, nullptr, nullptr);

    AVFrame* frame = av_frame_alloc();
    AVFrame* frameYUV = av_frame_alloc();
    int bufferSize = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, codecContext->width, codecContext->height, 1);
    uint8_t* buffer = (uint8_t*)av_malloc(bufferSize);
    av_image_fill_arrays(frameYUV->data, frameYUV->linesize, buffer, AV_PIX_FMT_YUV420P,
                         codecContext->width, codecContext->height, 1);

    AVPacket packet;
    bool quit = false;

    // Calculate frame delay
    double frameDelay = 40.0; // Default for ~25 FPS
    if (formatContext->streams[videoStreamIndex]->avg_frame_rate.num > 0) {
        double fps = av_q2d(formatContext->streams[videoStreamIndex]->avg_frame_rate);
        frameDelay = 1000.0 / fps; // Convert to milliseconds
    }

    while (av_read_frame(formatContext, &packet) >= 0 && !quit) {
        if (packet.stream_index == videoStreamIndex) {
            if (avcodec_send_packet(codecContext, &packet) == 0) {
                while (avcodec_receive_frame(codecContext, frame) == 0) {
                    sws_scale(swsContext, frame->data, frame->linesize, 0, codecContext->height,
                              frameYUV->data, frameYUV->linesize);

                    SDL_UpdateYUVTexture(texture, nullptr,
                                         frameYUV->data[0], frameYUV->linesize[0],
                                         frameYUV->data[1], frameYUV->linesize[1],
                                         frameYUV->data[2], frameYUV->linesize[2]);

                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
                    SDL_RenderPresent(renderer);

                    // Delay to control frame rate
                    SDL_Delay(static_cast<int>(frameDelay));

                    // Handle SDL events and playback state
                    SDL_Event event;
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            quit = true;
                        }
                    }

                    // Check `paused` and `playing` state
                    {
                        std::unique_lock<std::recursive_mutex> lock(stateMutex);
                        while (paused) {
                            lock.unlock(); // Unlock to allow state updates
                            SDL_Delay(100);
                            lock.lock();

                            // Allow `next` or `previous` commands while paused
                            if (!playing) {
                                quit = true;
                                break;
                            }
                        }
                        if (!playing || quit) {
                            quit = true;
                            break;
                        }
                    }
                }
            }
        }
        av_packet_unref(&packet);

        // Allow early exit while reading packets
        {
            std::unique_lock<std::recursive_mutex> lock(stateMutex);
            if (!playing || quit) {
                quit = true;
                break;
            }
        }
    }

    av_free(buffer);
    av_frame_free(&frameYUV);
    av_frame_free(&frame);
    sws_freeContext(swsContext);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
}

// // g++ -o med MediaModel.cpp MediaController.cpp MediaView.cpp main.cpp     -std=c++20 -I/usr/include/SDL2 -L/usr/lib/x86_64-linux-gnu -lavformat -lavcodec -lavutil -lswscale -lSDL2 -lSDL2_mixer -g

// // g++ -o med MediaModel.cpp MediaController.cpp MediaView.cpp main.cpp     -std=c++20 -I/usr/include/x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu     -lavformat -lavcodec -lavutil -lswscale -lavdevice -lavfilter -lSDL2 -lSDL2_mixer -pthread -g