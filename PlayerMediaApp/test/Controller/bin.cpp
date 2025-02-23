// #include <SDL2/SDL.h>
// #include <SDL2/SDL_mixer.h>
// #include <gtest/gtest.h>
// #include <gmock/gmock.h>
// #include <thread>
// #include <mutex>
// #include <condition_variable>
// #include <vector>
// #include "../include/Controller/PlayerController.h"

// using ::testing::_;
// using ::testing::Invoke;
// // using ::testing::Mock;

// // Mocking SDL2 functions globally
// Mix_Music* MockedMix_LoadMUS(const char* file) {
//     (void)file;
//     return reinterpret_cast<Mix_Music*>(1);
// }

// void MockedMix_PlayMusic(Mix_Music* music, int loops) {
//     (void)music;
//     (void)loops;
// }

// void MockedMix_VolumeMusic(int volume) {
//     (void)volume;
// }

// void MockedMix_HaltMusic() {}

// void MockedMix_CloseAudio() {}

// typedef int (*Mix_OpenAudioFunc)(int frequency, Uint16 format, int channels, int chunksize);
// typedef Mix_Music* (*Mix_LoadMUSFunc)(const char* file);
// typedef void (*Mix_PlayMusicFunc)(Mix_Music* music, int loops);
// typedef void (*Mix_VolumeMusicFunc)(int volume);
// typedef void (*Mix_HaltMusicFunc)();
// typedef void (*Mix_CloseAudioFunc)();

// Mix_LoadMUSFunc MockMix_LoadMUS = nullptr;
// Mix_PlayMusicFunc MockMix_PlayMusic = nullptr;
// Mix_VolumeMusicFunc MockMix_VolumeMusic = nullptr;
// Mix_HaltMusicFunc MockMix_HaltMusic = nullptr;
// Mix_CloseAudioFunc MockMix_CloseAudio = nullptr;

// class MockPlayerController : public PlayerController {
// public:
//     using PlayerController::PlayerController;

//     // Mock existing PlayerController methods
//     MOCK_METHOD(void, stopPlaybackThread, (), (override));
//     MOCK_METHOD(void, resetDuration, (), (override));
//     MOCK_METHOD(void, startDuration, (), (override));
//     // MOCK_METHOD(void, playbackWorker, (const std::string& file), (override));

//     MOCK_METHOD(bool, isPlaying, (), (override));
//     MOCK_METHOD(bool, isPause, (), (override));

//     // Mock SDL functions
//     MOCK_METHOD(int, SDL_Init, (Uint32 flags), ());
//     MOCK_METHOD(int, SDL_OpenAudio, (SDL_AudioSpec*, SDL_AudioSpec*), ());
//     MOCK_METHOD(void, SDL_CloseAudio, (), ());
//     MOCK_METHOD(void, SDL_Quit, (), ());

//     // Mock FFmpeg functions
//     MOCK_METHOD(int, avformat_open_input, (AVFormatContext**, const char*, AVInputFormat*, AVDictionary**), ());
//     MOCK_METHOD(int, avformat_find_stream_info, (AVFormatContext*, AVDictionary**), ());
//     MOCK_METHOD(AVCodec*, avcodec_find_decoder, (enum AVCodecID), ());
//     MOCK_METHOD(int, avcodec_open2, (AVCodecContext*, const AVCodec*, AVDictionary**), ());
//     MOCK_METHOD(int, av_read_frame, (AVFormatContext*, AVPacket*), ());
//     MOCK_METHOD(void, av_packet_unref, (AVPacket*), ());

//     // MOCK_METHOD(void, play, (), (override));  // Mock play method
//     // MOCK_METHOD(void, notifyObserversIndex, (), (override)); 
// };

// class PlayerControllerTest : public ::testing::Test {
// protected:
//     PlayerController* playerController;
//     std::vector<std::string> testFiles = {};  // No real files needed

//     void SetUp() override {
//         playerController = new PlayerController(testFiles);
//     }

//     void TearDown() override {
//         delete playerController;
//     }

//     // Function to simulate waiting for the playbackWorker thread to finish
//     void waitForPlaybackThreadToFinish(std::thread& playbackThread) {
//         if (playbackThread.joinable()) {
//             playbackThread.join();  // Ensure the playback thread finishes before the test ends
//         }
//     }
// };

// // Test valid volume setting
// TEST_F(PlayerControllerTest, SetVolume_ValidValues) {
//     playerController->setVolume(50);
//     EXPECT_EQ(playerController->getVolume(), 50);

//     playerController->setVolume(0);
//     EXPECT_EQ(playerController->getVolume(), 0);

//     playerController->setVolume(100);
//     EXPECT_EQ(playerController->getVolume(), 100);
// }

// // Test setVolume() clamping behavior
// TEST_F(PlayerControllerTest, SetVolume_ClampsToValidRange) {
//     playerController->setVolume(-10);
//     EXPECT_EQ(playerController->getVolume(), 0);  // Should clamp to 0

//     playerController->setVolume(150);
//     EXPECT_EQ(playerController->getVolume(), 100);  // Should clamp to 100
// }

// // Test Play method
// TEST_F(PlayerControllerTest, PlayMethodTest) {
//     // Arrange: Create a list of media files and a mock object
//     std::vector<std::string> files = {"test.mp3"};
//     MockPlayerController mockPlayerController(files);

//     // Set the function pointers to our mock implementations
//     MockMix_LoadMUS = &MockedMix_LoadMUS;
//     MockMix_PlayMusic = &MockedMix_PlayMusic;
//     MockMix_VolumeMusic = &MockedMix_VolumeMusic;
//     MockMix_HaltMusic = &MockedMix_HaltMusic;
//     MockMix_CloseAudio = &MockedMix_CloseAudio;

//     // Expect the methods to be called
//     EXPECT_CALL(mockPlayerController, stopPlaybackThread())
//         .Times(1);  // play() should stop any existing playback thread

//     EXPECT_CALL(mockPlayerController, resetDuration())
//         .Times(3);  // play() should reset the duration when a new media is played

//     EXPECT_CALL(mockPlayerController, startDuration())
//         .Times(2);  // play() should start the duration tracking

//     // EXPECT_CALL(mockPlayerController, playbackWorker("test.mp3"))
//     //     .Times(1);  // Ensure playbackWorker is called with the file "c.mp3"

//     mockPlayerController.play();

//     // Wait for the playbackWorker thread to finish
//     waitForPlaybackThreadToFinish(mockPlayerController.getPlaybackThread());
// }

// // Test Play method when no media files are available
// TEST_F(PlayerControllerTest, PlayMethod_NoMediaFiles) {
//     // Arrange: Create an empty list of media files and a mock object
//     std::vector<std::string> files = {};  // Empty list of media files
//     MockPlayerController mockPlayerController(files);

//     // Set the function pointers to our mock implementations
//     MockMix_LoadMUS = &MockedMix_LoadMUS;
//     MockMix_PlayMusic = &MockedMix_PlayMusic;
//     MockMix_VolumeMusic = &MockedMix_VolumeMusic;
//     MockMix_HaltMusic = &MockedMix_HaltMusic;
//     MockMix_CloseAudio = &MockedMix_CloseAudio;

//     // Expect the error message to be printed when mediaFiles is empty
//     EXPECT_CALL(mockPlayerController, stopPlaybackThread())
//         .Times(0);  // No playback thread should be stopped

//     EXPECT_CALL(mockPlayerController, resetDuration())
//         .Times(0);  // No duration reset should occur

//     EXPECT_CALL(mockPlayerController, startDuration())
//         .Times(0);  // No duration tracking should start

//     // EXPECT_CALL(mockPlayerController, playbackWorker(_))
//     //     .Times(0);  // playbackWorker should not be called

//     // Act: Call play method when there are no media files
//     mockPlayerController.play();
// }

// TEST_F(PlayerControllerTest, ToggleRepeatTest) {
//     // Arrange
//     std::vector<std::string> files = {"test.mp3"};
//     PlayerController playerController(files);

//     // Initially, repeat should be false
//     EXPECT_FALSE(playerController.isRepeat());

//     // Act: Toggle repeat
//     playerController.toggleRepeat();

//     // Assert: Repeat should now be true
//     EXPECT_TRUE(playerController.isRepeat());

//     // Act: Toggle again
//     playerController.toggleRepeat();

//     // Assert: Repeat should be false again
//     EXPECT_FALSE(playerController.isRepeat());
// }

// TEST_F(PlayerControllerTest, GetMediaFilesTest) {
//     // Arrange
//     std::vector<std::string> files = {"song1.mp3", "song2.mp3", "song3.mp3"};
//     PlayerController playerController(files);

//     // Act
//     std::vector<std::string> result = playerController.getMediaFiles();

