#include <cppspring/core/batch_status.hpp>
#include <stdexcept>

namespace cppspring::core {

    std::string to_string(BatchStatus status) {
        switch (status) {
        case BatchStatus::COMPLETED:  return "COMPLETED";
        case BatchStatus::STARTING:   return "STARTING";
        case BatchStatus::STARTED:    return "STARTED";
        case BatchStatus::STOPPING:   return "STOPPING";
        case BatchStatus::STOPPED:    return "STOPPED";
        case BatchStatus::FAILED:     return "FAILED";
        case BatchStatus::ABANDONED:  return "ABANDONED";
        case BatchStatus::UNKNOWN:    return "UNKNOWN";
        default:
            throw std::invalid_argument("Unknown BatchStatus");
        }
    }

    BatchStatus from_string(const std::string& status_str) {
        if (status_str == "COMPLETED")  return BatchStatus::COMPLETED;
        if (status_str == "STARTING")   return BatchStatus::STARTING;
        if (status_str == "STARTED")    return BatchStatus::STARTED;
        if (status_str == "STOPPING")   return BatchStatus::STOPPING;
        if (status_str == "STOPPED")    return BatchStatus::STOPPED;
        if (status_str == "FAILED")     return BatchStatus::FAILED;
        if (status_str == "ABANDONED")  return BatchStatus::ABANDONED;
        if (status_str == "UNKNOWN")    return BatchStatus::UNKNOWN;

        throw std::invalid_argument("Unknown status string: " + status_str);
    }

} // namespace cppspring::core