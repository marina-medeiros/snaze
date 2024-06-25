#ifndef _DATA_H_
#define _DATA_H_

#include <string>
#include <unordered_map>

class Data{
private:
    std::unordered_map<std::string, std::string> variablesAndValues;
    std::string iniPath;
public:
    Data(const std::string& iniFile) : iniPath(iniFile) {read_ini(iniFile);}
    void read_ini(std::string iniFile);
    const std::unordered_map<std::string, std::string>& get_variablesAndValues() const {
        return variablesAndValues;
    }
};

#endif