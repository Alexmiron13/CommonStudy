#!/bin/bash

echo "Проверка подключения ESP32-S3..."

# Проверяем, подключено ли устройство
if lsusb | grep -q "303a:1001"; then
    echo "Устройство ESP32-S3 найдено (ID 303a:1001)"
    
    # Запускаем OpenOCD в фоновом режиме
    echo "Запуск OpenOCD..."
    /home/alex/.espressif/tools/openocd-esp32/v0.12.0-esp32-20251215/openocd-esp32/bin/openocd \
        -s "/home/alex/.espressif/tools/openocd-esp32/v0.12.0-esp32-20251215/openocd-esp32/share/openocd/scripts" \
        -f interface/esp_usb_jtag.cfg \
        -f target/esp32s3.cfg &
    
    OPENOCD_PID=$!
    echo "OpenOCD запущен с PID: $OPENOCD_PID"
    
    # Ждем немного, чтобы OpenOCD запустился
    sleep 3
    
    # Проверяем, запущен ли OpenOCD
    if ps -p $OPENOCD_PID > /dev/null; then
        echo "OpenOCD успешно запущен. Теперь можно запустить отладку в VSCode."
        echo "Выберите конфигурацию 'ESP32 Debug (External Adapter)' в VSCode и нажмите F5."
        echo ""
        echo "Для остановки OpenOCD используйте: kill $OPENOCD_PID"
    else
        echo "Ошибка: OpenOCD не удалось запустить"
        exit 1
    fi
else
    echo "Ошибка: Устройство ESP32-S3 (ID 303a:1001) не найдено."
    echo "Пожалуйста, проверьте подключение USB к плате ESP32-S3."
    echo ""
    echo "Доступные USB устройства:"
    lsusb
    exit 1
fi