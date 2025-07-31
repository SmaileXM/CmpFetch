#include <sys/utsname.h>
#include <stdio.h>
#include <sys/system_properties.h>
#include <string.h>

//-----------------
// Конфигурация   |
//-----------------

// Логотип по умолчанию если файла с логотипом не будет.
// Вставьте один \n в конце каждой строки.
   
#define ASCII_ART \
    "\n" \
    "  ;,           ,;\n" \
    "   ';,.-----.,;'\n" \
    "  ,'           ',\n" \
    " /    O     O    \\ \n" \
    "|                 | \n" \
    "'-----------------'\n"
    
// Цвета для вывода. ANSI Коды
    
#define CLR_TITLE    "\033[1;34m"  // Цвет заголовков (синий)
#define CLR_VALUE    "\033[1;37m"  // Цвет значений (белый)
#define CLR_ACCENT   "\033[1;32m"  // Акцентный цвет (зеленый)
#define CLR_RESET    "\033[0m" // Сброс оформления
#define CLR_RED "\033[0;31m" // Красный цвет

// Вывод и информация системы

typedef enum {
    PROP_TYPE_SYSTEM, // Для создания строк с информацией о системе
    PROP_TYPE_CUSTOM // Для создания кастомных строк
} PropertyType;

static const struct {
    PropertyType type;
    const char *property;
    const char *description;
} SYSTEM_PROPERTIES[] = {
    
    // Системные свойства
    // { ТИП, "ключ", "Вывод" }
    // Ключи можно узнать через Set Edit в таблице Свойства Android / Android Properties.
    { PROP_TYPE_SYSTEM, "ro.build.version.release",        "OS Version:"},
    { PROP_TYPE_SYSTEM, "ro.product.model",                "Model:"},
    { PROP_TYPE_SYSTEM, "ro.build.date",                   "Build Date:"},
    { PROP_TYPE_SYSTEM, "ro.build.version.security_patch", "Security Patch:"},
    { PROP_TYPE_SYSTEM, "ro.soc.model",                    "CPU:"},
};

// Загрузка файла с логотипом

void logo() {
    FILE *file = fopen("logo.txt", "r"); // Вместо logo.txt впишите свой путь до файла с артом
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
// Конец конфигурации|
//--------------------
    
int is_empty_value(const char *val) {
    return strcmp(val, "[N/A]") == 0 || 
           strlen(val) == 0 || 
           strspn(val, " \t\n") == strlen(val);
}

// Основная функция вывода 

void fetch() {
    struct utsname kernel_info;
    uname(&kernel_info);
    
    logo();
    
    for (size_t i = 0; i < sizeof(SYSTEM_PROPERTIES)/sizeof(SYSTEM_PROPERTIES[0]); i++) {
        if (SYSTEM_PROPERTIES[i].type == PROP_TYPE_CUSTOM) {
            // Вывод кастом строки
            printf("%s%s%s\n", CLR_ACCENT, SYSTEM_PROPERTIES[i].property, CLR_RESET);
            continue;
        }
    
        // Простая обработка 
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

// Мейн функшион

int main() {
    fetch();
}