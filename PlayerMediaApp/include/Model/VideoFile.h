#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/textidentificationframe.h>
#include <taglib/tbytevector.h>
#include <taglib/mp4file.h>
#include <taglib/mp4tag.h>

#include "MediaFile.h"

class VideoFile : public MediaFile {
private:
    std::string codec;
    int bitrate;
    std::string resolution;
    
public:
    VideoFile();
    VideoFile(const std::string& , const std::string& , double , const std::string& , const std::string& ,
              const std::string& , int , const std::string&);

    std::string getCodec() const;
    int getBitrate() const;
    std::string getResolution() const;

    void inputMediaFile(std::string) override;
    void detailMediaFile() const override;
    void editMediaFile() override;
    std::string getType() override;
};

#endif