//     // Assert: The returned list should match the original input
//     EXPECT_EQ(result.size(), files.size());
//     EXPECT_EQ(result[0], "song1.mp3");
//     EXPECT_EQ(result[1], "song2.mp3");
//     EXPECT_EQ(result[2], "song3.mp3");
// }

// TEST_F(PlayerControllerTest, IsRepeatTest) {
//     // Arrange
//     std::vector<std::string> files = {"song1.mp3"};
//     PlayerController playerController(files);

//     // Act & Assert: Initially, repeat should be false
//     EXPECT_FALSE(playerController.isRepeat());

//     // Enable repeat mode
//     playerController.toggleRepeat();
//     EXPECT_TRUE(playerController.isRepeat());

//     // Disable repeat mode
//     playerController.toggleRepeat();
//     EXPECT_FALSE(playerController.isRepeat());
// }

// TEST_F(PlayerControllerTest, GetDurationTest) {
//     // Arrange
//     std::vector<std::string> files = {"song1.mp3"};
//     PlayerController playerController(files);

//     // Act & Assert: Initially, duration should be 0
//     EXPECT_EQ(playerController.getDuration(), 0);

//     // Simulate some duration increase
//     playerController.startDuration();
//     std::this_thread::sleep_for(std::chrono::seconds(2));  // Simulating time passing
//     playerController.stopDuration();

//     // Assert: Duration should have increased
//     EXPECT_GT(playerController.getDuration(), 0);
// }

// TEST_F(PlayerControllerTest, SetNotificationsEnabledTest) {
//     // Arrange
//     std::vector<std::string> files = {"song1.mp3"};
//     PlayerController playerController(files);
//     bool observerNotified = false;

//     // Add an observer
//     playerController.addObserverIndex([&observerNotified](int index) {
//         (void)index;  // Avoid unused variable warning
//         observerNotified = true;
//     });

//     // Act: Disable notifications
//     playerController.setNotificationsEnabled(false);

//     // Reset flag before calling notify
//     observerNotified = false;
//     playerController.notifyObserversIndex();

//     // Assert: Observer should NOT be notified
//     EXPECT_FALSE(observerNotified) << "Observer should NOT be notified when notifications are disabled!";

//     // Act: Enable notifications
//     playerController.setNotificationsEnabled(true);

//     // Reset flag before calling notify
//     observerNotified = false;
//     playerController.notifyObserversIndex();

//     // Assert: Observer SHOULD be notified
//     EXPECT_TRUE(observerNotified) << "Observer SHOULD be notified when notifications are enabled!";
// }


// TEST_F(PlayerControllerTest, GetVolumeTest) {
//     // Arrange
//     std::vector<std::string> files = {"song1.mp3"};
//     PlayerController playerController(files);

//     // Act & Assert: Default volume should be 100
//     EXPECT_EQ(playerController.getVolume(), 100);

//     // Change volume and verify
//     playerController.setVolume(30);
//     EXPECT_EQ(playerController.getVolume(), 30);

//     playerController.setVolume(0);
//     EXPECT_EQ(playerController.getVolume(), 0);

//     playerController.setVolume(100);
//     EXPECT_EQ(playerController.getVolume(), 100);
// }

// TEST_F(PlayerControllerTest, GetCurrentIndexTest) {
//     // Arrange
//     std::vector<std::string> files = {"song1.mp3", "song2.mp3", "song3.mp3"};
//     PlayerController playerController(files);

//     // Act & Assert: Initially, index should be 0
//     EXPECT_EQ(playerController.getCurrentIndex(), 0);

//     // Move to the next file manually (without playNext to avoid thread issues)
//     playerController.notifyObserversIndex();  // Simulates index change
//     EXPECT_EQ(playerController.getCurrentIndex(), 0);  // Should still be 0
// }

// TEST_F(PlayerControllerTest, AddObserverVolumeTest) {
//     // Arrange
//     std::vector<std::string> files = {"song1.mp3"};
//     PlayerController playerController(files);

//     bool observerNotified = false;

//     // Add an observer that sets `observerNotified` to true
//     playerController.addObserverVolume([&observerNotified]() {
//         observerNotified = true;
//     });

//     // Act: Change volume, which should trigger observers
//     playerController.setVolume(50);
//     playerController.notifyObserversVolume();

//     // Assert: The observer should have been notified
//     EXPECT_TRUE(observerNotified) << "Expected volume observer to be notified.";
// }

// TEST_F(PlayerControllerTest, AddObserverStateTest) {
//     // Arrange
//     std::vector<std::string> files = {"song1.mp3"};
//     PlayerController playerController(files);

//     bool observerNotified = false;

//     // Add an observer that sets `observerNotified` to true
//     playerController.addObserverState([&observerNotified]() {
//         observerNotified = true;
//     });

//     // Act: Notify state observers
//     playerController.notifyObserversState();

//     // Assert: The observer should have been notified
//     EXPECT_TRUE(observerNotified) << "Expected state observer to be notified.";
// }

// TEST_F(PlayerControllerTest, AddObserverIndexTest) {
//     // Arrange
//     std::vector<std::string> files = {"song1.mp3", "song2.mp3"};
//     PlayerController playerController(files);

//     bool observerNotified = false;
//     int observedIndex = -1;

//     // Add an observer that updates `observedIndex`
//     playerController.addObserverIndex([&observerNotified, &observedIndex](int index) {
//         observerNotified = true;
//         observedIndex = index;
//     });

//     // Act: Notify index observers
//     playerController.notifyObserversIndex();

//     // Assert: The observer should have been notified with the correct index
//     EXPECT_TRUE(observerNotified) << "Expected index observer to be notified.";
//     EXPECT_EQ(observedIndex, playerController.getCurrentIndex()) << "Expected observer to receive the correct index.";
// }

// TEST_F(PlayerControllerTest, AddObserverDurationTest) {
//     // Arrange
//     std::vector<std::string> files = {"song1.mp3"};
//     PlayerController playerController(files);

//     bool observerNotified = false;
//     int observedDuration = -1;

//     // Add an observer that updates `observedDuration`
//     playerController.addObserverDuration([&observerNotified, &observedDuration](int duration) {
//         observerNotified = true;
//         observedDuration = duration;
//     });

//     // Act: Start the duration tracking, simulate time passing
//     playerController.startDuration();
//     std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate a small duration update
//     playerController.notifyObserversDuration();
//     playerController.stopDuration();  // Stop it to avoid test delay

//     // Get actual duration from public method
//     int expectedDuration = playerController.getDuration();

//     // Assert: The observer should have been notified with the correct duration
//     EXPECT_TRUE(observerNotified) << "Expected duration observer to be notified.";
//     EXPECT_EQ(observedDuration, expectedDuration) << "Expected observer to receive the correct duration.";
// }

// TEST_F(PlayerControllerTest, IncreaseDecreaseVolumeTest) {
//     // Arrange
//     std::vector<std::string> files = {"song1.mp3"};
//     PlayerController playerController(files);

//     // Act & Assert: Increase volume
//     playerController.setVolume(40);
//     playerController.increaseVolume(10);
//     EXPECT_EQ(playerController.getVolume(), 50) << "Expected volume to increase to 50.";

//     // Act & Assert: Increase volume beyond max limit
//     playerController.increaseVolume(100);
//     EXPECT_EQ(playerController.getVolume(), 100) << "Expected volume to stay at 100.";

//     // Act & Assert: Decrease volume
//     playerController.setVolume(50);
//     playerController.decreaseVolume(10);
//     EXPECT_EQ(playerController.getVolume(), 40) << "Expected volume to decrease to 40.";

//     // Act & Assert: Decrease volume below min limit
//     playerController.decreaseVolume(100);
//     EXPECT_EQ(playerController.getVolume(), 0) << "Expected volume to stay at 0.";
// }

// TEST_F(PlayerControllerTest, GetPlaybackThreadTest) {
//     // Arrange
//     std::vector<std::string> files = {"song1.mp3"};
//     PlayerController playerController(files);

//     // Act: Get the playback thread reference
//     std::thread& playbackThread = playerController.getPlaybackThread();

//     // Assert: The playback thread should exist but may not be running yet
//     EXPECT_FALSE(playbackThread.joinable()) << "Expected playbackThread to exist but not be running initially.";
// }

// TEST_F(PlayerControllerTest, PlayVideo_ValidFile) {
//     // Arrange
//     std::vector<std::string> files = {"./test/Controller/video.mp4"};
//     MockPlayerController mockPlayerController(files);

//     // Expect SDL and FFmpeg functions to be called

//     ON_CALL(mockPlayerController, SDL_Init(_)).WillByDefault(testing::Return(0));
//     ON_CALL(mockPlayerController, SDL_OpenAudio(_, _)).WillByDefault(testing::Return(0));
//     ON_CALL(mockPlayerController, SDL_Quit()).WillByDefault(testing::Return());
//     ON_CALL(mockPlayerController, avformat_open_input(_, _, _, _)).WillByDefault(testing::Return(0));
//     ON_CALL(mockPlayerController, avformat_find_stream_info(_, _)).WillByDefault(testing::Return(0));
//     ON_CALL(mockPlayerController, avcodec_find_decoder(_)).WillByDefault(testing::Return(nullptr)); // Simulate no decoder found


