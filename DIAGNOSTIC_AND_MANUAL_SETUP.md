# Диагностика и ручная настройка ESP-IDF

Если вы продолжаете сталкиваться с ошибками при работе с ESP-IDF в VSCode, пожалуйста, выполните диагностику и ручную настройку согласно следующим инструкциям.

## Диагностика проблемы

1. Проверьте, что bash установлен и доступен:
   ```bash
   which bash
   bash --version
   ```

2. Проверьте, что все необходимые инструменты установлены:
   ```bash
   python3 --version
   python3 -c "import serial; print('PySerial OK')"
   ```

3. Проверьте права доступа к директории ESP-IDF:
   ```bash
   ls -la /home/alex/my_main_folder/Study/ESP32/ESP32_for_esp_idf/esp-idf
   ```

## Ручная настройка окружения

1. Закройте VSCode полностью
2. Откройте новый терминал
3. Выполните команды:
   ```bash
   cd /home/alex/my_main_folder/Study/ESP32/ESP32_for_esp_idf/esp-idf
   source export.sh
   cd /home/alex/my_main_folder/Study/ESP32/CommonStudy
   code .
   ```

4. В открывшемся VSCode не запускайте никакие автоматические настройки ESP-IDF

## Альтернативный способ запуска терминала ESP-IDF

Если терминал ESP-IDF не запускается внутри VSCode:

1. Откройте внешний терминал
2. Выполните:
   ```bash
   cd /home/alex/my_main_folder/Study/ESP32/ESP32_for_esp_idf/esp-idf
   source export.sh
   ```
3. Теперь все команды ESP-IDF будут доступны в этом терминале

## Управление инструментами ESP-IDF вручную

Для установки/обновления инструментов ESP-IDF:

```bash
cd /home/alex/my_main_folder/Study/ESP32/ESP32_for_esp_idf/esp-idf
./install.sh
```

Для проверки установленных инструментов:
```bash
python3 tools/idf_tools.py list
```

Для установки конкретных инструментов:
```bash
python3 tools/idf_tools.py install tool_name
```

## Настройка VSCode вручную

Если автоматическая настройка не работает:

1. Откройте VSCode
2. Нажмите Ctrl+Shift+P
3. Введите "ESP-IDF: Configure ESP-IDF extension"
4. Выберите "Use existing setup"
5. Укажите путь: `/home/alex/my_main_folder/Study/ESP32/ESP32_for_esp_idf/esp-idf`
6. Укажите путь к Python: `/home/alex/.espressif/python_env/idf6.1_py3.10_env/bin/python`

## Проверка установки

После настройки проверьте работоспособность:

```bash
echo $IDF_PATH
idf.py --version
idf.py list
```

## Если ничего не помогает

В крайнем случае, удалите и переустановите расширение ESP-IDF:

1. В VSCode: Extensions -> Найдите ESP-IDF -> Удалить
2. Удалите настройки расширения:
   ```bash
   rm -rf ~/.vscode/extensions/espressif.esp-idf-extension-*
   ```
3. Перезапустите VSCode
4. Установите расширение заново
5. Используйте ручную настройку, описанную выше