#ifndef VIEW_BASE_H
#define VIEW_BASE_H
#include <vector>
#include <iostream>
#include <string>
class ViewBase{
    private:
        
    public:
        ViewBase();
        
        virtual void showMenu();

        virtual void scanHomeDirectory(std::vector<std::string>&); 

        virtual void scanUSBDevices(std::vector<std::string>&);
        
        virtual ~ViewBase();

};

#endif
