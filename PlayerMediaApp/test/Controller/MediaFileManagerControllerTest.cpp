#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../include/Controller/MediaFileManagerController.h"

using ::testing::Return;
using ::testing::_;
using ::testing::AtLeast;
using ::testing::NiceMock;
using ::testing::Invoke;

// Mock các class liên quan
class MockMediaFileManager : public MediaFileManager {
public:
    MOCK_METHOD(bool, addMediaFile, (std::string, std::string), (override));
    MOCK_METHOD(bool, deleteMediaFile, (std::string), (override));
    MOCK_METHOD(std::shared_ptr<MediaFile>, getMediaFile, (std::string), (override));
};

class MockBaseView : public BaseView {
public:
    MOCK_METHOD(int, showMenu, (), (override));
    MOCK_METHOD(void, showNotificationMessage, (std::string, std::string), (override));
    MOCK_METHOD(bool, showConfirmMenu, (std::string), (override));
    MOCK_METHOD(std::string, displayAllMediaFile, (MediaFileManager), (override));
    MOCK_METHOD(std::string, displayAllMediaFileOfAudio, (MediaFileManager), (override));
    MOCK_METHOD(std::string, displayAllMediaFileOfVideo, (MediaFileManager), (override));
    MOCK_METHOD(int, showMenuWithMediaList, (MediaFileManager), (override)); // Thêm mock cho showMenuWithMediaList
};


class MockMediaFileController : public MediaFileController {
public:
    MOCK_METHOD(void, handlerMediaFile, (), (override));

    MockMediaFileController(std::shared_ptr<MediaFile> m, std::shared_ptr<BaseView> v)
        : MediaFileController(m, v) {}
};


class MockMediaScannerController : public MediaScannerController {
public:
    MockMediaScannerController(MediaFileManager& mediaManager, PlaylistManager& playlistManager, FolderManager& folderManager, std::shared_ptr<BaseView> view)
        : MediaScannerController(mediaManager, playlistManager, folderManager, view) {}

    MOCK_METHOD(void, handleScan, (bool), (override));
};

// Test fixture
class MediaFileManagerControllerTest : public ::testing::Test {
protected:
    NiceMock<MockMediaFileManager> mockMediaManager; 
    std::shared_ptr<NiceMock<MockBaseView>> mockView;
    std::shared_ptr<NiceMock<MockBaseView>> mockFileView;
    std::shared_ptr<NiceMock<MockMediaScannerController>> mockScannerController;
    std::shared_ptr<NiceMock<MockMediaFileController>> mockMediaFileController;
    std::unique_ptr<MediaFileManagerController> controller;
    PlaylistManager playlistManager;
    FolderManager folderManager;
    void SetUp() override {
        mockView = std::make_shared<NiceMock<MockBaseView>>();
        mockFileView = std::make_shared<NiceMock<MockBaseView>>();
        mockScannerController = std::make_shared<NiceMock<MockMediaScannerController>>(
            mockMediaManager, playlistManager, folderManager, mockView);

        controller = std::make_unique<MediaFileManagerController>(
            mockMediaManager, mockView, mockFileView, mockScannerController);

        testing::Mock::AllowLeak(mockView.get());
        testing::Mock::AllowLeak(mockFileView.get());
        testing::Mock::AllowLeak(mockScannerController.get());
    }

    void TearDown() override {
        testing::Mock::VerifyAndClearExpectations(mockView.get());
        testing::Mock::VerifyAndClearExpectations(mockFileView.get());
        testing::Mock::VerifyAndClearExpectations(mockScannerController.get());
    }

};

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_ExitMenu) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    EXPECT_NO_THROW(controller->handlerMediaFileManager());
}

// Test khi thêm file hợp lệ
TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_AddValidFileAudio) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(ADD_FILE_PATH))
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    std::string validPath = "/home/bluebird/test_folder_media/creepy.mp3";

    EXPECT_CALL(mockMediaManager, addMediaFile(validPath, "Audio")) // Sử dụng mock object trực tiếp
        .WillOnce(Return(1));

    EXPECT_CALL(*mockView, showNotificationMessage("File " + validPath + " has been added.", "success"));

    std::istringstream input(validPath + "\n0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerMediaFileManager();
}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_AddValidFileVideo) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(ADD_FILE_PATH))
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    std::string validPath = "/home/bluebird/test_folder_media/take.mp4";

    EXPECT_CALL(mockMediaManager, addMediaFile(validPath, "Video")) // Sử dụng mock object trực tiếp
        .WillOnce(Return(1));

    EXPECT_CALL(*mockView, showNotificationMessage("File " + validPath + " has been added.", "success"));

    std::istringstream input(validPath + "\n0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerMediaFileManager();
}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_AddInvalidPath) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(ADD_FILE_PATH))
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    std::string invalidPath = "invalidpath";

    EXPECT_CALL(*mockView, showNotificationMessage("Invalid file path: " + invalidPath, "error"));

    std::istringstream input(invalidPath + "\n0\n");  
    std::cin.rdbuf(input.rdbuf());  
    controller->handlerMediaFileManager();
}


