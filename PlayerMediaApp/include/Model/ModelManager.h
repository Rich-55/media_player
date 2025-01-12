#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include "../Model/MetadataManager.h"
#include "../Model/MediaFile.h"

class ModelManager{
    private:
        MetadataManager metadataManager;
    public:
        ModelManager();
    
        MetadataManager &getMetadataManager();

        std::shared_ptr<MediaFile> getMediaFile(std::string);


};


#endif
