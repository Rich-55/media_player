// #include <gtest/gtest.h>
// #include <gmock/gmock.h>
// #include "../include/utils/UARTSetup.h"
// #include "../include/Controller/PlayerController.h"

// using ::testing::Return;
// using ::testing::_;  

// // Mock PlayerController
// class MockPlayerController : public PlayerController {
// public:
//     MockPlayerController() : PlayerController(std::vector<std::string>()) {}
    
//     MOCK_METHOD(void, togglePlayback, (), (override));
//     MOCK_METHOD(void, stop, (), (override));
//     MOCK_METHOD(void, playNext, (), (override));
//     MOCK_METHOD(void, playPrevious, (), (override));
//     MOCK_METHOD(void, setVolume, (int), (override));
// };

// // Mock UARTManager
// class MockUARTManager : public UARTManager {
// public:
//     MOCK_METHOD(bool, checkPortConnection, (const std::string& port, unsigned int baud_rate), (override));
//     MOCK_METHOD(std::vector<std::string>, getPortList, (), (override));
//     MOCK_METHOD(std::vector<std::string>, getBaudRateOptions, (), (override));
// };

// // Test checkPortConnection - Success
// TEST(UARTManagerTest, CheckPortConnection_Success) {
//     MockUARTManager mockUartManager;

//     std::vector<std::string> availablePorts = {"/dev/ttyACM0", "/dev/ttyACM1"};
    
//     EXPECT_CALL(mockUartManager, getPortList()).WillOnce(Return(availablePorts));
    
//     std::vector<std::string> ports = mockUartManager.getPortList();
    
//     if (!ports.empty()) {
//         EXPECT_CALL(mockUartManager, checkPortConnection(ports[0], 115200)).WillOnce(Return(true));
        
//         EXPECT_TRUE(mockUartManager.checkPortConnection(ports[0], 115200));
//     } else {
//         GTEST_SKIP(); 
//     }
// }

// // Test checkPortConnection - Failure
// TEST(UARTManagerTest, CheckPortConnection_Failure) {
//     MockUARTManager mockUartManager;
//     EXPECT_CALL(mockUartManager, checkPortConnection("InvalidCOM", 9600)).WillOnce(Return(false));

//     EXPECT_FALSE(mockUartManager.checkPortConnection("InvalidCOM", 9600));
// }

// // Test getPortList
// TEST(UARTManagerTest, GetPortList_ReturnsCorrectValues) {
//     MockUARTManager mockUartManager;
    
//     EXPECT_CALL(mockUartManager, getPortList())
//         .WillOnce(Return(std::vector<std::string>{"/dev/ttyACM0"}))  
//         .WillOnce(Return(std::vector<std::string>{"/dev/ttyACM1"})); 

   
//     std::vector<std::string> ports1 = mockUartManager.getPortList();
//     EXPECT_EQ(ports1.size(), 1);  
//     EXPECT_EQ(ports1[0], "/dev/ttyACM0");

//     std::vector<std::string> ports2 = mockUartManager.getPortList();
//     EXPECT_EQ(ports2.size(), 1);  
//     EXPECT_EQ(ports2[0], "/dev/ttyACM1"); 
// }


// // Test getBaudRateOptions
// TEST(UARTManagerTest, GetBaudRateOptions_ReturnsCorrectValues) {
//     UARTManager uartManager;
//     std::vector<std::string> baudRates = uartManager.getBaudRateOptions();

//     std::vector<std::string> expectedBaudRates = {
//         "1200", "2400", "4800", "9600", "14400", "19200",
//         "38400", "57600", "115200", "230400", "460800", "921600"
//     };

//     EXPECT_EQ(baudRates, expectedBaudRates);
// }

// // Test stopUART
// TEST(UARTManagerTest, StopUART_SafeToCall) {
//     UARTManager uartManager;
    
//     EXPECT_NO_THROW(uartManager.stopUART());
// }

// // Test RunMediaUart
// TEST(UARTManagerTest, RunMediaUart_CallsAsyncHandleUart) {
//     UARTManager uartManager;
//     auto mockPlayerController = std::make_shared<MockPlayerController>();
//     std::shared_ptr<PlayerController> playerController = std::static_pointer_cast<PlayerController>(mockPlayerController);

//     EXPECT_TRUE(uartManager.checkPortConnection("/dev/ttyACM0", 115200));
//     EXPECT_NO_THROW(uartManager.runMediaUart(playerController));
// }
