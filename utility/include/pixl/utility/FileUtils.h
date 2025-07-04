#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"

#include <string>
#include <sstream>
#include <vector>
#include <iostream>

namespace px
{
    namespace FileUtils
    {
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
    }
}