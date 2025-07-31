/*
   ____             __ _         _     
  / ___|___  _ __  / _(_) __ _  | |__  
 | |   / _ \| '_ \| |_| |/ _` | | '_ \ 
 | |__| (_) | | | |  _| | (_| |_| | | |
  \____\___/|_| |_|_| |_|\__, (_)_| |_|
                         |___/         
*/
// Данный файл предназначен для ознакомления с типами вывода и так далее.


#ifndef CONFIG_H
#define CONFIG_H

// Библиотеки.

#include <sys/utsname.h>
#include <sys/system_properties.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Цвета вывода в ANSI.

#define CLR_TITLE    "\033[1;34m"
#define CLR_VALUE    "\033[1;37m"
#define CLR_ACCENT   "\033[1;32m"
#define CLR_RESET    "\033[0m"

// Структура конфига.

// Типы.
typedef enum {
    // Тип для получения информации о системе через ключи.
    // Их можно найти используя SetEdit в таблице Свойства Android / Android Properties.
    // Пример: { PROP_TYPE_SYSTEM, "ro.build.date", "Build Date:" }
    // > Build Date: ~
    PROP_TYPE_SYSTEM,
    // Тип для создания кастомных строк которые не выводят какую либо информацию.
    // Пример: { PROP_TYPE_CUSTOM, "--------------------", NULL }
    // > --------------------
    PROP_TYPE_CUSTOM,
    // Тип для специальных модулей (RAM, SWAP, SIZE, SHELL)
    // Пример: { PROP_TYPE_SPECIAL, "RAM", "RAM:" }
    // > RAM: 1.2 GB / 4.0 GB
    PROP_TYPE_SPECIAL
} PropertyType;

// Строка в конфиге

typedef struct {
    PropertyType type; // Тип. О них было сказано выше
    const char *property; // Ключ. В него вписывается значение, для каждого типа оно разное.
    const char *description; // Вывод.  Вписывается после Типа и Ключа. Для PROP_TYPE_CUSTOM оно всегда пустое.
    // Пример: { Тип, "Ключ", "Output ->" }
    // > Output -> Например дата сборки системы.
} SystemProperty;

// Структура получения значений памяти.

typedef struct {
    long mem_total; // Общее количество памяти.
    long mem_available; // Доступное количество памяти.
    long swap_total; // Общий размер подкачки.
    long swap_free; // Свободное количество подкачки.
} MemoryInfo;

// Размер конфига. Должен соответствовать количеству элементов в SYSTEM_PROPERTIES.
// Если вы добавили строку в конфиге, обновите это значение.
extern const SystemProperty SYSTEM_PROPERTIES[6];

// Логотип по умолчанию. Редактировать можно в config.c
extern const char *ASCII_ART;

// Обьявление функций

void logo(void); // Функция вывода логотипа
void fetch(void); // Главная функция вывода конфига
int is_empty_value(const char *val); // Функция пропускающая строки с пустым значением.
void format_memory(char *buf, size_t size, long kb); // Функция для преобразования памяти в GB
void get_storage_info(char *value, size_t size); // Функция для получения размера хранилища.
void get_memory_info(MemoryInfo *mem); // Функция для получения значения памяти 

#endif