#include "config.h"
#include <sys/statvfs.h>

// Skip empty fields

int is_empty_value(const char *val) {
    return !val || strcmp(val, "[N/A]") == 0 || 
           strlen(val) == 0 || 
           strspn(val, " \t\n") == strlen(val);
}

// Memory formatting 

void format_memory(char *buf, size_t size, long kb) {
    if (kb >= 1024 * 1024) {
        snprintf(buf, size, "%.1f GB", kb / (1024.0 * 1024));
    } else {
        snprintf(buf, size, "%.1f MB", kb / 1024.0);
    }
}

// Get storage size

void get_storage_info(char *value, size_t size) {
    struct statvfs vfs;
    if (statvfs("/storage/emulated/0/", &vfs) == 0) {
        double total = (vfs.f_blocks * vfs.f_frsize) / (1024.0 * 1024 * 1024);
        double free = (vfs.f_bfree * vfs.f_frsize) / (1024.0 * 1024 * 1024);
        snprintf(value, size, "%.1fGB / %.1fGB", free, total);
    } else {
        snprintf(value, size, "[N/A]");
    }
}

// Get memory value

void get_memory_info(MemoryInfo *mem) {
    FILE *file = fopen("/proc/meminfo", "r"); // Open /proc/meminfo to view memory.
    if (!file) return;
    
    char line[128];
    while (fgets(line, sizeof(line), file)) {
        char *colon = strchr(line, ':');
        if (!colon) continue;
        
        *colon = '\0';
        long value = strtol(colon + 1, NULL, 10);
        
        if (strcmp(line, "MemTotal") == 0) mem->mem_total = value; // Total memory
        else if (strcmp(line, "MemAvailable") == 0) mem->mem_available = value; // Available memory
        else if (strcmp(line, "SwapTotal") == 0) mem->swap_total = value; // Total swap
        else if (strcmp(line, "SwapFree") == 0) mem->swap_free = value; // Free swap
    }
    fclose(file); // Close /proc/meminfo
}

// Display all information

void fetch() {
    MemoryInfo mem = {0};
    get_memory_info(&mem);
    
    // Display logo
    
    logo();
    
    // Display config.
    
    for (size_t i = 0; i < sizeof(SYSTEM_PROPERTIES)/sizeof(SYSTEM_PROPERTIES[0]); i++) { // Read config.
        char value[PROP_VALUE_MAX] = "[N/A]"; // Default value.
        const char *description = SYSTEM_PROPERTIES[i].description;
        
        switch (SYSTEM_PROPERTIES[i].type) {
            case PROP_TYPE_SYSTEM: // Process regular modules.
                __system_property_get(SYSTEM_PROPERTIES[i].property, value); // Get value from key.
                break;
                
            case PROP_TYPE_SPECIAL: // Process special modules.
                if (strcmp(SYSTEM_PROPERTIES[i].property, "RAM") == 0) { // Check for RAM in config.
                    char total[32], avail[32];
                    format_memory(total, sizeof(total), mem.mem_total); // Convert mem_total.
                    format_memory(avail, sizeof(avail), mem.mem_available); // Convert mem_available.
                    snprintf(value, sizeof(value), "%s / %s", avail, total); // Output string. Format: available / total.
                }
                else if (strcmp(SYSTEM_PROPERTIES[i].property, "SWAP") == 0) { // Check for SWAP in config.
                    
                    char total[32], free[32];
                    
                    format_memory(total, sizeof(total), mem.swap_total); // Convert swap_total.
                    format_memory(free, sizeof(free), mem.swap_free); // Convert swap_free.
                    snprintf(value, sizeof(value), "%s / %s", free, total); // Output string. Format: free / total.
                }
                else if (strcmp(SYSTEM_PROPERTIES[i].property, "SIZE") == 0) { // Check for SIZE in config.
                    get_storage_info(value, sizeof(value)); // Output string with size.
                }
                else if (strcmp(SYSTEM_PROPERTIES[i].property, "SHELL") == 0) {  // Check for SHELL in config.
                    char *shell = getenv("SHELL"); // Get shell from variable.
                    if (shell) snprintf(value, sizeof(value), "%s", shell); // Output string with shell.
                }
                break;
                
            case PROP_TYPE_CUSTOM: // Output custom strings
                printf("%s%s%s\n", CLR_ACCENT, SYSTEM_PROPERTIES[i].property, CLR_RESET);
                continue; 
        }
        
        if (is_empty_value(value)) continue; // Skip lines with empty values.
        
        printf("%s%-16s%s %s%s%s\n", 
               CLR_ACCENT, description,
               CLR_RESET, CLR_VALUE, value, CLR_RESET); // Output
    }
}

// Main function. Do not touch

int main() {
    fetch();
    return 0;
}
