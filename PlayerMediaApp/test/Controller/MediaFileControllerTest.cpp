#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../include/Model/MediaFile.h"
#include "../include/View/BaseView.h"
#include "../include/Controller/MediaFileController.h"

using ::testing::Return;
using ::testing::_;
using ::testing::AtLeast; 


class MockMediaFile : public MediaFile {
public:
    MOCK_METHOD(bool, addNewKey, (const std::string&, const std::string&), (override));
    MOCK_METHOD(bool, editKey, (const std::string&, const std::string&), (override));
    MOCK_METHOD(bool, deleteKey, (const std::string&), (override));
};

class MockBaseView : public BaseView {
public:
    MOCK_METHOD(void, displayDetailMediaFile, (std::shared_ptr<MediaFile>, std::string), (override));
    MOCK_METHOD(int, showMenu, (), (override));
    MOCK_METHOD((std::pair<std::string, std::string>), displayMenuAddMetadata, (std::shared_ptr<MediaFile>, std::string), (override));
    MOCK_METHOD((std::pair<std::string, std::string>), displayMenuEditMetadata, (std::shared_ptr<MediaFile>, std::string), (override));
    MOCK_METHOD(std::string, displayMenuDeleteMetadata, (std::shared_ptr<MediaFile>), (override));
    MOCK_METHOD(bool, showConfirmMenu, (std::string), (override));
};

class MediaFileControllerTest : public ::testing::Test {
protected:
    std::shared_ptr<MockMediaFile> mockMediaFile;
    std::shared_ptr<MockBaseView> mockView;
    std::unique_ptr<MediaFileController> controller;

    void SetUp() override {
        mockMediaFile = std::make_shared<MockMediaFile>();
        mockView = std::make_shared<MockBaseView>();
        controller = std::make_unique<MediaFileController>(mockMediaFile, mockView);
        ON_CALL(*mockView, displayDetailMediaFile(_, _)).WillByDefault(Return());
    }
};

TEST_F(MediaFileControllerTest, HandleMediaFile_AddMetadata) {
    EXPECT_CALL(*mockView, displayDetailMediaFile(_, _)).Times(::testing::AtLeast(1));

    EXPECT_CALL(*mockView, showMenu())
        .WillOnce(Return(ADD_METADATA))
        .WillOnce(Return(EXIT_MENU_METADATA_EDIT));

    EXPECT_CALL(*mockView, displayMenuAddMetadata(_, _))
        .WillOnce(Return(std::make_pair("year", "2024")));

    EXPECT_CALL(*mockMediaFile, addNewKey("year", "2024"))
        .WillOnce(Return(true));

    controller->handlerMediaFile();
}

TEST_F(MediaFileControllerTest, HandleMediaFile_AddMetadata_InvalidYearFormat) {
    EXPECT_CALL(*mockView, displayDetailMediaFile(_, _)).Times(::testing::AtLeast(1));

    EXPECT_CALL(*mockView, showMenu())
        .WillOnce(Return(ADD_METADATA))
        .WillOnce(Return(EXIT_MENU_METADATA_EDIT));

    EXPECT_CALL(*mockView, displayMenuAddMetadata(_, _))
        .WillOnce(Return(std::make_pair("year", "")))   
        .WillOnce(Return(std::make_pair("year", "20"))) 
        .WillOnce(Return(std::make_pair("year", "2024"))) 
        .WillOnce(Return(std::make_pair("0", ""))); 

    EXPECT_CALL(*mockMediaFile, addNewKey("year", "2024"))
        .WillOnce(Return(false));  

    controller->handlerMediaFile();
}

