#ifndef MEDIA_FILE_CONTROLLER_H
#define MEDIA_FILE_CONTROLLER_H

#include <memory>
#include <vector>
#include "../Model/MetadataManager.h"
#include "../Model/MediaFile.h"
#include "../Model/AudioFile.h"
#include "../View/MediaFileView.h"

class MediaFileController {
private:
    MediaFile* model;
    MediaFileView& view;

public:
    MediaFileController(MediaFile* model, MediaFileView& view); 

    void updateView();

    void setTrackName(const std::string& newTrackName);
    void setAlbum(const std::string& newAlbum);
    void setArtist(const std::string& newArtist);
    void setGenre(const std::string& newGenre);
    void saveChanges();


    // void setData(int data);
    
    // void addMediaFile(const std::string& filePath);

    // void handleMenuInput(int choice);
    // void addMediaFileFromUSB(const std::shared_ptr<MediaFile> &file);

    // void addMediaFileFromPC(const std::shared_ptr<MediaFile> &file);

    // void editMediaFile(const std::shared_ptr<MediaFile> &file);

    // void removeMediaFile(const std::shared_ptr<MediaFile> &file);

    // void displayAllMediaFiles(MediaFileView &view);

    // void sendMediaFileToView(MediaFileView &view, const std::shared_ptr<MediaFile> &file);
};

#endif
