/*
   ____             __ _       
  / ___|___  _ __  / _(_) __ _ 
 | |   / _ \| '_ \| |_| |/ _` |
 | |__| (_) | | | |  _| | (_| |
  \____\___/|_| |_|_| |_|\__, |
                         |___/ 
*/
// Please review config.h as it contains much useful information.
// This file is needed to configure the output.

#include "config.h"

// Default logo, if the logo file is missing.
// At the end of each line, add " \n " for correct display.

const char *ASCII_ART = 
    "\n"
    "  ;,           ,;\n"
    "   ';,.-----.,;'\n"
    "  ,'           ',\n"
    " /    O     O    \\ \n"
    "|                 | \n"
    "'-----------------'\n";
// It is not recommended to change this logo. It is recommended to create a <logo.txt> file with your logo and place it in the same directory as the program.
    
// Output information
    
const SystemProperty SYSTEM_PROPERTIES[] = {
    { PROP_TYPE_SYSTEM, "ro.build.version.release",        "OS Version:"},
    { PROP_TYPE_SYSTEM, "ro.product.model",                "Model:"},
    { PROP_TYPE_SPECIAL, "RAM",                           "RAM:"},
    { PROP_TYPE_SPECIAL, "SWAP",                          "SWAP:"},
    { PROP_TYPE_SPECIAL, "SIZE",                          "Storage:"},
    { PROP_TYPE_SPECIAL, "SHELL",                         "Shell:"}
    // The number of elements must match SYSTEM_PROPERTIES[6] in config.h
    // Format: { TYPE, "Key", "Output" }
    // If you haven't reviewed config.h, it is recommended to do so.
};

void logo() {
    FILE *file = fopen("logo.txt", "r"); // Replace logo.txt with your logo file.
    if (file) {
        char line[256]; 
        printf("%s\n", CLR_ACCENT);
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
        printf("%s\n", CLR_RESET);
        fclose(file);
    } else {
        printf("%s%s%s", CLR_ACCENT, ASCII_ART, CLR_RESET);
    }
}