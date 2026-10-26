#include "settings.h"

/**
 * Settings Initialiser
 * 
 * Sets up the Settings Object.
 */
Settings::Settings() :
    _bulkOptions DEFAULT_BULK_OPTIONS {}

/**
 * Settings Begin Function
 * 
 * Used in main.cpp's setup function, this function is used to 
 * initialise the project's preferences object, and to initialise
 * the class variables such as the bulk-options array, the size of
 * the array, and the hold time.
 */
void Settings::begin() {
    // Initialise Project Preferences
    pref.begin(PROJECT_KEY, false);

    // pref.remove(BULK_OPTIONS_KEY); // remove this line
    // Handle Bulk Options
    if (pref.isKey(BULK_OPTIONS_KEY)) {
        // Extract values from Preferences and Fill Array
        pref.getBytes(BULK_OPTIONS_KEY, _bulkOptions, MAX_BULK_OPTIONS);
        _bulkOptionCount = countBulkOptions(); // Count non-zero entries
    } else {
        // Save default bulk options
        putBulkOptions(_bulkOptions); // save to preferences
    }
    
    // pref.remove(HOLD_POS_KEY); // remove this line
    // Handle Hold Position
    if (pref.isKey(HOLD_POS_KEY)) {
        // Extract hold position from Preferences
        _holdPos = pref.getLong(HOLD_POS_KEY);
    } else {
        // Save default hold position
        putHoldPosition(DEFAULT_HOLD_POS);
    }

    // pref.remove(RELEASE_POS_KEY); // remove this line
    // Handle Release Position
    if (pref.isKey(RELEASE_POS_KEY)) {
        // Extract release position from Preferences
        _releasePos = pref.getLong(RELEASE_POS_KEY);
    } else {
        // Save default release position
        putReleasePosition(DEFAULT_RELEASE_POS);
    }

    // pref.remove(HOLD_TIME_KEY); // remove this line
    // Handle Hold Time
    if (pref.isKey(HOLD_TIME_KEY)) {
        // Extract hold time from Preferences
        _holdTimeMs = pref.getULong(HOLD_TIME_KEY);
    } else {
        // Save default hold time
        putHoldTime(DEFAULT_HOLD_TIME);
    }

    // pref.remove(MOTOR_PULSE_WIDTH_KEY); // remove this line
    // Handle Motor Pulse Width
    if (pref.isKey(MOTOR_PULSE_WIDTH_KEY)) {
        // Extract motor pulse width from Preferences
        _pulseWidth = pref.getInt(MOTOR_PULSE_WIDTH_KEY);
    } else {
        // Save default pulse width
        putMotorPulseWidth(DEFAULT_MOTOR_PULSE_WIDTH);
    }

    // pref.remove(MOTOR_MAX_SPEED_KEY); // remove this line
    // Handle Motor Max Speed
    if (pref.isKey(MOTOR_MAX_SPEED_KEY)) {
        // Extract motor max speed from Preferences
        _maxSpeed = pref.getFloat(MOTOR_MAX_SPEED_KEY);
    } else {
        // Save default max speed
        putMotorMaxSpeed(DEFAULT_MOTOR_MAX_SPEED);
    }

    // pref.remove(MOTOR_ACCELERATION_KEY); // remove this line
    // Handle Motor Acceleration
    if (pref.isKey(MOTOR_ACCELERATION_KEY)) {
        // Extract motor acceleration from Preferences
        _acceleration = pref.getFloat(MOTOR_ACCELERATION_KEY);
    } else {
        // Save default acceleration
        putMotorAcceleration(DEFAULT_MOTOR_ACCELERATION);
    }

    // pref.remove(HALL_SENSITIVITY_KEY); // remove this line
    // Handle Hall Sensor Sensitivity
    if (pref.isKey(HALL_SENSITIVITY_KEY)) {
        // Extract hall sensor sensitivity from Preferences
        _hallSensitivity = pref.getInt(HALL_SENSITIVITY_KEY);
    } else {
        // Save default hall sensor sensitivity
        putHallSensitivity(DEFAULT_HALL_SENSITIVITY);
    }

    // Handle Help Time
    if (pref.isKey(HELP_TIME_KEY)) {
        // Extract help time from Preferences
        _helpTimeMs = pref.getULong(HELP_TIME_KEY);
    } else {
        // Save defualt help time
        putHelpTime(DEFAULT_HELP_TIME);
    }

    // Handle Joystick Sensitivity
    // TODO: handle joystick sensitivity
}

/**
 * Settings Get Bulk Options Function
 * 
 * Used to fill pointer array with non-zero entries from array 
 * saved in preferences.
 *
 * @param arr pointer array
 */
void Settings::getBulkOptions(int* arr) {
    for (int i = 0; i < _bulkOptionCount; i++) {
        arr[i] = _bulkOptions[i];
    }
}

