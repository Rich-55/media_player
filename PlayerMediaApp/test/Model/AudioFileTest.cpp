// #include <gtest/gtest.h>
// #include <memory>
// #include "Model/AudioFile.h"
// #include "Model/MediaFile.h"   



// // Test AudioFile initialization
// TEST(AudioFileTest, ConstructorTest) {
//     // AudioFile audio("song.mp3", "/music/song.mp3", 5120, "2024-02-01", "3:45", "mp3");
//     std::shared_ptr<MediaFile> audio = std::make_shared<AudioFile>("creepy.mp3", "database/audio/creepy.mp3", 5120, "2024-02-01", "00:11", "mp3");

//     EXPECT_EQ(audio->getFileName(), "creepy.mp3");
//     EXPECT_EQ(audio->getPath(), "/database/audio/creepy.mp3");
//     EXPECT_EQ(audio->getSize(), 5120);
//     EXPECT_EQ(audio->getDateCreated(), "2024-02-01");
//     EXPECT_EQ(audio->getDuration(), "3:45");
//     EXPECT_EQ(audio->getType(), "mp3");
// }

// // Test getType()
// TEST(AudioFileTest, GetTypeTest) {
//     AudioFile audio;
//     EXPECT_EQ(audio.getType(), "Audio");
// }

// // Test getMetadata() when metadata exists
// TEST(AudioFileTest, GetMetadata_ExistingKey) {
//     AudioFile audio;
//     audio.addNewKey("artist", "John Doe");
//     EXPECT_EQ(audio.getMetadata("artist"), "John Doe");
// }

// // Test getMetadata() when metadata does not exist
// TEST(AudioFileTest, GetMetadata_NonExistingKey) {
//     AudioFile audio;
//     EXPECT_EQ(audio.getMetadata("producer"), "");  // Returns an empty string
// }

// // Test getAllMetadata()
// TEST(AudioFileTest, GetAllMetadata) {
//     AudioFile audio;
//     audio.addNewKey("artist", "John Doe");
//     audio.addNewKey("album", "Greatest Hits");

//     auto metadata = audio.getAllMetadata();
//     EXPECT_EQ(metadata["artist"], "John Doe");
//     EXPECT_EQ(metadata["album"], "Greatest Hits");
// }

// // Test addNewKey()
// TEST(AudioFileTest, AddNewKeyTest) {
//     AudioFile audio;
//     bool result = audio.addNewKey("artist", "John Doe");
//     EXPECT_TRUE(result);
//     EXPECT_EQ(audio.getMetadata("artist"), "John Doe");
// }

// // Test addNewKey() when the key already exists
// TEST(AudioFileTest, AddNewKey_ExistingKey) {
//     AudioFile audio;
//     audio.addNewKey("artist", "John Doe");
//     bool result = audio.addNewKey("artist", "Jane Doe");
//     EXPECT_FALSE(result);  // Cannot add an existing key
// }

// // Test editKey()
// TEST(AudioFileTest, EditKeyTest) {
//     AudioFile audio;
//     audio.addNewKey("artist", "John Doe");
//     bool result = audio.editKey("artist", "Jane Doe");
//     EXPECT_TRUE(result);
//     EXPECT_EQ(audio.getMetadata("artist"), "Jane Doe");
// }

// // Test editKey() when the key does not exist
// TEST(AudioFileTest, EditKey_NonExistingKey) {
//     AudioFile audio;
//     bool result = audio.editKey("composer", "Beethoven");
//     EXPECT_FALSE(result);
// }

// // Test deleteKey()
// TEST(AudioFileTest, DeleteKeyTest) {
//     AudioFile audio;
//     audio.addNewKey("artist", "John Doe");
//     bool result = audio.deleteKey("artist");
//     EXPECT_TRUE(result);
//     EXPECT_EQ(audio.getMetadata("artist"), "");
// }

// // Test deleteKey() when the key does not exist
// TEST(AudioFileTest, DeleteKey_NonExistingKey) {
//     AudioFile audio;
//     bool result = audio.deleteKey("genre");
//     EXPECT_FALSE(result);
// }

// // Test inputMediaFile() with a valid file
// TEST(AudioFileTest, InputMediaFile_Valid) {
//     AudioFile audio;
//     audio.inputMediaFile("valid_audio.mp3", false);

//     auto metadata = audio.getAllMetadata();
//     EXPECT_NE(metadata["title"], "Unknown");
//     EXPECT_NE(metadata["artist"], "Unknown");
//     EXPECT_NE(metadata["album"], "Unknown");
// }

// // Test inputMediaFile() with an invalid file
// TEST(AudioFileTest, InputMediaFile_Invalid) {
//     AudioFile audio;
//     testing::internal::CaptureStderr();
//     audio.inputMediaFile("invalid_audio.mp3", false);
//     std::string output = testing::internal::GetCapturedStderr();
    
//     EXPECT_TRUE(output.find("Error opening file") != std::string::npos);
// }