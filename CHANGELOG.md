# CHANGELOG

Этот файл описывает изменения, которые были внесены в текущую рабочую версию библиотеки.

Текущая целевая версия:

- `0.1.0`

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

### 12. Добавлены GitHub Actions для CI и release

Добавлены workflow'ы:

- проверка версии и тестов на `dev` и `main`
- автоматический релиз на тегах `vX.Y.Z`

Что делает CI:

- проверяет формат версии в `library.json`
- запускает `pio test` для `nodemcuv2`

Что делает release workflow:

- проверяет совпадение git-тега и версии из `library.json`
- проверяет, что тег указывает на коммит из `main`
- публикует GitHub Release
- прикладывает zip-архив исходников

Файлы:

- [ci.yml](/C:/OSPanel/home/AssistentLibary/AssistentLibary/.github/workflows/ci.yml)
- [release.yml](/C:/OSPanel/home/AssistentLibary/AssistentLibary/.github/workflows/release.yml)

### 13. Добавлена документация по версионированию

Появился отдельный документ с правилами:

- работаем в `dev`
- стабилизируем в `main`
- релизы создаем тегами `vX.Y.Z`
- версия в теге должна совпадать с `library.json`

Файл:

- [VERSIONING.md](/C:/OSPanel/home/AssistentLibary/AssistentLibary/VERSIONING.md)

### 14. Версия библиотеки обновлена до `0.1.0`

Версия в манифесте библиотеки поднята до `0.1.0` как базовая точка для нового API и релизного процесса.

Файл:

- [library.json](/C:/OSPanel/home/AssistentLibary/AssistentLibary/lib/AssistentLibrary/library.json)

### 15. Исправлена установка `AESLib` в CI

`PlatformIO Registry` не находил зависимость `davylandman/AESLib @ ^2.2.1`, из-за чего GitHub Actions падал ещё до сборки тестов. Зависимость переведена на прямое подключение из GitHub-репозитория.

Файл:

- [platformio.ini](/C:/OSPanel/home/AssistentLibary/AssistentLibary/platformio.ini)

### 16. CI адаптирован для embedded-сборки без физической платы

GitHub Actions больше не пытается запускать `pio test` как полноценный upload/run цикл на `nodemcuv2`, что неработоспособно в облаке без подключенного устройства.

Теперь CI:

- собирает library test firmware через `pio test --without-uploading --without-testing`
- проверяет компиляцию библиотечного API через тестовые файлы

Файлы:

- [ci.yml](/C:/OSPanel/home/AssistentLibary/AssistentLibary/.github/workflows/ci.yml)
- [Readme.md](/C:/OSPanel/home/AssistentLibary/AssistentLibary/Readme.md)
- [VERSIONING.md](/C:/OSPanel/home/AssistentLibary/AssistentLibary/VERSIONING.md)

### 17. Добавлен compile-smoke тест для `AssistentEasy`

Чтобы CI проверял не только старый low-level API, но и новый упрощенный фасад, добавлен отдельный тестовый файл, который компилирует:

- `AssistentRelayDevice`
- `AssistentClimateDevice`
- `on(...)`
- `onRequest(...)`
- `set(...)`
- `relay(...)`

Файлы:

- [test_easy_api_compile.cpp](/C:/OSPanel/home/AssistentLibary/AssistentLibary/test/test_easy_api_compile.cpp)
- [test_main.cpp](/C:/OSPanel/home/AssistentLibary/AssistentLibary/test/test_main.cpp)

### 18. Исправлен include для `base64_arduino`

В `AssistentAES.h` использовалось неверное имя заголовка `arduino_base64.hpp`, из-за чего CI не мог собрать проект после установки зависимости `base64_arduino`.

Теперь используется корректный include:

- `base64_arduino.hpp`

Файл:

- [AssistentAES.h](/C:/OSPanel/home/AssistentLibary/AssistentLibary/lib/AssistentAES/src/AssistentAES.h)

## Что не менялось

- криптографическая логика `AssistentAES` не изменялась
- старый low-level API `AssistenWiFi` сохранен
- старые примеры и существующий стиль использования не удалялись
