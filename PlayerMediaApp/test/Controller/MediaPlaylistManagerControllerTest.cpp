// #include <gtest/gtest.h>
// #include <gmock/gmock.h>

// #include "../include/Controller/MediaPlaylistManagerController.h"

// using ::testing::Return;
// using ::testing::_;
// using ::testing::NiceMock;
// using ::testing::Invoke;

// // Mock class PlaylistManager
// class MockPlaylistManager : public PlaylistManager {
// public:
//     MOCK_METHOD(bool, checkPlaylist, (), (override));
//     MOCK_METHOD(bool, checkPlaylistName, (std::string), (override));
//     MOCK_METHOD(std::shared_ptr<Playlist>, getPlaylist, (std::string), (override));
//     MOCK_METHOD(void, addPlaylist, (std::shared_ptr<Playlist>), (override));
//     MOCK_METHOD(bool, deletePlaylist, (std::string), (override));
//     MOCK_METHOD(std::vector<std::shared_ptr<Playlist>>, getAllPlaylist, (), (override));
// };

// // Mock class MediaFileManager
// class MockMediaFileManager : public MediaFileManager {
// public:
//     MOCK_METHOD(std::shared_ptr<MediaFile>, getMediaFile, (std::string), (override));
// };

// // Mock class FolderManager
// class MockFolderManager : public FolderManager {
// public:
//     MOCK_METHOD(std::unordered_set<std::string>, getListFolderDirectory, (), (override));
// };

// // Mock class BaseView
// class MockBaseView : public BaseView {
// public:
//     MOCK_METHOD(int, showMenu, (), (override));
//     MOCK_METHOD(void, showNotificationMessage, (std::string, std::string), (override));
//     MOCK_METHOD(bool, showConfirmMenu, (std::string), (override));
//     MOCK_METHOD(std::string, displayAllPlaylist, (PlaylistManager), (override));
//     MOCK_METHOD(int, showMenuWithPlaylist, (std::vector<std::shared_ptr<Playlist>>), (override));
//     MOCK_METHOD(std::string, showMenuCreatePlaylist, (), (override));
// };

// // Test fixture
// class MediaPlaylistManagerControllerTest : public ::testing::Test {
// protected:
//     MockPlaylistManager mockPlaylistManager; 
//     MockMediaFileManager mockMediaFileManager;
//     MockFolderManager mockFolderManager;  

//     std::shared_ptr<MockBaseView> mockMediaManagerView;
//     std::shared_ptr<MockBaseView> mockPlaylistManagerView;
//     std::shared_ptr<MockBaseView> mockPlaylistHandlerView;
    
//     std::unique_ptr<MediaPlaylistManagerController> controller;

//     void SetUp() override {
//         mockMediaManagerView = std::make_shared<MockBaseView>();
//         mockPlaylistManagerView = std::make_shared<MockBaseView>();
//         mockPlaylistHandlerView = std::make_shared<MockBaseView>();

//         controller = std::make_unique<MediaPlaylistManagerController>(
//             mockPlaylistManager, mockMediaFileManager, mockFolderManager,
//             mockMediaManagerView, mockPlaylistManagerView, mockPlaylistHandlerView
//         );
//     }

//     void TearDown() override {
//         testing::Mock::VerifyAndClearExpectations(mockMediaManagerView.get());
//         testing::Mock::VerifyAndClearExpectations(mockPlaylistManagerView.get());
//         testing::Mock::VerifyAndClearExpectations(mockPlaylistHandlerView.get());
//     }
// };

// // Dummy Test
// TEST_F(MediaPlaylistManagerControllerTest, SetupWorksWithoutMock) {
//     std::shared_ptr<MockBaseView> view = std::make_shared<MockBaseView>();
//     EXPECT_NO_THROW({
//         controller = std::make_unique<MediaPlaylistManagerController>(
//             mockPlaylistManager, mockMediaFileManager, mockFolderManager,
//             view, view, view
//         );
//     });
// }