/**
 * Settings Get Bulk Option Count Function
 * 
 * Used to get the non-zero entry count from saved array.
 * 
 * @return non-zero count
 */
size_t Settings::getBulkOptionCount() { return _bulkOptionCount; }

/**
 * Settings Put Bulk Options Function
 * 
 * Used to save entries in pointer array to preferences.
 * 
 * @param arr pointer array
 */
void Settings::putBulkOptions(int* arr) { pref.putBytes(BULK_OPTIONS_KEY, arr, MAX_BULK_OPTIONS); }

/**
 * Settings Add Bulk Option Function
 * 
 * Used to add new option to pointer array and to save option to 
 * preferences array.
 * 
 * @param option new bulk option to save
 * @param arr pointer array
 */
void Settings::addBulkOption(int option, int* arr) {
    // Confirm option isn't already in array
    if (!contains(option)) {
        // Add option and increment array size
        _bulkOptions[_bulkOptionCount] = option;
        _bulkOptionCount++;

        // Sort array and save to preferences
        quickSort(_bulkOptions, 0, _bulkOptionCount - 1);
        putBulkOptions(_bulkOptions);
    }
}

/**
 * Settings Get Hold Position Function
 * 
 * Used to get the hold-position for the stepper motor.
 * 
 * @return hold-position (steps)
 */
long Settings::getHoldPosition() { return _holdPos; }

/**
 * Settings Put Hold Position Function
 * 
 * Used to save the hold-position for the stepper motor in the
 * preferences.
 * 
 * @param pos new hold-position (steps)
 */
void Settings::putHoldPosition(long pos) { pref.putLong(HOLD_POS_KEY, pos); }

/**
 * Settings Get Release Position Function
 * 
 * Used to get the release-position for the stepper motor.
 * 
 * @return release-position (steps)
 */
long Settings::getReleasePosition() { return _releasePos; }

/**
 * Settings Put Release Position Function
 * 
 * Used to save the release-postition for the stepper motor
 * in the preferences.
 * 
 * @param pos new release-position (steps)
 */
void Settings::putReleasePosition(long pos) { pref.putLong(RELEASE_POS_KEY, pos); }

/**
 * Settings Get Hold Time Function
 * 
 * Used to get the hold-time for the stepper motor.
 * 
 * @return hold-time (milliseconds)
 */
unsigned long Settings::getHoldTime() { return _holdTimeMs; }

/**
 * Settings Put Hold Time Function
 * 
 * Used to save the hold-time for the stepper motor in the
 * preferences.
 * 
 * @param time new hold-time (milliseconds)
 */
void Settings::putHoldTime(unsigned long time) {
    // Ensure hold-time is longer than 1 second
    if (time >= 1000) {
        pref.putULong(HOLD_TIME_KEY, time);
        _holdTimeMs = time;
    }
}

/**
 * Settings Get Motor Pulse Width Function
 * 
 * Used to get the pulse-width for the stepper motor.
 * 
 * @return motor pulse-width
 */
int Settings::getMotorPulseWidth() { return _pulseWidth; }

/**
 * Settings Put Motor Pulse Width Function
 * 
 * Used to save the pulse-width for the stepper motor
 * in the preferences.
 * 
 * @param pw motor pulse-width
 */
void Settings::putMotorPulseWidth(int pw) { pref.putInt(MOTOR_PULSE_WIDTH_KEY, pw); }

/**
 * Settings Get Motor Max Speed Function
 * 
 * Used to get the maximum speed for the stepper motor.
 * 
 * @return motor max-speed (steps/second)
 */
float Settings::getMotorMaxSpeed() { return _maxSpeed; }

/**
 * Settings Put Motor Max Speed Function
 * 
 * Used to save the maximum speed for the stepper motor
 * in the preferences.
 * 
 * @param ms motor max-speed (steps/second)
 */
void Settings::putMotorMaxSpeed(float ms) { pref.putFloat(MOTOR_MAX_SPEED_KEY, ms); }

/**
 * Settings Get Motor Acceleration Function
 * 
 * Used to get the acceleration for the stepper motor.
 * 
 * @return motor acceleration (steps/second^2)
 */
float Settings::getMotorAcceleration() { return _acceleration; }

/**
 * Settings Put Motor Acceleration Function
 * 
 * Used to save the acceleration for the stepper motor in
 * the preferences.
 * 
 * @param a motor acceleration (steps/second^2)
 */
void Settings::putMotorAcceleration(float a) { pref.putFloat(MOTOR_ACCELERATION_KEY, a); }

/**
 * Settings Get Hall Sensitivity Function
 * 
 * Used to get the hall sensor's sensitivity.
 * 
 * @return hall sensor sensitivity
 */
int Settings::getHallSensitivity() { return _hallSensitivity; }

/**
 * Settings Put Hall Sensitivity Function
 * 
 * Used to save the sensitivity of the hall sensor in the 
 * preferences.
 * 
 * @param s hall sensor sensitivity
 */
