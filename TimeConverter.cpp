// Oudom Thach
// Computer Science II - Project Lab 7
// Program: 24-Hour to 12-Hour Time Converter
// Description: Converts 24-hour time to 12-hour time format.
//              Uses a custom TimeFormatMistake exception class to handle
//              invalid input such as out-of-range values or non-numeric characters.
//              The program loops until the user decides to quit.

// Include the iostream library for console input/output (cin, cout)
#include <iostream>
// Include the string library so we can use the string data type
#include <string>
// Include stdexcept so we can inherit from runtime_error for our custom exception
#include <stdexcept>
// Use the standard namespace so we don't have to type std:: everywhere
using namespace std;

//==============================================================================
// Exception Class: TimeFormatMistake
// This is a custom exception class that inherits from runtime_error.
// We throw this exception whenever the user enters a bad time format.
// For example: "10:65", "$$:87", "hello", or ":30" would all trigger this.
//==============================================================================
class TimeFormatMistake : public runtime_error
{
public:
    // Constructor that accepts an error message and passes it to runtime_error
    TimeFormatMistake(const string& message);
};

//==============================================================================
// Function Prototypes
//==============================================================================

// Prompts the user to enter a time in 24-hour format (HH:MM)
void getTime(string& timeStr);

// Validates the time string; throws TimeFormatMistake if invalid
void validateTime(const string& timeStr);

// Converts a validated 24-hour time string to 12-hour format with AM/PM
string convertTo12Hour(const string& timeStr);

// Asks the user whether they want to convert another time
bool askToContinue();

//==============================================================================
// Main Function
// This is the entry point of the program. It uses a do-while loop so the
// user can keep converting times until they choose to quit. Inside the loop,
// we use a try-catch block to handle any TimeFormatMistake exceptions that
// get thrown when the user enters bad input.
//==============================================================================
int main()
{
    // Variable to hold the user's input string
    string timeStr;

    // Display the program title/banner
    cout << "========================================" << endl;
    cout << " 24-Hour to 12-Hour Time Converter" << endl;
    cout << "========================================" << endl;

    // do-while loop: runs at least once, then repeats if user types 'y'
    do
    {
        // Ask the user to type in a time
        getTime(timeStr);

        // Try to validate and convert the time
        // If anything is wrong, a TimeFormatMistake exception gets thrown
        try
        {
            // First, check if the input is a valid time format
            validateTime(timeStr);

            // If we get here, the time is valid, so convert it
            string result = convertTo12Hour(timeStr);
            cout << timeStr << " is " << result << " in 12-hour format." << endl;
        }
        catch (TimeFormatMistake& e)
        {
            // If validateTime() threw an exception, we catch it here
            // and print the error message instead of crashing
            cout << "TimeFormatMistake: " << e.what() << endl;
        }

    } while (askToContinue()); // Keep going if user types 'y'

    // User typed 'n', so say goodbye
    cout << "\nThank you for using the Time Converter. Goodbye!" << endl;

    return 0;
}

//==============================================================================
// Function Implementations
//==============================================================================

// TimeFormatMistake constructor implementation
// It takes the error message and passes it up to runtime_error using
// an initializer list (the ": runtime_error(message)" part).
// This way, calling e.what() later will return our custom message.
TimeFormatMistake::TimeFormatMistake(const string& message)
    : runtime_error(message)
{
}

// Prompts the user to enter a time and stores it in the timeStr variable.
// We pass timeStr by reference so the value gets saved back to main().
void getTime(string& timeStr)
{
    cout << "\nEnter a time in 24-hour format (HH:MM): ";
    cin >> timeStr;
}

// validateTime - This is where all the error checking happens.
// It checks the user's input step by step:
//   1. Is there a colon (:) separating hours and minutes?
//   2. Are the hour and minute parts not empty?
//   3. Are all characters actual digits (no letters or symbols)?
//   4. Are the hours between 0 and 23?
//   5. Are the minutes between 0 and 59?
// If ANY of these checks fail, we throw a TimeFormatMistake exception.
// The program does NOT crash -- the catch block in main() handles it.
void validateTime(const string& timeStr)
{
    // Step 1: Look for the colon separator
    // find() returns string::npos if there's no colon
    size_t colonPos = timeStr.find(':');
    if (colonPos == string::npos)
    {
        throw TimeFormatMistake("Invalid format. Expected HH:MM (missing colon).");
    }

    // Step 2: Split the string into the part before and after the colon
    // For "13:07" -> hourStr = "13", minStr = "07"
    string hourStr = timeStr.substr(0, colonPos);
    string minStr = timeStr.substr(colonPos + 1);

    // Make sure neither part is empty (handles inputs like ":30" or "12:")
    if (hourStr.empty() || minStr.empty())
    {
        throw TimeFormatMistake("Invalid format. Expected HH:MM (missing hour or minute).");
    }

    // Step 3: Check that the hour part only has digit characters (0-9)
    // This catches inputs like "$$:87" or "ab:30"
    for (size_t i = 0; i < hourStr.length(); i++)
    {
        if (!isdigit(hourStr[i]))
        {
            throw TimeFormatMistake("\"" + timeStr + "\" is not a valid time. "
                                    "Non-numeric characters found in hours.");
        }
    }

    // Check that the minute part only has digit characters too
    for (size_t i = 0; i < minStr.length(); i++)
    {
        if (!isdigit(minStr[i]))
        {
            throw TimeFormatMistake("\"" + timeStr + "\" is not a valid time. "
                                    "Non-numeric characters found in minutes.");
        }
    }

    // Step 4 & 5: Convert strings to integers so we can check the ranges
    int hours = stoi(hourStr);
    int minutes = stoi(minStr);

    // Hours in 24-hour format must be 0 to 23
    if (hours < 0 || hours > 23)
    {
        throw TimeFormatMistake("Hours must be 0-23. You entered " +
                                to_string(hours) + ".");
    }

    // Minutes must be 0 to 59
    if (minutes < 0 || minutes > 59)
    {
        throw TimeFormatMistake("Minutes must be 0-59. You entered " +
                                to_string(minutes) + ".");
    }
}

