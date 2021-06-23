#pragma once

#include <fstream>
#include <exception>
#include <iostream>
#include <iomanip>
#include <nlohmann/json.hpp>

#define ConfigFilePath "assets/.config"

using json = nlohmann::json;

class Settings {
public:
    Settings();

    void setValue(const ::std::string &key, const json &value);

    json getValue(const ::std::string &key);

    static Settings *singleton();

protected:
    static Settings *m_instance;

private:
    json m_settings;
};

#define settings Settings::singleton()


