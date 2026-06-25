// Engine\Core\Logger.cpp
#include "Logger.h"
#include <iostream>

void Logger::init()
{
    std::cout << "=== Void Conquer Logger initialized ===\n";
}

void Logger::log(const std::string& message)
{
    std::cout << "[LOG] " << message << std::endl;
}

void Logger::error(const std::string& message)
{
    std::cout << "[ERROR] " << message << std::endl;
}