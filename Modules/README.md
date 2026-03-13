Модули BlesslessEngine
======================

Каждый C++ модуль описывается дескриптором `*.bemodule` (JSON) и бинарем DLL.

## Пример дескриптора

```json
{
  "Name": "Rendering",
  "Type": "Engine",
  "Binary": "BE-Rendering.dll",
  "EditorOnly": false,
  "RuntimeOnly": false,
  "RequiredOnStart": true,
  "MinEngineVersion": "0.1.0",
  "MaxEngineVersion": null,
  "Dependencies": [ "AssetSystem" ]
}
```

## Поведение загрузчика модулей

- Дескрипторы ищутся в:
  - `Engine/Modules/**.bemodule` — встроенные модули движка.
  - `Projects/<ProjectName>/Modules/**.bemodule` — модули проекта.
- На основе `Engine/Config/EngineModules.json` и `project.bless` строится список активных модулей.
- `FModuleManager`:
  - Парсит все дескрипторы.
  - Фильтрует по версии движка и флагам `EditorOnly`/`RuntimeOnly`.
  - Строит граф зависимостей и определяет порядок загрузки.
  - Для каждого модуля:
    - Загружает DLL.
    - Находит и вызывает `BE_Module_Startup()`.
  - При завершении:
    - Вызывает `BE_Module_Shutdown()` модулей в обратном порядке зависимостей.

