#include "config.h"

/**
 * Index Tracker Class
 * 
 * Class object used to track the indices of various screens.
 */
class IndexTracker {
    private:
        int _mainIdx;       // Main Menu Index
        int _mainOverflow;  // Main Overflow Tracker
        int _bulkIdx;       // Bulk Index
        int _bulkOverflow;  // Bulk Overflow Tracker
        int _settIdx;       // Settings Index
        int _settOverflow;  // Settings Overflow Tracker
    public:
        IndexTracker();
        int getMainIndex();
        void setMainIndex(int);
        int getMainOverflow();
        void setMainOverflow(int);
        void incrementMainIndex();
        void decrementMainIndex();
        void incrementMainOverflow();
        void decrementMainOverflow();
        int getBulkIndex();
        void setBulkIndex(int);
        int getBulkOverflow();
        void setBulkOverflow(int);
        void incrementBulkIndex();
        void decrementBulkIndex();
        void incrementBulkOverflow();
        void decrementBulkOverflow();
        int getSettingsIndex();
        void setSettingsIndex(int);
        int getSettingsOverflow();
        void setSettingsOverflow(int);
        void incrementSettingsIndex();
        void decrementSettingsIndex();
        void incrementSettingsOverflow();
        void decrementSettingsOverflow();
};