// Test khi thêm file có định dạng không hỗ trợ
TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_AddUnsupportedFileType) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(ADD_FILE_PATH))
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    std::string unsupportedFile = "/home/bluebird/didgerido.mp5";

    EXPECT_CALL(*mockView, showNotificationMessage("Unsupported file type: " + unsupportedFile, "error"));

    std::istringstream input(unsupportedFile + "\n0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerMediaFileManager();
}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_AddFileAlreadyExists) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(ADD_FILE_PATH))
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    std::string filePath = "/home/bluebird/test_folder_media/drum.mp3";

    EXPECT_CALL(mockMediaManager, addMediaFile(filePath, "Audio"))
        .WillOnce(Return(0));  

    EXPECT_CALL(*mockView, showNotificationMessage("File already exists: " + filePath, "error"));

    std::istringstream input(filePath + "\n0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerMediaFileManager();
}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_AddFilePath_EmptyInput) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(ADD_FILE_PATH))
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    std::istringstream input("\n0\n");  
    std::cin.rdbuf(input.rdbuf());  

    controller->handlerMediaFileManager(); 
}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_InvalidFilePathException) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(ADD_FILE_PATH))
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    std::istringstream input("invalid_path\n0\n"); 
    std::cin.rdbuf(input.rdbuf());  

    controller->handlerMediaFileManager(); 
}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_FileNotFoundException) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(ADD_FILE_PATH))
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    std::istringstream input("/home/bluebird/test_folder_media/Filenotfound.test\n0\n"); 
    std::cin.rdbuf(input.rdbuf());  

    controller->handlerMediaFileManager(); 
}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_ReturnToMenu) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(ADD_FILE_PATH))
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerMediaFileManager();
}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_DeleteFile_Success) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(DELETE_FILE))
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    std::string fileToDelete = "drum.mp3";
    std::string filePath = "/home/bluebird/test_folder_media/drum.mp3";

    auto mediaFile = std::make_shared<MediaFile>(
        fileToDelete, filePath, 1024, "2024-01-01", "03:45", "Audio");

    EXPECT_CALL(mockMediaManager, getMediaFile(fileToDelete))
        .WillRepeatedly(Return(mediaFile));

    EXPECT_CALL(*mockView, displayAllMediaFile(_))
        .WillOnce(Return(fileToDelete));

    EXPECT_CALL(*mockView, showConfirmMenu("Do you want to delete this file: " + fileToDelete + " ?"))
        .WillOnce(Return(true));

    EXPECT_CALL(mockMediaManager, deleteMediaFile(fileToDelete))
        .WillOnce(Return(true));

    EXPECT_CALL(*mockView, showNotificationMessage("File " + fileToDelete + " has been deleted.", "success"));

    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerMediaFileManager();
}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_DeleteFile_Fail) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(DELETE_FILE))
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    std::string fileToDelete = "drum.mp3";
    std::string filePath = "/home/bluebird/test_folder_media/drum.mp3";

    auto mediaFile = std::make_shared<MediaFile>(
        fileToDelete, filePath, 1024, "2024-01-01", "03:45", "Audio");

    EXPECT_CALL(mockMediaManager, getMediaFile(_))
        .WillRepeatedly(Invoke([&](const std::string& fileName) {
            if (fileName.empty()) return std::shared_ptr<MediaFile>(nullptr);
            return mediaFile;
        }));

    EXPECT_CALL(*mockView, displayAllMediaFile(_))
        .Times(2)
        .WillOnce(Return(fileToDelete))  
        .WillOnce(Return(""));  

    EXPECT_CALL(*mockView, showConfirmMenu("Do you want to delete this file: " + fileToDelete + " ?"))
        .Times(1)
        .WillOnce(Return(false));  

    controller->handlerMediaFileManager();
}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_DeleteFile_Exit) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(DELETE_FILE))
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    EXPECT_CALL(*mockView, displayAllMediaFile(_))
        .WillOnce(Return("exit")); 

    std::istringstream input("0\n"); 
    std::cin.rdbuf(input.rdbuf());

    controller->handlerMediaFileManager();
}

