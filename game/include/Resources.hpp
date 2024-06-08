#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include "../../include/GravyEngine.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <cstdint>

class Resources
{
public:
    static void LoadAll();
    static uint8_t *GetData(const std::string &filename, size_t &size);
private:
    static std::unique_ptr<ResourcePack> pack;
    static std::unordered_map<std::string,std::vector<uint8_t>> data;
};

#endif