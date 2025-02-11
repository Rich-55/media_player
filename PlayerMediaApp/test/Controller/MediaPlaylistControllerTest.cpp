#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../include/Controller/MediaPlaylistController.h"

using ::testing::Return;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::StrictMock;

using ::testing::Invoke;
using ::testing::MatchesRegex;
using ::testing::AtLeast;


class MockPlaylistHandlerView : public BaseView {
public:
    MOCK_METHOD(int, showMenu, (), (override));
    MOCK_METHOD(int, showMenuWithMediaListInPlaylist, (std::shared_ptr<Playlist> playlist), (override));
    MOCK_METHOD(void, showNotificationMessage, (std::string message, std::string type), (override));
    MOCK_METHOD(bool, showConfirmMenu, (std::string message), (override));
    MOCK_METHOD(std::string, displayAllMediaFileInPlaylist, (std::shared_ptr<Playlist> playlist), (override));
    MOCK_METHOD(std::string, displayAllMediaFile, (MediaFileManager mediaFileManager), (override));
    MOCK_METHOD(std::string, showMenuCreatePlaylist, (), (override));
    MOCK_METHOD((std::pair<std::string, std::string>), displayAllFolder,
            ((std::pair<std::unordered_set<std::string>, std::unordered_set<std::string>>)), (override));
};

class MockMediaFileManager : public MediaFileManager {
public:
    MOCK_METHOD(std::shared_ptr<MediaFile>, getMediaFile, (std::string), (override));
    MOCK_METHOD(std::shared_ptr<MediaFile>, getMediaFileByPath, (std::string path), (override));
    MOCK_METHOD(bool, addMediaFile, (std::string pathName, std::string type), (override));
    
};

class MockFolderManager : public FolderManager {
public:
    MOCK_METHOD(std::unordered_set<std::string>, getListFolderDirectory, (), (override));
    MOCK_METHOD(std::unordered_set<std::string>, getListFolderUSB, (), (override));
    MOCK_METHOD(std::unordered_set<std::string>, getListPathDirectory, (std::string folderName), (override));
    MOCK_METHOD(std::unordered_set<std::string>, getListPathUSB, (std::string folderName), (override));
};

class MockPlaylist : public Playlist {
public:
    MOCK_METHOD(void, addMediaFile, (std::shared_ptr<MediaFile> mediaFile), (override));
    MOCK_METHOD(bool, checkMediaFile, (std::string fileName), (override));
    MOCK_METHOD(void, deleteMediaFile, (std::string fileName), (override));
    MOCK_METHOD(std::vector<std::string>, getListPathMediaFiles, (), (override));
    MOCK_METHOD(void, setPlaylistName, (std::string name), (override));
    
    // Constructor giả lập
    MockPlaylist() : Playlist("Mock Playlist") {}
};

class MediaPlaylistControllerTest : public ::testing::Test {
protected:
    StrictMock<MockMediaFileManager> mockMediaFileManager;

    NiceMock<MockFolderManager> mockFolderManager;
    std::shared_ptr<MockPlaylist> mockPlaylist;
    std::shared_ptr<NiceMock<MockPlaylistHandlerView>> mockView;
    std::unique_ptr<MediaPlaylistController> controller;

    void SetUp() override {
        mockPlaylist = std::make_shared<MockPlaylist>();
        mockView = std::make_shared<NiceMock<MockPlaylistHandlerView>>();
        
        controller = std::make_unique<MediaPlaylistController>(
            mockMediaFileManager, mockFolderManager, mockPlaylist, mockView, mockView);


        testing::Mock::AllowLeak(mockView.get());

    }

    void TearDown() override {
        testing::Mock::VerifyAndClearExpectations(mockView.get());
    }
};

TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_AddFileToPlaylist) {

    EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
        .WillOnce(Return(ADD_MEDIA_FILE_TO_PLAYLIST))
        .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER));

    std::string fileName = "drum.mp3";
    std::string filePath = "/home/bluebird/test_folder_media/drum.mp3";

    EXPECT_CALL(*mockView, displayAllMediaFile(_))
        .WillOnce(Return(fileName));

    auto mediaFile = std::make_shared<MediaFile>(
        fileName, filePath, 1024, "2024-01-01", "03:45", "Audio");

    EXPECT_CALL(mockMediaFileManager, getMediaFile(fileName))
        .WillOnce(Return(mediaFile));
    
    EXPECT_CALL(*mockPlaylist, checkMediaFile(fileName))
        .WillOnce(Return(false));
    
    EXPECT_CALL(*mockPlaylist, addMediaFile(mediaFile))
        .Times(1);

    EXPECT_CALL(*mockView, showNotificationMessage("File " + fileName + " has been added to the playlist.", "success"));
    
    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerPlaylist();
    std::cin.rdbuf(orig);
}

TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_AddFileToPlaylistAlready) {

    EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
        .WillOnce(Return(ADD_MEDIA_FILE_TO_PLAYLIST))
        .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER));

    std::string fileName = "drum.mp3";
    std::string filePath = "/home/bluebird/test_folder_media/drum.mp3";

    EXPECT_CALL(*mockView, displayAllMediaFile(_))
        .WillOnce(Return(fileName));

    auto mediaFile = std::make_shared<MediaFile>(
        fileName, filePath, 1024, "2024-01-01", "03:45", "Audio");

    EXPECT_CALL(mockMediaFileManager, getMediaFile(fileName))
        .WillOnce(Return(mediaFile));
    
    EXPECT_CALL(*mockPlaylist, checkMediaFile(fileName))
        .WillOnce(Return(true));

    EXPECT_CALL(*mockView, showNotificationMessage("File " + fileName + " is already in the playlist.", "error"));

    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerPlaylist();
    std::cin.rdbuf(orig);
}

TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_DeleteFile_Exit) {
    EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
        .WillOnce(Return(ADD_MEDIA_FILE_TO_PLAYLIST))
        .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER));

    EXPECT_CALL(*mockView, displayAllMediaFile(_))
        .WillOnce(Return("exit")); 

    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("0\n"); 
    std::cin.rdbuf(input.rdbuf());

    controller->handlerPlaylist();
    std::cin.rdbuf(orig);
}

TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_AddFileToPlaylistByPath_Directory) {
    EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
        .WillOnce(Return(ADD_MEDIA_FILE_TO_PLAYLIST_BY_PATH)) 
        .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER));

    EXPECT_CALL(*mockView, displayAllFolder(_))
        .WillOnce(Return(std::make_pair("Directory", "/home/bluebird/test_folder_media")));  

    std::unordered_set<std::string> mediaPaths = {
        "/home/bluebird/test_folder_media/creepy.mp3",
        "/home/bluebird/test_folder_media/drum.mp3"
    };

    EXPECT_CALL(mockFolderManager, getListPathDirectory(_))
        .WillOnce(Return(std::unordered_set<std::string>{
            "/home/bluebird/test_folder_media/drum.mp3",
            "/home/bluebird/test_folder_media/creepy.mp3"
        }));

    std::shared_ptr<MediaFile> drumFile = std::make_shared<MediaFile>("drum.mp3", "/home/bluebird/test_folder_media/drum.mp3", 1024, "2024-01-01", "03:45", "Audio");
    std::shared_ptr<MediaFile> creepyFile = std::make_shared<MediaFile>("creepy.mp3", "/home/bluebird/test_folder_media/creepy.mp3", 2048, "2024-01-01", "04:00", "Audio");
    
    EXPECT_CALL(mockMediaFileManager, getMediaFileByPath(_))
        .Times(AtLeast(1))  
        .WillRepeatedly([drumFile, creepyFile](const std::string& path) -> std::shared_ptr<MediaFile> {
            if (path == "/home/bluebird/test_folder_media/drum.mp3") {
                return drumFile;
            }
            if (path == "/home/bluebird/test_folder_media/creepy.mp3") {
                return creepyFile;
            }
            return nullptr;
        });

    EXPECT_CALL(*mockPlaylist, checkMediaFile("creepy.mp3"))
        .WillOnce(Return(false));  

    EXPECT_CALL(*mockPlaylist, addMediaFile(creepyFile)).Times(1);

    EXPECT_CALL(*mockPlaylist, checkMediaFile("drum.mp3"))
        .WillOnce(Return(false));  

    EXPECT_CALL(*mockPlaylist, addMediaFile(drumFile)).Times(1);


    EXPECT_CALL(*mockView, showNotificationMessage("All files in the folder have been added to the playlist.", "success"));

    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerPlaylist();
    std::cin.rdbuf(orig);
}

TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_AddFileToPlaylistByPath_ChoiceEmpty) {
    EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
        .WillOnce(Return(ADD_MEDIA_FILE_TO_PLAYLIST_BY_PATH)) 
        .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER));

    EXPECT_CALL(*mockView, displayAllFolder(_))
        .WillOnce(Return(std::make_pair("", "")));  

    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerPlaylist();
    std::cin.rdbuf(orig);
}

TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_AddFileToPlaylistByPath_Directory_MediaFileNull)
{
    EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
        .WillOnce(Return(ADD_MEDIA_FILE_TO_PLAYLIST_BY_PATH)) 
        .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER));

    EXPECT_CALL(*mockView, displayAllFolder(_))
        .WillOnce(Return(std::make_pair("Directory", "/home/bluebird/test_folder_media")));  

    std::unordered_set<std::string> mediaPaths = {
        "/home/bluebird/test_folder_media/drum.mp3",
    };

    EXPECT_CALL(mockFolderManager, getListPathDirectory(_))
        .WillOnce(Return(std::unordered_set<std::string>{
            "/home/bluebird/test_folder_media/drum.mp3",
        }));

    EXPECT_CALL(mockMediaFileManager, getMediaFileByPath(_))
        .WillOnce(Return(nullptr));

    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerPlaylist();
    std::cin.rdbuf(orig);
}

TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_AddFileToPlaylistByPath_Directory_MediaFileIsAlready)
{
    EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
        .WillOnce(Return(ADD_MEDIA_FILE_TO_PLAYLIST_BY_PATH)) 
        .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER));

    EXPECT_CALL(*mockView, displayAllFolder(_))
        .WillOnce(Return(std::make_pair("Directory", "/home/bluebird/test_folder_media")));  

    std::unordered_set<std::string> mediaPaths = {
        "/home/bluebird/test_folder_media/drum.mp3",
    };

    EXPECT_CALL(mockFolderManager, getListPathDirectory(_))
        .WillOnce(Return(std::unordered_set<std::string>{
            "/home/bluebird/test_folder_media/drum.mp3",
        }));

    
    std::shared_ptr<MediaFile> drumFile = std::make_shared<MediaFile>("drum.mp3", "/home/bluebird/test_folder_media/drum.mp3", 1024, "2024-01-01", "03:45", "Audio");
    
    EXPECT_CALL(mockMediaFileManager, getMediaFileByPath(_))
        .Times(AtLeast(1))  
        .WillRepeatedly([drumFile](const std::string& path) -> std::shared_ptr<MediaFile> {
            if (path == "/home/bluebird/test_folder_media/drum.mp3") {
                return drumFile;
            }
            return nullptr;
        });

    EXPECT_CALL(*mockPlaylist, checkMediaFile("drum.mp3"))
        .WillOnce(Return(true));

    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerPlaylist();
    std::cin.rdbuf(orig);
}

// TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_AddFileToPlaylistByPath_USB) {
//     EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
//         .WillOnce(Return(ADD_MEDIA_FILE_TO_PLAYLIST_BY_PATH)) 
//         .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER));

//     EXPECT_CALL(*mockView, displayAllFolder(_))
//         .WillOnce(Return(std::make_pair("USB", "/media/bluebird/SD_4GB/file")));  

//     std::unordered_set<std::string> mediaPaths = {
//         "/media/bluebird/SD_4GB/file/creepy.mp3",
//         "/media/bluebird/SD_4GB/file/drum.mp3"
//     };

//     EXPECT_CALL(mockFolderManager, getListPathDirectory(_))
//         .WillOnce(Return(std::unordered_set<std::string>{
//             "/media/bluebird/SD_4GB/file/drum.mp3",
//             "/media/bluebird/SD_4GB/file/creepy.mp3"
//         }));

//     std::shared_ptr<MediaFile> drumFile = std::make_shared<MediaFile>("drum.mp3", "/media/bluebird/SD_4GB/file/drum.mp3", 1024, "2024-01-01", "03:45", "Audio");
//     std::shared_ptr<MediaFile> creepyFile = std::make_shared<MediaFile>("creepy.mp3", "/media/bluebird/SD_4GB/file/creepy.mp3", 2048, "2024-01-01", "04:00", "Audio");
    
//     EXPECT_CALL(mockMediaFileManager, getMediaFileByPath(_))
//         .Times(AtLeast(1))  
//         .WillRepeatedly([drumFile, creepyFile](const std::string& path) -> std::shared_ptr<MediaFile> {
//             if (path == "/media/bluebird/SD_4GB/file/drum.mp3") {
//                 return drumFile;
//             }
//             if (path == "/media/bluebird/SD_4GB/file/creepy.mp3") {
//                 return creepyFile;
//             }
//             return nullptr;
//         });

//     EXPECT_CALL(*mockPlaylist, checkMediaFile("creepy.mp3"))
//         .WillOnce(Return(false));  

//     EXPECT_CALL(*mockPlaylist, addMediaFile(creepyFile)).Times(1);

