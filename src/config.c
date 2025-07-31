/*
   ____             __ _       
  / ___|___  _ __  / _(_) __ _ 
 | |   / _ \| '_ \| |_| |/ _` |
 | |__| (_) | | | |  _| | (_| |
  \____\___/|_| |_|_| |_|\__, |
                         |___/ 
*/
// Пожалуйста ознакомьтесь с config.h в нем много полезной информации.
// Данный файл нужен для настройки вывода

#include "config.h"

// Логотип по умолчанию, если файл с ним будет отсутствовать.
// В конце каждой строки выставляйте " \n " для правильного отображения.

const char *ASCII_ART = 
    "\n"
    "  ;,           ,;\n"
    "   ';,.-----.,;'\n"
    "  ,'           ',\n"
    " /    O     O    \\ \n"
    "|                 | \n"
    "'-----------------'\n";
// Этот логотип менять не рекомендуется. Рекомендую создать файл <logo.txt> с вашим логотипом и кинуть его в одну директорию с программой.
    
// Вывод информации
    
const SystemProperty SYSTEM_PROPERTIES[] = {
    { PROP_TYPE_SYSTEM, "ro.build.version.release",        "OS Version:"},
    { PROP_TYPE_SYSTEM, "ro.product.model",                "Model:"},
    { PROP_TYPE_SPECIAL, "RAM",                           "RAM:"},
    { PROP_TYPE_SPECIAL, "SWAP",                          "SWAP:"},
    { PROP_TYPE_SPECIAL, "SIZE",                          "Storage:"},
    { PROP_TYPE_SPECIAL, "SHELL",                         "Shell:"}
    // количество элементов должно совпадать с SYSTEM_PROPERTIES[6] В config.h
    // Формат: { ТИП, "Ключ", "Вывод" }
    // Если вы не смотрели config.h то все таки рекомендую его посмотреть.
};

void logo() {
    FILE *file = fopen("logo.txt", "r"); // Замените logo.txt на свой файл с логотипом.
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