#include "index-tracker.h"

/**
 * Index Tracker Initialiser
 * 
 * Function that initialises the variables.
 */
IndexTracker::IndexTracker() :
    _mainIdx(0),
    _mainOverflow(0),
    _bulkIdx(0),
    _bulkOverflow(0),
    _settIdx(0),
    _settOverflow(0) {}

/**
 * Index Tracker Get Main Index Function
 * 
 * Function for getting the main index.
 * 
 * @return main index
 */
int IndexTracker::getMainIndex() { return _mainIdx; }

/**
 * Index Tracker Set Main Index Function
 * 
 * Function for setting the main index.
 * 
 * @param idx new index
 */
void IndexTracker::setMainIndex(int idx) { _mainIdx = idx; }

/**
 * Index Tracker Get Main Overflow Function
 * 
 * Function for getting the main overflow.
 * 
 * @return main overflow
 */
int IndexTracker::getMainOverflow() { return _mainOverflow; }

/**
 * Index Tracker Set Main Overflow Function
 * 
 * Function for setting the main overflow.
 * 
 * @param idx new index
 */
void IndexTracker::setMainOverflow(int idx) { _mainOverflow = idx; }

/**
 * Index Tracker Increment Main Index Function
 * 
 * Function for incrementing the main index.
 */
void IndexTracker::incrementMainIndex() { _mainIdx++; }

/**
 * Index Tracker Decrement Main Index Function
 * 
 * Function for decrementing the main index.
 */
void IndexTracker::decrementMainIndex() { _mainIdx--; }

/**
 * Index Tracker Increment Main Overflow Function
 * 
 * Function for incrementing the main overflow.
 */
void IndexTracker::incrementMainOverflow() { _mainOverflow++; }

/**
 * Index Tracker Decrement Main Overflow Function
 * 
 * Function for decrementing the main overflow.
 */
void IndexTracker::decrementMainOverflow() { _mainOverflow--; }

/**
 * Index Tracker Get Bulk Index Function
 * 
 * Function for getting the bulk index.
 * 
 * @return bulk index
 */
int IndexTracker::getBulkIndex() { return _bulkIdx; }

/**
 * Index Tracker Set Bulk Index Function
 * 
 * Function for setting the bulk index.
 * 
 * @param idx new index
 */
void IndexTracker::setBulkIndex(int idx) { _bulkIdx = idx; }

/**
 * Index Tracker Get Bulk Overflow Function
 * 
 * Function for getting the bulk overflow.
 * 
 * @return bulk overflow
 */
int IndexTracker::getBulkOverflow() { return _bulkOverflow; }

/**
 * Index Tracker Set Bulk Overflow Function
 * 
 * Function for setting the bulk overflow.
 * 
 * @param idx new index
 */
void IndexTracker::setBulkOverflow(int idx) { _bulkOverflow = idx; }

/**
 * Index Tracker Increment Bulk Index Function
 * 
 * Function for incrementing the bulk index.
 */
void IndexTracker::incrementBulkIndex() { _bulkIdx++; }

/**
 * Index Tracker Decrement Bulk Index Function
 * 
 * Function for decrementing the bulk index.
 */
void IndexTracker::decrementBulkIndex() { _bulkIdx--; }

/**
 * Index Tracker Increment Bulk Overflow Function
 * 
 * Function for incrementing the bulk overflow.
 */
void IndexTracker::incrementBulkOverflow() { _bulkOverflow++; }

/**
 * Index Tracker Decrement Bulk Overflow Function
 * 
 * Function for decrementing the bulk overflow.
 */
void IndexTracker::decrementBulkOverflow() { _bulkOverflow--; }

/**
 * Index Tracker Get Settings Index Function
 * 
 * Function for getting the settings index.
 * 
 * @return settings index
 */
int IndexTracker::getSettingsIndex() { return _settIdx; }

/**
 * Index Tracker Set Settings Index Function
 * 
 * Function for setting the settings index.
 * 
 * @param idx new index
 */
void IndexTracker::setSettingsIndex(int idx) { _settIdx = idx; }

/**
 * Index Tracker Get Settings Overflow Function
 * 
 * Function for getting the settings overflow.
 * 
 * @return settings overflow
 */
int IndexTracker::getSettingsOverflow() { return _settOverflow; }

/**
 * Index Tracker Set Settings Overflow Function
 * 
 * Function for setting the settings Overflow.
 * 
 * @param idx new index
 */
void IndexTracker::setSettingsOverflow(int idx) { _settOverflow = idx; }

/**
 * Index Tracker Increment Settings Index Function
 * 
 * Function for incrementing the settings index.
 */
void IndexTracker::incrementSettingsIndex() { _settIdx++; }

/**
 * Index Tracker Decrement Settings Index Function
 * 
 * Function for decrementing the settings index.
 */
void IndexTracker::decrementSettingsIndex() { _settIdx--; }

/**
 * Index Tracker Increment Settings Overflow Function
 * 
 * Function for incrementing the settings overflow.
 */
void IndexTracker::incrementSettingsOverflow() { _settOverflow++; }

/**
 * Index Tracker Decrement Settings Overflow Function
 * 
 * Function for decrementing the settings overflow.
 */
void IndexTracker::decrementSettingsOverflow() { _settOverflow--; }