/*
   ____             __ _         _     
  / ___|___  _ __  / _(_) __ _  | |__  
 | |   / _ \| '_ \| |_| |/ _` | | '_ \ 
 | |__| (_) | | | |  _| | (_| |_| | | |
  \____\___/|_| |_|_| |_|\__, (_)_| |_|
                         |___/         
*/
// This file is intended for familiarization with output types and more.


#ifndef CONFIG_H
#define CONFIG_H

// Libraries.

#include <sys/utsname.h>
#include <sys/system_properties.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ANSI output colors.

#define CLR_TITLE    "\033[1;34m"
#define CLR_VALUE    "\033[1;37m"
#define CLR_ACCENT   "\033[1;32m"
#define CLR_RESET    "\033[0m"

// Config structure.

// Types.
typedef enum {
    // Type for getting system information through keys.
    // They can be found using SetEdit in the Android Properties table.
    // Example: { PROP_TYPE_SYSTEM, "ro.build.date", "Build Date:" }
    // > Build Date: ~
    PROP_TYPE_SYSTEM,
    // Type for creating custom strings that do not output any information.
    // Example: { PROP_TYPE_CUSTOM, "--------------------", NULL }
    // > --------------------
    PROP_TYPE_CUSTOM,
    // Type for special modules (RAM, SWAP, SIZE, SHELL)
    // Example: { PROP_TYPE_SPECIAL, "RAM", "RAM:" }
    // > RAM: 1.2 GB / 4.0 GB
    PROP_TYPE_SPECIAL
} PropertyType;

// Config line

typedef struct {
    PropertyType type; // Type. Explained above.
    const char *property; // Key. The value entered here varies for each type.
    const char *description; // Output. Written after Type and Key. For PROP_TYPE_CUSTOM it is always empty.
    // Example: { Type, "Key", "Output ->" }
    // > Output -> For example, system build date.
} SystemProperty;

// Structure for getting memory values.

typedef struct {
    long mem_total; // Total memory amount.
    long mem_available; // Available memory amount.
    long swap_total; // Total swap size.
    long swap_free; // Free swap amount.
} MemoryInfo;

// Config size. Must match the number of elements in SYSTEM_PROPERTIES.
// If you added a line to the config, update this value.
extern const SystemProperty SYSTEM_PROPERTIES[6];

// Default logo. Can be edited in config.c
extern const char *ASCII_ART;

// Function declarations

void logo(void); // Function to display the logo
void fetch(void); // Main function to display the config
int is_empty_value(const char *val); // Function to skip lines with empty values.
void format_memory(char *buf, size_t size, long kb); // Function to convert memory to GB
void get_storage_info(char *value, size_t size); // Function to get storage size.
void get_memory_info(MemoryInfo *mem); // Function to get memory value 

#endif