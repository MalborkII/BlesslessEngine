"""
BlesslessEngine Editor — Python-редактор.
Запуск: python main.py <path_to_project.bless>
Через project.bless: run_editor.bat Projects/SampleProject/project.bless
"""

import json
import os
import sys
from pathlib import Path
from typing import Optional


def load_project(project_path: str) -> Optional[dict]:
    """Читает project.bless и возвращает конфиг проекта."""
    path = Path(project_path).resolve()
    if not path.exists():
        return None
    try:
        with open(path, encoding="utf-8") as f:
            return json.load(f)
    except (json.JSONDecodeError, OSError):
        return None


def main() -> None:
    # Путь к project.bless
    project_path = sys.argv[1] if len(sys.argv) > 1 else None
    if not project_path:
        default = Path(__file__).resolve().parent.parent.parent / "Projects" / "SampleProject" / "project.bless"
        project_path = str(default)

    project = load_project(project_path)
    project_name = project.get("ProjectName", "BlesslessEngine") if project else "BlesslessEngine"
    project_dir = str(Path(project_path).resolve().parent) if project_path else ""

    import tkinter

    root = tkinter.Tk()
    root.title(f"{project_name} — BlesslessEditor")
    root.geometry("1280x720")
    root.minsize(800, 600)

    # Центральная область (пустое окно)
    frame = tkinter.Frame(root, bg="#1e1e1e")
    frame.pack(fill=tkinter.BOTH, expand=True)

    label = tkinter.Label(
        frame,
        text=f"Project: {project_name}\n{project_dir}",
        fg="#cccccc",
        bg="#1e1e1e",
        font=("Segoe UI", 12),
    )
    label.place(relx=0.5, rely=0.5, anchor=tkinter.CENTER)

    root.mainloop()


if __name__ == "__main__":
    main()
