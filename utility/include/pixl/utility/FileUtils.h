#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"

#include <string>
#include <sstream>
#include <vector>
#include <iostream>

namespace px
{
    class FileUtils
    {
    public:
        /**
         * @brief Combines multiple path sections into one full path with the OS's separator. (e.g.: CombinePaths("assets", "images", "beans.png") will become "assets/images/beans.png")
         * 
         * @param first First part of the path.
         * @param other Other parts of the path.
         * 
         * @returns The combined path.
         */
        template<typename... Paths>
        static std::string CombinePaths(CREFSTR first, const Paths&... other)
        {
            std::vector<std::string> parts = { first, other... };
            std::ostringstream combined;

            for (size_t i = 0; i < parts.size(); ++i) {
                std::string part = parts[i];

                if (i > 0 && !part.empty() && part.front() == PX_SEPARATOR)
                    part.erase(0, 1);

                if (!part.empty() && part.back() == PX_SEPARATOR && i != parts.size() - 1)
                    part.pop_back();

                if (i != 0 && !combined.str().empty() && combined.str().back() != PX_SEPARATOR)
                    combined << PX_SEPARATOR;

                combined << part;
            }

            return combined.str();
        }

        PX_API static bool FileExists(CREFSTR path);
        PX_API static bool DirectoryExists(CREFSTR path);
    };
}