#include "Controller/MediaScannerController.h"
#include "Model/FolderManager.h"
#include "Model/MediaFileManager.h"
#include "Model/PlaylistManager.h"
#include "View/BaseView.h"
#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sstream>

using ::testing::Return;
using ::testing::Throw;

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
  MockMediaScannerController(MediaFileManager &m, PlaylistManager &p,
                             FolderManager &f, std::shared_ptr<BaseView> v)
      : MediaScannerController(m, p, f, v) {}

  MOCK_METHOD(void, scanHomeDirectory, (), (override));
  MOCK_METHOD(void, scanUSBDevices, (), (override));

  MOCK_METHOD(void, loadData, (), (override));
  MOCK_METHOD(void, loadFolder, (), (override));
  MOCK_METHOD(void, loadMediaPlaylist, (), (override));

  MOCK_METHOD(void, addDataFileWithFolder,
              (std::string nameFolder, std::string nameLocation), (override));

  MOCK_METHOD(std::vector<std::string>, list_folders, (const std::string &path),
              (override));
  MOCK_METHOD(std::vector<std::string>, list_media_files,
              (const std::string &path), (override));
  MOCK_METHOD(bool, fileExists, (const std::string &path), (override));
  MOCK_METHOD(std::unordered_set<std::string>, scanFolder,
              (const std::string &path), (override));
};

class MediaScannerControllerTest : public ::testing::Test {
protected:
  MockMediaFileManager mockMediaFileManager;
  MockPlaylistManager mockPlaylistManager;
  MockFolderManager mockFolderManager;
  std::shared_ptr<MockBaseView> mockView;

  std::shared_ptr<MockMediaScannerController> mockScanner;

  void SetUp() override {
    mockView = std::make_shared<MockBaseView>();
    mockScanner = std::make_shared<MockMediaScannerController>(
        mockMediaFileManager, mockPlaylistManager, mockFolderManager, mockView);
  }

  void TearDown() override {
    // Explicitly verify and clear expectations before resetting
    if (mockScanner) {
      testing::Mock::VerifyAndClearExpectations(mockScanner.get());
    }

    // Allow leak for mockScanner as a last resort
    testing::Mock::AllowLeak(mockScanner.get());

    mockScanner.reset();
    mockView.reset();

    testing::Mock::AllowLeak(&mockMediaFileManager);
    testing::Mock::AllowLeak(&mockPlaylistManager);
    testing::Mock::AllowLeak(&mockFolderManager);
  }
};

// Test checkFolderDirectory()
TEST_F(MediaScannerControllerTest, CheckFolderDirectory_Empty) {
  EXPECT_CALL(mockFolderManager, getListFolderDirectory())
      .WillOnce(Return(std::unordered_set<std::string>{}));

  bool result = mockScanner->checkFolderDirectory();
  EXPECT_TRUE(result);
}
TEST_F(MediaScannerControllerTest, HasExtension_Valid) {
  EXPECT_TRUE(mockScanner->has_extension("song.mp3", ".mp3"));
  EXPECT_TRUE(mockScanner->has_extension("video.mp4", ".mp4"));
  EXPECT_TRUE(mockScanner->has_extension("document.txt", ".txt"));
}

TEST_F(MediaScannerControllerTest, HasExtension_Invalid) {
  EXPECT_FALSE(mockScanner->has_extension("song.mp3", ".mp4"));
  EXPECT_FALSE(mockScanner->has_extension("video.mp4", ".mp3"));
  EXPECT_FALSE(mockScanner->has_extension("document", ".txt"));
}

TEST_F(MediaScannerControllerTest, HasExtension_EmptyFilename) {
  EXPECT_FALSE(mockScanner->has_extension("", ".mp3"));
}

TEST_F(MediaScannerControllerTest, HasExtension_EmptyExtension) {
  EXPECT_FALSE(mockScanner->has_extension("song.mp3", ""));
  EXPECT_FALSE(mockScanner->has_extension("", ".mp3"));
}

TEST_F(MediaScannerControllerTest, HasExtension_FilenameTooShort) {
  EXPECT_FALSE(mockScanner->has_extension("a.txt", ".mp4"));

  EXPECT_FALSE(mockScanner->has_extension("b", ".mp3"));

  EXPECT_FALSE(mockScanner->has_extension("f", ".mp4"));
}

TEST_F(MediaScannerControllerTest, ListFolders_Success) {
  std::string testPath = "/home/bluebird/testPath";
  std::vector<std::string> mockFolders = {"/home/bluebird/testPath/Music",
                                          "/home/bluebird/testPath/Videos"};

  ON_CALL(*mockScanner, list_folders(testPath))
      .WillByDefault(Return(mockFolders));

  std::vector<std::string> result = mockScanner->list_folders(testPath);
  EXPECT_EQ(result, mockFolders);
}

TEST_F(MediaScannerControllerTest,
       ListFolders_ActualImplementation_InvalidPath) {
  MediaScannerController realScanner(mockMediaFileManager, mockPlaylistManager,
                                     mockFolderManager, mockView);

  std::string invalidPath = "/invalid_path";

  EXPECT_THROW(realScanner.list_folders(invalidPath), DirectoryOpenException);
}

