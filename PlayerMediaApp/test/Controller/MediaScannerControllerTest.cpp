#include "Controller/MediaScannerController.h"
#include "Model/FolderManager.h"
#include "Model/MediaFileManager.h"
#include "Model/PlaylistManager.h"
#include "View/BaseView.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

// Mock MediaFileManager
class MockMediaFileManager : public MediaFileManager {
public:
  MOCK_METHOD(bool, addMediaFile, (std::string pathName, std::string type),
              (override));
  MOCK_METHOD(bool, loadMediaFile, (std::string pathName, std::string type),
              (override));
  MOCK_METHOD(std::vector<std::shared_ptr<MediaFile>>, getAllMediaFile, (),
              (override));
};

// Mock PlaylistManager
class MockPlaylistManager : public PlaylistManager {
public:
  MOCK_METHOD(void, loadPlaylist, (std::shared_ptr<Playlist> playlist),
              (override));
};
// Mock FolderManager
class MockFolderManager : public FolderManager {
public:
  MOCK_METHOD(std::unordered_set<std::string>, getListFolderDirectory, (),
              (override));
  MOCK_METHOD(std::unordered_set<std::string>, getListFolderUSB, (),
              (override));
  MOCK_METHOD(std::unordered_set<std::string>, getListPathDirectory,
              (std::string nameFolder), (override));
  MOCK_METHOD(std::unordered_set<std::string>, getListPathUSB,
              (std::string nameFolder), (override));

  MOCK_METHOD(std::string, getFolderDirectory, (std::string folderName),
              (override));
  MOCK_METHOD(std::string, getFolderUSB, (std::string folderName), (override));

  MOCK_METHOD(void, saveFolderDirectory, (std::string folderName), (override));
  MOCK_METHOD(void, saveFolderUSB, (std::string folderName), (override));

  MOCK_METHOD(void, updateFolderDirectory, (std::string folderName),
              (override));
  MOCK_METHOD(void, updateFolderUSB, (std::string folderName), (override));

  MOCK_METHOD(void, addDataFolderDirectory,
              (const std::string &folder,
               std::unordered_set<std::string> listFiles),
              (override));
  MOCK_METHOD(void, addDataFolderUSB,
              (const std::string &folder,
               std::unordered_set<std::string> listFiles),
              (override));
  
};

// Mock BaseView
class MockBaseView : public BaseView {
public:
  MOCK_METHOD(int, showListFolder, (std::vector<std::string> folders),
              (override));
  MOCK_METHOD(int, showListUSBName, (std::vector<std::string> usb_devices),
              (override));
  MOCK_METHOD(int, showMenu, (), (override));
};

class MockMediaScannerController : public MediaScannerController {
public:
    MockMediaScannerController(MediaFileManager& m, PlaylistManager& p, FolderManager& f, std::shared_ptr<BaseView> v)
        : MediaScannerController(m, p, f, v) {}

    MOCK_METHOD(std::vector<std::string>, list_folders, (const std::string &path), (override));
    MOCK_METHOD(std::vector<std::string>, list_media_files, (const std::string &path), (override));
    MOCK_METHOD(std::vector<std::string>, scan_all_folders, (const std::string &path), (override));
    MOCK_METHOD(bool, fileExists, (const std::string &path), (override));
    //Mock std::unordered_set<std::string> scanFolder(const std::string &path);
    MOCK_METHOD(std::unordered_set<std::string>, scanFolder, (const std::string &path), (override));
};


class MediaScannerControllerTest : public ::testing::Test {
protected:
  MockMediaFileManager mockMediaFileManager;
  MockPlaylistManager mockPlaylistManager;
  MockFolderManager mockFolderManager;
  std::shared_ptr<MockBaseView> mockView;

  MediaScannerController *scannerController;

  void SetUp() override {
    mockView = std::make_shared<MockBaseView>();
    scannerController = new MediaScannerController(
        mockMediaFileManager, mockPlaylistManager, mockFolderManager, mockView);
  }
  
  void TearDown() override 
  { 
    delete scannerController; 
  }
};

// Test checkFolderDirectory()
TEST_F(MediaScannerControllerTest, CheckFolderDirectory_Empty) {
  EXPECT_CALL(mockFolderManager, getListFolderDirectory())
      .WillOnce(Return(std::unordered_set<std::string>{}));

  bool result = scannerController->checkFolderDirectory();
  EXPECT_TRUE(result);
}