// convertTo12Hour - Does the actual math to convert 24-hour to 12-hour time.
// This function only gets called AFTER validateTime() has already confirmed
// the input is good, so we don't need to check for errors again.
//
// The conversion logic works like this:
//   - 0 hours  (midnight)  -> 12 AM
//   - 1-11 hours (morning) -> same number, AM
//   - 12 hours (noon)      -> 12 PM
//   - 13-23 hours (afternoon/evening) -> subtract 12, PM
//
// Returns a string like "1:07 PM" or "12:00 AM".
string convertTo12Hour(const string& timeStr)
{
    // Split the string at the colon and convert to integers
    size_t colonPos = timeStr.find(':');
    int hours = stoi(timeStr.substr(0, colonPos));
    int minutes = stoi(timeStr.substr(colonPos + 1));

    // This will hold either "AM" or "PM"
    string period;

    // Figure out AM vs PM and adjust the hour number
    if (hours == 0)
    {
        // Special case: midnight (0:00) displays as 12:00 AM
        hours = 12;
        period = "AM";
    }
    else if (hours < 12)
    {
        // Morning hours (1-11) stay the same, just add AM
        period = "AM";
    }
    else if (hours == 12)
    {
        // Special case: noon (12:00) stays 12, but it's PM
        period = "PM";
    }
    else
    {
        // Afternoon/evening (13-23): subtract 12 to get 1-11 PM
        hours -= 12;
        period = "PM";
    }

    // Build the output string
    // We need to add a leading zero to minutes less than 10
    // so that 9:5 displays as 9:05 instead
    string result = to_string(hours) + ":";
    if (minutes < 10)
    {
        result += "0";
    }
    result += to_string(minutes) + " " + period;

    return result;
}

// askToContinue - Asks the user if they want to do another conversion.
// Returns true if they type 'y' or 'Y', false for anything else.
// This is what controls the do-while loop in main().
bool askToContinue()
{
    char choice;
    cout << "Would you like to convert another time? (y/n): ";
    cin >> choice;
    return (choice == 'y' || choice == 'Y');
}

//==============================================================================
// Testing & Results
// Author:  Oudom Thach
// Version: 4/15/2026
//==============================================================================
//
// HOW TO COMPILE AND RUN:
// -----------------------
// 1. Open a terminal (Command Prompt, PowerShell, or Linux/Mac terminal).
// 2. Navigate to the folder containing this file:
//        cd "path/to/Project 7"
// 3. Compile the program using g++:
//        g++ -o TimeConverter TimeConverter.cpp -std=c++11
// 4. Run the program:
//    - On Windows:   TimeConverter.exe    or   .\TimeConverter.exe
//    - On Mac/Linux: ./TimeConverter
// 5. Enter a time in 24-hour format when prompted (e.g., 13:07).
// 6. The program will display the 12-hour equivalent (e.g., 1:07 PM).
// 7. If you enter an invalid time (e.g., 10:65 or $$:87), the program
//    will catch a TimeFormatMistake exception and display an error.
// 8. Type 'y' to convert another time, or 'n' to quit.
//
// HOW TO TEST:
// ------------
// To reproduce the test results below, run the program and enter each
// input one at a time. After each conversion, type 'y' to continue.
// For the last test, type 'n' to quit. Verify that your output matches
// the expected output listed for each test case.
//
// Below are the test cases and their results:
//
// ---- Test 1: Standard PM conversion ----
// Input:  13:07
// Output: 13:07 is 1:07 PM in 12-hour format.
// Result: PASS
//
// ---- Test 2: Midnight (edge case) ----
// Input:  0:00
// Output: 0:00 is 12:00 AM in 12-hour format.
// Result: PASS
//
// ---- Test 3: Noon (edge case) ----
// Input:  12:00
// Output: 12:00 is 12:00 PM in 12-hour format.
// Result: PASS
//
// ---- Test 4: Last minute of the day (edge case) ----
// Input:  23:59
// Output: 23:59 is 11:59 PM in 12-hour format.
// Result: PASS
//
// ---- Test 5: Morning time with leading zero minute ----
// Input:  9:05
// Output: 9:05 is 9:05 AM in 12-hour format.
// Result: PASS
//
// ---- Test 6: Invalid minutes (out of range) ----
// Input:  10:65
// Output: TimeFormatMistake: Minutes must be 0-59. You entered 65.
// Result: PASS (exception caught, program continued)
//
// ---- Test 7: Non-numeric characters ----
// Input:  $$:87
// Output: TimeFormatMistake: "$$:87" is not a valid time.
//         Non-numeric characters found in hours.
// Result: PASS (exception caught, program continued)
//
// ---- Test 8: No colon separator ----
// Input:  hello
// Output: TimeFormatMistake: Invalid format. Expected HH:MM (missing colon).
// Result: PASS (exception caught, program continued)
//
// ---- Test 9: Missing hour portion ----
// Input:  :30
// Output: TimeFormatMistake: Invalid format. Expected HH:MM
//         (missing hour or minute).
// Result: PASS (exception caught, program continued)
//
// ---- Test 10: User quits ----
// Input:  n (when asked to continue)
// Output: Thank you for using the Time Converter. Goodbye!
// Result: PASS (program exited gracefully)
//
// All 10 test cases passed. The program correctly converts valid 24-hour
// times and throws/catches TimeFormatMistake for all invalid inputs.
//==============================================================================