void Settings::putHallSensitivity(int s) {
    if (s > 0 || s <= 4095) {
        pref.putInt(HALL_SENSITIVITY_KEY, s);
    } else {
        Serial.println("ERROR: Hall Sensor sensitivity outside of range of 0 and 4095!");
    }
}

/**
 * Settings Get Help Time Function
 * 
 * Used to get the help-message time for the display.
 * 
 * @return help time (milliseconds)
 */
unsigned long Settings::getHelpTime() { return _helpTimeMs; }

/**
 * Settings Put Help Time Function
 * 
 * Used to save the help-message time for the display in the
 * preferences.
 * 
 * @param time new help-time (milliseconds)
 */
void Settings::putHelpTime(unsigned long time) {
    // Ensure help-time is longer than 1 second
    if (time >= 1000) {
        pref.putULong(HELP_TIME_KEY, time);
        _helpTimeMs = time;
    }
}

/**
 * Settings Factory Reset Function
 * 
 * Used to save default values for each setting value in the
 * preferences, including the bulk options, the hold position,
 * the release position, the motor pulse width, the motor
 * acceleration, the hall sensor sensitivity and the help 
 * message cycle time.
 */
void Settings::factoryReset() {
    // Create temporary array for bulk options
    int temp[] = DEFAULT_BULK_OPTIONS;

    // Copy from the temporary array into bulk options array
    std::copy(std::begin(temp), std::end(temp), std::begin(_bulkOptions));
    
    // Save Default Values
    putBulkOptions(_bulkOptions);
    putHoldPosition(DEFAULT_HOLD_POS);
    putReleasePosition(DEFAULT_RELEASE_POS);
    putHoldTime(DEFAULT_HOLD_TIME);
    putMotorPulseWidth(DEFAULT_MOTOR_PULSE_WIDTH);
    putMotorMaxSpeed(DEFAULT_MOTOR_MAX_SPEED);
    putMotorAcceleration(DEFAULT_MOTOR_ACCELERATION);
    putHallSensitivity(DEFAULT_HALL_SENSITIVITY);
    putHelpTime(DEFAULT_HELP_TIME);
}

/**
 * Settings Count Bulk Options Function
 * 
 * Used to count the bulk options saved in the preferences.
 * 
 * @return number of bulk options
 */
size_t Settings::countBulkOptions() {
    size_t count = 0;
    for (int i = 0; i < MAX_BULK_OPTIONS; i++) {
        if (_bulkOptions[i] != 0) {
            count++;
        }
    }

    return count;
}

/**
 * Settings Contains Function
 * 
 * Used to check if a given value exists in the bulk options
 * saved in the preferences.
 * 
 * @param value value to check
 * @returns true if value is in array, false otherwise
 */
bool Settings::contains(int value) {
    bool check = false;
    for (int i = 0; i < _bulkOptionCount; i++) {
        if (_bulkOptions[i] == value) {
            check = true;
            break;
        }
    }
    return check;
}

/**
 * Settings Partition Function
 * 
 * Used in the Quick-Sort function to re-arrange the arry around the
 * pivot value (in this case, taken as the median value). After the 
 * partitioning, all elements smaller than (down to the lower reference
 * index) the pivot will be on its left, and all the elements greater 
 * than (up to the higher reference index) the pivot will be on its 
 * right, returning the greatest and least indices from that pivot.
 * 
 * @param arr array to be sorted
 * @param low lower reference index
 * @param high higher reference index
 * @returns least and greates indices
 */
std::tuple<int, int> Settings::partition(int arr[], int low, int high) {
    // Find the median index
    int pIdx = floor((low + high) / 2);
    int pivot = arr[pIdx];
    int least = low;
    int eq = low;
    int greatest = high;

    // Loop until array is sorted
    while (eq <= greatest) {
        if (arr[eq] < pivot) {
            swap(arr[eq], arr[least]);
            least++;
            eq++;
        } else if (arr[eq] > pivot) {
            swap(arr[eq], arr[greatest]);
            greatest--;
        } else if (arr[eq] == pivot) {
            eq++;
        }
    }

    return tie(least, greatest);
}

/**
 * Settings Quick-Sort Method
 * 
 * Using the Quick-Sort algorithm, the function will sort the 
 * bulk options. Algorithm follows the 3 steps:
 * 
 * 1. Choose a pivot
 * 2. Partition the array
 * 3. Recursively call
 * 
 * @param arr array to be sorted
 * @param low lower reference index
 * @param high higher reference index
 */
void Settings::quickSort(int arr[], int low, int high) {
    if (low >= 0 && low < high) {
        int least, greatest;
        tie(least, greatest) = partition(arr, low, high);
        quickSort(arr, low, least - 1);
        quickSort(arr, greatest + 1, high);
    }
}