#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include "../Model/MetadataManager.h"

class ModelManager{
    private:
        MetadataManager metadataManager;
    public:
        ModelManager();

        //void addMetaData(std::vector<std::string>);

        MetadataManager &getMetadataManager();

        void showdata();

};


#endif