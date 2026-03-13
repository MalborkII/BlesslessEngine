BlesslessEngine
================

Этот репозиторий содержит скелет модульного игрового движка BlesslessEngine.

## Высокоуровневая структура

- `Engine/` — C++ ядро движка (static lib + базовые C++ подсистемы).
- `Modules/` — C++ DLL-модули движка (рендеринг, физика, аудио и т.п.).
- `Game/` — C# runtime-логика проекта / игры (`Game.dll`, `GameEditor.dll`).
- `Editor/` — Python‑редактор и его плагины.
- `Projects/` — пользовательские проекты с файлами `project.bless`.

См. план архитектуры в `.cursor/plans/blesslessengine-architecture_2dd307e5.plan.md` для детальной схемы.

## Запуск по двойному клику на project.bless

1. **Сборка**: из корня репозитория запустите `build.bat`. В `Binaries/Win64/` появятся `BEVersionSelector.exe` и `BlesslessEngine.exe`.

2. **Ассоциация файлов**: при необходимости привяжите расширение `.bless` к `Binaries/Win64/BEVersionSelector.exe` (ПКМ по project.bless → «Открыть с помощью» → указать exe). После этого двойной клик по `project.bless` запускает редактор.

3. **Цепочка**: двойной клик по `project.bless` → `BEVersionSelector.exe` → читает `EngineVersion` → запускает `BlesslessEngine.exe project.bless` → BlesslessEngine запускает `Editor/Python/main.py` с путём к проекту.

Требуется Python в PATH (или переменная `PYTHON` с путём к `python.exe`).

