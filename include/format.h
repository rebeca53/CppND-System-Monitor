#ifndef FORMAT_H
#define FORMAT_H

#include <string>

/**
 * Convert time in seconds to a string in the format HH:MM:SS
 */
namespace Format {
std::string ElapsedTime(long times);
};  // namespace Format

#endif