TEST_F(MediaScannerControllerTest, ListFolders_Empty) {

  std::string testPath = "/home/bluebird/testPathEmpty";
  std::vector<std::string> emptyFolders = {};

  ON_CALL(*mockScanner, list_folders(testPath))
      .WillByDefault(Return(emptyFolders));

  std::vector<std::string> result = mockScanner->list_folders(testPath);
  EXPECT_TRUE(result.empty());
}

TEST_F(MediaScannerControllerTest, ListFolders_InvalidPath) {

  std::string invalidPath = "/invalid_path";

  ON_CALL(*mockScanner, list_folders(invalidPath))
      .WillByDefault(testing::Throw(DirectoryOpenException(invalidPath)));

  EXPECT_THROW(mockScanner->list_folders(invalidPath), DirectoryOpenException);
}

TEST_F(MediaScannerControllerTest, ListMediaFiles_InvalidPath) {

  std::string invalidPath = "/invalid_path";

  ON_CALL(*mockScanner, list_media_files(invalidPath))
      .WillByDefault(testing::Throw(DirectoryOpenException(invalidPath)));

  EXPECT_THROW(mockScanner->list_media_files(invalidPath),
               DirectoryOpenException);
}

TEST_F(MediaScannerControllerTest, ListMediaFiles_NoValidFiles) {

  std::string testPath = "/home/bluebird/Documents";

  ON_CALL(*mockScanner, list_media_files(testPath))
      .WillByDefault(Return(std::vector<std::string>{}));

  std::vector<std::string> result = mockScanner->list_media_files(testPath);
  EXPECT_TRUE(result.empty());
}

TEST_F(MediaScannerControllerTest, ListMediaFiles_Success) {

  std::string testPath = "/home/bluebird/Videos";
  std::vector<std::string> mockMediaFiles = {
      "/home/bluebird/Videos/yorushika.mp4", "/home/bluebird/Videos/take.mp4"};

  ON_CALL(*mockScanner, list_media_files(testPath))
      .WillByDefault(Return(mockMediaFiles));

  std::vector<std::string> result = mockScanner->list_media_files(testPath);

  std::sort(mockMediaFiles.begin(), mockMediaFiles.end());
  std::sort(result.begin(), result.end());

  EXPECT_EQ(result, mockMediaFiles);
}

TEST_F(MediaScannerControllerTest,
       ListMediaFiles_ThrowsException_WhenOpendirFails) {

  std::string testPath = "/home/bluebird/invalid_folder";

  ON_CALL(*mockScanner, list_media_files(testPath))
      .WillByDefault(testing::Throw(DirectoryOpenException(testPath)));

  EXPECT_THROW(mockScanner->list_media_files(testPath), DirectoryOpenException);
}

TEST_F(MediaScannerControllerTest, ListMediaFiles_ValidButEmptyFolder) {

  std::string testPath = "/home/bluebird/testPathEmpty";

  ON_CALL(*mockScanner, list_media_files(testPath))
      .WillByDefault(Return(std::vector<std::string>{}));

  std::vector<std::string> result = mockScanner->list_media_files(testPath);
  EXPECT_TRUE(result.empty());
}

TEST_F(MediaScannerControllerTest, ListMediaFiles_NoPermission) {

  std::string testPath = "/home/bluebird/RestrictedFolder";

  ON_CALL(*mockScanner, list_media_files(testPath))
      .WillByDefault(testing::Throw(DirectoryOpenException(testPath)));

  EXPECT_THROW(mockScanner->list_media_files(testPath), DirectoryOpenException);
}

TEST_F(MediaScannerControllerTest, ListMediaFiles_PathDoesNotExist) {

  std::string testPath = "/home/bluebird/NonExistentFolder";

  ON_CALL(*mockScanner, list_media_files(testPath))
      .WillByDefault(testing::Throw(DirectoryOpenException(testPath)));

  EXPECT_THROW(mockScanner->list_media_files(testPath), DirectoryOpenException);
}

TEST_F(MediaScannerControllerTest, CheckFolderDirectory_NotEmpty) {
  EXPECT_CALL(mockFolderManager, getListFolderDirectory())
      .WillOnce(Return(std::unordered_set<std::string>{"folder1", "folder2"}));

  bool result = mockScanner->checkFolderDirectory();
  EXPECT_FALSE(result);
}

TEST_F(MediaScannerControllerTest, FileExists_ValidFile) {
  std::string validPath = "/home/bluebird/test_folder_media/drum.mp3";

  MediaScannerController realScanner(mockMediaFileManager, mockPlaylistManager,
                                     mockFolderManager, mockView);
  bool result = realScanner.fileExists(validPath);
  EXPECT_TRUE(result);
}

TEST_F(MediaScannerControllerTest, FileExists_NonExistentFile) {
  std::string invalidPath = "/home/bluebird/nonexistentfile.mp3";

  ON_CALL(*mockScanner, fileExists(invalidPath)).WillByDefault(Return(false));

  bool result = mockScanner->fileExists(invalidPath);
  EXPECT_FALSE(result);
}

TEST_F(MediaScannerControllerTest, FileExists_InvalidPath) {
  std::string invalidPath = "/home/bluebird/music_folder/";

  ON_CALL(*mockScanner, fileExists(invalidPath)).WillByDefault(Return(false));

  bool result = mockScanner->fileExists(invalidPath);

  EXPECT_FALSE(result);
}

