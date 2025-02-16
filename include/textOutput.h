
#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <string>
#include <map>

namespace textOutput {

    void setLanguage(const std::string& lang);
    std::string getCurrentLanguage();

    std::string getText(const std::string& key);
}

#endif // LANGUAGEMANAGER_H