// Test khi file đang được phát và không thể xóa
TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_DeleteFile_FileIsPlaying) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(DELETE_FILE))
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    std::string fileToDelete = "drum.mp3";
    std::string filePath = "/home/bluebird/test_folder_media/drum.mp3";
    std::shared_ptr<MediaFile> mediaFile = std::make_shared<MediaFile>(
        fileToDelete, filePath, 1024, "2024-01-01", "03:45", "Audio");

    PlayerController::currentPlayingFile = filePath; 

    EXPECT_CALL(mockMediaManager, getMediaFile(fileToDelete))
        .WillRepeatedly(Return(mediaFile));

    EXPECT_CALL(*mockView, displayAllMediaFile(_))
        .WillOnce(Return(fileToDelete));

    EXPECT_CALL(*mockView, showNotificationMessage("File is running, can't delete", "error"));

    std::istringstream input("0\n");  
    std::cin.rdbuf(input.rdbuf());

    controller->handlerMediaFileManager();
}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_ShowAllMediaFiles) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(SHOW_ALL_MEDIAFILE))  
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    std::string mediaList = "drum.mp3";  
    EXPECT_CALL(*mockView, displayAllMediaFile(_))
        .WillOnce(Return(mediaList));  

    std::string selectedFile = "drum.mp3";
    auto mediaFile = std::make_shared<MediaFile>(
        selectedFile, "/home/bluebird/test_folder_media/drum.mp3", 1024, "2024-01-01", "03:45", "Audio");

    EXPECT_CALL(mockMediaManager, getMediaFile(selectedFile))
        .WillOnce(Return(mediaFile));

    std::istringstream input("0\n");  
    std::cin.rdbuf(input.rdbuf());

    controller->handlerMediaFileManager();  
}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_ShowAllMediaFiles_Exit) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(SHOW_ALL_MEDIAFILE)) 
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));  

    EXPECT_CALL(*mockView, displayAllMediaFile(_))
        .WillOnce(Return("exit")); 

    std::istringstream input("0\n");  
    std::cin.rdbuf(input.rdbuf());  

    controller->handlerMediaFileManager(); 
}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_showAllMediaFileOfVideo) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(SHOW_ALL_MEDIAFILE_VIDEO))  
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    std::string mediaList = "take.mp4";  
    EXPECT_CALL(*mockView, displayAllMediaFileOfVideo(_))
        .WillOnce(Return(mediaList));  

    std::string selectedFile = "take.mp4";
    auto mediaFile = std::make_shared<MediaFile>(
        selectedFile, "/home/bluebird/test_folder_media/take.mp4", 1024, "2024-01-01", "03:45", "Video");

    EXPECT_CALL(mockMediaManager, getMediaFile(selectedFile))
        .WillOnce(Return(mediaFile));

    std::istringstream input("0\n");  
    std::cin.rdbuf(input.rdbuf());

    controller->handlerMediaFileManager();  
}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_showAllMediaFileOfVideo_Exit) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(SHOW_ALL_MEDIAFILE_VIDEO)) 
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));  

    EXPECT_CALL(*mockView, displayAllMediaFileOfVideo(_))
        .WillOnce(Return("exit")); 

    std::istringstream input("0\n");  
    std::cin.rdbuf(input.rdbuf());  

    controller->handlerMediaFileManager(); 
}


TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_showAllMediaFileOfAudio) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(SHOW_ALL_MEDIAFILE_AUDIO))  
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    std::string mediaList = "drum.mp3";  
    EXPECT_CALL(*mockView, displayAllMediaFileOfAudio(_))
        .WillOnce(Return(mediaList));  

    std::string selectedFile = "drum.mp3";
    auto mediaFile = std::make_shared<MediaFile>(
        selectedFile, "/home/bluebird/test_folder_media/drum.mp3", 1024, "2024-01-01", "03:45", "Audio");

    EXPECT_CALL(mockMediaManager, getMediaFile(selectedFile))
        .WillOnce(Return(mediaFile));

    std::istringstream input("0\n");  
    std::cin.rdbuf(input.rdbuf());

    controller->handlerMediaFileManager();  
}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_showAllMediaFileOfAudio_Exit) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(SHOW_ALL_MEDIAFILE_AUDIO)) 
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));  

    EXPECT_CALL(*mockView, displayAllMediaFileOfAudio(_))
        .WillOnce(Return("exit")); 

    std::istringstream input("0\n");  
    std::cin.rdbuf(input.rdbuf());  

    controller->handlerMediaFileManager(); 
}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_AddFolder) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(ADD_FOLDER_PATH))
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER));

    EXPECT_CALL(*mockScannerController, handleScan(true))
        .Times(1);

    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf());

    controller->handlerMediaFileManager();
}

TEST_F(MediaFileManagerControllerTest, GetMediaFileController_FileExists) {
    std::string fileName = "drum.mp3";

    controller->addMediaFileController(fileName, mockMediaFileController);

    auto result = controller->getMediaFileController(fileName);
    
    EXPECT_EQ(result, mockMediaFileController);  
}

TEST_F(MediaFileManagerControllerTest, GetMediaFileController_FileNotFound) {
    std::string fileName = "non_existent_file.mp3";

    auto result = controller->getMediaFileController(fileName);

    EXPECT_EQ(result, nullptr);  

}

TEST_F(MediaFileManagerControllerTest, HandleMediaFileManager_InvalidChoiceException) {
    EXPECT_CALL(*mockView, showMenuWithMediaList(_))
        .WillOnce(Return(999)) 
        .WillOnce(Return(EXIT_MENU_MEDIAFILE_MANAGER)); 

    testing::internal::CaptureStderr();

    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf()); 

    controller->handlerMediaFileManager(); 

    std::cin.rdbuf(nullptr);

    std::string output = testing::internal::GetCapturedStderr(); 

    EXPECT_NE(output.find("Error:"), std::string::npos); 
}