TEST_F(MediaScannerControllerTest, CheckFolderUSB_NotEmpty) {
  EXPECT_CALL(mockFolderManager, getListFolderUSB())
      .WillOnce(Return(std::unordered_set<std::string>{"usb1", "usb2"}));

  bool result = mockScanner->checkFolderUSB();
  EXPECT_FALSE(result);
}

TEST_F(MediaScannerControllerTest, CheckFolderUSB_Empty) {
  EXPECT_CALL(mockFolderManager, getListFolderUSB())
      .WillOnce(Return(std::unordered_set<std::string>{}));

  bool result = mockScanner->checkFolderUSB();
  EXPECT_TRUE(result);
}

TEST_F(MediaScannerControllerTest, ScanFolder_Success) {
  std::string testPath = "/home/bluebird/test_folder_media";
  std::vector<std::string> mockMediaFiles = {
      "/home/bluebird/test_folder_media/drum.mp3",
      "/home/bluebird/test_folder_media/creepy.mp3",
      "/home/bluebird/test_folder_media/take.mp4",
      "/home/bluebird/test_folder_media/yorushika.mp4"};
  std::unordered_set<std::string> expectedMediaFiles(mockMediaFiles.begin(),
                                                     mockMediaFiles.end());

  ON_CALL(*mockScanner, scanFolder(testPath))
      .WillByDefault(Return(expectedMediaFiles));

  std::unordered_set<std::string> result = mockScanner->scanFolder(testPath);

  EXPECT_FALSE(result.empty());
  EXPECT_EQ(result, expectedMediaFiles);
}

TEST_F(MediaScannerControllerTest, ScanFolder_ActualImplementation) {
  MediaScannerController realScanner(mockMediaFileManager, mockPlaylistManager,
                                     mockFolderManager, mockView);
  std::string testPath = "/home/bluebird/test_folder_media";

  std::unordered_set<std::string> result = realScanner.scanFolder(testPath);
  EXPECT_FALSE(result.empty());
}

// // Đoạn mã kiểm thử
// TEST_F(MediaScannerControllerTest, AddDataFileWithFolderDirectory_ValidFiles)
// {
//     MockMediaFileManager mockMediaFileManager;
//     MockFolderManager mockFolderManager;
//     MediaScannerController realScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);

//     std::unordered_set<std::string> mockFiles =
//     {"/home/bluebird/test_folder_media/drum.mp3",
//     "/home/bluebird/test_folder_media/take.mp4"};

//     EXPECT_CALL(mockFolderManager,
//     getListPathDirectory("/home/bluebird/test_folder_media"))
//         .WillOnce(Return(mockFiles));

//     EXPECT_CALL(mockMediaFileManager,
//     addMediaFile("/home/bluebird/test_folder_media/drum.mp3", "Audio"))
//         .WillOnce(Return(true));

//     EXPECT_CALL(mockMediaFileManager,
//     addMediaFile("/home/bluebird/test_folder_media/take.mp4", "Video"))
//         .WillOnce(Return(true));

//     realScanner.addDataFileWithFolder("/home/bluebird/test_folder_media",
//     "Directory");
// }

// TEST_F(MediaScannerControllerTest, AddDataFileWithFolderUSB_ValidFiles) {
//     MockMediaFileManager mockMediaFileManager;
//     MockFolderManager mockFolderManager;
//     MediaScannerController realScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);

//     std::unordered_set<std::string> mockFiles =
//     {"/media/bluebird/SD_4GB/file/creepy.mp3",
//     "/media/bluebird/SD_4GB/file/yorushika.mp4"};

//     EXPECT_CALL(mockFolderManager,
//     getListPathUSB("/media/bluebird/SD_4GB/file"))
//         .WillOnce(Return(mockFiles));

//     EXPECT_CALL(mockMediaFileManager,
//     addMediaFile("/media/bluebird/SD_4GB/file/creepy.mp3", "Audio"))
//         .WillOnce(Return(true));

//     EXPECT_CALL(mockMediaFileManager,
//     addMediaFile("/media/bluebird/SD_4GB/file/yorushika.mp4", "Video"))
//         .WillOnce(Return(true));

//     realScanner.addDataFileWithFolder("/media/bluebird/SD_4GB/file", "USB");
// }

// TEST_F(MediaScannerControllerTest, AddDataFileWithFolder_InvalidFileType) {
//     MockMediaFileManager mockMediaFileManager;
//     MockFolderManager mockFolderManager;
//     MediaScannerController realScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);

//     std::unordered_set<std::string> mockFiles = {
//         "/home/bluebird/test_folder_media/document.txt",  // Không phải .mp3
//         hoặc .mp4
//         "/home/bluebird/test_folder_media/another_file.pdf" // Không phải
//         .mp3 hoặc .mp4
//     };

//     EXPECT_CALL(mockFolderManager,
//     getListPathDirectory("/home/bluebird/test_folder_media"))
//         .WillOnce(Return(mockFiles));

//     // Không kỳ vọng `addMediaFile` sẽ được gọi vì các tệp không hợp lệ
//     EXPECT_CALL(mockMediaFileManager, addMediaFile(_, _)).Times(0);

//     // Cbluebirdển hướng `std::cerr` để kiểm tra thông báo lỗi
//     testing::internal::CaptureStderr();

//     realScanner.addDataFileWithFolder("/home/bluebird/test_folder_media",
//     "Directory");