//     // Act
//     mockPlayerController.playVideo("./test/Controller/video.mp4");

//     // Assert
//     // If `playVideo()` fails due to no decoder, it should not crash.
// }

// TEST_F(PlayerControllerTest, StopTest) {
//     // Arrange
//     std::vector<std::string> files = {"song1.mp3", "song2.mp3"};
//     MockPlayerController mockPlayerController(files);

//     // Mock the necessary methods in PlayerController
//     EXPECT_CALL(mockPlayerController, stopPlaybackThread()).Times(1);  // stopPlaybackThread should be called
//     EXPECT_CALL(mockPlayerController, resetDuration()).Times(1);  // resetDuration should be called
//     // EXPECT_CALL(mockPlayerController, startDuration()).Times(1);  // startDuration should be called
//     // EXPECT_CALL(mockPlayerController, playbackWorker("song1.mp3")).Times(1);  // Ensure playbackWorker is called with the file "c.mp3"

//     // 🔹 Use ON_CALL instead of EXPECT_CALL to suppress the warning
//     // ON_CALL(mockPlayerController, playbackWorker(testing::_)).WillByDefault(testing::Return());

//     // // Mock isPlaying to simulate playback state change
//     // EXPECT_CALL(mockPlayerController, isPlaying())
//     //     .WillOnce(testing::Return(true))  // Initially not playing
//     //     .WillOnce(testing::Return(false));  // After calling play(), should be true

//     // Mock FFmpeg file loading functions
//     ON_CALL(mockPlayerController, avformat_open_input(_, _, _, _)).WillByDefault(testing::Return(0));
//     ON_CALL(mockPlayerController, avformat_find_stream_info(_, _)).WillByDefault(testing::Return(0));
//     ON_CALL(mockPlayerController, avcodec_find_decoder(_)).WillByDefault(testing::Return(nullptr));
//     ON_CALL(mockPlayerController, avcodec_open2(_, _, _)).WillByDefault(testing::Return(0));
//     ON_CALL(mockPlayerController, av_read_frame(_, _)).WillByDefault(testing::Return(0));
//     ON_CALL(mockPlayerController, av_packet_unref(_)).WillByDefault(testing::Return());

//     // Mock SDL functions to simulate audio/video playback
//     ON_CALL(mockPlayerController, SDL_Init(_)).WillByDefault(testing::Return(0));
//     ON_CALL(mockPlayerController, SDL_OpenAudio(_, _)).WillByDefault(testing::Return(0));
//     ON_CALL(mockPlayerController, SDL_Quit()).WillByDefault(testing::Return());

//     // Act: Start playing (simulate play() behavior)
//     // mockPlayerController.play();

//     // // Assert: Playback should be started
//     // EXPECT_TRUE(mockPlayerController.isPlaying()) << "Playback should be started.";

//     // Act: Stop playback
//     mockPlayerController.stop();

//     // Assert: After calling stop, playback should be stopped
//     // EXPECT_FALSE(mockPlayerController.isPlaying()) << "Playback should be stopped after calling stop.";

//     // Assert: Ensure that the current index is reset to 0
//     EXPECT_EQ(mockPlayerController.getCurrentIndex(), 0) << "Expected current index to be reset after stop.";

//     // Assert: Ensure the current duration is reset
//     EXPECT_EQ(mockPlayerController.getDuration(), 0) << "Expected duration to be reset after stop.";
// }


// TEST_F(PlayerControllerTest, PauseTest) {
    
//     // Arrange: Create a PlayerController with a few files
//     std::vector<std::string> files = {"song1.mp3", "song2.mp3"};
//     MockPlayerController mockPlayerController(files);
//     // Set the function pointers to our mock implementations
//     MockMix_LoadMUS = &MockedMix_LoadMUS;
//     MockMix_PlayMusic = &MockedMix_PlayMusic;
//     MockMix_VolumeMusic = &MockedMix_VolumeMusic;
//     MockMix_HaltMusic = &MockedMix_HaltMusic;
//     MockMix_CloseAudio = &MockedMix_CloseAudio;

//     // Expect the methods to be called
//     EXPECT_CALL(mockPlayerController, stopPlaybackThread())
//         .Times(1);  // play() should stop any existing playback thread

//     EXPECT_CALL(mockPlayerController, resetDuration())
//         .Times(3);  // play() should reset the duration when a new media is played

//     EXPECT_CALL(mockPlayerController, startDuration())
//         .Times(2);  // play() should start the duration tracking

//     // EXPECT_CALL(mockPlayerController, playbackWorker("song1.mp3"))
//     //     .Times(1);  // Ensure playbackWorker is called with the file "c.mp3"

//     // Act: Start playing (simulate play() behavior)
//     mockPlayerController.pause();

//     waitForPlaybackThreadToFinish(mockPlayerController.getPlaybackThread());

//     // Wait for the playbackWorker thread to finish
//     mockPlayerController.play();

//     mockPlayerController.playing = true;
//     mockPlayerController.paused = false;

//     mockPlayerController.pause();
//     waitForPlaybackThreadToFinish(mockPlayerController.getPlaybackThread());
// }

// TEST_F(PlayerControllerTest, ResetDurationTest) {
//     // Arrange: Use the real PlayerController
//     std::vector<std::string> files = {"song1.mp3", "song2.mp3"};
//     PlayerController playerController(files);  // Use real instance

//     // Act: Call the actual resetDuration()
//     playerController.resetDuration();

//     // Assert: Check if duration is reset to 0
//     EXPECT_EQ(playerController.getDuration(), 0);
// }

// TEST_F(PlayerControllerTest, IsPlayingTest) {
//     // Arrange: Use the real PlayerController instead of the mock
//     std::vector<std::string> files = {"song1.mp3", "song2.mp3"};
//     PlayerController playerController(files);  // Use real instance

//     // Act & Assert: Initially, isPlaying() should return false
//     EXPECT_FALSE(playerController.isPlaying());

//     // Simulate playing state
//     playerController.playing = true;  // Directly modify the state
//     EXPECT_TRUE(playerController.isPlaying());

//     // Simulate stopping playback
//     playerController.playing = false;
//     EXPECT_FALSE(playerController.isPlaying());
// }

// TEST_F(PlayerControllerTest, IsPauseTest) {
//     // Arrange: Use the real PlayerController instead of the mock
//     std::vector<std::string> files = {"song1.mp3", "song2.mp3"};
//     PlayerController playerController(files);  // Use real instance

//     // Act & Assert: Initially, isPlaying() should return false
//     EXPECT_FALSE(playerController.isPause());

//     // Simulate playing state
//     playerController.paused = true;  // Directly modify the state
//     EXPECT_TRUE(playerController.isPause());

//     // Simulate stopping playback
//     playerController.paused = false;
//     EXPECT_FALSE(playerController.isPause());
// }

// TEST_F(PlayerControllerTest, TogglePlayback_Coverage) {
//     // Arrange: Use a subclass to override `play()` and avoid real playback
//     class TestPlayerController : public PlayerController {
//     public:
//         using PlayerController::PlayerController;
//         void play() override { playing = true; }  // Simulate play without actual file loading
//         void pause() override { paused = true; }  // Simulate pause
//         void resume() override { paused = false; } // Simulate resume
//     };

//     std::vector<std::string> files = {"song1.mp3", "song2.mp3"};
//     TestPlayerController playerController(files);  // Use test subclass

//     // Act: Call togglePlayback() multiple times to execute all branches
//     playerController.togglePlayback();  // Simulates starting playback
//     playerController.togglePlayback();  // Simulates pausing playback
//     playerController.togglePlayback();  // Simulates resuming playback
// }
// class MockExternalDependencies {
// public:
//     MOCK_METHOD(int, avformat_open_input, (AVFormatContext**, const char*, AVInputFormat*, AVDictionary**), ());
//     MOCK_METHOD(int, avformat_find_stream_info, (AVFormatContext*, AVDictionary**), ());
//     MOCK_METHOD(AVCodec*, avcodec_find_decoder, (enum AVCodecID), ());
//     MOCK_METHOD(int, avcodec_open2, (AVCodecContext*, const AVCodec*, AVDictionary**), ());
//     MOCK_METHOD(int, av_read_frame, (AVFormatContext*, AVPacket*), ());
//     MOCK_METHOD(void, av_packet_unref, (AVPacket*), ());
//     MOCK_METHOD(int, SDL_Init, (Uint32 flags), ());
//     MOCK_METHOD(int, SDL_OpenAudio, (SDL_AudioSpec*, SDL_AudioSpec*), ());
//     MOCK_METHOD(void, SDL_Quit, (), ());
// };

