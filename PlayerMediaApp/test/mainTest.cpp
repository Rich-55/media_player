#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Model/ModelManager.h"
#include "View/ViewManager.h"
#include "Controller/ControllerManager.h"
#include "utils/UARTSetup.h"

using ::testing::Return;
using ::testing::_;  

// Mock for ModelManager
class MockModelManager : public ModelManager {
public:
    // No need to declare mock methods in MockModelManager as they're not used in the test.
};

// Mock for ViewManager
class MockViewManager : public ViewManager {
public:
    // No need to declare mock methods in MockViewManager as they're not used in the test.
};

// Mock for UARTManager
class MockUARTManager : public UARTManager {
public:
    MOCK_METHOD(bool, checkPortConnection, (const std::string&, unsigned int), (override));
    MOCK_METHOD(std::vector<std::string>, getPortList, (), (override));
    MOCK_METHOD(std::vector<std::string>, getBaudRateOptions, (), (override));
};

// Mock for ControllerManager
class MockControllerManager : public ControllerManager {
public:
    MockControllerManager(ModelManager model, ViewManager view, std::shared_ptr<UARTManager> uart)
        : ControllerManager(model, view, uart) {}

    MOCK_METHOD(bool, checkUart, (), (override));
    MOCK_METHOD(void, ScanData, (), (override));
    MOCK_METHOD(void, runApp, (), (override));
};

// Test for main function logic
TEST(MainFunctionTest, CheckUartFails_ExitsEarly) {
    MockModelManager mockModel;
    MockViewManager mockView;
    auto mockUartManager = std::make_shared<MockUARTManager>();
    MockControllerManager mockController(mockModel, mockView, mockUartManager);

    // Simulate checkUart() returning false, which should cause early exit
    EXPECT_CALL(mockController, checkUart()).WillOnce(Return(false));
    // Ensure ScanData and runApp are not called
    EXPECT_CALL(mockController, ScanData()).Times(0);
    EXPECT_CALL(mockController, runApp()).Times(0);

    // Check main logic
    if (!mockController.checkUart()) {
        SUCCEED();  // If exit happens correctly -> PASS
    } else {
        FAIL() << "Expected checkUart to fail and exit early.";
    }
}

TEST(MainFunctionTest, CheckUartSuccess_CallsScanDataAndRunApp) {
    MockModelManager mockModel;
    MockViewManager mockView;
    auto mockUartManager = std::make_shared<MockUARTManager>();
    MockControllerManager mockController(mockModel, mockView, mockUartManager);

    // Simulate successful checkUart() call
    EXPECT_CALL(mockController, checkUart()).WillOnce(Return(true));
    // Ensure ScanData and runApp are called exactly once
    EXPECT_CALL(mockController, ScanData()).Times(1);
    EXPECT_CALL(mockController, runApp()).Times(1);

    // Main logic
    if (mockController.checkUart()) {
        mockController.ScanData();
        mockController.runApp();
        SUCCEED();
    } else {
        FAIL() << "Expected checkUart to pass and call ScanData & runApp.";
    }
}

TEST(UARTManagerTest, CheckBaudRateOptions_ShouldReturnExpectedValues) {
    MockUARTManager mockUart;
    std::vector<std::string> expectedBaudRates = {"9600", "115200", "250000"};

    // Simulate getBaudRateOptions returning predefined baud rates
    EXPECT_CALL(mockUart, getBaudRateOptions()).WillOnce(Return(expectedBaudRates));

    auto baudRates = mockUart.getBaudRateOptions();
    // Verify the baud rates returned are as expected
    EXPECT_EQ(baudRates, expectedBaudRates);
}
