#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include <iostream>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/textidentificationframe.h>
#include <taglib/tbytevector.h>

#include "MediaFile.h"

class VideoFile : public MediaFile {
private:
    std::string codec;
    int bitrate;
    std::string resolution;

public:
    VideoFile(std::string, std::string, double, std::string, std::string,
              std::string , int, std::string);

    std::string getCodec() const;
    int getBitrate() const;
    std::string getResolution() const;

    void detailMediaFile() const override;
    void editMediaFile() override;
    std::string getType() const override;
};

#endif
