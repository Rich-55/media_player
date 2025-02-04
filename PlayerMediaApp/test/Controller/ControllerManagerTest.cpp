// #include <gtest/gtest.h>
// #include <gmock/gmock.h>
// #include "Controller/ControllerManager.h"
// #include "View/ViewManager.h"
// #include "Model/ModelManager.h"
// #include "utils/UARTSetup.h"
// #include <memory>

// using ::testing::Return;
// using ::testing::_;
// using ::testing::NiceMock;

// // Mock UARTManager

// class MockUARTManager : public UARTManager {
// public:
//     MOCK_METHOD(std::vector<std::string>, getPortList, (), (override));  
//     MOCK_METHOD(std::vector<std::string>, getBaudRateOptions, (), (override));

//     MOCK_METHOD(bool, checkPortConnection, (const std::string&, unsigned int), (override));
// };


// // Mock ViewManager
// class MockViewManager : public ViewManager {
// public:
//     MOCK_METHOD(std::shared_ptr<BaseView>, getView, (std::string), (override));
// };

// // Mock BaseView
// class MockBaseView : public BaseView {
// public:
//     MOCK_METHOD(int, showMenu, (), (override));
//     MOCK_METHOD(void, showNotificationMessage, (std::string, std::string), (override));
//     MOCK_METHOD(bool, showConfirmMenu, (std::string), (override));
//     MOCK_METHOD(int, showMenuWithPlayer, (MediaFileManager, std::shared_ptr<PlayerController>&, std::string, std::string&), (override));
//     MOCK_METHOD(std::string, displayAllMediaFile, (MediaFileManager), (override));
//     MOCK_METHOD(std::string, displayAllMediaFileOfAudio, (MediaFileManager), (override));
//     MOCK_METHOD(std::string, displayAllMediaFileOfVideo, (MediaFileManager), (override));
//     MOCK_METHOD(int, showMenuWithMediaList, (MediaFileManager), (override));
//     MOCK_METHOD(void, displayDetailMediaFile, (std::shared_ptr<MediaFile>, std::string), (override));
//     MOCK_METHOD((std::pair<std::string, std::string>), displayMenuAddMetadata, (std::shared_ptr<MediaFile>, std::string), (override));
//     MOCK_METHOD((std::pair<std::string, std::string>), displayMenuEditMetadata, (std::shared_ptr<MediaFile>, std::string), (override));
//     MOCK_METHOD(std::string, displayMenuDeleteMetadata, (std::shared_ptr<MediaFile>), (override));
//     MOCK_METHOD(std::string, displayAllMediaFileInPlaylist, (std::shared_ptr<Playlist>), (override));
//     MOCK_METHOD(int, showMenuWithMediaListInPlaylist, (std::shared_ptr<Playlist>), (override));
//     MOCK_METHOD((std::pair<std::string, std::string>), displayAllFolder, 
//         ((std::pair<std::unordered_set<std::string>, std::unordered_set<std::string>>)), (override));
//     MOCK_METHOD(std::string, displayAllPlaylist, (PlaylistManager), (override));
//     MOCK_METHOD(int, showMenuWithPlaylist, (std::vector<std::shared_ptr<Playlist>>), (override));
//     MOCK_METHOD(std::string, showMenuCreatePlaylist, (), (override));
//     MOCK_METHOD(int, showListFolder, (std::vector<std::string>), (override));
//     MOCK_METHOD(int, showListUSBName, (std::vector<std::string>), (override));
//     MOCK_METHOD(void, setListPathNameIsAdded, (std::unordered_set<std::string>), (override));
//     MOCK_METHOD(void, showFileAdded, (), (override));
//     MOCK_METHOD((std::pair<std::string, unsigned int>), getPortAndBaudRate, (std::vector<std::string>, std::vector<std::string>), (override));
// };
// // **Test Constructor**
// TEST(ControllerManagerTest, ConstructorTest) {
//     ModelManager model;
//     NiceMock<MockViewManager> view;
//     auto uartManager = std::make_shared<NiceMock<MockUARTManager>>();

