#include "../../include/Model/ModelManager.h"

ModelManager::ModelManager(){}

// void ModelManager::addMetaData(std::vector<std::string> listPathName){

// }

MetadataManager &ModelManager::getMetadataManager(){
    return this->metadataManager;
}

std::shared_ptr<MediaFile> ModelManager::getMediaFile(std::string fileName){
    for(std::shared_ptr<MediaFile> file : metadataManager.getAllMediaFile()){
        if(file->getName() == fileName){
            return file;
        }
    }
    throw std::invalid_argument("File not found!");
}

void ModelManager::showdata(){
    metadataManager.getData();
}