//     // Kiểm tra xem có thông báo lỗi đã được in ra không
//     std::string output = testing::internal::GetCapturedStderr();
//     EXPECT_TRUE(output.find("Unsupported file type: document.txt") !=
//     std::string::npos); EXPECT_TRUE(output.find("Unsupported file type:
//     another_file.pdf") != std::string::npos);
// }

// TEST_F(MediaScannerControllerTest, LoadData_Success) {
//     MediaScannerController realScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);

//     // Giả lập file video và audio
//     std::vector<std::string> mockVideoFiles = {
//         "/home/bluebird/test_folder_media/yorushika.mp4",
//         "/home/bluebird/test_folder_media/take.mp4",
//     };
//     std::vector<std::string> mockAudioFiles = {
//         "/home/bluebird/test_folder_media/drum.mp3",
//         "/home/bluebird/test_folder_media/creepy.mp3",
//     };

//     // Mock fileExists() trả về true cho các file này
//     EXPECT_CALL(*mockScanner, fileExists)
//     .WillRepeatedly([](const std::string& path) {
//         return path == "/home/bluebird/test_folder_media/yorushika.mp4" ||
//                path == "/home/bluebird/test_folder_media/take.mp4" ||
//                path == "/home/bluebird/test_folder_media/drum.mp3" ||
//                path == "/home/bluebird/test_folder_media/creepy.mp3";
//     });

//     // Mock loadMediaFile() cho video
//     EXPECT_CALL(mockMediaFileManager,
//     loadMediaFile("/home/bluebird/test_folder_media/yorushika.mp4",
//     "Video")).Times(1); EXPECT_CALL(mockMediaFileManager,
//     loadMediaFile("/home/bluebird/test_folder_media/take.mp4",
//     "Video")).Times(1);
//     // Mock loadMediaFile() cho audio
//     EXPECT_CALL(mockMediaFileManager,
//     loadMediaFile("/home/bluebird/test_folder_media/drum.mp3",
//     "Audio")).Times(1); EXPECT_CALL(mockMediaFileManager,
//     loadMediaFile("/home/bluebird/test_folder_media/creepy.mp3",
//     "Audio")).Times(1);

//     realScanner.loadData();
// }

// TEST_F(MediaScannerControllerTest, LoadMediaPlaylist_Success) {
//     MediaScannerController realScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);

//     std::string playlistDir = "database/playlist";
//     std::vector<std::string> mockPlaylists = {
//         "database/playlist/play1.playlist",
//     };

//     auto mediaFile1 = std::make_shared<MediaFile>("drum.mp3",
//     "/home/bluebird/drum.mp3", 5000, "2024-01-01", "3m 45s", "Audio"); auto
//     mediaFile2 = std::make_shared<MediaFile>("take.mp4",
//     "/home/bluebird/take.mp4", 10000, "2024-01-02", "10m 30s", "Video");

//     std::vector<std::shared_ptr<MediaFile>> mockMediaFiles = {mediaFile1,
//     mediaFile2};

//     EXPECT_CALL(mockMediaFileManager, getAllMediaFile())
//         .WillOnce(Return(mockMediaFiles));

//     EXPECT_CALL(mockPlaylistManager,
//     loadPlaylist(testing::_)).Times(mockPlaylists.size());

//     realScanner.loadMediaPlaylist();
// }

// // ✅ **TEST 1: LoadFolder thành công**
// TEST_F(MediaScannerControllerTest, LoadFolder_Success) {
//     MediaScannerController realScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);

//     std::string directoryPath = "/home/bluebird/test_folder_media";
//     std::string usbPath = "/media/bluebird/SD_4GB/file";

//     std::unordered_set<std::string> mockMediaFilesDirectory = {
//         "/home/bluebird/test_folder_media/yorushika.mp4",
//         "/home/bluebird/test_folder_media/take.mp4",
//         "/home/bluebird/test_folder_media/creepy.mp3",
//         "/home/bluebird/test_folder_media/drum.mp3"
//     };

//     std::unordered_set<std::string> mockMediaFilesUSB = {
//         "/media/bluebird/SD_4GB/file/creepy.mp3",
//         "/media/bluebird/SD_4GB/file/didgerido.mp3",
//         "/media/bluebird/SD_4GB/file/drum.mp3",
//         "/media/bluebird/SD_4GB/file/yorushika.mp4"
//     };

//     // ✅ Kiểm tra addDataFolderDirectory() và addDataFolderUSB() được gọi
//     đúng EXPECT_CALL(mockFolderManager, addDataFolderDirectory(directoryPath,
//     mockMediaFilesDirectory))
//         .Times(1);

//     EXPECT_CALL(mockFolderManager, addDataFolderUSB(usbPath,
//     mockMediaFilesUSB))
//         .Times(1);

//     // ✅ Tạo file giả lập để đảm bảo loadFolder() có thể mở file
//     std::ofstream dirFile("database/listFolderDirectory.data");
//     dirFile << directoryPath << std::endl;
//     dirFile.close();

//     std::ofstream usbFile("database/listFolderUSB.data");
//     usbFile << usbPath << std::endl;
//     usbFile.close();

//     // ✅ Gọi loadFolder()
//     realScanner.loadFolder();
// }

// // ✅ **TEST 2: Ném ngoại lệ khi không mở được `listFolderDirectory.data`**
// TEST_F(MediaScannerControllerTest,
// LoadFolder_ThrowsException_WhenListFolderDirectoryDataNotFound) {
//     MediaScannerController realScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);

