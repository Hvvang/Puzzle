//
// Created by Artem Shemidko on 6/22/21.
//

#include "Settings.hpp"

Settings *Settings::m_instance = nullptr;

Settings::Settings() {
    std::ifstream file(ConfigFilePath);
    if (!file.is_open()) {
        throw std::invalid_argument(ConfigFilePath);
    }
    m_settings = json::parse(file);
    file.close();
    m_instance = this;
}

void Settings::setValue(const std::string &key, const json &value) {
    m_settings[key] = value;

    std::fstream file(ConfigFilePath);
    if (!file.is_open()) {
        throw std::invalid_argument(ConfigFilePath);
    }
    file << m_settings.dump(4);
    file.close();
}

json Settings::getValue(const std::string &key) {
    return m_settings[key];
}

Settings *Settings::singleton() {
    if (!m_instance) {
        m_instance = new Settings;
    }
    return m_instance;
}
