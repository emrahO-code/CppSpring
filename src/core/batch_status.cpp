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

    bool is_running(BatchStatus status) {
        return status == BatchStatus::STARTING || status == BatchStatus::STARTED || status == BatchStatus::STOPPING;
    }

    bool is_unsuccessful(BatchStatus status) {
        return status == BatchStatus::FAILED || status == BatchStatus::ABANDONED || status == BatchStatus::STOPPED;
    }

    int get_priority(BatchStatus status) {
        // Reverse order of enum declaration - worst statuses have highest priority
        switch (status) {
        case BatchStatus::UNKNOWN:    return 7;  // Worst
        case BatchStatus::ABANDONED:  return 6;
        case BatchStatus::FAILED:     return 5;
        case BatchStatus::STOPPED:    return 4;
        case BatchStatus::STOPPING:   return 3;
        case BatchStatus::STARTED:    return 2;
        case BatchStatus::STARTING:   return 1;
        case BatchStatus::COMPLETED:  return 0;  // Best
        default:
            return 7; // Treat unknown values as worst case
        }
    }

    bool is_greater_than(BatchStatus status1, BatchStatus status2) {
        return get_priority(status1) > get_priority(status2);
    }

    bool is_less_than(BatchStatus status1, BatchStatus status2) {
        return get_priority(status1) < get_priority(status2);
    }

    bool is_less_than_or_equal_to(BatchStatus status1, BatchStatus status2) {
        return get_priority(status1) <= get_priority(status2);
    }

    BatchStatus max(BatchStatus status1, BatchStatus status2) {
        return is_greater_than(status1, status2) ? status1 : status2;
    }

    std::vector<BatchStatus> get_all_values() {
        return {
            BatchStatus::COMPLETED,
            BatchStatus::STARTING,
            BatchStatus::STARTED,
            BatchStatus::STOPPING,
            BatchStatus::STOPPED,
            BatchStatus::FAILED,
            BatchStatus::ABANDONED,
            BatchStatus::UNKNOWN
        };
    }

    BatchStatus match(const std::string& value) {
        // Iterate through all BatchStatus values
        for (BatchStatus status : get_all_values()) {
            std::string status_str = to_string(status);

            // Check if input string starts with this status string
            if (value.length() >= status_str.length() &&
                value.substr(0, status_str.length()) == status_str) {
                return status;
                }
        }

        // Default match should be the lowest priority (COMPLETED)
        return BatchStatus::COMPLETED;
    }

    BatchStatus upgrade_to(BatchStatus status1, BatchStatus status2) {
        if (is_greater_than(status1, BatchStatus::STARTED) ||
        is_greater_than(status2, BatchStatus::STARTED)) {
            return max(status1, status2);
        }
        // Both are "STARTED or better" (COMPLETED, STARTING, STARTED)
        // Special case: if either is COMPLETED, prefer COMPLETED
        if (status1 == BatchStatus::COMPLETED || status2 == BatchStatus::COMPLETED) {
            return BatchStatus::COMPLETED;
        }
        // Both are STARTING or STARTED, use normal max
        return max(status1, status2);
    }

} // namespace cppspring::core