//     // Xóa file để giả lập lỗi mở file
//     std::remove("database/listFolderDirectory.data");

//     // ✅ Redirect std::cerr để kiểm tra thông báo lỗi
//     std::stringstream buffer;
//     std::streambuf* oldCerr = std::cerr.rdbuf(buffer.rdbuf());

//     // ✅ Gọi loadFolder(), ngoại lệ sẽ bị bắt trong catch
//     realScanner.loadFolder();

//     // ✅ Khôi phục std::cerr
//     std::cerr.rdbuf(oldCerr);

//     // ✅ Kiểm tra thông báo lỗi xuất hiện đúng
//     std::string expectedError = "Error in loadFolder: Could not open
//     listFolderDirectory.data\n"; std::string output = buffer.str();
//     EXPECT_NE(output.find(expectedError), std::string::npos);
// }

// // ✅ **TEST 3: LoadFolder - Bỏ qua dòng rỗng**
// TEST_F(MediaScannerControllerTest, LoadFolder_IgnoresEmptyLines) {
//     MediaScannerController realScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);

//     std::string directoryPath = "/home/bluebird/test_folder_media";

//     // ✅ Giả lập file `listFolderDirectory.data` có dòng rỗng
//     std::ofstream dirFile("database/listFolderDirectory.data");
//     dirFile << "\n";  // Dòng rỗng
//     dirFile << directoryPath << std::endl;  // Dòng hợp lệ
//     dirFile.close();

//     // ✅ `addDataFolderDirectory` chỉ nên được gọi với `directoryPath`
//     EXPECT_CALL(mockFolderManager, addDataFolderDirectory("", _))
//         .Times(0);  // Không bao giờ gọi với dòng rỗng

//     EXPECT_CALL(mockFolderManager, addDataFolderDirectory(directoryPath, _))
//         .Times(1);  // Chỉ gọi với dòng hợp lệ

//     // ✅ Gọi loadFolder()
//     realScanner.loadFolder();
// }

// // ✅ **TEST: LoadFolder - Ném ngoại lệ khi không mở được listFolderUSB.data**
// TEST_F(MediaScannerControllerTest,
// LoadFolder_ThrowsException_WhenListFolderUSBDataNotFound) {
//     MediaScannerController realScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);

//     // ✅ Xóa file để giả lập lỗi mở file
//     std::remove("database/listFolderUSB.data");

//     // ✅ Redirect std::cerr để kiểm tra thông báo lỗi
//     std::stringstream buffer;
//     std::streambuf* oldCerr = std::cerr.rdbuf(buffer.rdbuf());

//     // ✅ Gọi loadFolder(), ngoại lệ sẽ bị bắt trong catch
//     realScanner.loadFolder();

//     // ✅ Khôi phục std::cerr
//     std::cerr.rdbuf(oldCerr);

//     // ✅ Kiểm tra thông báo lỗi xuất hiện đúng
//     std::string expectedError = "Error in loadFolder: Could not open
//     listFolderUSB.data\n"; std::string output = buffer.str();
//     EXPECT_NE(output.find(expectedError), std::string::npos);
// }

// TEST_F(MediaScannerControllerTest, LoadFolder_IgnoresEmptyLinesInUSBList) {
//     MediaScannerController realScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);

//     std::string usbPath = "/media/bluebird/SD_4GB/file";

//     // ✅ Giả lập file `listFolderUSB.data` có dòng rỗng
//     std::ofstream usbFile("database/listFolderUSB.data");
//     usbFile << "\n";  // Dòng rỗng
//     usbFile << usbPath << std::endl;  // Dòng hợp lệ
//     usbFile.close();

//     // ✅ `addDataFolderUSB` chỉ nên được gọi với `usbPath`
//     EXPECT_CALL(mockFolderManager, addDataFolderUSB("", _))
//         .Times(0);  // Không bao giờ gọi với dòng rỗng

//     EXPECT_CALL(mockFolderManager, addDataFolderUSB(usbPath, _))
//         .Times(1);  // Chỉ gọi với dòng hợp lệ

//     // ✅ Gọi loadFolder()
//     realScanner.loadFolder();
// }

// TEST_F(MediaScannerControllerTest, ScanHomeDirectory_Success) {
//     MediaScannerController realScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView); std::string homePath =
//     "/home/bluebird"; std::vector<std::string> fullMockFolders = {
//         "/home/bluebird/test_list_media_file",
//         "/home/bluebird/mock_project_app_music_video_player",
//         "/home/bluebird/.config",
//         "/home/bluebird/Desktop",
//         "/home/bluebird/Pictures",
//         "/home/bluebird/.cache",
//         "/home/bluebird/.thunderbird",
//         "/home/bluebird/test_folder_media",
//         "/home/bluebird/snap",
//         "/home/bluebird/Music",
//         "/home/bluebird/workspace.kds",
//         "/home/bluebird/video_audio",
//         "/home/bluebird/Documents",
//         "/home/bluebird/.vscode",
//         "/home/bluebird/mcuxpresso",
//         "/home/bluebird/.swt",
//         "/home/bluebird/unit_test_scan",
//         "/home/bluebird/.local",
//         "/home/bluebird/Processor Expert",
//         "/home/bluebird/.mozilla",
//         "/home/bluebird/play_music_video_APP",
//         "/home/bluebird/.dotnet",
//         "/home/bluebird/.p2",
//         "/home/bluebird/.eclipse",
//         "/home/bluebird/.java",
//         "/home/bluebird/.nxp",
//         "/home/bluebird/.pki",
//         "/home/bluebird/.SEGGER",
//         "/home/bluebird/Public",
//         "/home/bluebird/Videos",
//         "/home/bluebird/Templates",
//         "/home/bluebird/.fontconfig",
//         "/home/bluebird/Backup",
//         "/home/bluebird/MyComponents",
//         "/home/bluebird/Downloads"
//     };
//     std::string selectedFolder = "/home/bluebird/test_list_media_file";
//     std::vector<std::string> mockMediaFiles = {
//         "/home/bluebird/test_list_media_file/creepy.mp3",
//         "/home/bluebird/test_list_media_file/drum.mp3"
//     };

