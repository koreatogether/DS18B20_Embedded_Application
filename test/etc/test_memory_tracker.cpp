#include <unity.h>
#include "../mocks/MockMemoryTracker.h"
#include "../mocks/MockMemoryAnalyzer.h"
#include "../../src/application/MemoryMonitorService.h"
#include "../../src/application/MemoryTrackerService.h"

// Test memory tracker interface
void test_memory_tracker_interface()
{
    MockMemoryTracker tracker;

    // Initially not tracking
    TEST_ASSERT_FALSE(tracker.isTracking());
    TEST_ASSERT_EQUAL(0, tracker.getSnapshotCount());

    // Start tracking
    tracker.startTracking();
    TEST_ASSERT_TRUE(tracker.isTracking());

    // Add snapshots - only recorded when tracking
    MemorySnapshot snapshot1;
    snapshot1.timestamp = 1000;
    snapshot1.freeMemory = 2000;
    snapshot1.usedMemory = 500;
    snapshot1.totalMemory = 2500;
    snapshot1.fragmentationLevel = 10;

    tracker.recordSnapshot(snapshot1);
    TEST_ASSERT_EQUAL(1, tracker.getSnapshotCount());

    // Stop tracking
    tracker.stopTracking();
    TEST_ASSERT_FALSE(tracker.isTracking());

    // Snapshots not recorded when not tracking
    MemorySnapshot snapshot2;
    snapshot2.timestamp = 2000;
    snapshot2.freeMemory = 1800;
    snapshot2.usedMemory = 700;

    tracker.recordSnapshot(snapshot2);
    TEST_ASSERT_EQUAL(1, tracker.getSnapshotCount()); // Still 1, not 2

    // Clear history
    tracker.clearHistory();
    TEST_ASSERT_EQUAL(0, tracker.getSnapshotCount());
}

void test_memory_tracker_report_generation()
{
    MockMemoryTracker tracker;
    tracker.startTracking();

    // Add multiple snapshots
    tracker.addMockSnapshot(1000, 2000, 500); // Free: 2000, Used: 500
    tracker.addMockSnapshot(2000, 1800, 700); // Free: 1800, Used: 700
    tracker.addMockSnapshot(3000, 1900, 600); // Free: 1900, Used: 600

    MemoryReport report = tracker.generateReport();

    TEST_ASSERT_EQUAL(3, report.totalSnapshots);
    TEST_ASSERT_EQUAL(1000, report.startSnapshot.timestamp);
    TEST_ASSERT_EQUAL(3000, report.endSnapshot.timestamp);
    TEST_ASSERT_EQUAL(2000, report.startSnapshot.freeMemory);
    TEST_ASSERT_EQUAL(1900, report.endSnapshot.freeMemory);

    // Min/Max memory
    TEST_ASSERT_EQUAL(1800, report.minFreeMemory); // Minimum free memory
    TEST_ASSERT_EQUAL(2000, report.maxFreeMemory); // Maximum free memory
}

void test_memory_tracker_csv_export()
{
    MockMemoryTracker tracker;
    tracker.startTracking();

    tracker.addMockSnapshot(1000, 2000, 500);
    tracker.addMockSnapshot(2000, 1800, 700);

    std::string csv = tracker.exportToCsv();

    // Check header
    TEST_ASSERT_TRUE(csv.find("Timestamp,FreeMemory,UsedMemory,TotalMemory,FragmentationLevel") != std::string::npos);

    // Check data rows
    TEST_ASSERT_TRUE(csv.find("1000,2000,500,2500") != std::string::npos);
    TEST_ASSERT_TRUE(csv.find("2000,1800,700,2500") != std::string::npos);
}