TEST_F(MediaScannerControllerTest, CheckFolderDirectory_NotEmpty) {
  EXPECT_CALL(mockFolderManager, getListFolderDirectory())
      .WillOnce(Return(std::unordered_set<std::string>{"folder1", "folder2"}));

  bool result = scannerController->checkFolderDirectory();
  EXPECT_FALSE(result); 
}

TEST_F(MediaScannerControllerTest, CheckFolderUSB_NotEmpty) {
  EXPECT_CALL(mockFolderManager, getListFolderUSB())
      .WillOnce(Return(std::unordered_set<std::string>{"usb1", "usb2"}));

  bool result = scannerController->checkFolderUSB();
  EXPECT_FALSE(result);  
}

TEST_F(MediaScannerControllerTest, CheckFolderUSB_Empty) {
  EXPECT_CALL(mockFolderManager, getListFolderUSB())
      .WillOnce(Return(std::unordered_set<std::string>{}));

  bool result = scannerController->checkFolderUSB();
  EXPECT_TRUE(result);
}

TEST_F(MediaScannerControllerTest, ScanHomeDirectory_Success) {
    MockMediaScannerController mockScanner(mockMediaFileManager, mockPlaylistManager, mockFolderManager, mockView);

    std::vector<std::string> mockFolders = {
        "/home/bluebird/Music",
        "/home/bluebird/video_audio",
        "/home/bluebird/Documents"
    };
    std::vector<std::string> mockMediaFiles = {"/home/bluebird/video_audio/drum.mp3", "/home/bluebird/video_audio/take.mp4"};

    EXPECT_CALL(mockScanner, list_folders(_))
        .WillOnce(Return(mockFolders));

    EXPECT_CALL(*mockView, showListFolder(_))
        .WillOnce(Return(1)); 

    EXPECT_CALL(mockScanner, list_media_files("/home/bluebird/video_audio"))
        .WillOnce(Return(mockMediaFiles));

    EXPECT_CALL(mockFolderManager, getFolderDirectory("/home/bluebird/video_audio"))
        .WillOnce(Return("Folder not found."));

    EXPECT_CALL(mockFolderManager, saveFolderDirectory("/home/bluebird/video_audio"));

    EXPECT_CALL(mockFolderManager, getListPathDirectory("/home/bluebird/video_audio"))
        .WillOnce(Return(std::unordered_set<std::string>{"/home/bluebird/video_audio/drum.mp3", "/home/bluebird/video_audio/take.mp4"}));

    EXPECT_CALL(mockFolderManager, addDataFolderDirectory("/home/bluebird/video_audio", 
      UnorderedElementsAre("/home/bluebird/video_audio/drum.mp3", "/home/bluebird/video_audio/take.mp4")));


    EXPECT_CALL(mockMediaFileManager, addMediaFile("/home/bluebird/video_audio/drum.mp3", "Audio"))
        .Times(AtLeast(1))
        .WillOnce(Return(true));

    EXPECT_CALL(mockMediaFileManager, addMediaFile("/home/bluebird/video_audio/take.mp4", "Video"))
        .Times(AtLeast(1))
        .WillOnce(Return(true));

    mockScanner.scanHomeDirectory();
}

TEST_F(MediaScannerControllerTest, ScanUSBDevices_Success) {
    MockMediaScannerController mockScanner(mockMediaFileManager, mockPlaylistManager, mockFolderManager, mockView);

    std::vector<std::string> mockUSBDevices = {"/media/bluebird/SD_4GB"};
    std::vector<std::string> mockFolders = {"/media/bluebird/SD_4GB/file", "/media/bluebird/SD_4GB/file_test"};
    std::vector<std::string> mockMediaFiles = {"/media/bluebird/SD_4GB/file/creepy.mp3", "/media/bluebird/SD_4GB/file/yorushika.mp4"};

    EXPECT_CALL(mockScanner, list_folders("/media/bluebird"))
        .WillOnce(Return(mockUSBDevices));

    EXPECT_CALL(*mockView, showListUSBName(_))
        .WillOnce(Return(0));

    EXPECT_CALL(mockScanner, list_folders("/media/bluebird/SD_4GB"))
        .WillOnce(Return(mockFolders));

    EXPECT_CALL(*mockView, showListFolder(_))
        .WillOnce(Return(0));

    EXPECT_CALL(mockScanner, list_media_files("/media/bluebird/SD_4GB/file"))
        .WillOnce(Return(mockMediaFiles));

    EXPECT_CALL(mockFolderManager, getFolderUSB("/media/bluebird/SD_4GB/file"))
        .WillOnce(Return("Folder not found."));

    EXPECT_CALL(mockFolderManager, saveFolderUSB("/media/bluebird/SD_4GB/file"));

    EXPECT_CALL(mockFolderManager, getListPathUSB("/media/bluebird/SD_4GB/file"))
        .WillOnce(Return(std::unordered_set<std::string>{"/media/bluebird/SD_4GB/file/creepy.mp3", "/media/bluebird/SD_4GB/file/yorushika.mp4"}));

    EXPECT_CALL(mockFolderManager, addDataFolderUSB("/media/bluebird/SD_4GB/file", 
        UnorderedElementsAre("/media/bluebird/SD_4GB/file/creepy.mp3", "/media/bluebird/SD_4GB/file/yorushika.mp4")));

    EXPECT_CALL(mockMediaFileManager, addMediaFile("/media/bluebird/SD_4GB/file/creepy.mp3", "Audio"))
        .Times(AtLeast(1))
        .WillOnce(Return(true));

    EXPECT_CALL(mockMediaFileManager, addMediaFile("/media/bluebird/SD_4GB/file/yorushika.mp4", "Video"))
        .Times(AtLeast(1))
        .WillOnce(Return(true));

    mockScanner.scanUSBDevices();
}