//     EXPECT_CALL(*mockView, showListFolder(fullMockFolders))
//         .WillOnce(Return(7));

//     EXPECT_CALL(mockFolderManager, getFolderDirectory(selectedFolder))
//         .WillOnce(Return("Folder found"));

//     EXPECT_CALL(mockFolderManager, updateFolderDirectory(selectedFolder))
//         .Times(1);

//     std::unordered_set<std::string> mockFiles =
//     {"/home/bluebird/test_list_media_file/creepy.mp3",
//     "/home/bluebird/test_list_media_file/drum.mp3"};

//     EXPECT_CALL(mockFolderManager,
//     getListPathDirectory("/home/bluebird/test_list_media_file"))
//         .WillOnce(Return(mockFiles));

//     EXPECT_CALL(mockFolderManager, addDataFolderDirectory(selectedFolder,
//     testing::_))
//         .Times(1);

//     EXPECT_CALL(mockMediaFileManager,
//     addMediaFile("/home/bluebird/test_list_media_file/drum.mp3", "Audio"))
//         .WillOnce(Return(true));

//     EXPECT_CALL(mockMediaFileManager,
//     addMediaFile("/home/bluebird/test_list_media_file/creepy.mp3", "Audio"))
//         .WillOnce(Return(true));

//     realScanner.scanHomeDirectory();
// }

// TEST_F(MediaScannerControllerTest, ScanHomeDirectory_SaveFolderDirectory) {
//     MediaScannerController realScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView); std::string homePath =
//     "/home/bluebird"; std::vector<std::string> fullMockFolders = {
//         "/home/bluebird/test_list_media_file",
//         "/home/bluebird/mock_project_app_music_video_player",
//         "/home/bluebird/.config",
//         "/home/bluebird/Desktop",
//         "/home/bluebird/Pictures",
//         "/home/bluebird/.cache",
//         "/home/bluebird/.thunderbird",
//         "/home/bluebird/test_folder_media",
//         "/home/bluebird/snap",
//         "/home/bluebird/Music",
//         "/home/bluebird/workspace.kds",
//         "/home/bluebird/video_audio",
//         "/home/bluebird/Documents",
//         "/home/bluebird/.vscode",
//         "/home/bluebird/mcuxpresso",
//         "/home/bluebird/.swt",
//         "/home/bluebird/unit_test_scan",
//         "/home/bluebird/.local",
//         "/home/bluebird/Processor Expert",
//         "/home/bluebird/.mozilla",
//         "/home/bluebird/play_music_video_APP",
//         "/home/bluebird/.dotnet",
//         "/home/bluebird/.p2",
//         "/home/bluebird/.eclipse",
//         "/home/bluebird/.java",
//         "/home/bluebird/.nxp",
//         "/home/bluebird/.pki",
//         "/home/bluebird/.SEGGER",
//         "/home/bluebird/Public",
//         "/home/bluebird/Videos",
//         "/home/bluebird/Templates",
//         "/home/bluebird/.fontconfig",
//         "/home/bluebird/Backup",
//         "/home/bluebird/MyComponents",
//         "/home/bluebird/Downloads"
//     };
//     std::string selectedFolder = "/home/bluebird/test_list_media_file";
//     std::vector<std::string> mockMediaFiles = {
//         "/home/bluebird/test_list_media_file/creepy.mp3",
//         "/home/bluebird/test_list_media_file/drum.mp3"
//     };

//     EXPECT_CALL(*mockView, showListFolder(fullMockFolders))
//         .WillOnce(Return(7));

//     EXPECT_CALL(mockFolderManager, getFolderDirectory(selectedFolder))
//         .WillOnce(Return("Folder not found."));

//     EXPECT_CALL(mockFolderManager, saveFolderDirectory(selectedFolder))
//         .Times(1);

//     std::unordered_set<std::string> mockFiles =
//     {"/home/bluebird/test_list_media_file/creepy.mp3",
//     "/home/bluebird/test_list_media_file/drum.mp3"};

//     EXPECT_CALL(mockFolderManager,
//     getListPathDirectory("/home/bluebird/test_list_media_file"))
//         .WillOnce(Return(mockFiles));

//     EXPECT_CALL(mockFolderManager, addDataFolderDirectory(selectedFolder,
//     testing::_))
//         .Times(1);

//     EXPECT_CALL(mockMediaFileManager,
//     addMediaFile("/home/bluebird/test_list_media_file/drum.mp3", "Audio"))
//         .WillOnce(Return(true));

