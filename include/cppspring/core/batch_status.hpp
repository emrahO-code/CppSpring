#pragma once
#include <string>
#include <vector>

namespace cppspring::core {

     /*
      * The order of the status values is significant because it can be used to aggregate a
      * set of status values. The result should be the maximum value. Since {@code
      * COMPLETED} is first in the order, only if all elements of an execution are {@code
      * COMPLETED} can the aggregate status be COMPLETED. A running execution is expected
      * to move from {@code STARTING} to {@code STARTED} to {@code COMPLETED} (through the
      * order defined by {@link #upgradeTo(BatchStatus)}). Higher values than {@code
      * STARTED} signify more serious failures. {@code ABANDONED} is used for steps that
      * have finished processing but were not successful and where they should be skipped
      * on a restart (so {@code FAILED} is the wrong status).
      */
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

    //Booleans
    bool is_running(BatchStatus status);
    bool is_unsuccessful(BatchStatus status);
    bool is_greater_than(BatchStatus status1, BatchStatus status2);
    bool is_less_than(BatchStatus status1, BatchStatus status2);
    bool is_less_than_or_equal_to(BatchStatus status1, BatchStatus status2);

    //priority list
    int get_priority(BatchStatus status);

    BatchStatus max(BatchStatus status1, BatchStatus status2);
    BatchStatus upgrade_to(BatchStatus status1, BatchStatus status2);
    BatchStatus match(const std::string& value);
    std::vector<BatchStatus> get_all_values();

} // namespace cppspring::core