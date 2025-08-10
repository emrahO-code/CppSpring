#pragma once
#include <string>

namespace cppspring::core {

    enum class BatchStatus {
        COMPLETED,
        STARTING,
        STARTED,
        STOPPING,
        STOPPED,
        FAILED,
        ABANDONED,
        UNKNOWN
    };

    // Helper functions
    std::string to_string(BatchStatus status);
    BatchStatus from_string(const std::string& status_str);

} // namespace cppspring::core