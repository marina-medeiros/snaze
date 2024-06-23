#ifndef _DATA_H_
#define _DATA_H_

#include <array>
#include <set>
#include <string>
#include <unordered_map>
#include <map>


class Data{
private:
    std::unordered_map<std::string, std::string> variablesAndValues;
    std::string iniPath;
public:
    Data(const std::string& iniFile) : iniPath(iniFile) {read_ini(iniFile);}
    void read_ini(std::string iniFile);// Getter for variablesAndValues
    const std::unordered_map<std::string, std::string>& get_variablesAndValues() const {
        return variablesAndValues;
    }
};

#endif