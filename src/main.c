#include "config.h"
#include <sys/statvfs.h>

// Пропуск пустых полей

int is_empty_value(const char *val) {
    return !val || strcmp(val, "[N/A]") == 0 || 
           strlen(val) == 0 || 
           strspn(val, " \t\n") == strlen(val);
}

// Форматирование памяти 

void format_memory(char *buf, size_t size, long kb) {
    if (kb >= 1024 * 1024) {
        snprintf(buf, size, "%.1f GB", kb / (1024.0 * 1024));
    } else {
        snprintf(buf, size, "%.1f MB", kb / 1024.0);
    }
}

// Получение размера хранилища

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

// Получение значения памяти

void get_memory_info(MemoryInfo *mem) {
    FILE *file = fopen("/proc/meminfo", "r"); // Открытие /proc/meminfo для просмотра памяти.
    if (!file) return;
    
    char line[128];
    while (fgets(line, sizeof(line), file)) {
        char *colon = strchr(line, ':');
        if (!colon) continue;
        
        *colon = '\0';
        long value = strtol(colon + 1, NULL, 10);
        
        if (strcmp(line, "MemTotal") == 0) mem->mem_total = value; // Общая память
        else if (strcmp(line, "MemAvailable") == 0) mem->mem_available = value; // Свободная память
        else if (strcmp(line, "SwapTotal") == 0) mem->swap_total = value; // Общий свап
        else if (strcmp(line, "SwapFree") == 0) mem->swap_free = value; // Свободный свап
    }
    fclose(file); // Закрытие /proc/meminfo
}

// Вывод всей информации

void fetch() {
    MemoryInfo mem = {0};
    get_memory_info(&mem);
    
    // Вывод логотипа
    
    logo();
    
    // Вывод конфига.
    
    for (size_t i = 0; i < sizeof(SYSTEM_PROPERTIES)/sizeof(SYSTEM_PROPERTIES[0]); i++) { // Чтение конфига.
        char value[PROP_VALUE_MAX] = "[N/A]"; // Значение по умолчанию.
        const char *description = SYSTEM_PROPERTIES[i].description;
        
        switch (SYSTEM_PROPERTIES[i].type) {
            case PROP_TYPE_SYSTEM: // Обработка обычных модулей.
                __system_property_get(SYSTEM_PROPERTIES[i].property, value); // Получение значения из ключа.
                break;
                
            case PROP_TYPE_SPECIAL: // Обработка специальных модулей.
                if (strcmp(SYSTEM_PROPERTIES[i].property, "RAM") == 0) { // Проверка на присутствие RAM в конфиге.
                    char total[32], avail[32];
                    format_memory(total, sizeof(total), mem.mem_total); // Преобразование для mem_total.
                    format_memory(avail, sizeof(avail), mem.mem_available); // Преобразование для mem_available.
                    snprintf(value, sizeof(value), "%s / %s", avail, total); // Вывод строки. Формат: available / total.
                }
                else if (strcmp(SYSTEM_PROPERTIES[i].property, "SWAP") == 0) { // Проверка на присутствие SWAP в конфиге.
                    
                    char total[32], free[32];
                    
                    format_memory(total, sizeof(total), mem.swap_total); // Преобразование для swap_total.
                    format_memory(free, sizeof(free), mem.swap_free); // Преобразование для swap_free.
                    snprintf(value, sizeof(value), "%s / %s", free, total); // Вывод строки. Формат: free / total.
                }
                else if (strcmp(SYSTEM_PROPERTIES[i].property, "SIZE") == 0) { // Проверка на присутствие SIZE в конфиге.
                    get_storage_info(value, sizeof(value)); // Вывод строки с size.
                }
                else if (strcmp(SYSTEM_PROPERTIES[i].property, "SHELL") == 0) {  // Проверка на присутствие SHELL в конфиге. 
                    char *shell = getenv("SHELL"); // Получение shell из переменной.
                    if (shell) snprintf(value, sizeof(value), "%s", shell); // Вывод строки с shell.
                }
                break;
                
            case PROP_TYPE_CUSTOM: // Вывод кастомных строк
                printf("%s%s%s\n", CLR_ACCENT, SYSTEM_PROPERTIES[i].property, CLR_RESET);
                continue; 
        }
        
        if (is_empty_value(value)) continue; // Убирает строки если их значение пустое.
        
        printf("%s%-16s%s %s%s%s\n", 
               CLR_ACCENT, description,
               CLR_RESET, CLR_VALUE, value, CLR_RESET); // Вывод
    }
}

// Главная функция. Трогать не нужно

int main() {
    fetch();
    return 0;
}