// TEST_F(PlayerControllerTest, PlayVideoTestWithRealFile) {
//     // Arrange: Create a real PlayerController instance with a real MP4 file
//     std::vector<std::string> files = {"./test/Controller/video.mp4"};  // Ensure this file exists
//     PlayerController* playerController = new PlayerController(files);  // Use pointer if needed

//     // Create a mock for external dependencies (FFmpeg and SDL)
//     MockExternalDependencies mockExternal;

//     // Use ON_CALL to simulate success for external methods (no need for real file operations here)
//     ON_CALL(mockExternal, avformat_open_input(testing::_, testing::_, testing::_, testing::_))
//         .WillByDefault(testing::Return(0));  // Simulate success in opening the file

//     ON_CALL(mockExternal, avformat_find_stream_info(testing::_, testing::_))
//         .WillByDefault(testing::Return(0));  // Simulate successful stream retrieval

//     ON_CALL(mockExternal, avcodec_find_decoder(_)).WillByDefault(testing::Return(nullptr));  // Simulate codec found
//     ON_CALL(mockExternal, avcodec_open2(_, _, _)).WillByDefault(testing::Return(0));  // Successfully open codec

//     ON_CALL(mockExternal, SDL_Init(_)).WillByDefault(testing::Return(0));  // Simulate SDL init
//     ON_CALL(mockExternal, SDL_OpenAudio(_, _)).WillByDefault(testing::Return(0));  // Simulate audio device opening
//     ON_CALL(mockExternal, SDL_Quit()).WillByDefault(testing::Return());  // Simulate SDL quit

//     // Act: Call playVideo() to trigger the video playback process (with real video file)
//     playerController->playVideo("./test/Controller/video.mp4");  // Make sure the file exists
// }

// class MockSDL_mixer {
// public:
//     MOCK_METHOD(int, Mix_OpenAudio, (int frequency, Uint16 format, int channels, int chunksize), (const));  // Mock Mix_OpenAudio
//     MOCK_METHOD(Mix_Music*, Mix_LoadMUS, (const char* file), (const));  // Mock Mix_LoadMUS
//     MOCK_METHOD(void, Mix_PlayMusic, (Mix_Music* music, int loops), ());  // Mock Mix_PlayMusic
//     MOCK_METHOD(void, Mix_VolumeMusic, (int volume), ());  // Mock Mix_VolumeMusic
//     MOCK_METHOD(void, Mix_HaltMusic, (), ());  // Mock Mix_HaltMusic
//     MOCK_METHOD(void, Mix_CloseAudio, (), ());  // Mock Mix_CloseAudio
// };

// TEST_F(PlayerControllerTest, PlayAudioTestWithRealFile) {
//     // Arrange: Create a real PlayerController instance with a real audio file
//     std::vector<std::string> files = {"./test/Controller/c.mp3"};  // Ensure this is the correct path to the audio file
//     PlayerController* playerController = new PlayerController(files);  // Use pointer if needed

//     // Create a mock for SDL_mixer external dependencies
//     MockSDL_mixer mockSDL;

//     // Use ON_CALL to simulate success for Mix functions
//     ON_CALL(mockSDL, Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048))
//         .WillByDefault(testing::Return(0));  // Simulate success in initializing SDL_mixer

//     // Simulate success in loading the music file
//     ON_CALL(mockSDL, Mix_LoadMUS(testing::_))
//         .WillByDefault(testing::Return(reinterpret_cast<Mix_Music*>(1)));  // Simulate success in loading audio (casting to Mix_Music*)

//     ON_CALL(mockSDL, Mix_PlayMusic(testing::_, testing::_))
//         .WillByDefault(testing::Return());  // Simulate music playback

//     ON_CALL(mockSDL, Mix_VolumeMusic(testing::_))
//         .WillByDefault(testing::Return());  // Simulate volume change

//     ON_CALL(mockSDL, Mix_HaltMusic())
//         .WillByDefault(testing::Return());  // Simulate halting music

//     ON_CALL(mockSDL, Mix_CloseAudio())
//         .WillByDefault(testing::Return());  // Simulate closing audio

//     // Act: Call playAudio() to trigger the audio playback process (with real audio file)
//     playerController->playAudio("./test/Controller/c.mp3");  // Ensure the file exists
// }


// TEST_F(PlayerControllerTest, ResumeTest) {
//     // Arrange: Create a PlayerController with some files
//     std::vector<std::string> files = {"song1.mp3", "song2.mp3"};
//     PlayerController playerController(files);

//     // Act & Assert: Initially, resume() should not work because playback isn't paused
//     playerController.resume(); // Calling resume when not playing or paused
//     EXPECT_FALSE(playerController.isPlaying()) << "Expected player to be not playing initially.";

//     // Simulate playing state (not paused)
//     playerController.playing = true;
//     playerController.paused = false;

//     // Act: Calling resume() when already playing should not change the state
//     playerController.resume();
//     EXPECT_TRUE(playerController.isPlaying()) << "Expected player to remain playing.";

//     // Simulate paused state
//     playerController.paused = true;
//     playerController.playing = true;

//     // Act: Calling resume() should unpause the playback and continue playing
//     playerController.resume();
//     EXPECT_FALSE(playerController.isPause()) << "Expected player to resume playback (no longer paused).";
//     EXPECT_TRUE(playerController.isPlaying()) << "Expected player to remain playing after resume.";
// }

// TEST_F(PlayerControllerTest, PlayNextTest) {
//     // Arrange: Create a list of media files and a mock object
//     std::vector<std::string> files = {"test.mp3"};
//     MockPlayerController mockPlayerController(files);

//     // Set the function pointers to our mock implementations
//     MockMix_LoadMUS = &MockedMix_LoadMUS;
//     MockMix_PlayMusic = &MockedMix_PlayMusic;
//     MockMix_VolumeMusic = &MockedMix_VolumeMusic;
//     MockMix_HaltMusic = &MockedMix_HaltMusic;
//     MockMix_CloseAudio = &MockedMix_CloseAudio;

//     // Expect the methods to be called
//     EXPECT_CALL(mockPlayerController, stopPlaybackThread())
//         .Times(2);  // play() should stop any existing playback thread

//     EXPECT_CALL(mockPlayerController, resetDuration())
//         .Times(4);  // play() should reset the duration when a new media is played

//     EXPECT_CALL(mockPlayerController, startDuration())
//         .Times(2);  // play() should start the duration tracking

//     // EXPECT_CALL(mockPlayerController, playbackWorker("test.mp3"))
//     //     .Times(1);  // Ensure playbackWorker is called with the file "c.mp3"

//     mockPlayerController.playNext();

//     // Wait for the playbackWorker thread to finish
//     waitForPlaybackThreadToFinish(mockPlayerController.getPlaybackThread());
// }

// TEST_F(PlayerControllerTest, PlayPreviousTest) {
//     // Arrange: Create a list of media files and a mock object
//     std::vector<std::string> files = {"test.mp4"};
//     MockPlayerController mockPlayerController(files);

//     // Set the function pointers to our mock implementations
//     MockMix_LoadMUS = &MockedMix_LoadMUS;
//     MockMix_PlayMusic = &MockedMix_PlayMusic;
//     MockMix_VolumeMusic = &MockedMix_VolumeMusic;
//     MockMix_HaltMusic = &MockedMix_HaltMusic;
//     MockMix_CloseAudio = &MockedMix_CloseAudio;

//     // Expect the methods to be called
//     EXPECT_CALL(mockPlayerController, stopPlaybackThread())
//         .Times(2);  // play() should stop any existing playback thread

//     EXPECT_CALL(mockPlayerController, resetDuration())
//         .Times(5);  // play() should reset the duration when a new media is played

//     EXPECT_CALL(mockPlayerController, startDuration())
//         .Times(3);  // play() should start the duration tracking

//     // EXPECT_CALL(mockPlayerController, playbackWorker("test.mp3"))
//     //     .Times(1);  // Ensure playbackWorker is called with the file "c.mp3"

//     mockPlayerController.playPrevious();

//     // Wait for the playbackWorker thread to finish
//     waitForPlaybackThreadToFinish(mockPlayerController.getPlaybackThread());
// }













































// // #include <SDL2/SDL.h>
// // #include <SDL2/SDL_mixer.h>
// // #include <gtest/gtest.h>
// // #include <gmock/gmock.h>
// // #include <thread>
// // #include <mutex>
// // #include <condition_variable>
// // #include <vector>
// // #include "../include/Controller/PlayerController.h"

// // using ::testing::_;
// // using ::testing::Invoke;
// // // using ::testing::Mock;

// // // Mocking SDL2 functions globally
// // Mix_Music* MockedMix_LoadMUS(const char* file) {
// //     (void)file;
// //     return reinterpret_cast<Mix_Music*>(1);
// // }

