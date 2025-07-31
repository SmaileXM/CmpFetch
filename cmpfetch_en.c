#include <sys/utsname.h>
#include <stdio.h>
#include <sys/system_properties.h>
#include <string.h>

//-----------------
// Configuration   |
//-----------------

// Default logo if the logo file is not found.
// Insert one \n at the end of each line.
   
#define ASCII_ART \
    "\n" \
    "  ;,           ,;\n" \
    "   ';,.-----.,;'\n" \
    "  ,'           ',\n" \
    " /    O     O    \\ \n" \
    "|                 | \n" \
    "'-----------------'\n"
    
// Output colors. ANSI codes
    
#define CLR_TITLE    "\033[1;34m"  // Title color (blue)
#define CLR_VALUE    "\033[1;37m"  // Value color (white)
#define CLR_ACCENT   "\033[1;32m"  // Accent color (green)
#define CLR_RESET    "\033[0m"     // Reset formatting
#define CLR_RED      "\033[0;31m"  // Red color

// System output and information

typedef enum {
    PROP_TYPE_SYSTEM, // For creating system information strings
    PROP_TYPE_CUSTOM  // For creating custom strings
} PropertyType;

static const struct {
    PropertyType type;
    const char *property;
    const char *description;
} SYSTEM_PROPERTIES[] = {
    
    // System properties
    // { TYPE, "key", "Output" }
    // Keys can be found via Set Edit in Android Properties table.
    { PROP_TYPE_SYSTEM, "ro.build.version.release",        "OS Version:"},
    { PROP_TYPE_SYSTEM, "ro.product.model",                "Model:"},
    { PROP_TYPE_SYSTEM, "ro.build.date",                   "Build Date:"},
    { PROP_TYPE_SYSTEM, "ro.build.version.security_patch", "Security Patch:"},
    { PROP_TYPE_SYSTEM, "ro.soc.model",                    "CPU:"},
};

// Load logo file

void logo() {
    FILE *file = fopen("logo.txt", "r"); // Instead of logo.txt, specify your path to the art file
    if (file) {
        char line[256]; 
        printf("%s", CLR_ACCENT);
        
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
        
        printf("%s\n", CLR_RESET);
        fclose(file);
    } else {
        printf("%s%s%s", CLR_ACCENT, ASCII_ART, CLR_RESET);
    }
}

//--------------------
// End configuration |
//--------------------
    
int is_empty_value(const char *val) {
    return strcmp(val, "[N/A]") == 0 || 
           strlen(val) == 0 || 
           strspn(val, " \t\n") == strlen(val);
}

// Main output function 

void fetch() {
    struct utsname kernel_info;
    uname(&kernel_info);
    
    logo();
    
    for (size_t i = 0; i < sizeof(SYSTEM_PROPERTIES)/sizeof(SYSTEM_PROPERTIES[0]); i++) {
        if (SYSTEM_PROPERTIES[i].type == PROP_TYPE_CUSTOM) {
            // Output custom string
            printf("%s%s%s\n", CLR_ACCENT, SYSTEM_PROPERTIES[i].property, CLR_RESET);
            continue;
        }
    
        // Simple processing 
        char value[PROP_VALUE_MAX] = "[N/A]";
        __system_property_get(SYSTEM_PROPERTIES[i].property, value);
    
        if (is_empty_value(value)) {
            continue;
        }
        printf("%s%-16s%s %s%s%s\n", 
                CLR_ACCENT, SYSTEM_PROPERTIES[i].description,
                CLR_RESET, CLR_VALUE, value, CLR_RESET);
    }
}

// Main function

int main() {
    fetch();
}