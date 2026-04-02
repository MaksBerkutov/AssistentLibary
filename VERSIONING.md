# VERSIONING

В проекте используется простая схема:

- `dev` — рабочая ветка для всей активной разработки
- `main` — стабильная ветка для готового состояния
- `vX.Y.Z` — релизный тег по SemVer

## Правило работы

1. Все новые изменения делаем в `dev`.
2. Когда набор изменений готов, переносим его в `main`.
3. Перед релизом обновляем версию в [library.json](/C:/OSPanel/home/AssistentLibary/AssistentLibary/lib/AssistentLibrary/library.json).
4. На `main` создаем тег формата `vX.Y.Z`.
5. После push тега GitHub Actions автоматически создаст GitHub Release.

## Что считается версией

Источник истины для версии:

- [library.json](/C:/OSPanel/home/AssistentLibary/AssistentLibary/lib/AssistentLibrary/library.json)

Тег должен точно совпадать с версией из `library.json`:

- `library.json`: `0.1.0`
- git tag: `v0.1.0`

Если тег не совпадает с `library.json`, release workflow завершится с ошибкой.

## SemVer

Используется обычный SemVer:

- `MAJOR` — ломающие изменения API
- `MINOR` — новые возможности без ломки совместимости
- `PATCH` — исправления багов, тестов, документации, сборки

Примеры:

- `0.1.0` — первый нормальный feature-release
- `0.1.1` — фиксы без изменения API
- `0.2.0` — новый функционал без breaking changes
- `1.0.0` — стабильный публичный API

## Текущий процесс релиза

### Проверка в CI

Workflow [ci.yml](/C:/OSPanel/home/AssistentLibary/AssistentLibary/.github/workflows/ci.yml):

- запускается на `push` в `dev` и `main`
- запускается на `pull_request` в `dev` и `main`
- валидирует формат версии в `library.json`
- запускает `pio test` для `nodemcuv2`

### Релиз

Workflow [release.yml](/C:/OSPanel/home/AssistentLibary/AssistentLibary/.github/workflows/release.yml):

- запускается на тегах `v*.*.*`
- проверяет совпадение тега и `library.json`
- проверяет, что тег указывает на коммит из `main`
- создает GitHub Release
- прикладывает zip-архив исходников

## Как выпустить новую версию

Пример для версии `0.1.0`:

```bash
git switch dev
# работа над кодом

git switch main
git merge dev
git tag v0.1.0
git push origin main
git push origin v0.1.0
```

Если версия изменилась, сначала обновите `library.json`, потом создавайте тег.