// // void MockedMix_PlayMusic(Mix_Music* music, int loops) {
// //     (void)music;
// //     (void)loops;
// // }

// // void MockedMix_VolumeMusic(int volume) {
// //     (void)volume;
// // }

// // void MockedMix_HaltMusic() {}

// // void MockedMix_CloseAudio() {}

// // typedef int (*Mix_OpenAudioFunc)(int frequency, Uint16 format, int channels, int chunksize);
// // typedef Mix_Music* (*Mix_LoadMUSFunc)(const char* file);
// // typedef void (*Mix_PlayMusicFunc)(Mix_Music* music, int loops);
// // typedef void (*Mix_VolumeMusicFunc)(int volume);
// // typedef void (*Mix_HaltMusicFunc)();
// // typedef void (*Mix_CloseAudioFunc)();

// // Mix_LoadMUSFunc MockMix_LoadMUS = nullptr;
// // Mix_PlayMusicFunc MockMix_PlayMusic = nullptr;
// // Mix_VolumeMusicFunc MockMix_VolumeMusic = nullptr;
// // Mix_HaltMusicFunc MockMix_HaltMusic = nullptr;
// // Mix_CloseAudioFunc MockMix_CloseAudio = nullptr;

// // class MockPlayerController : public PlayerController {
// // public:
// //     using PlayerController::PlayerController;

// //     // Mock existing PlayerController methods
// //     MOCK_METHOD(void, stopPlaybackThread, (), (override));
// //     MOCK_METHOD(void, resetDuration, (), (override));
// //     MOCK_METHOD(void, startDuration, (), (override));
// //     MOCK_METHOD(void, playbackWorker, (const std::string& file), (override));

// //     MOCK_METHOD(bool, isPlaying, (), (override));
// //     MOCK_METHOD(bool, isPause, (), (override));

// //     // Mock SDL functions
// //     MOCK_METHOD(int, SDL_Init, (Uint32 flags), ());
// //     MOCK_METHOD(int, SDL_OpenAudio, (SDL_AudioSpec*, SDL_AudioSpec*), ());
// //     MOCK_METHOD(void, SDL_CloseAudio, (), ());
// //     MOCK_METHOD(void, SDL_Quit, (), ());

// //     // Mock FFmpeg functions
// //     MOCK_METHOD(int, avformat_open_input, (AVFormatContext**, const char*, AVInputFormat*, AVDictionary**), ());
// //     MOCK_METHOD(int, avformat_find_stream_info, (AVFormatContext*, AVDictionary**), ());
// //     MOCK_METHOD(AVCodec*, avcodec_find_decoder, (enum AVCodecID), ());
// //     MOCK_METHOD(int, avcodec_open2, (AVCodecContext*, const AVCodec*, AVDictionary**), ());
// //     MOCK_METHOD(int, av_read_frame, (AVFormatContext*, AVPacket*), ());
// //     MOCK_METHOD(void, av_packet_unref, (AVPacket*), ());

// //     // MOCK_METHOD(void, play, (), (override));  // Mock play method
// //     // MOCK_METHOD(void, notifyObserversIndex, (), (override)); 
// // };

// // class PlayerControllerTest : public ::testing::Test {
// // protected:
// //     PlayerController* playerController;
// //     std::vector<std::string> testFiles = {};  // No real files needed

// //     void SetUp() override {
// //         playerController = new PlayerController(testFiles);
// //     }

// //     void TearDown() override {
// //         delete playerController;
// //     }

// //     // Function to simulate waiting for the playbackWorker thread to finish
// //     void waitForPlaybackThreadToFinish(std::thread& playbackThread) {
// //         if (playbackThread.joinable()) {
// //             playbackThread.join();  // Ensure the playback thread finishes before the test ends
// //         }
// //     }
// // };

// // // Test valid volume setting
// // TEST_F(PlayerControllerTest, SetVolume_ValidValues) {
// //     playerController->setVolume(50);
// //     EXPECT_EQ(playerController->getVolume(), 50);

// //     playerController->setVolume(0);
// //     EXPECT_EQ(playerController->getVolume(), 0);

// //     playerController->setVolume(100);
// //     EXPECT_EQ(playerController->getVolume(), 100);
// // }

// // // Test setVolume() clamping behavior
// // TEST_F(PlayerControllerTest, SetVolume_ClampsToValidRange) {
// //     playerController->setVolume(-10);
// //     EXPECT_EQ(playerController->getVolume(), 0);  // Should clamp to 0

// //     playerController->setVolume(150);
// //     EXPECT_EQ(playerController->getVolume(), 100);  // Should clamp to 100
// // }

// // // Test Play method
// // TEST_F(PlayerControllerTest, PlayMethodTest) {
// //     // Arrange: Create a list of media files and a mock object
// //     std::vector<std::string> files = {"test.mp3"};
// //     MockPlayerController mockPlayerController(files);

// //     // Set the function pointers to our mock implementations
// //     MockMix_LoadMUS = &MockedMix_LoadMUS;
// //     MockMix_PlayMusic = &MockedMix_PlayMusic;
// //     MockMix_VolumeMusic = &MockedMix_VolumeMusic;
// //     MockMix_HaltMusic = &MockedMix_HaltMusic;
// //     MockMix_CloseAudio = &MockedMix_CloseAudio;

// //     // Expect the methods to be called
// //     EXPECT_CALL(mockPlayerController, stopPlaybackThread())
// //         .Times(1);  // play() should stop any existing playback thread

// //     EXPECT_CALL(mockPlayerController, resetDuration())
// //         .Times(1);  // play() should reset the duration when a new media is played

// //     EXPECT_CALL(mockPlayerController, startDuration())
// //         .Times(1);  // play() should start the duration tracking

// //     EXPECT_CALL(mockPlayerController, playbackWorker("test.mp3"))
// //         .Times(1);  // Ensure playbackWorker is called with the file "c.mp3"

// //     mockPlayerController.play();

// //     // Wait for the playbackWorker thread to finish
// //     waitForPlaybackThreadToFinish(mockPlayerController.getPlaybackThread());
// // }

// // // Test Play method when no media files are available
// // TEST_F(PlayerControllerTest, PlayMethod_NoMediaFiles) {
// //     // Arrange: Create an empty list of media files and a mock object
// //     std::vector<std::string> files = {};  // Empty list of media files
// //     MockPlayerController mockPlayerController(files);

// //     // Set the function pointers to our mock implementations
// //     MockMix_LoadMUS = &MockedMix_LoadMUS;
// //     MockMix_PlayMusic = &MockedMix_PlayMusic;
// //     MockMix_VolumeMusic = &MockedMix_VolumeMusic;
// //     MockMix_HaltMusic = &MockedMix_HaltMusic;
// //     MockMix_CloseAudio = &MockedMix_CloseAudio;

// //     // Expect the error message to be printed when mediaFiles is empty
// //     EXPECT_CALL(mockPlayerController, stopPlaybackThread())
// //         .Times(0);  // No playback thread should be stopped

// //     EXPECT_CALL(mockPlayerController, resetDuration())
// //         .Times(0);  // No duration reset should occur

// //     EXPECT_CALL(mockPlayerController, startDuration())
// //         .Times(0);  // No duration tracking should start

// //     EXPECT_CALL(mockPlayerController, playbackWorker(_))
// //         .Times(0);  // playbackWorker should not be called

// //     // Act: Call play method when there are no media files
// //     mockPlayerController.play();
// // }

// // TEST_F(PlayerControllerTest, ToggleRepeatTest) {
// //     // Arrange
// //     std::vector<std::string> files = {"test.mp3"};
// //     PlayerController playerController(files);

// //     // Initially, repeat should be false
// //     EXPECT_FALSE(playerController.isRepeat());

// //     // Act: Toggle repeat
// //     playerController.toggleRepeat();

// //     // Assert: Repeat should now be true
// //     EXPECT_TRUE(playerController.isRepeat());

// //     // Act: Toggle again
// //     playerController.toggleRepeat();

// //     // Assert: Repeat should be false again
// //     EXPECT_FALSE(playerController.isRepeat());
// // }

// // TEST_F(PlayerControllerTest, GetMediaFilesTest) {
// //     // Arrange
// //     std::vector<std::string> files = {"song1.mp3", "song2.mp3", "song3.mp3"};
// //     PlayerController playerController(files);

// //     // Act
// //     std::vector<std::string> result = playerController.getMediaFiles();

// //     // Assert: The returned list should match the original input
// //     EXPECT_EQ(result.size(), files.size());
// //     EXPECT_EQ(result[0], "song1.mp3");
// //     EXPECT_EQ(result[1], "song2.mp3");
// //     EXPECT_EQ(result[2], "song3.mp3");
// // }

// // TEST_F(PlayerControllerTest, IsRepeatTest) {
// //     // Arrange
// //     std::vector<std::string> files = {"song1.mp3"};
// //     PlayerController playerController(files);