//     EXPECT_CALL(mockMediaFileManager,
//     addMediaFile("/home/bluebird/test_list_media_file/creepy.mp3", "Audio"))
//         .WillOnce(Return(true));

//     realScanner.scanHomeDirectory();
// }

// TEST_F(MediaScannerControllerTest, ScanUSBDevices_Success) {
//     MediaScannerController realScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);

//     std::string usbBasePath = "/media/bluebird";
//     std::vector<std::string> mockUSBDevices = {
//      "/media/bluebird/CDROM",
//      "/media/bluebird/Ubuntu 22.04.5 LTS amd64",
//      "/media/bluebird/SD_4GB"
//     };
//     std::vector<std::string> mockFolders = {
//         "/media/bluebird/SD_4GB/file",
//         "/media/bluebird/SD_4GB/file_test",
//         "/media/bluebird/SD_4GB/.Trash-1000",
//         "/media/bluebird/SD_4GB/System Volume Information"
//     };
//     std::string selectedUSB = "/media/bluebird/SD_4GB";
//     std::string selectedFolder = "/media/bluebird/SD_4GB/file_test";

//     std::vector<std::string> mockMediaFiles = {
//         "/media/bluebird/SD_4GB/file_test/creepy.mp3",
//         "/media/bluebird/SD_4GB/file_test/take.mp4"
//     };

//     EXPECT_CALL(*mockView, showListUSBName(mockUSBDevices))
//         .WillOnce(Return(2));

//     EXPECT_CALL(*mockView, showListFolder(mockFolders))
//         .WillOnce(Return(1));

//     EXPECT_CALL(mockFolderManager, getFolderUSB(selectedFolder))
//         .WillOnce(Return("Folder found"));

//     EXPECT_CALL(mockFolderManager, updateFolderUSB(selectedFolder))
//         .Times(1);

//     std::unordered_set<std::string> mockFiles =
//     {"/media/bluebird/SD_4GB/file_test/creepy.mp3",
//     "/media/bluebird/SD_4GB/file_test/take.mp4"};

//     EXPECT_CALL(mockFolderManager,
//     getListPathUSB("/media/bluebird/SD_4GB/file_test"))
//         .WillOnce(Return(mockFiles));

//     EXPECT_CALL(mockFolderManager, addDataFolderUSB(selectedFolder,
//     testing::_))
//         .Times(1);

//     EXPECT_CALL(mockMediaFileManager,
//     addMediaFile("/media/bluebird/SD_4GB/file_test/creepy.mp3", "Audio"))
//         .WillOnce(Return(true));

//     EXPECT_CALL(mockMediaFileManager,
//     addMediaFile("/media/bluebird/SD_4GB/file_test/take.mp4", "Video"))
//         .WillOnce(Return(true));

//     realScanner.scanUSBDevices();
// }

// TEST_F(MediaScannerControllerTest, ScanUSBDevices_SaveFolderUSB) {
//     MediaScannerController realScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);

//     std::string usbBasePath = "/media/bluebird";
//     std::vector<std::string> mockUSBDevices = {
//      "/media/bluebird/CDROM",
//      "/media/bluebird/SD_4GB"
//     };
//     std::vector<std::string> mockFolders = {
//         "/media/bluebird/SD_4GB/file",
//         "/media/bluebird/SD_4GB/file_test",
//         "/media/bluebird/SD_4GB/.Trash-1000",
//         "/media/bluebird/SD_4GB/System Volume Information"
//     };
//     std::string selectedUSB = "/media/bluebird/SD_4GB";
//     std::string selectedFolder = "/media/bluebird/SD_4GB/file_test";

//     std::vector<std::string> mockMediaFiles = {
//         "/media/bluebird/SD_4GB/file_test/creepy.mp3",
//         "/media/bluebird/SD_4GB/file_test/take.mp4"
//     };

//     EXPECT_CALL(*mockView, showListUSBName(mockUSBDevices))
//         .WillOnce(Return(2));

//     EXPECT_CALL(*mockView, showListFolder(mockFolders))
//         .WillOnce(Return(1));

//     EXPECT_CALL(mockFolderManager, getFolderUSB(selectedFolder))
//         .WillOnce(Return("Folder not found."));

//     EXPECT_CALL(mockFolderManager, saveFolderUSB(selectedFolder))
//         .Times(1);

//     std::unordered_set<std::string> mockFiles =
//     {"/media/bluebird/SD_4GB/file_test/creepy.mp3",
//     "/media/bluebird/SD_4GB/file_test/take.mp4"};

//     EXPECT_CALL(mockFolderManager,
//     getListPathUSB("/media/bluebird/SD_4GB/file_test"))
//         .WillOnce(Return(mockFiles));

//     EXPECT_CALL(mockFolderManager, addDataFolderUSB(selectedFolder,
//     testing::_))
//         .Times(1);

//     EXPECT_CALL(mockMediaFileManager,
//     addMediaFile("/media/bluebird/SD_4GB/file_test/creepy.mp3", "Audio"))
//         .WillOnce(Return(true));

//     EXPECT_CALL(mockMediaFileManager,
//     addMediaFile("/media/bluebird/SD_4GB/file_test/take.mp4", "Video"))
//         .WillOnce(Return(true));

//     realScanner.scanUSBDevices();
// }

