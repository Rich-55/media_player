#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Model/ModelManager.h"
#include "View/ViewManager.h"
#include "Controller/ControllerManager.h"
#include "utils/UARTSetup.h"

using ::testing::Return;
using ::testing::_;  // Chấp nhận mọi giá trị tham số

// Mock ModelManager
class MockModelManager : public ModelManager {
public:
    MOCK_METHOD(MediaFileManager&, getMediaFileManager, (), (override));
    MOCK_METHOD(PlaylistManager&, getPlaylistManager, (), (override));
    MOCK_METHOD(FolderManager&, getFolderManager, (), (override));
};

// Mock ViewManager
class MockViewManager : public ViewManager {
public:
    MOCK_METHOD(std::shared_ptr<BaseView>, getView, (std::string viewName), (override));
};

// Mock UARTManager
class MockUARTManager : public UARTManager {
public:
    MOCK_METHOD(bool, checkPortConnection, (const std::string&, unsigned int), (override));
    MOCK_METHOD(std::vector<std::string>, getPortList, (), (override));
    MOCK_METHOD(std::vector<std::string>, getBaudRateOptions, (), (override));
};

// Mock ControllerManager
class MockControllerManager : public ControllerManager {
public:
    MockControllerManager(ModelManager model, ViewManager view, std::shared_ptr<UARTManager> uart)
        : ControllerManager(model, view, uart) {}

    MOCK_METHOD(bool, checkUart, (), (override));
    MOCK_METHOD(void, ScanData, (), (override));
    MOCK_METHOD(void, runApp, (), (override));
};

// Test main function logic
TEST(MainFunctionTest, CheckUartFails_ExitsEarly) {
    MockModelManager mockModel;
    MockViewManager mockView;
    auto mockUartManager = std::make_shared<MockUARTManager>();
    MockControllerManager mockController(mockModel, mockView, mockUartManager);

    // Giả lập `checkUart()` trả về false -> chương trình thoát sớm
    EXPECT_CALL(mockController, checkUart()).WillOnce(Return(false));

    // Giả lập `ScanData()` và `runApp()` không được gọi
    EXPECT_CALL(mockController, ScanData()).Times(0);
    EXPECT_CALL(mockController, runApp()).Times(0);

    // Kiểm tra logic main()
    if (!mockController.checkUart()) {
        SUCCEED();  // Nếu thoát đúng cách -> PASS
    } else {
        FAIL() << "Expected checkUart to fail and exit early.";
    }
}

TEST(MainFunctionTest, CheckUartSuccess_CallsScanDataAndRunApp) {
    MockModelManager mockModel;
    MockViewManager mockView;
    auto mockUartManager = std::make_shared<MockUARTManager>();
    MockControllerManager mockController(mockModel, mockView, mockUartManager);

    // Giả lập `checkUart()` thành công
    EXPECT_CALL(mockController, checkUart()).WillOnce(Return(true));

    // Kiểm tra `ScanData()` và `runApp()` được gọi đúng 1 lần
    EXPECT_CALL(mockController, ScanData()).Times(1);
    EXPECT_CALL(mockController, runApp()).Times(1);

    // Logic của main()
    if (mockController.checkUart()) {
        mockController.ScanData();
        mockController.runApp();
        SUCCEED();
    } else {
        FAIL() << "Expected checkUart to pass and call ScanData & runApp.";
    }
}
