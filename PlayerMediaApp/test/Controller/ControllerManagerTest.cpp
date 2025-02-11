#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../include/Controller/ControllerManager.h"

using ::testing::Return;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::StrictMock;

using ::testing::Invoke;
using ::testing::MatchesRegex;
using ::testing::AtLeast;
using ::testing::InSequence;

class MockViewManager : public ViewManager {
public:
    MOCK_METHOD(std::shared_ptr<BaseView>, getView, (std::string), (override));
};

// Mock UARTManager
class MockUARTManager : public UARTManager {
public:
    MOCK_METHOD(std::vector<std::string>, getPortList, (), (override));
    MOCK_METHOD(std::vector<std::string>, getBaudRateOptions, (), (override));
    MOCK_METHOD(bool, checkPortConnection, (const std::string&, unsigned int), (override));
};

class MockBaseView : public BaseView {
public:
    MOCK_METHOD(int, showMenu, (), (override));
    MOCK_METHOD(void, showNotificationMessage, (std::string, std::string), (override));
    MOCK_METHOD(bool, showConfirmMenu, (std::string), (override));
    MOCK_METHOD(int, showMenuWithPlayer, (MediaFileManager, std::shared_ptr<PlayerController>&, std::string, std::string&), (override));
   
};

class ControllerManagerTest : public ::testing::Test {
protected:
    NiceMock<MockViewManager> mockViewManager;
    std::shared_ptr<MockBaseView> mockBaseView;
    std::shared_ptr<MockUARTManager> mockUartManager;
    ModelManager model;

    std::shared_ptr<ControllerManager> controllerManager;

    void SetUp() override {
        mockBaseView = std::make_shared<MockBaseView>();
        mockUartManager = std::make_shared<MockUARTManager>();
        controllerManager = std::make_shared<ControllerManager>(model, mockViewManager, mockUartManager);
    }
};

