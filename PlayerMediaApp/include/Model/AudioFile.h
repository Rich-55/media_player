#ifndef AUDIO_FILE_H
#define AUDIO_FILE_H

#include "MediaFile.h"
#include <map>
#include <unordered_set>    

class AudioFile : public MediaFile {
    private:
        std::map<std::string, std::string> metadataAudio; 
        const static std::unordered_set<std::string> allowedKeys ;
    public:
        AudioFile();
        AudioFile(const std::string& fileName, const std::string& pathName, unsigned long long size, const std::string& duration, const std::string& fileType);

        // Get metadata
        std::string getMetadata(const std::string& key) const;
        std::map<std::string, std::string> getAllMetadata() const override;
        // Input file and initialize metadata
        void inputMediaFile(std::string pathName, bool isSame) override;

        std::string getType() override;

        bool addNewKey(const std::string& key, const std::string& value) override;
        bool editKey(const std::string& key, const std::string& value) override;
        bool deleteKey(const std::string& key) override;
};

#endif