TEST_F(MediaFileControllerTest, HandleMediaFile_AddMetadata_InvalidTrackFormat) {
    EXPECT_CALL(*mockView, displayDetailMediaFile(_, _)).Times(::testing::AtLeast(1));

    EXPECT_CALL(*mockView, showMenu())
        .WillOnce(Return(ADD_METADATA))
        .WillOnce(Return(EXIT_MENU_METADATA_EDIT));

    EXPECT_CALL(*mockView, displayMenuAddMetadata(_, _))
        .WillOnce(Return(std::make_pair("track", "abc"))) 
        .WillOnce(Return(std::make_pair("track", "-3")))   
        .WillOnce(Return(std::make_pair("track", "1a/2"))) 
        .WillOnce(Return(std::make_pair("0", "")));      

    EXPECT_CALL(*mockMediaFile, addNewKey(_, _)).Times(0); 

    controller->handlerMediaFile();
}

TEST_F(MediaFileControllerTest, HandleMediaFile_EditMetadata_Success) {
    EXPECT_CALL(*mockView, displayDetailMediaFile(_, _)).Times(::testing::AtLeast(1));

    EXPECT_CALL(*mockView, showMenu())
        .WillOnce(Return(EDIT_METADATA))
        .WillOnce(Return(EXIT_MENU_METADATA_EDIT));

    EXPECT_CALL(*mockView, displayMenuEditMetadata(_, _))
        .WillOnce(Return(std::make_pair("track", "5/10")));

    EXPECT_CALL(*mockMediaFile, editKey("track", "5/10"))
        .WillOnce(Return(true));

    controller->handlerMediaFile();
}

TEST_F(MediaFileControllerTest, HandleMediaFile_EditMetadata_Fail) {
    EXPECT_CALL(*mockView, displayDetailMediaFile(_, _)).Times(::testing::AtLeast(1));

    EXPECT_CALL(*mockView, showMenu())
        .WillOnce(Return(EDIT_METADATA))
        .WillOnce(Return(EXIT_MENU_METADATA_EDIT));

    EXPECT_CALL(*mockView, displayMenuEditMetadata(_, _))
        .WillOnce(Return(std::make_pair("track", "5/10")))
        .WillOnce(Return(std::make_pair("0", "")));   

    EXPECT_CALL(*mockMediaFile, editKey("track", "5/10"))
        .WillOnce(Return(false));

    controller->handlerMediaFile();
}

TEST_F(MediaFileControllerTest, HandleMediaFile_EditMetadata_EmptyValue) {
    EXPECT_CALL(*mockView, displayDetailMediaFile(_, _)).Times(::testing::AtLeast(1));

    EXPECT_CALL(*mockView, showMenu())
        .WillOnce(Return(EDIT_METADATA))
        .WillOnce(Return(EXIT_MENU_METADATA_EDIT));

    EXPECT_CALL(*mockView, displayMenuEditMetadata(_, _))
        .WillOnce(Return(std::make_pair("artist", "")))  
        .WillOnce(Return(std::make_pair("0", "")));      

    EXPECT_CALL(*mockMediaFile, editKey(_, _)).Times(0); 

    controller->handlerMediaFile();
}

TEST_F(MediaFileControllerTest, HandleMediaFile_EditMetadata_InvalidYearFormat) {
    EXPECT_CALL(*mockView, displayDetailMediaFile(_, _)).Times(::testing::AtLeast(1));

    EXPECT_CALL(*mockView, showMenu())
        .WillOnce(Return(EDIT_METADATA))
        .WillOnce(Return(EXIT_MENU_METADATA_EDIT));

    EXPECT_CALL(*mockView, displayMenuEditMetadata(_, _))
        .WillOnce(Return(std::make_pair("year", "20")))  
        .WillOnce(Return(std::make_pair("year", "abcd"))) 
        .WillOnce(Return(std::make_pair("0", "")));     

    EXPECT_CALL(*mockMediaFile, editKey(_, _)).Times(0); 

    controller->handlerMediaFile();
}


