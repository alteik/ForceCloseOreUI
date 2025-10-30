//
// Created by alteik on 30.10.2025.
//

#pragma once

#include <functional>
#include <unordered_map>

class OreUIConfig
{
public:
    void* mField1;
    void* mField2;
    std::function<bool()> mFunc1;
    std::function<bool()> mFunc2;
};

class OreUi
{
public:
    std::unordered_map<std::string, OreUIConfig> mConfigs;
};