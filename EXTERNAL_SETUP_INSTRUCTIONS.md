# Внешние инструкции по настройке ESP-IDF

Если вы продолжаете сталкиваться с ошибкой "The terminal process "/usr/bin/bash" terminated with exit code: 127" при попытке использовать "Open ESP-IDF Installation Manager" или другие функции расширения ESP-IDF, пожалуйста, следуйте этим инструкциям:

## Метод 1: Использование внешнего терминала

1. Откройте внешний терминал (например, GNOME Terminal, Konsole или xterm)
2. Перейдите в директорию вашего проекта:
   ```bash
   cd /home/alex/my_main_folder/Study/ESP32/CommonStudy
   ```
3. Запустите скрипт настройки окружения:
   ```bash
   source /home/alex/my_main_folder/Study/ESP32/ESP32_for_esp_idf/esp-idf/export.sh
   ```
4. После успешной инициализации окружения, запустите VSCode из этого же терминала:
   ```bash
   code .
   ```

## Метод 2: Ручная установка инструментов

Если вам нужно обновить или переустановить инструменты ESP-IDF:

1. Откройте внешний терминал
2. Перейдите в директорию ESP-IDF:
   ```bash
   cd /home/alex/my_main_folder/Study/ESP32/ESP32_for_esp_idf/esp-idf
   ```
3. Запустите установку:
   ```bash
   ./install.sh
   ```

## Метод 3: Использование скриптов проекта

Вы можете использовать предоставленные скрипты для настройки окружения:

1. Внешний терминал:
   ```bash
   cd /home/alex/my_main_folder/Study/ESP32/CommonStudy
   ./setup_environment.sh
   code .
   ```

## Метод 4: Проверка установки

Чтобы проверить, что ESP-IDF правильно установлен:

1. В терминале с активированным окружением ESP-IDF выполните:
   ```bash
   echo $IDF_PATH
   idf.py --version
   ```

Эти методы должны помочь вам обойти проблему с внутренним терминалом VSCode и успешно настроить ESP-IDF.