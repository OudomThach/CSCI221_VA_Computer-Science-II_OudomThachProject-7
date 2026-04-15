/*
 * 24-Hour to 12-Hour Time Converter - JavaScript Logic
 * By Oudom Thach - Version 4/15/2026
 * Computer Science II - Project Lab 7
 *
 * This file contains all the conversion logic and UI interaction code.
 * The validation and conversion functions mirror the C++ implementation
 * exactly, producing the same output and error messages.
 */

// =============================================================================
// TimeFormatMistake Exception Class
// Mirrors the C++ TimeFormatMistake exception class that inherits from
// runtime_error. In JavaScript, we extend the built-in Error class instead.
// =============================================================================
class TimeFormatMistake extends Error {
    constructor(message) {
        super(message);
        this.name = "TimeFormatMistake";
    }
}

// =============================================================================
// validateTime(timeStr)
// Validates the time string format and values.
// Throws TimeFormatMistake if invalid.
// Mirrors the C++ validateTime() function exactly.
//
// Checks performed (same order as C++ version):
//   1. Is there a colon (:) separator?
//   2. Are the hour and minute parts not empty?
//   3. Are all characters digits (no letters or symbols)?
//   4. Are hours between 0 and 23?
//   5. Are minutes between 0 and 59?
// =============================================================================
function validateTime(timeStr) {
    // Step 1: Check for colon separator
    var colonPos = timeStr.indexOf(':');
    if (colonPos === -1) {
        throw new TimeFormatMistake("Invalid format. Expected HH:MM (missing colon).");
    }

    // Step 2: Split into hour and minute parts
    var hourStr = timeStr.substring(0, colonPos);
    var minStr = timeStr.substring(colonPos + 1);

    // Make sure neither part is empty (handles ":30" or "12:")
    if (hourStr.length === 0 || minStr.length === 0) {
        throw new TimeFormatMistake("Invalid format. Expected HH:MM (missing hour or minute).");
    }

    // Step 3: Check that hour part contains only digits
    // This catches inputs like "$$:87" or "ab:30"
    for (var i = 0; i < hourStr.length; i++) {
        if (!/\d/.test(hourStr[i])) {
            throw new TimeFormatMistake(
                '"' + timeStr + '" is not a valid time. Non-numeric characters found in hours.'
            );
        }
    }

    // Check that minute part contains only digits
    for (var i = 0; i < minStr.length; i++) {
        if (!/\d/.test(minStr[i])) {
            throw new TimeFormatMistake(
                '"' + timeStr + '" is not a valid time. Non-numeric characters found in minutes.'
            );
        }
    }

    // Step 4 & 5: Convert to integers and check ranges
    var hours = parseInt(hourStr, 10);
    var minutes = parseInt(minStr, 10);

    // Hours in 24-hour format must be 0 to 23
    if (hours < 0 || hours > 23) {
        throw new TimeFormatMistake("Hours must be 0-23. You entered " + hours + ".");
    }

    // Minutes must be 0 to 59
    if (minutes < 0 || minutes > 59) {
        throw new TimeFormatMistake("Minutes must be 0-59. You entered " + minutes + ".");
    }
}

// =============================================================================
// convertTo12Hour(timeStr)
// Converts a validated 24-hour time string to 12-hour format.
// Returns a string like "1:07 PM" or "12:00 AM".
// Mirrors the C++ convertTo12Hour() function exactly.
//
// Conversion logic:
//   - 0 hours  (midnight)              -> 12 AM
//   - 1-11 hours (morning)             -> same number, AM
//   - 12 hours (noon)                  -> 12 PM
//   - 13-23 hours (afternoon/evening)  -> subtract 12, PM
// =============================================================================
function convertTo12Hour(timeStr) {
    // Split the string at the colon and convert to integers
    var colonPos = timeStr.indexOf(':');
    var hours = parseInt(timeStr.substring(0, colonPos), 10);
    var minutes = parseInt(timeStr.substring(colonPos + 1), 10);

    // This will hold either "AM" or "PM"
    var period;

    // Figure out AM vs PM and adjust the hour number
    if (hours === 0) {
        // Special case: midnight (0:00) displays as 12:00 AM
        hours = 12;
        period = "AM";
    } else if (hours < 12) {
        // Morning hours (1-11) stay the same, just add AM
        period = "AM";
    } else if (hours === 12) {
        // Special case: noon (12:00) stays 12, but it's PM
        period = "PM";
    } else {
        // Afternoon/evening (13-23): subtract 12 to get 1-11 PM
        hours -= 12;
        period = "PM";
    }

    // Build the output string
    // Add a leading zero to minutes less than 10 (so 9:5 becomes 9:05)
    var result = hours + ":";
    if (minutes < 10) {
        result += "0";
    }
    result += minutes + " " + period;

    return result;
}

