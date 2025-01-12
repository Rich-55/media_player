#include "../../include/Model/ModelManager.h"

ModelManager::ModelManager(){}

// void ModelManager::addMetaData(std::vector<std::string> listPathName){

// }

MetadataManager &ModelManager::getMetadataManager()
{
    return this->metadataManager;
}

std::shared_ptr<MediaFile> ModelManager::getMediaFile(std::string fileName)
{
    return metadataManager.getMediaFile(fileName);
}