// //     // Act & Assert: Initially, repeat should be false
// //     EXPECT_FALSE(playerController.isRepeat());

// //     // Enable repeat mode
// //     playerController.toggleRepeat();
// //     EXPECT_TRUE(playerController.isRepeat());

// //     // Disable repeat mode
// //     playerController.toggleRepeat();
// //     EXPECT_FALSE(playerController.isRepeat());
// // }

// // TEST_F(PlayerControllerTest, GetDurationTest) {
// //     // Arrange
// //     std::vector<std::string> files = {"song1.mp3"};
// //     PlayerController playerController(files);

// //     // Act & Assert: Initially, duration should be 0
// //     EXPECT_EQ(playerController.getDuration(), 0);

// //     // Simulate some duration increase
// //     playerController.startDuration();
// //     std::this_thread::sleep_for(std::chrono::seconds(2));  // Simulating time passing
// //     playerController.stopDuration();

// //     // Assert: Duration should have increased
// //     EXPECT_GT(playerController.getDuration(), 0);
// // }

// // TEST_F(PlayerControllerTest, SetNotificationsEnabledTest) {
// //     // Arrange
// //     std::vector<std::string> files = {"song1.mp3"};
// //     PlayerController playerController(files);
// //     bool observerNotified = false;

// //     // Add an observer
// //     playerController.addObserverIndex([&observerNotified](int index) {
// //         (void)index;  // Avoid unused variable warning
// //         observerNotified = true;
// //     });

// //     // Act: Disable notifications
// //     playerController.setNotificationsEnabled(false);

// //     // Reset flag before calling notify
// //     observerNotified = false;
// //     playerController.notifyObserversIndex();

// //     // Assert: Observer should NOT be notified
// //     EXPECT_FALSE(observerNotified) << "Observer should NOT be notified when notifications are disabled!";

// //     // Act: Enable notifications
// //     playerController.setNotificationsEnabled(true);

// //     // Reset flag before calling notify
// //     observerNotified = false;
// //     playerController.notifyObserversIndex();

// //     // Assert: Observer SHOULD be notified
// //     EXPECT_TRUE(observerNotified) << "Observer SHOULD be notified when notifications are enabled!";
// // }


// // TEST_F(PlayerControllerTest, GetVolumeTest) {
// //     // Arrange
// //     std::vector<std::string> files = {"song1.mp3"};
// //     PlayerController playerController(files);

// //     // Act & Assert: Default volume should be 100
// //     EXPECT_EQ(playerController.getVolume(), 100);

// //     // Change volume and verify
// //     playerController.setVolume(30);
// //     EXPECT_EQ(playerController.getVolume(), 30);

// //     playerController.setVolume(0);
// //     EXPECT_EQ(playerController.getVolume(), 0);

// //     playerController.setVolume(100);
// //     EXPECT_EQ(playerController.getVolume(), 100);
// // }

// // TEST_F(PlayerControllerTest, GetCurrentIndexTest) {
// //     // Arrange
// //     std::vector<std::string> files = {"song1.mp3", "song2.mp3", "song3.mp3"};
// //     PlayerController playerController(files);

// //     // Act & Assert: Initially, index should be 0
// //     EXPECT_EQ(playerController.getCurrentIndex(), 0);

// //     // Move to the next file manually (without playNext to avoid thread issues)
// //     playerController.notifyObserversIndex();  // Simulates index change
// //     EXPECT_EQ(playerController.getCurrentIndex(), 0);  // Should still be 0
// // }

// // TEST_F(PlayerControllerTest, AddObserverVolumeTest) {
// //     // Arrange
// //     std::vector<std::string> files = {"song1.mp3"};
// //     PlayerController playerController(files);

// //     bool observerNotified = false;

// //     // Add an observer that sets `observerNotified` to true
// //     playerController.addObserverVolume([&observerNotified]() {
// //         observerNotified = true;
// //     });

// //     // Act: Change volume, which should trigger observers
// //     playerController.setVolume(50);
// //     playerController.notifyObserversVolume();

// //     // Assert: The observer should have been notified
// //     EXPECT_TRUE(observerNotified) << "Expected volume observer to be notified.";
// // }

// // TEST_F(PlayerControllerTest, AddObserverStateTest) {
// //     // Arrange
// //     std::vector<std::string> files = {"song1.mp3"};
// //     PlayerController playerController(files);

// //     bool observerNotified = false;

// //     // Add an observer that sets `observerNotified` to true
// //     playerController.addObserverState([&observerNotified]() {
// //         observerNotified = true;
// //     });

// //     // Act: Notify state observers
// //     playerController.notifyObserversState();

// //     // Assert: The observer should have been notified
// //     EXPECT_TRUE(observerNotified) << "Expected state observer to be notified.";
// // }

// // TEST_F(PlayerControllerTest, AddObserverIndexTest) {
// //     // Arrange
// //     std::vector<std::string> files = {"song1.mp3", "song2.mp3"};
// //     PlayerController playerController(files);

// //     bool observerNotified = false;
// //     int observedIndex = -1;

// //     // Add an observer that updates `observedIndex`
// //     playerController.addObserverIndex([&observerNotified, &observedIndex](int index) {
// //         observerNotified = true;
// //         observedIndex = index;
// //     });

// //     // Act: Notify index observers
// //     playerController.notifyObserversIndex();

// //     // Assert: The observer should have been notified with the correct index
// //     EXPECT_TRUE(observerNotified) << "Expected index observer to be notified.";
// //     EXPECT_EQ(observedIndex, playerController.getCurrentIndex()) << "Expected observer to receive the correct index.";
// // }

// // TEST_F(PlayerControllerTest, AddObserverDurationTest) {
// //     // Arrange
// //     std::vector<std::string> files = {"song1.mp3"};
// //     PlayerController playerController(files);

// //     bool observerNotified = false;
// //     int observedDuration = -1;

// //     // Add an observer that updates `observedDuration`
// //     playerController.addObserverDuration([&observerNotified, &observedDuration](int duration) {
// //         observerNotified = true;
// //         observedDuration = duration;
// //     });

// //     // Act: Start the duration tracking, simulate time passing
// //     playerController.startDuration();
// //     std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate a small duration update
// //     playerController.notifyObserversDuration();
// //     playerController.stopDuration();  // Stop it to avoid test delay

// //     // Get actual duration from public method
// //     int expectedDuration = playerController.getDuration();

// //     // Assert: The observer should have been notified with the correct duration
// //     EXPECT_TRUE(observerNotified) << "Expected duration observer to be notified.";
// //     EXPECT_EQ(observedDuration, expectedDuration) << "Expected observer to receive the correct duration.";
// // }

// // TEST_F(PlayerControllerTest, IncreaseDecreaseVolumeTest) {
// //     // Arrange
// //     std::vector<std::string> files = {"song1.mp3"};
// //     PlayerController playerController(files);

// //     // Act & Assert: Increase volume
// //     playerController.setVolume(40);
// //     playerController.increaseVolume(10);
// //     EXPECT_EQ(playerController.getVolume(), 50) << "Expected volume to increase to 50.";

// //     // Act & Assert: Increase volume beyond max limit
// //     playerController.increaseVolume(100);
// //     EXPECT_EQ(playerController.getVolume(), 100) << "Expected volume to stay at 100.";

// //     // Act & Assert: Decrease volume
// //     playerController.setVolume(50);
// //     playerController.decreaseVolume(10);
// //     EXPECT_EQ(playerController.getVolume(), 40) << "Expected volume to decrease to 40.";

// //     // Act & Assert: Decrease volume below min limit
// //     playerController.decreaseVolume(100);
// //     EXPECT_EQ(playerController.getVolume(), 0) << "Expected volume to stay at 0.";
// // }

// // TEST_F(PlayerControllerTest, GetPlaybackThreadTest) {
// //     // Arrange
// //     std::vector<std::string> files = {"song1.mp3"};
// //     PlayerController playerController(files);

// //     // Act: Get the playback thread reference
// //     std::thread& playbackThread = playerController.getPlaybackThread();

// //     // Assert: The playback thread should exist but may not be running yet
// //     EXPECT_FALSE(playbackThread.joinable()) << "Expected playbackThread to exist but not be running initially.";
// // }

// // TEST_F(PlayerControllerTest, PlayVideo_ValidFile) {
// //     // Arrange
// //     std::vector<std::string> files = {"./test/Controller/video.mp4"};
// //     MockPlayerController mockPlayerController(files);

// //     // Expect SDL and FFmpeg functions to be called