// =============================================================================
// UI Elements - grab references to all the HTML elements we need
// =============================================================================
var timeInput = document.getElementById('timeInput');
var resultBox = document.getElementById('resultBox');
var resultLabel = document.getElementById('resultLabel');
var resultTime = document.getElementById('resultTime');
var errorType = document.getElementById('errorType');
var terminal = document.getElementById('terminal');
var emptyMsg = document.getElementById('emptyMsg');

// Counter for the conversion log entries
var historyCount = 0;

// =============================================================================
// handleConvert()
// Main conversion handler - this is called when the user clicks the button
// or presses Enter. It mirrors the try/catch block inside main() in the C++.
// =============================================================================
function handleConvert() {
    var timeStr = timeInput.value.trim();

    // If the input is empty, shake it to let the user know
    if (timeStr.length === 0) {
        shakeInput();
        return;
    }

    // Try to validate and convert (same as the C++ try/catch)
    try {
        // First, check if the input is a valid time format
        validateTime(timeStr);

        // If we get here, the time is valid -- convert it
        var result = convertTo12Hour(timeStr);

        // Show the success result on screen
        showResult("success", "Converted Time", result);

        // Log to the terminal (matches the C++ console output format)
        addToHistory(
            '<span class="prompt">&gt;</span> ' +
            escapeHtml(timeStr) + ' is <span class="success-text">' + escapeHtml(result) +
            '</span> in 12-hour format.'
        );

    } catch (e) {
        if (e instanceof TimeFormatMistake) {
            // Catch the TimeFormatMistake and display the error (same as C++)
            showResult("error", "TimeFormatMistake", e.message);

            // Log the error to the terminal
            addToHistory(
                '<span class="prompt">&gt;</span> ' +
                '<span class="error-text">TimeFormatMistake: ' +
                escapeHtml(e.message) + '</span>'
            );
        }
    }

    // Clear the input and refocus for the next conversion
    timeInput.value = '';
    timeInput.focus();
}

// =============================================================================
// showResult(type, label, text)
// Displays the conversion result or error in the result box with animation.
// type: "success" or "error"
// =============================================================================
function showResult(type, label, text) {
    resultBox.className = 'result-box ' + type;
    resultLabel.textContent = label;

    if (type === 'error') {
        errorType.style.display = 'block';
        errorType.textContent = 'TimeFormatMistake';
        resultTime.textContent = text;
    } else {
        errorType.style.display = 'none';
        resultTime.textContent = text;
    }

    // Trigger the slide-in animation by toggling the 'show' class
    resultBox.classList.remove('show');
    void resultBox.offsetWidth; // force browser reflow so animation replays
    resultBox.classList.add('show');
}

// =============================================================================
// addToHistory(html)
// Adds a new line to the conversion log terminal at the bottom.
// Each entry gets a number like [1], [2], [3], etc.
// =============================================================================
function addToHistory(html) {
    // Remove the "No conversions yet" placeholder if it's still there
    if (emptyMsg) {
        emptyMsg.remove();
    }

    historyCount++;
    var line = document.createElement('div');
    line.className = 'line';
    line.innerHTML = '<span class="dim">[' + historyCount + ']</span> ' + html;
    terminal.appendChild(line);

    // Auto-scroll to the newest entry
    terminal.scrollTop = terminal.scrollHeight;
}

// =============================================================================
// clearHistory()
// Clears all entries from the conversion log and resets the counter.
// =============================================================================
function clearHistory() {
    terminal.innerHTML = '<div class="empty-history" id="emptyMsg">No conversions yet. Try one above!</div>';
    historyCount = 0;
}

// =============================================================================
// shakeInput()
// Plays a quick shake animation on the input field when the user tries
// to convert with an empty input. Just a small UX touch.
// =============================================================================
function shakeInput() {
    timeInput.style.animation = 'none';
    void timeInput.offsetWidth; // reset animation
    timeInput.style.animation = 'shake 0.4s ease';
}

// =============================================================================
// escapeHtml(text)
// Escapes special HTML characters to prevent XSS when displaying user input
// in the terminal log. For example, "<script>" becomes "&lt;script&gt;".
// =============================================================================
function escapeHtml(text) {
    var div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}

// =============================================================================
// toggleTestGuide()
// Opens/closes the "How to Test This Program" collapsible section.
// =============================================================================
function toggleTestGuide() {
    var toggle = document.getElementById('testToggle');
    var content = document.getElementById('testContent');
    toggle.classList.toggle('open');
    content.classList.toggle('open');
}

// =============================================================================
// Event Listener: Enter key triggers conversion
// So the user can just type and hit Enter without clicking the button.
// =============================================================================
timeInput.addEventListener('keydown', function(e) {
    if (e.key === 'Enter') {
        handleConvert();
    }
});
