#include <iostream>
#include <cassert>
#include <iomanip>
#include <cppspring/cppspring.hpp>

using namespace cppspring::core;

void test_string_conversions() {
    std::cout << "=== Testing String Conversions ===" << std::endl;

    // Test to_string
    assert(to_string(BatchStatus::COMPLETED) == "COMPLETED");
    assert(to_string(BatchStatus::FAILED) == "FAILED");
    assert(to_string(BatchStatus::UNKNOWN) == "UNKNOWN");
    std::cout << "✅ to_string() works correctly" << std::endl;

    // Test from_string
    assert(from_string("COMPLETED") == BatchStatus::COMPLETED);
    assert(from_string("FAILED") == BatchStatus::FAILED);
    assert(from_string("UNKNOWN") == BatchStatus::UNKNOWN);
    std::cout << "✅ from_string() works correctly" << std::endl;

    // Test round-trip conversion
    for (auto status : get_all_values()) {
        assert(from_string(to_string(status)) == status);
    }
    std::cout << "✅ Round-trip conversion works for all statuses" << std::endl;

    // Test invalid string (should throw)
    try {
        from_string("INVALID_STATUS");
        assert(false && "Should have thrown exception");
    } catch (const std::invalid_argument&) {
        std::cout << "✅ from_string() properly throws on invalid input" << std::endl;
    }
}

void test_status_categories() {
    std::cout << "\n=== Testing Status Categories ===" << std::endl;

    // Test is_running
    assert(is_running(BatchStatus::STARTING) == true);
    assert(is_running(BatchStatus::STARTED) == true);
    assert(is_running(BatchStatus::STOPPING) == true);
    assert(is_running(BatchStatus::COMPLETED) == false);
    assert(is_running(BatchStatus::FAILED) == false);
    std::cout << "✅ is_running() works correctly" << std::endl;

    // Test is_unsuccessful
    assert(is_unsuccessful(BatchStatus::FAILED) == true);
    assert(is_unsuccessful(BatchStatus::ABANDONED) == true);
    assert(is_unsuccessful(BatchStatus::STOPPED) == true);
    assert(is_unsuccessful(BatchStatus::COMPLETED) == false);
    assert(is_unsuccessful(BatchStatus::STARTED) == false);
    std::cout << "✅ is_unsuccessful() works correctly" << std::endl;
}

void test_priority_system() {
    std::cout << "\n=== Testing Priority System ===" << std::endl;

    // Test priority order (higher number = worse/higher priority)
    assert(get_priority(BatchStatus::UNKNOWN) > get_priority(BatchStatus::FAILED));
    assert(get_priority(BatchStatus::FAILED) > get_priority(BatchStatus::COMPLETED));
    assert(get_priority(BatchStatus::ABANDONED) > get_priority(BatchStatus::COMPLETED));
    std::cout << "✅ get_priority() returns correct hierarchy" << std::endl;

    // Test is_greater_than
    assert(is_greater_than(BatchStatus::FAILED, BatchStatus::COMPLETED) == true);
    assert(is_greater_than(BatchStatus::UNKNOWN, BatchStatus::FAILED) == true);
    assert(is_greater_than(BatchStatus::COMPLETED, BatchStatus::FAILED) == false);
    std::cout << "✅ is_greater_than() works correctly" << std::endl;

    // Test is_less_than
    assert(is_less_than(BatchStatus::COMPLETED, BatchStatus::FAILED) == true);
    assert(is_less_than(BatchStatus::FAILED, BatchStatus::COMPLETED) == false);
    std::cout << "✅ is_less_than() works correctly" << std::endl;
}

void test_max_function() {
    std::cout << "\n=== Testing Max Function ===" << std::endl;

    // Test max picks the worse status
    assert(max(BatchStatus::COMPLETED, BatchStatus::FAILED) == BatchStatus::FAILED);
    assert(max(BatchStatus::FAILED, BatchStatus::COMPLETED) == BatchStatus::FAILED);
    assert(max(BatchStatus::ABANDONED, BatchStatus::COMPLETED) == BatchStatus::ABANDONED);
    assert(max(BatchStatus::STARTED, BatchStatus::STOPPED) == BatchStatus::STOPPED);
    assert(max(BatchStatus::COMPLETED, BatchStatus::COMPLETED) == BatchStatus::COMPLETED);
    std::cout << "✅ max() always returns the worse status" << std::endl;

    // Test all combinations to ensure worst status always wins
    auto all_statuses = get_all_values();
    for (auto s1 : all_statuses) {
        for (auto s2 : all_statuses) {
            auto result = max(s1, s2);
            // Result should be the one with higher priority (worse status)
            assert(get_priority(result) >= get_priority(s1));
            assert(get_priority(result) >= get_priority(s2));
        }
    }
    std::cout << "✅ max() works correctly for all status combinations" << std::endl;
}

