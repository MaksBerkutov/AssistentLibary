# CHANGELOG

Этот файл описывает изменения, которые были внесены в текущую рабочую версию библиотеки.

## Новое

### 1. Добавлен упрощенный фасад `AssistentEasy`

Появились новые классы для коротких скетчей:

- `AssistentEasyDevice`
- `AssistentRelayDevice`
- `AssistentClimateDevice`

Что дает новый слой:

- не нужны массивы `CMD`, `HandlerCMD`, `CMDRec`, `HandlerCMDRec`
- не нужно вручную создавать `AssistentVariable::Variable` в обычных сценариях
- можно писать короткие скетчи через `addRelay(...)`, `addDht22(...)`, `begin()`, `run()`
- можно отправлять данные через `set(...)` и `sendState()`
- можно регистрировать кастомные команды через `on(...)` и `onRequest(...)`

Для relay-устройств автоматически регистрируются команды:

- `ON`
- `OFF`
- `TOGGLE`
- `STATE`

Для climate-устройств автоматически регистрируется:

- `STATE`

Файлы:

- [AssistentEasy.h](/C:/OSPanel/home/AssistentLibary/AssistentLibary/lib/AssistentLibrary/src/AssistentEasy.h)
- [AssistentEasy.cpp](/C:/OSPanel/home/AssistentLibary/AssistentLibary/lib/AssistentLibrary/src/AssistentEasy.cpp)

## Исправления

### 2. Исправлена сборка при `ASSISTENT_OTA`

Поле `CONFIGURE` теперь существует независимо от `ASSISTENT_OTA`, поэтому библиотека больше не ломается на условной компиляции.

Файл:

- [AssistentLibrary.h](/C:/OSPanel/home/AssistentLibary/AssistentLibary/lib/AssistentLibrary/src/AssistentLibrary.h)

### 3. Исправлена логика кастомного обработчика

Раньше пользовательский обработчик в `Begin(...)` подключался с неверной проверкой, а `Reader()` все равно шел только в стандартный маршрут. Теперь кастомный handler действительно работает.

Файл:

- [AssistentLibrary.cpp](/C:/OSPanel/home/AssistentLibary/AssistentLibary/lib/AssistentLibrary/src/AssistentLibrary.cpp)

### 4. Исправлен баг в `LinkedList::prev()`

Метод `prev()` был инвертирован по логике и мог переводить указатель в невалидное состояние. Теперь переход назад работает корректно.

Файл:

- [LinkedList.h](/C:/OSPanel/home/AssistentLibary/AssistentLibary/lib/AssistentVariable/src/LinkedList.h)

### 5. Исправлена сериализация пустого `AssistentVariable`

Раньше `GetString()` мог некорректно удалить символ из пустой строки. Теперь пустое состояние сериализуется безопасно.

Дополнительно:

- `ToType(...)` теперь безопасно обрабатывает `nullptr`
- появился метод `Get(key)` для чтения значения без авто-добавления пустого слота
- сериализация пропускает `nullptr`-элементы

Файлы:

- [AssistentVariable.h](/C:/OSPanel/home/AssistentLibary/AssistentLibary/lib/AssistentVariable/src/AssistentVariable.h)
- [Map.h](/C:/OSPanel/home/AssistentLibary/AssistentLibary/lib/AssistentVariable/src/Map.h)

### 6. В `DataRele` добавлен `IsOn()`

Это понадобилось для упрощенного API и безопасного `toggle()` без внешней логики.

Файл:

- [DataRele.h](/C:/OSPanel/home/AssistentLibary/AssistentLibary/lib/AssistentVariable/include/Types/DataRele.h)

## Сборка

### 7. `platformio.ini` сделан переносимым

Абсолютные Linux-пути были заменены на нормальные зависимости `PlatformIO`, поэтому проект теперь проще собрать на другой машине.

Файл:

- [platformio.ini](/C:/OSPanel/home/AssistentLibary/AssistentLibary/platformio.ini)

## Тесты

### 8. Переписаны тесты для AES-сценария

Тесты больше не сравнивают случайные `base64`-строки как будто это валидный шифротекст. Теперь проверяется более реалистичный путь `encrypt -> decrypt`.

Важно:

- реализация `AssistentAES` не изменялась
- менялись только тесты вокруг нее

Файл:

- [test_decrypt.cpp](/C:/OSPanel/home/AssistentLibary/AssistentLibary/test/test_decrypt.cpp)

### 9. Убран дубликат в тестах IP-валидации

Повторный вызов одного и того же теста удален.

Файл:

- [tes_validate_ip.cpp](/C:/OSPanel/home/AssistentLibary/AssistentLibary/test/tes_validate_ip.cpp)

## Документация и примеры

### 10. Полностью переписан `Readme`

Теперь в документации есть:

- выбор между `AssistentEasy` и low-level API
- быстрый старт
- описание методов
- ограничения easy-слоя
- примеры кастомных команд
- краткий обзор изменений

Файл:

- [Readme.md](/C:/OSPanel/home/AssistentLibary/AssistentLibary/Readme.md)

### 11. Добавлены easy-примеры

Новые примеры:

- relay-скетч
- climate-скетч

Файлы:

- [EasyRelay.cpp](/C:/OSPanel/home/AssistentLibary/AssistentLibary/examples/Easy/EasyRelay.cpp)
- [EasyClimate.cpp](/C:/OSPanel/home/AssistentLibary/AssistentLibary/examples/Easy/EasyClimate.cpp)

## Что не менялось

- `AssistentAES` не изменялся
- старый low-level API `AssisenWiFi` сохранен
- старые примеры и существующий стиль использования не удалялись
