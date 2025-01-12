#include "../../include/Model/ModelManager.h"

ModelManager::ModelManager(){}

// void ModelManager::addMetaData(std::vector<std::string> listPathName){

// }


ModelManager::ModelManager(){}

// void ModelManager::addMetaData(std::vector<std::string> listPathName){

// }

MetadataManager &ModelManager::getMetadataManager(){
    return this->metadataManager;
}

void ModelManager::showdata(){
    metadataManager.getData();
}