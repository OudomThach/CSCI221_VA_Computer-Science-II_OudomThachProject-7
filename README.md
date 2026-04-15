# CSCI221 Project 7: 24-Hour to 12-Hour Time Converter

Created by Oudom Thach.

## What This Program Does

This project converts a time written in 24-hour format into 12-hour format with AM/PM.

Examples:
- `13:07` becomes `1:07 PM`
- `0:00` becomes `12:00 AM`
- `23:59` becomes `11:59 PM`

The program also checks for invalid input. If the user enters something like `10:65`, `$$:87`, `hello`, or `:30`, it throws and catches a custom exception called `TimeFormatMistake` instead of crashing.

## Project Versions

This repository includes two versions of the same project:

- `TimeConverter.cpp` is the original C++ console version for the assignment.
- `index.html`, `style.css`, and `script.js` make up the web version that runs in a browser.

The web version follows the same conversion logic, validation rules, and error messages as the C++ version.

## Main Features

- Converts valid 24-hour times to 12-hour AM/PM format
- Handles edge cases such as midnight and noon
- Detects invalid hours, invalid minutes, missing colon, and non-numeric input
- Uses a custom `TimeFormatMistake` exception in the C++ version
- Lets the user continue testing more inputs after an error
- Includes a browser-based version with a visual interface and conversion log

## Repository Structure

- `README.md` - project overview
- `TimeConverter.cpp` - C++ source code for the assignment
- `Testing_Guide.txt` - test cases, expected results, and sample run output
- `C++ and Web Deployment Explanation.txt` - full explanation of the C++ and web implementations
- `index.html` - structure of the web page
- `style.css` - styling, layout, and animations for the web version
- `script.js` - JavaScript validation, conversion logic, and UI behavior

## How To Run

### C++ Version

Compile:

```bash
g++ -o TimeConverter TimeConverter.cpp -std=c++11
```

Run:

```bash
./TimeConverter
```

On Windows, you can also run:

```bash
TimeConverter.exe
```

### Web Version

Open `index.html` in a web browser.

You can also run it with a local server if needed, but opening the file directly is enough for this project.

## Testing

The project includes test cases for:

- regular time conversion
- midnight and noon
- last minute of the day
- leading zero minutes
- invalid ranges
- non-numeric characters
- missing format parts

See `Testing_Guide.txt` for the full testing details and expected results.