void test_memory_tracker_markdown_export()
{
    MockMemoryTracker tracker;
    tracker.startTracking();

    tracker.addMockSnapshot(1000, 2000, 500);
    tracker.addMockSnapshot(2000, 1800, 700);

    std::string markdown = tracker.exportToMarkdown();

    // Check markdown format
    TEST_ASSERT_TRUE(markdown.find("# Memory Tracking Report") != std::string::npos);
    TEST_ASSERT_TRUE(markdown.find("| Timestamp | Free Memory") != std::string::npos);
    TEST_ASSERT_TRUE(markdown.find("| 1000 | 2000") != std::string::npos);
    TEST_ASSERT_TRUE(markdown.find("| 2000 | 1800") != std::string::npos);
}

void test_memory_tracker_service_integration()
{
    // Test MemoryTrackerService implementation
    MemoryTrackerService trackerService;

    // Initially not tracking
    TEST_ASSERT_FALSE(trackerService.isTracking());

    // Start tracking
    trackerService.startTracking();
    TEST_ASSERT_TRUE(trackerService.isTracking());

    // Add snapshots
    MemorySnapshot snapshot1;
    snapshot1.timestamp = 1000;
    snapshot1.freeMemory = 2000;
    snapshot1.usedMemory = 500;
    snapshot1.totalMemory = 2500;
    snapshot1.fragmentationLevel = 10;

    trackerService.recordSnapshot(snapshot1);

    std::vector<MemorySnapshot> snapshots = trackerService.getSnapshots();
    TEST_ASSERT_EQUAL(1, snapshots.size());
    TEST_ASSERT_EQUAL(1000, snapshots[0].timestamp);
    TEST_ASSERT_EQUAL(2000, snapshots[0].freeMemory);

    // Stop tracking
    trackerService.stopTracking();
    TEST_ASSERT_FALSE(trackerService.isTracking());
}

void test_memory_monitor_with_tracker_integration()
{
    MockMemoryAnalyzer analyzer;
    MemoryTrackerService tracker;
    MemoryMonitorService monitor(&analyzer, &tracker);

    // Initially tracking is off
    TEST_ASSERT_FALSE(tracker.isTracking());

    // Toggle tracking on
    std::string result = monitor.toggleTracking();
    TEST_ASSERT_TRUE(result.find("enabled") != std::string::npos);
    TEST_ASSERT_TRUE(tracker.isTracking());

    // Toggle tracking off
    result = monitor.toggleTracking();
    TEST_ASSERT_TRUE(result.find("disabled") != std::string::npos);
    TEST_ASSERT_FALSE(tracker.isTracking());
}

void test_memory_report_generation_integration()
{
    MockMemoryAnalyzer analyzer;
    MemoryTrackerService tracker;
    MemoryMonitorService monitor(&analyzer, &tracker);

    // Start tracking
    tracker.startTracking();

    // Simulate some memory snapshots
    MemorySnapshot snapshot1;
    snapshot1.timestamp = 1000;
    snapshot1.freeMemory = 2000;
    snapshot1.usedMemory = 500;
    tracker.recordSnapshot(snapshot1);

    MemorySnapshot snapshot2;
    snapshot2.timestamp = 2000;
    snapshot2.freeMemory = 1800;
    snapshot2.usedMemory = 700;
    tracker.recordSnapshot(snapshot2);

    // Get memory report
    std::string report = monitor.getMemoryReport();

    // Check report content
    TEST_ASSERT_TRUE(report.find("Memory Tracking Report") != std::string::npos);
    TEST_ASSERT_TRUE(report.find("Total Snapshots: 2") != std::string::npos);
    TEST_ASSERT_TRUE(report.find("Start Time: 1000") != std::string::npos);
    TEST_ASSERT_TRUE(report.find("End Time: 2000") != std::string::npos);
}

void setUp(void)
{
    // Set up test environment if needed
}

void tearDown(void)
{
    // Clean up after each test
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_memory_tracker_interface);
    RUN_TEST(test_memory_tracker_report_generation);
    RUN_TEST(test_memory_tracker_csv_export);
    RUN_TEST(test_memory_tracker_markdown_export);
    RUN_TEST(test_memory_tracker_service_integration);
    RUN_TEST(test_memory_monitor_with_tracker_integration);
    RUN_TEST(test_memory_report_generation_integration);

    return UNITY_END();
}