// //     ON_CALL(mockPlayerController, SDL_Init(_)).WillByDefault(testing::Return(0));
// //     ON_CALL(mockPlayerController, SDL_OpenAudio(_, _)).WillByDefault(testing::Return(0));
// //     ON_CALL(mockPlayerController, SDL_Quit()).WillByDefault(testing::Return());
// //     ON_CALL(mockPlayerController, avformat_open_input(_, _, _, _)).WillByDefault(testing::Return(0));
// //     ON_CALL(mockPlayerController, avformat_find_stream_info(_, _)).WillByDefault(testing::Return(0));
// //     ON_CALL(mockPlayerController, avcodec_find_decoder(_)).WillByDefault(testing::Return(nullptr)); // Simulate no decoder found


// //     // Act
// //     mockPlayerController.playVideo("./test/Controller/video.mp4");

// //     // Assert
// //     // If `playVideo()` fails due to no decoder, it should not crash.
// // }

// // TEST_F(PlayerControllerTest, StopTest) {
// //     // Arrange
// //     std::vector<std::string> files = {"song1.mp3", "song2.mp3"};
// //     MockPlayerController mockPlayerController(files);

// //     // Mock the necessary methods in PlayerController
// //     EXPECT_CALL(mockPlayerController, stopPlaybackThread()).Times(2);  // stopPlaybackThread should be called
// //     EXPECT_CALL(mockPlayerController, resetDuration()).Times(2);  // resetDuration should be called
// //     EXPECT_CALL(mockPlayerController, startDuration()).Times(1);  // startDuration should be called
// //     // EXPECT_CALL(mockPlayerController, playbackWorker("song1.mp3")).Times(1);  // Ensure playbackWorker is called with the file "c.mp3"

// //     // 🔹 Use ON_CALL instead of EXPECT_CALL to suppress the warning
// //     ON_CALL(mockPlayerController, playbackWorker(testing::_)).WillByDefault(testing::Return());

// //     // Mock isPlaying to simulate playback state change
// //     EXPECT_CALL(mockPlayerController, isPlaying())
// //         .WillOnce(testing::Return(true))  // Initially not playing
// //         .WillOnce(testing::Return(false));  // After calling play(), should be true

// //     // Mock FFmpeg file loading functions
// //     ON_CALL(mockPlayerController, avformat_open_input(_, _, _, _)).WillByDefault(testing::Return(0));
// //     ON_CALL(mockPlayerController, avformat_find_stream_info(_, _)).WillByDefault(testing::Return(0));
// //     ON_CALL(mockPlayerController, avcodec_find_decoder(_)).WillByDefault(testing::Return(nullptr));
// //     ON_CALL(mockPlayerController, avcodec_open2(_, _, _)).WillByDefault(testing::Return(0));
// //     ON_CALL(mockPlayerController, av_read_frame(_, _)).WillByDefault(testing::Return(0));
// //     ON_CALL(mockPlayerController, av_packet_unref(_)).WillByDefault(testing::Return());

// //     // Mock SDL functions to simulate audio/video playback
// //     ON_CALL(mockPlayerController, SDL_Init(_)).WillByDefault(testing::Return(0));
// //     ON_CALL(mockPlayerController, SDL_OpenAudio(_, _)).WillByDefault(testing::Return(0));
// //     ON_CALL(mockPlayerController, SDL_Quit()).WillByDefault(testing::Return());

// //     // Act: Start playing (simulate play() behavior)
// //     mockPlayerController.play();

// //     // Assert: Playback should be started
// //     EXPECT_TRUE(mockPlayerController.isPlaying()) << "Playback should be started.";

// //     // Act: Stop playback
// //     mockPlayerController.stop();

// //     // Assert: After calling stop, playback should be stopped
// //     EXPECT_FALSE(mockPlayerController.isPlaying()) << "Playback should be stopped after calling stop.";

// //     // Assert: Ensure that the current index is reset to 0
// //     EXPECT_EQ(mockPlayerController.getCurrentIndex(), 0) << "Expected current index to be reset after stop.";

// //     // Assert: Ensure the current duration is reset
// //     EXPECT_EQ(mockPlayerController.getDuration(), 0) << "Expected duration to be reset after stop.";
// // }


// // TEST_F(PlayerControllerTest, PauseTest) {
    
// //     // Arrange: Create a PlayerController with a few files
// //     std::vector<std::string> files = {"song1.mp3", "song2.mp3"};
// //     MockPlayerController mockPlayerController(files);
// //     // Set the function pointers to our mock implementations
// //     MockMix_LoadMUS = &MockedMix_LoadMUS;
// //     MockMix_PlayMusic = &MockedMix_PlayMusic;
// //     MockMix_VolumeMusic = &MockedMix_VolumeMusic;
// //     MockMix_HaltMusic = &MockedMix_HaltMusic;
// //     MockMix_CloseAudio = &MockedMix_CloseAudio;

// //     // Expect the methods to be called
// //     EXPECT_CALL(mockPlayerController, stopPlaybackThread())
// //         .Times(1);  // play() should stop any existing playback thread

// //     EXPECT_CALL(mockPlayerController, resetDuration())
// //         .Times(1);  // play() should reset the duration when a new media is played

// //     EXPECT_CALL(mockPlayerController, startDuration())
// //         .Times(1);  // play() should start the duration tracking

// //     EXPECT_CALL(mockPlayerController, playbackWorker("song1.mp3"))
// //         .Times(1);  // Ensure playbackWorker is called with the file "c.mp3"

// //     // Act: Start playing (simulate play() behavior)
// //     mockPlayerController.pause();

// //     waitForPlaybackThreadToFinish(mockPlayerController.getPlaybackThread());

// //     // Wait for the playbackWorker thread to finish
// //     mockPlayerController.play();

// //     mockPlayerController.playing = true;
// //     mockPlayerController.paused = false;

// //     mockPlayerController.pause();
// //     waitForPlaybackThreadToFinish(mockPlayerController.getPlaybackThread());
// // }

// // TEST_F(PlayerControllerTest, ResetDurationTest) {
// //     // Arrange: Use the real PlayerController
// //     std::vector<std::string> files = {"song1.mp3", "song2.mp3"};
// //     PlayerController playerController(files);  // Use real instance

// //     // Act: Call the actual resetDuration()
// //     playerController.resetDuration();

// //     // Assert: Check if duration is reset to 0
// //     EXPECT_EQ(playerController.getDuration(), 0);
// // }

// // TEST_F(PlayerControllerTest, IsPlayingTest) {
// //     // Arrange: Use the real PlayerController instead of the mock
// //     std::vector<std::string> files = {"song1.mp3", "song2.mp3"};
// //     PlayerController playerController(files);  // Use real instance

// //     // Act & Assert: Initially, isPlaying() should return false
// //     EXPECT_FALSE(playerController.isPlaying());

// //     // Simulate playing state
// //     playerController.playing = true;  // Directly modify the state
// //     EXPECT_TRUE(playerController.isPlaying());

// //     // Simulate stopping playback
// //     playerController.playing = false;
// //     EXPECT_FALSE(playerController.isPlaying());
// // }

// // TEST_F(PlayerControllerTest, IsPauseTest) {
// //     // Arrange: Use the real PlayerController instead of the mock
// //     std::vector<std::string> files = {"song1.mp3", "song2.mp3"};
// //     PlayerController playerController(files);  // Use real instance

// //     // Act & Assert: Initially, isPlaying() should return false
// //     EXPECT_FALSE(playerController.isPause());

// //     // Simulate playing state
// //     playerController.paused = true;  // Directly modify the state
// //     EXPECT_TRUE(playerController.isPause());

// //     // Simulate stopping playback
// //     playerController.paused = false;
// //     EXPECT_FALSE(playerController.isPause());
// // }

// // TEST_F(PlayerControllerTest, TogglePlayback_Coverage) {
// //     // Arrange: Use a subclass to override `play()` and avoid real playback
// //     class TestPlayerController : public PlayerController {
// //     public:
// //         using PlayerController::PlayerController;
// //         void play() override { playing = true; }  // Simulate play without actual file loading
// //         void pause() override { paused = true; }  // Simulate pause
// //         void resume() override { paused = false; } // Simulate resume
// //     };

// //     std::vector<std::string> files = {"song1.mp3", "song2.mp3"};
// //     TestPlayerController playerController(files);  // Use test subclass

// //     // Act: Call togglePlayback() multiple times to execute all branches
// //     playerController.togglePlayback();  // Simulates starting playback
// //     playerController.togglePlayback();  // Simulates pausing playback
// //     playerController.togglePlayback();  // Simulates resuming playback
// // }
// // class MockExternalDependencies {
// // public:
// //     MOCK_METHOD(int, avformat_open_input, (AVFormatContext**, const char*, AVInputFormat*, AVDictionary**), ());
// //     MOCK_METHOD(int, avformat_find_stream_info, (AVFormatContext*, AVDictionary**), ());
// //     MOCK_METHOD(AVCodec*, avcodec_find_decoder, (enum AVCodecID), ());
// //     MOCK_METHOD(int, avcodec_open2, (AVCodecContext*, const AVCodec*, AVDictionary**), ());
// //     MOCK_METHOD(int, av_read_frame, (AVFormatContext*, AVPacket*), ());
// //     MOCK_METHOD(void, av_packet_unref, (AVPacket*), ());
// //     MOCK_METHOD(int, SDL_Init, (Uint32 flags), ());
// //     MOCK_METHOD(int, SDL_OpenAudio, (SDL_AudioSpec*, SDL_AudioSpec*), ());
// //     MOCK_METHOD(void, SDL_Quit, (), ());
// // };