// TEST_F(MediaScannerControllerTest, GetListPaths_EmptyInitially) {
//     MediaScannerController realScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);

//     std::unordered_set<std::string> result = realScanner.getListPaths();

//     EXPECT_TRUE(result.empty()) << "Expected listPaths to be initially
//     empty.";
// }

// TEST_F(MediaScannerControllerTest, HandleScan_LoadsDataWhenFoldersNotEmpty) {
//     MockMediaScannerController mockScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);
//     std::unordered_set<std::string> mockFolderSet = {"folder1"};

//     EXPECT_CALL(mockFolderManager, getListFolderDirectory())
//         .WillOnce(Return(mockFolderSet));

//     EXPECT_CALL(mockFolderManager, getListFolderUSB())
//         .Times(AnyNumber());

//     EXPECT_CALL(mockScanner, loadData()).Times(1);
//     EXPECT_CALL(mockScanner, loadFolder()).Times(1);
//     EXPECT_CALL(mockScanner, loadMediaPlaylist()).Times(1);

//     mockScanner.handleScan(false);
// }

// TEST_F(MediaScannerControllerTest, HandleScan_EntersMenuLoopWhenFoldersEmpty)
// {
//     MockMediaScannerController mockScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);
//     EXPECT_CALL(mockFolderManager, getListFolderDirectory())
//         .WillOnce(Return(std::unordered_set<std::string>{}));

//     EXPECT_CALL(mockFolderManager, getListFolderUSB())
//         .WillOnce(Return(std::unordered_set<std::string>{}));

//     EXPECT_CALL(*mockView, showMenu())
//         .WillOnce(Return(SCAN_HOME_DIRECTORY))
//         .WillOnce(Return(EXIT_MENU_SCAN));

//     EXPECT_CALL(mockScanner, scanHomeDirectory()).Times(1);
//     EXPECT_CALL(mockScanner, scanUSBDevices()).Times(0);

//     mockScanner.handleScan(false);
// }

// TEST_F(MediaScannerControllerTest, HandleScan_ScansUSBWhenSelected) {
//     MockMediaScannerController mockScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);
//     EXPECT_CALL(mockFolderManager, getListFolderDirectory())
//         .WillOnce(Return(std::unordered_set<std::string>{}));

//     EXPECT_CALL(mockFolderManager, getListFolderUSB())
//         .WillOnce(Return(std::unordered_set<std::string>{}));

//     EXPECT_CALL(*mockView, showMenu())
//         .WillOnce(Return(SCAN_USB))
//         .WillOnce(Return(EXIT_MENU_SCAN));

//     EXPECT_CALL(mockScanner, scanUSBDevices()).Times(1);
//     EXPECT_CALL(mockScanner, scanHomeDirectory()).Times(0);

//     mockScanner.handleScan(false);
// }

// TEST_F(MediaScannerControllerTest, HandleScan_ExitsOnExitCommand) {
//     MockMediaScannerController mockScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);
//     EXPECT_CALL(mockFolderManager, getListFolderDirectory())
//         .WillOnce(Return(std::unordered_set<std::string>{}));

//     EXPECT_CALL(mockFolderManager, getListFolderUSB())
//         .WillOnce(Return(std::unordered_set<std::string>{}));

//     EXPECT_CALL(*mockView, showMenu())
//         .WillOnce(Return(EXIT_MENU_SCAN));

//     EXPECT_CALL(mockScanner, scanHomeDirectory()).Times(0);
//     EXPECT_CALL(mockScanner, scanUSBDevices()).Times(0);

//     mockScanner.handleScan(false);
// }

// TEST_F(MediaScannerControllerTest, HandleScan_CatchesInvalidChoiceException)
// {
//     MockMediaScannerController mockScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);
//     EXPECT_CALL(mockFolderManager, getListFolderDirectory())
//         .WillOnce(Return(std::unordered_set<std::string>{}));

//     EXPECT_CALL(mockFolderManager, getListFolderUSB())
//         .WillOnce(Return(std::unordered_set<std::string>{}));

//     EXPECT_CALL(*mockView, showMenu())
//         .WillOnce(Return(-99))
//         .WillOnce(Return(EXIT_MENU_SCAN));

//     EXPECT_CALL(mockScanner, scanHomeDirectory()).Times(0);
//     EXPECT_CALL(mockScanner, scanUSBDevices()).Times(0);
//     mockScanner.handleScan(false);
// }
// TEST_F(MediaScannerControllerTest, HandleScan_FolderAndUSBExist) {
//     MockMediaScannerController mockScanner(mockMediaFileManager,
//     mockPlaylistManager, mockFolderManager, mockView);

//     EXPECT_CALL(mockFolderManager, getListFolderDirectory()) // ✅ Mock trả về
//     rỗng
//         .WillOnce(Return(std::unordered_set<std::string>{}));

//     EXPECT_CALL(mockFolderManager, getListFolderUSB())  // ✅ Giờ sẽ được gọi
//         .WillOnce(Return(std::unordered_set<std::string>{"/media/bluebird/SD_4GB/file"}));

//     EXPECT_CALL(mockScanner, loadData()).Times(1);
//     EXPECT_CALL(mockScanner, loadFolder()).Times(1);
//     EXPECT_CALL(mockScanner, loadMediaPlaylist()).Times(1);

//     mockScanner.handleScan(false);
// }
