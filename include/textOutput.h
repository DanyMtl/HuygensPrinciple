
#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H




#include <string>
#include <map>

std::string getSystemLanguage() ;

namespace textOutput {

    void setLanguage(const std::string& lang);
    void getCurrentLanguage();

    std::string getText(const std::string& key);
}

#endif // LANGUAGEMANAGER_H