TEST_F(MediaFileControllerTest, HandleMediaFile_EditMetadata_InvalidTrackFormat) {
    EXPECT_CALL(*mockView, displayDetailMediaFile(_, _)).Times(::testing::AtLeast(1));

    EXPECT_CALL(*mockView, showMenu())
        .WillOnce(Return(EDIT_METADATA))
        .WillOnce(Return(EXIT_MENU_METADATA_EDIT));

    EXPECT_CALL(*mockView, displayMenuEditMetadata(_, _))
        .WillOnce(Return(std::make_pair("track", "abc"))) 
        .WillOnce(Return(std::make_pair("track", "-3")))   
        .WillOnce(Return(std::make_pair("0", "")));     

    EXPECT_CALL(*mockMediaFile, editKey(_, _)).Times(0); 

    controller->handlerMediaFile();
}


TEST_F(MediaFileControllerTest, HandleMediaFile_DeleteMetadata_Success) {
    EXPECT_CALL(*mockView, displayDetailMediaFile(_, _)).Times(::testing::AtLeast(1));

    EXPECT_CALL(*mockView, showMenu())
        .WillOnce(Return(DELETE_METADATA))
        .WillOnce(Return(EXIT_MENU_METADATA_EDIT));

    EXPECT_CALL(*mockView, displayMenuDeleteMetadata(_))
        .WillOnce(Return("artist"));

    EXPECT_CALL(*mockView, showConfirmMenu(_))
        .WillOnce(Return(true));

    EXPECT_CALL(*mockMediaFile, deleteKey("artist"))
        .WillOnce(Return(true));

    controller->handlerMediaFile();
}

TEST_F(MediaFileControllerTest, HandleMediaFile_DeleteMetadata_Fail) {
    EXPECT_CALL(*mockView, displayDetailMediaFile(_, _)).Times(::testing::AtLeast(1));

    EXPECT_CALL(*mockView, showMenu())
        .WillOnce(Return(DELETE_METADATA))
        .WillOnce(Return(EXIT_MENU_METADATA_EDIT));

    EXPECT_CALL(*mockView, displayMenuDeleteMetadata(_))
        .WillOnce(Return("artist"))
        .WillOnce(Return("0"));

    EXPECT_CALL(*mockView, showConfirmMenu(_))
        .WillOnce(Return(true));

    EXPECT_CALL(*mockMediaFile, deleteKey("artist"))
        .WillOnce(Return(false));

    controller->handlerMediaFile();
}

TEST_F(MediaFileControllerTest, HandleMediaFile_DeleteMetadata_Cancel) {
    EXPECT_CALL(*mockView, displayDetailMediaFile(_, _)).Times(::testing::AtLeast(1));

    EXPECT_CALL(*mockView, showMenu())
        .WillOnce(Return(DELETE_METADATA))  
        .WillOnce(Return(EXIT_MENU_METADATA_EDIT)); 

    EXPECT_CALL(*mockView, displayMenuDeleteMetadata(_))
        .WillOnce(Return("artist"))
        .WillOnce(Return("0"));

    EXPECT_CALL(*mockView, showConfirmMenu(_))
        .WillOnce(Return(false));

    controller->handlerMediaFile();
}


TEST_F(MediaFileControllerTest, HandleMediaFile_ExitMenu) {
    EXPECT_CALL(*mockView, displayDetailMediaFile(_, _)).Times(::testing::AtLeast(1));
    
    EXPECT_CALL(*mockView, showMenu())
        .WillOnce(Return(EXIT_MENU_METADATA_EDIT));

    controller->handlerMediaFile();
}


TEST_F(MediaFileControllerTest, HandleMediaFile_InvalidChoice) {
    EXPECT_CALL(*mockView, displayDetailMediaFile(_, _)).Times(::testing::AtLeast(1));
    EXPECT_CALL(*mockView, showMenu())
        .WillOnce(Return(-999))
        .WillOnce(Return(EXIT_MENU_METADATA_EDIT));  

    controller->handlerMediaFile();
}
