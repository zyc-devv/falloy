#include <iostream>
#include <fstream>
#include <map>
#include <cstdlib>
#include "include/CLI11.hpp"
#include "include/nlohmann/json.hpp"

#ifndef APP_VERSION
#define APP_VERSION "0.0.0-unknown"
#endif

std::map<std::string, std::string> loadLanguagePack(const std::string& locale) {
    std::string filePath = "locales/" + locale + ".json";
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Warning: Language pack '" << locale << "' not found, falling back to en-US.\n";
        file.open("locales/en-US.json");
        if (!file.is_open()) {
            std::cerr << "Fatal: Default language pack 'en-US' also missing.\n";
            return {};
        }
    }

    try {
        nlohmann::json j;
        file >> j;
        return j.get<std::map<std::string, std::string>>();
    } catch (const std::exception& e) {
        std::cerr << "Error loading language pack: " << e.what() << "\n";
        return {};
    }
}

int main(int argc, char** argv) {
    std::string lang = "en-US";
    const char* langEnv = getenv("LANG");
    if (langEnv && std::string(langEnv).find("zh") != std::string::npos) {
        lang = "zh-CN";
    }

    auto messages = loadLanguagePack(lang);

    CLI::App app{messages.count("app_description") ? messages.at("app_description") : "Compiler Tool"};

    app.get_formatter()->label("REQUIRED", messages.count("required_label") ? messages.at("required_label") : "(REQUIRED)");
    app.get_formatter()->label("OPTIONAL", messages.count("optional_label") ? messages.at("optional_label") : "(OPTIONAL)");

    bool verbose = false;
    std::string output;
    app.add_flag("-v,--verbose", verbose,
                 messages.count("verbose_desc") ? messages.at("verbose_desc") : "Enable verbose output");
    app.add_option("-o,--output", output,
                   messages.count("output_desc") ? messages.at("output_desc") : "Output file path");

    app.set_version_flag("--version", APP_VERSION);

    CLI11_PARSE(app, argc, argv);

    return 0;
}