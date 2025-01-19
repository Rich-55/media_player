#ifndef AUDIO_FILE_H
#define AUDIO_FILE_H

#include "MediaFile.h"
#include <map>
#include <unordered_set>    

class AudioFile : public MediaFile {
    private:
        std::map<std::string, std::string> metadataAudio; 
    public:
        AudioFile();
        AudioFile(const std::string& fileName, const std::string& pathName, unsigned long long size, const std::string& dateCreated, const std::string& duration, const std::string& fileType);

        std::string getMetadata(const std::string& key) const;
        std::map<std::string, std::string> getAllMetadata() const override;
        
        void inputMediaFile(std::string pathName, bool isSame) override;

        std::string getType() override;

        bool addNewKey(const std::string& key, const std::string& value) override;
        bool editKey(const std::string& key, const std::string& value) override;
        bool deleteKey(const std::string& key) override;
};

#endif
