#include "print.hpp"
#include "spdlog/spdlog.h"

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[])
{
    auto console = spdlog::stdout_color_mt("console");
    console->info("calling print");

    print();
    return 0;
}