//     EXPECT_CALL(*mockPlaylist, checkMediaFile("drum.mp3"))
//         .WillOnce(Return(false));  

//     EXPECT_CALL(*mockPlaylist, addMediaFile(drumFile)).Times(1);


//     EXPECT_CALL(*mockView, showNotificationMessage("All files in the folder have been added to the playlist.", "success"));

//     std::streambuf* orig = std::cin.rdbuf();
//     std::istringstream input("0\n");
//     std::cin.rdbuf(input.rdbuf());

//     controller->handlerPlaylist();
//     std::cin.rdbuf(orig);
// }


// TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_AddFileToPlaylistByPath_USB_MediaFileNull)
// {
//     EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
//         .WillOnce(Return(ADD_MEDIA_FILE_TO_PLAYLIST_BY_PATH)) 
//         .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER));

//     EXPECT_CALL(*mockView, displayAllFolder(_))
//         .WillOnce(Return(std::make_pair("USB", "/media/bluebird/SD_4GB/file")));  

//     std::unordered_set<std::string> mediaPaths = {
//         "/media/bluebird/SD_4GB/file/drum.mp3",
//     };

//     EXPECT_CALL(mockFolderManager, getListPathDirectory(_))
//         .WillOnce(Return(std::unordered_set<std::string>{
//             "/media/bluebird/SD_4GB/file/drum.mp3",
//         }));

//     EXPECT_CALL(mockMediaFileManager, getMediaFileByPath(_))
//         .WillOnce(Return(nullptr));

//     std::streambuf* orig = std::cin.rdbuf();
//     std::istringstream input("0\n");
//     std::cin.rdbuf(input.rdbuf());

//     controller->handlerPlaylist();
//     std::cin.rdbuf(orig);
// }

// TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_AddFileToPlaylistByPath_USB_MediaFileIsAlready)
// {
//     EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
//         .WillOnce(Return(ADD_MEDIA_FILE_TO_PLAYLIST_BY_PATH)) 
//         .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER));

//     EXPECT_CALL(*mockView, displayAllFolder(_))
//         .WillOnce(Return(std::make_pair("USB", "/media/bluebird/SD_4GB/file")));  

//     std::unordered_set<std::string> mediaPaths = {
//         "/media/bluebird/SD_4GB/file/drum.mp3",
//     };

//     EXPECT_CALL(mockFolderManager, getListPathDirectory(_))
//         .WillOnce(Return(std::unordered_set<std::string>{
//             "/media/bluebird/SD_4GB/file/drum.mp3",
//         }));

    
//     std::shared_ptr<MediaFile> drumFile = std::make_shared<MediaFile>("drum.mp3", "/media/bluebird/SD_4GB/file/drum.mp3", 1024, "2024-01-01", "03:45", "Audio");
    
//     EXPECT_CALL(mockMediaFileManager, getMediaFileByPath(_))
//         .Times(AtLeast(1))  
//         .WillRepeatedly([drumFile](const std::string& path) -> std::shared_ptr<MediaFile> {
//             if (path == "/media/bluebird/SD_4GB/file/drum.mp3") {
//                 return drumFile;
//             }
//             return nullptr;
//         });

//     EXPECT_CALL(*mockPlaylist, checkMediaFile("drum.mp3"))
//         .WillOnce(Return(true));

//     std::streambuf* orig = std::cin.rdbuf();
//     std::istringstream input("0\n");
//     std::cin.rdbuf(input.rdbuf());

//     controller->handlerPlaylist();
//     std::cin.rdbuf(orig);
// }

TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_DeleteFileFromPlaylist) {
    EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
        .WillOnce(Return(DELETE_MEDIA_FILE_FROM_PLAYLIST))
        .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER));

    std::string fileName = "drum.mp3";

    EXPECT_CALL(*mockView, displayAllMediaFileInPlaylist(_))
        .WillOnce(Return(fileName));

    EXPECT_CALL(*mockPlaylist, checkMediaFile(fileName))
        .WillOnce(Return(true));

    EXPECT_CALL(*mockView, showConfirmMenu("Do you want to delete this file: " + fileName + " ?"))
        .WillOnce(Return(true));
        
    EXPECT_CALL(*mockPlaylist, deleteMediaFile(fileName))
        .Times(1);

    EXPECT_CALL(*mockView, showNotificationMessage("File " + fileName + " has been deleted from the playlist.", "success"));

    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerPlaylist();
    std::cin.rdbuf(orig);
}

TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_DeleteFileFromPlaylist_FileNotFound) {
    EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
        .WillOnce(Return(DELETE_MEDIA_FILE_FROM_PLAYLIST))
        .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER));

    std::string fileName = "drum.mp3";

    EXPECT_CALL(*mockView, displayAllMediaFileInPlaylist(_))
        .WillOnce(Return(fileName));

    EXPECT_CALL(*mockPlaylist, checkMediaFile(fileName))
        .WillOnce(Return(false));
    
    EXPECT_CALL(*mockView, showNotificationMessage("File " + fileName + " not found in the playlist.", "error"));

    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerPlaylist();
    std::cin.rdbuf(orig);
}

TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_DeleteFileFromPlaylist_UserDeclines) {
    EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
        .WillOnce(Return(DELETE_MEDIA_FILE_FROM_PLAYLIST))
        .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER));

    std::string fileName = "drum.mp3";

    EXPECT_CALL(*mockView, displayAllMediaFileInPlaylist(_))
        .WillOnce(Return(fileName));

    EXPECT_CALL(*mockPlaylist, checkMediaFile(fileName))
        .WillOnce(Return(true));

    EXPECT_CALL(*mockView, showConfirmMenu("Do you want to delete this file: " + fileName + " ?"))
        .WillOnce(Return(false));

    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerPlaylist();
    std::cin.rdbuf(orig);
}

TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_DeleteFileFromPlaylist_Exit) {
    EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
        .WillOnce(Return(DELETE_MEDIA_FILE_FROM_PLAYLIST))
        .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER));

    EXPECT_CALL(*mockView, displayAllMediaFileInPlaylist(_))
        .WillOnce(Return("exit"));

    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerPlaylist();
    std::cin.rdbuf(orig);
}

TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_RenamePlaylist_Success) {
    EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
        .WillOnce(Return(RENAME_PLAYLIST))
        .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER));

    std::string newPlaylistName = "My Favorite Songs";

    EXPECT_CALL(*mockView, showMenuCreatePlaylist())
        .WillOnce(Return(newPlaylistName));

    EXPECT_CALL(*mockPlaylist, setPlaylistName(newPlaylistName))
        .Times(1);

    EXPECT_CALL(*mockView, showNotificationMessage("Playlist has been renamed to " + newPlaylistName, "success"));

    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerPlaylist();
    std::cin.rdbuf(orig);
}

TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_RenamePlaylist_Exit) {
    EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
        .WillOnce(Return(RENAME_PLAYLIST))
        .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER));

    EXPECT_CALL(*mockView, showMenuCreatePlaylist())
        .WillOnce(Return("exit"));

    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerPlaylist();
    std::cin.rdbuf(orig);
}

TEST_F(MediaPlaylistControllerTest, GetListPathMediaFiles_ReturnsCorrectList) {
    std::vector<std::string> expectedPaths = {
        "/home/bluebird/test_folder_media/song1.mp3",
        "/home/bluebird/test_folder_media/song2.mp3"
    };

    EXPECT_CALL(*mockPlaylist, getListPathMediaFiles())
        .WillOnce(Return(expectedPaths));

    std::vector<std::string> actualPaths = controller->getListPathMediaFiles();

    EXPECT_EQ(actualPaths, expectedPaths);
}

TEST_F(MediaPlaylistControllerTest, SetNamePlaylist_Success) {
    std::string newPlaylistName = "Chill Vibes";

    EXPECT_CALL(*mockPlaylist, setPlaylistName(newPlaylistName))
        .Times(1);

    controller->setNamePlaylist(newPlaylistName);
}

TEST_F(MediaPlaylistControllerTest, DisplayAllMediaFilesInPlaylist_CallsDisplayFunction) {
    EXPECT_CALL(*mockView, displayAllMediaFileInPlaylist(_))
        .Times(1);

    controller->displayAllMediaFilesInPlaylist();
}

TEST_F(MediaPlaylistControllerTest, HandleMediaPlaylist_InvalidChoiceException) {
    EXPECT_CALL(*mockView, showMenuWithMediaListInPlaylist(_))
        .WillOnce(Return(999)) 
        .WillOnce(Return(EXIT_MENU_PLAYLIST_HANDLER)); 

    testing::internal::CaptureStderr();

    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf()); // Giả lập input

    controller->handlerPlaylist(); // Gọi hàm xử lý

    std::cin.rdbuf(nullptr); // Khôi phục lại std::cin

    std::string output = testing::internal::GetCapturedStderr(); // Lấy output lỗi

    EXPECT_NE(output.find("Error:"), std::string::npos); // Kiểm tra xem có thông báo lỗi không
}
