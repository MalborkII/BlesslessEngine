import tkinter
from pathlib import Path
from typing import Any, Dict


def register(root: tkinter.Tk, project_dir: str, project: Dict[str, Any] | None) -> None:
    """
    Editor-плагин Logger.
    Создаёт отдельное окно и показывает содержимое Saved/Logs/Blessless.log.
    """
    log_window = tkinter.Toplevel(root)
    log_window.title("Logger")
    log_window.geometry("640x480")

    text = tkinter.Text(log_window, bg="#101010", fg="#dddddd", wrap="none")
    text.pack(fill=tkinter.BOTH, expand=True)

    log_path = Path(project_dir) / "Saved" / "Logs" / "Blessless.log"

    def refresh_logs() -> None:
        if log_path.exists():
            try:
                with open(log_path, encoding="utf-8", errors="ignore") as f:
                    content = f.read()
            except OSError:
                content = ""
            text.delete("1.0", tkinter.END)
            text.insert(tkinter.END, content)
            text.see(tkinter.END)
        root.after(500, refresh_logs)

    refresh_logs()

