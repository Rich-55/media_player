#ifndef VIEW_BASE_H
#define VIEW_BASE_H
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>
class ViewBase{
    private:
    public:
        ViewBase();
        
        virtual void showMenu() = 0;

        virtual void scanHomeDirectory(std::vector<std::string>&); 

        virtual void scanUSBDevices(std::vector<std::string>&);
        
        virtual ~ViewBase();

};

#endif