// // TEST_F(PlayerControllerTest, PlayVideoTestWithRealFile) {
// //     // Arrange: Create a real PlayerController instance with a real MP4 file
// //     std::vector<std::string> files = {"./test/Controller/video.mp4"};  // Ensure this file exists
// //     PlayerController* playerController = new PlayerController(files);  // Use pointer if needed

// //     // Create a mock for external dependencies (FFmpeg and SDL)
// //     MockExternalDependencies mockExternal;

// //     // Use ON_CALL to simulate success for external methods (no need for real file operations here)
// //     ON_CALL(mockExternal, avformat_open_input(testing::_, testing::_, testing::_, testing::_))
// //         .WillByDefault(testing::Return(0));  // Simulate success in opening the file

// //     ON_CALL(mockExternal, avformat_find_stream_info(testing::_, testing::_))
// //         .WillByDefault(testing::Return(0));  // Simulate successful stream retrieval

// //     ON_CALL(mockExternal, avcodec_find_decoder(_)).WillByDefault(testing::Return(nullptr));  // Simulate codec found
// //     ON_CALL(mockExternal, avcodec_open2(_, _, _)).WillByDefault(testing::Return(0));  // Successfully open codec

// //     ON_CALL(mockExternal, SDL_Init(_)).WillByDefault(testing::Return(0));  // Simulate SDL init
// //     ON_CALL(mockExternal, SDL_OpenAudio(_, _)).WillByDefault(testing::Return(0));  // Simulate audio device opening
// //     ON_CALL(mockExternal, SDL_Quit()).WillByDefault(testing::Return());  // Simulate SDL quit

// //     // Act: Call playVideo() to trigger the video playback process (with real video file)
// //     playerController->playVideo("./test/Controller/video.mp4");  // Make sure the file exists
// // }

// // class MockSDL_mixer {
// // public:
// //     MOCK_METHOD(int, Mix_OpenAudio, (int frequency, Uint16 format, int channels, int chunksize), (const));  // Mock Mix_OpenAudio
// //     MOCK_METHOD(Mix_Music*, Mix_LoadMUS, (const char* file), (const));  // Mock Mix_LoadMUS
// //     MOCK_METHOD(void, Mix_PlayMusic, (Mix_Music* music, int loops), ());  // Mock Mix_PlayMusic
// //     MOCK_METHOD(void, Mix_VolumeMusic, (int volume), ());  // Mock Mix_VolumeMusic
// //     MOCK_METHOD(void, Mix_HaltMusic, (), ());  // Mock Mix_HaltMusic
// //     MOCK_METHOD(void, Mix_CloseAudio, (), ());  // Mock Mix_CloseAudio
// // };

// // TEST_F(PlayerControllerTest, PlayAudioTestWithRealFile) {
// //     // Arrange: Create a real PlayerController instance with a real audio file
// //     std::vector<std::string> files = {"./test/Controller/c.mp3"};  // Ensure this is the correct path to the audio file
// //     PlayerController* playerController = new PlayerController(files);  // Use pointer if needed

// //     // Create a mock for SDL_mixer external dependencies
// //     MockSDL_mixer mockSDL;

// //     // Use ON_CALL to simulate success for Mix functions
// //     ON_CALL(mockSDL, Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048))
// //         .WillByDefault(testing::Return(0));  // Simulate success in initializing SDL_mixer

// //     // Simulate success in loading the music file
// //     ON_CALL(mockSDL, Mix_LoadMUS(testing::_))
// //         .WillByDefault(testing::Return(reinterpret_cast<Mix_Music*>(1)));  // Simulate success in loading audio (casting to Mix_Music*)

// //     ON_CALL(mockSDL, Mix_PlayMusic(testing::_, testing::_))
// //         .WillByDefault(testing::Return());  // Simulate music playback

// //     ON_CALL(mockSDL, Mix_VolumeMusic(testing::_))
// //         .WillByDefault(testing::Return());  // Simulate volume change

// //     ON_CALL(mockSDL, Mix_HaltMusic())
// //         .WillByDefault(testing::Return());  // Simulate halting music

// //     ON_CALL(mockSDL, Mix_CloseAudio())
// //         .WillByDefault(testing::Return());  // Simulate closing audio

// //     // Act: Call playAudio() to trigger the audio playback process (with real audio file)
// //     playerController->playAudio("./test/Controller/c.mp3");  // Ensure the file exists
// // }


// // TEST_F(PlayerControllerTest, ResumeTest) {
// //     // Arrange: Create a PlayerController with some files
// //     std::vector<std::string> files = {"song1.mp3", "song2.mp3"};
// //     PlayerController playerController(files);

// //     // Act & Assert: Initially, resume() should not work because playback isn't paused
// //     playerController.resume(); // Calling resume when not playing or paused
// //     EXPECT_FALSE(playerController.isPlaying()) << "Expected player to be not playing initially.";

// //     // Simulate playing state (not paused)
// //     playerController.playing = true;
// //     playerController.paused = false;

// //     // Act: Calling resume() when already playing should not change the state
// //     playerController.resume();
// //     EXPECT_TRUE(playerController.isPlaying()) << "Expected player to remain playing.";

// //     // Simulate paused state
// //     playerController.paused = true;
// //     playerController.playing = true;

// //     // Act: Calling resume() should unpause the playback and continue playing
// //     playerController.resume();
// //     EXPECT_FALSE(playerController.isPause()) << "Expected player to resume playback (no longer paused).";
// //     EXPECT_TRUE(playerController.isPlaying()) << "Expected player to remain playing after resume.";
// // }

// // TEST_F(PlayerControllerTest, PlayNextTest) {
// //     // Arrange: Create a list of media files and a mock object
// //     std::vector<std::string> files = {"test.mp3"};
// //     MockPlayerController mockPlayerController(files);

// //     // Set the function pointers to our mock implementations
// //     MockMix_LoadMUS = &MockedMix_LoadMUS;
// //     MockMix_PlayMusic = &MockedMix_PlayMusic;
// //     MockMix_VolumeMusic = &MockedMix_VolumeMusic;
// //     MockMix_HaltMusic = &MockedMix_HaltMusic;
// //     MockMix_CloseAudio = &MockedMix_CloseAudio;

// //     // Expect the methods to be called
// //     EXPECT_CALL(mockPlayerController, stopPlaybackThread())
// //         .Times(2);  // play() should stop any existing playback thread

// //     EXPECT_CALL(mockPlayerController, resetDuration())
// //         .Times(2);  // play() should reset the duration when a new media is played

// //     EXPECT_CALL(mockPlayerController, startDuration())
// //         .Times(1);  // play() should start the duration tracking

// //     EXPECT_CALL(mockPlayerController, playbackWorker("test.mp3"))
// //         .Times(1);  // Ensure playbackWorker is called with the file "c.mp3"

// //     mockPlayerController.playNext();

// //     // Wait for the playbackWorker thread to finish
// //     waitForPlaybackThreadToFinish(mockPlayerController.getPlaybackThread());
// // }

// // TEST_F(PlayerControllerTest, PlayPreviousTest) {
// //     // Arrange: Create a list of media files and a mock object
// //     std::vector<std::string> files = {"test.mp3"};
// //     MockPlayerController mockPlayerController(files);

// //     // Set the function pointers to our mock implementations
// //     MockMix_LoadMUS = &MockedMix_LoadMUS;
// //     MockMix_PlayMusic = &MockedMix_PlayMusic;
// //     MockMix_VolumeMusic = &MockedMix_VolumeMusic;
// //     MockMix_HaltMusic = &MockedMix_HaltMusic;
// //     MockMix_CloseAudio = &MockedMix_CloseAudio;

// //     // Expect the methods to be called
// //     EXPECT_CALL(mockPlayerController, stopPlaybackThread())
// //         .Times(2);  // play() should stop any existing playback thread

// //     EXPECT_CALL(mockPlayerController, resetDuration())
// //         .Times(2);  // play() should reset the duration when a new media is played

// //     EXPECT_CALL(mockPlayerController, startDuration())
// //         .Times(1);  // play() should start the duration tracking

// //     EXPECT_CALL(mockPlayerController, playbackWorker("test.mp3"))
// //         .Times(1);  // Ensure playbackWorker is called with the file "c.mp3"

// //     mockPlayerController.playPrevious();

// //     // Wait for the playbackWorker thread to finish
// //     waitForPlaybackThreadToFinish(mockPlayerController.getPlaybackThread());
// // }