TEST_F(MediaScannerControllerTest, ScanFolder_Success) {
    MockMediaScannerController mockScanner(mockMediaFileManager, mockPlaylistManager, mockFolderManager, mockView);

    std::string testPath = "/home/bluebird/video_audio";
    std::vector<std::string> mockMediaFiles = {"/home/bluebird/video_audio/drum.mp3", "/home/bluebird/video_audio/take.mp4"};
    std::unordered_set<std::string> expectedMediaFiles(mockMediaFiles.begin(), mockMediaFiles.end());

    EXPECT_CALL(mockScanner, list_media_files(testPath))
        .WillOnce(Return(mockMediaFiles));

    EXPECT_CALL(mockScanner, scanFolder(testPath))
        .WillOnce([&](const std::string& path) {
            std::vector<std::string> files = mockScanner.list_media_files(path);
            return std::unordered_set<std::string>(files.begin(), files.end());
        });

    std::unordered_set<std::string> result = mockScanner.scanFolder(testPath);

    EXPECT_EQ(result, expectedMediaFiles);
}



TEST_F(MediaScannerControllerTest, LoadData_Success) {
    MockMediaScannerController mockScanner(mockMediaFileManager, mockPlaylistManager, mockFolderManager, mockView);

    std::vector<std::string> mockVideoFiles = {"/home/bluebird/video_audio/take.mp4", "/home/bluebird/video_audio/yorushika.mp4"};
    std::vector<std::string> mockAudioFiles = {"/home/bluebird/video_audio/drum.mp3", "/home/bluebird/video_audio/creepy.mp3"};

    EXPECT_CALL(mockScanner, fileExists(_)).WillRepeatedly(Return(true));
    
    for (const auto& file : mockVideoFiles) {
        EXPECT_CALL(mockMediaFileManager, loadMediaFile(file, "Video"));
    }

    for (const auto& file : mockAudioFiles) {
        EXPECT_CALL(mockMediaFileManager, loadMediaFile(file, "Audio"));
    }

    mockScanner.loadData();
}

TEST_F(MediaScannerControllerTest, LoadFolder_Success) {
    MockMediaScannerController mockScanner(mockMediaFileManager, mockPlaylistManager, mockFolderManager, mockView);

    std::vector<std::string> mockFolders = {"/home/bluebird/video_audio", "/home/bluebird/Music"};
    std::unordered_set<std::string> mockMediaFilesVideo = {"/home/bluebird/video_audio/drum.mp3", "/home/bluebird/video_audio/yorushika.mp4"};
    std::unordered_set<std::string> mockMediaFilesMusic = {"/home/bluebird/Music/song1.mp3", "/home/bluebird/Music/song2.mp3"};

    EXPECT_CALL(mockScanner, scanFolder("/home/bluebird/video_audio"))
        .WillOnce(Return(mockMediaFilesVideo));

    EXPECT_CALL(mockScanner, scanFolder("/home/bluebird/Music"))
        .WillOnce(Return(mockMediaFilesMusic));

    EXPECT_CALL(mockFolderManager, addDataFolderDirectory("/home/bluebird/video_audio", mockMediaFilesVideo))
        .Times(1);

    EXPECT_CALL(mockFolderManager, addDataFolderDirectory("/home/bluebird/Music", mockMediaFilesMusic))
        .Times(1);

    EXPECT_CALL(mockFolderManager, addDataFolderUSB(_, _)).Times(0);

    mockScanner.loadFolder();
}