void test_match_function() {
    std::cout << "\n=== Testing Match Function ===" << std::endl;

    // Test exact matches
    assert(match("COMPLETED") == BatchStatus::COMPLETED);
    assert(match("FAILED") == BatchStatus::FAILED);
    assert(match("STARTED") == BatchStatus::STARTED);
    std::cout << "✅ match() handles exact matches" << std::endl;

    // Test prefix matches
    assert(match("FAILED: Database timeout") == BatchStatus::FAILED);
    assert(match("COMPLETED with 1000 records") == BatchStatus::COMPLETED);
    assert(match("STARTED at 10:30 AM") == BatchStatus::STARTED);
    std::cout << "✅ match() handles prefix matches" << std::endl;

    // Test no match (should return COMPLETED as default)
    assert(match("INVALID_STATUS") == BatchStatus::COMPLETED);
    assert(match("random text") == BatchStatus::COMPLETED);
    assert(match("") == BatchStatus::COMPLETED);
    std::cout << "✅ match() returns COMPLETED for invalid inputs" << std::endl;

    // Test case sensitivity
    assert(match("completed") == BatchStatus::COMPLETED); // Should work - lowercase 'completed' starts with empty string, defaults to COMPLETED
    assert(match("failed") == BatchStatus::COMPLETED);    // Should default to COMPLETED since no exact match
    std::cout << "✅ match() is case sensitive as expected" << std::endl;
}

void test_upgrade_function() {
    std::cout << "\n=== Testing Upgrade Function ===" << std::endl;

    // Test critical case: COMPLETED should NOT win over error statuses
    assert(upgrade_to(BatchStatus::COMPLETED, BatchStatus::FAILED) == BatchStatus::FAILED);
    assert(upgrade_to(BatchStatus::FAILED, BatchStatus::COMPLETED) == BatchStatus::FAILED);
    std::cout << "✅ upgrade_to() properly handles COMPLETED vs error statuses" << std::endl;

    // Test COMPLETED preference among "good" statuses
    assert(upgrade_to(BatchStatus::STARTING, BatchStatus::COMPLETED) == BatchStatus::COMPLETED);
    assert(upgrade_to(BatchStatus::COMPLETED, BatchStatus::STARTING) == BatchStatus::COMPLETED);
    assert(upgrade_to(BatchStatus::STARTED, BatchStatus::COMPLETED) == BatchStatus::COMPLETED);
    std::cout << "✅ upgrade_to() prefers COMPLETED among good statuses" << std::endl;

    // Test normal progression
    assert(upgrade_to(BatchStatus::STARTING, BatchStatus::STARTED) == BatchStatus::STARTED);
    assert(upgrade_to(BatchStatus::STARTED, BatchStatus::STARTING) == BatchStatus::STARTED);
    std::cout << "✅ upgrade_to() handles normal status progression" << std::endl;

    // Test error statuses always win
    assert(upgrade_to(BatchStatus::STARTED, BatchStatus::STOPPED) == BatchStatus::STOPPED);
    assert(upgrade_to(BatchStatus::COMPLETED, BatchStatus::ABANDONED) == BatchStatus::ABANDONED);
    assert(upgrade_to(BatchStatus::STARTING, BatchStatus::UNKNOWN) == BatchStatus::UNKNOWN);
    std::cout << "✅ upgrade_to() ensures error statuses always take precedence" << std::endl;
}

void print_priority_table() {
    std::cout << "\n=== Priority Table (Higher = Worse) ===" << std::endl;
    std::cout << "Status        | Priority" << std::endl;
    std::cout << "------------- | --------" << std::endl;

    for (auto status : get_all_values()) {
        std::cout << std::left << std::setw(13) << to_string(status)
                  << " | " << get_priority(status) << std::endl;
    }
}

void print_status_categories() {
    std::cout << "\n=== Status Categories ===" << std::endl;
    std::cout << "Status        | Running | Unsuccessful" << std::endl;
    std::cout << "------------- | ------- | ------------" << std::endl;

    for (auto status : get_all_values()) {
        std::cout << std::left << std::setw(13) << to_string(status)
                  << " | " << std::setw(7) << (is_running(status) ? "Yes" : "No")
                  << " | " << (is_unsuccessful(status) ? "Yes" : "No") << std::endl;
    }
}

int main() {
    std::cout << "🚀 CppSpring BatchStatus Comprehensive Test" << std::endl;
    std::cout << "============================================" << std::endl;

    try {
        test_string_conversions();
        test_status_categories();
        test_priority_system();
        test_max_function();
        test_match_function();
        test_upgrade_function();

        print_priority_table();
        print_status_categories();

        std::cout << "\n🎉 ALL TESTS PASSED! BatchStatus implementation is working perfectly!" << std::endl;
        std::cout << "\nYour BatchStatus class is ready for production use! ✨" << std::endl;

    } catch (const std::exception& e) {
        std::cout << "\n❌ TEST FAILED: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}