//     EXPECT_NO_THROW(ControllerManager manager(model, view, uartManager));
// }

// // **Test getView()**
// TEST(ControllerManagerTest, GetView_ValidView) {
//     ModelManager model;
//     NiceMock<MockViewManager> view;
//     auto uartManager = std::make_shared<NiceMock<MockUARTManager>>();
//     ControllerManager manager(model, view, uartManager);

//     auto mockView = std::make_shared<MockBaseView>();
//     EXPECT_CALL(view, getView("ScanView")).WillOnce(Return(mockView));

//     EXPECT_NO_THROW(manager.getView("ScanView"));
//     EXPECT_EQ(manager.getView("ScanView"), mockView);
// }

// TEST(ControllerManagerTest, GetView_InvalidView) {
//     ModelManager model;
//     NiceMock<MockViewManager> view;
//     auto uartManager = std::make_shared<NiceMock<MockUARTManager>>();
//     ControllerManager manager(model, view, uartManager);

//     EXPECT_CALL(view, getView("InvalidView")).WillOnce(Return(nullptr));

//     EXPECT_THROW(manager.getView("InvalidView"), std::runtime_error);
// }

// // **Test checkUart()**
// TEST(ControllerManagerTest, CheckUart_Success) {
//     ModelManager model;
//     NiceMock<MockViewManager> view;
//     auto uartManager = std::make_shared<NiceMock<MockUARTManager>>();
//     ControllerManager manager(model, view, uartManager);

//     auto mockView = std::make_shared<MockBaseView>();
//     EXPECT_CALL(view, getView("UartView")).WillOnce(Return(mockView));
//     EXPECT_CALL(*uartManager, getPortList()).WillOnce(Return(std::vector<std::string>{"/dev/ttyACM0", "/dev/ttyACM1"}));
//     EXPECT_CALL(*uartManager, getBaudRateOptions()).WillOnce(Return(std::vector<std::string>{"9600", "115200"}));
//     EXPECT_CALL(*mockView, getPortAndBaudRate(testing::_, testing::_))
//         .WillOnce(Return(std::make_pair("/dev/ttyACM0", 115200)));
//     EXPECT_CALL(*uartManager, checkPortConnection("/dev/ttyACM0", 115200)).WillOnce(Return(true));
//     EXPECT_TRUE(manager.checkUart());
// }

// // **Test ScanData()**
// TEST(ControllerManagerTest, ScanData_CallsScannerController) {
//     ModelManager model;
//     NiceMock<MockViewManager> view;
//     auto uartManager = std::make_shared<NiceMock<MockUARTManager>>();
//     ControllerManager manager(model, view, uartManager);

//     auto mockView = std::make_shared<MockBaseView>();
//     EXPECT_CALL(view, getView("ScanView")).WillOnce(Return(mockView));

//     EXPECT_NO_THROW(manager.ScanData());
// }

// // **Test mediaFileHandler()**
// TEST(ControllerManagerTest, MediaFileHandler_FileRunning) {
//     ModelManager model;
//     NiceMock<MockViewManager> view;
//     auto uartManager = std::make_shared<NiceMock<MockUARTManager>>();
//     ControllerManager manager(model, view, uartManager);

//     PlayerController::currentPlayingFile = "database/audio/creepy.mp3";
//     EXPECT_EQ(manager.mediaFileHandler(), "File is running, can't edit");
// }

// // **Test playMusicHandler()**
// TEST(ControllerManagerTest, PlayMusicHandler_ValidFile) {
//     ModelManager model;
//     NiceMock<MockViewManager> view;
//     auto uartManager = std::make_shared<NiceMock<MockUARTManager>>();
//     ControllerManager manager(model, view, uartManager);

//     auto mockView = std::make_shared<MockBaseView>();
//     EXPECT_CALL(view, getView("MediaFileManagerView")).WillOnce(Return(mockView));
    
//     EXPECT_EQ(manager.playMusicHandler(), "exit");
// }
