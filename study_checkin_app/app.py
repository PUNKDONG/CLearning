from __future__ import annotations

import datetime as dt
import os
import re
import sqlite3
from dataclasses import dataclass
from pathlib import Path
from zoneinfo import ZoneInfo

from flask import Flask, flash, redirect, render_template, request, url_for


APP_DIR = Path(__file__).resolve().parent
DB_PATH = APP_DIR / "checkin.db"
PLAN_PATH = APP_DIR.parent / "C_learning_plan.md"

DAY_LINE_RE = re.compile(r"- Day(\d+)\s+\((\d{4}-\d{2}-\d{2})\):\s*(.+)$")
CHINA_TZ = ZoneInfo("Asia/Shanghai")

app = Flask(__name__)
app.config["SECRET_KEY"] = "local-checkin-secret-key"


@dataclass
class PlanTask:
    day_num: int
    planned_date: str
    title: str
    stage: str


def get_conn() -> sqlite3.Connection:
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    return conn


def init_db() -> None:
    conn = get_conn()
    conn.execute(
        """
        CREATE TABLE IF NOT EXISTS tasks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            day_num INTEGER NOT NULL UNIQUE,
            stage TEXT NOT NULL,
            planned_date TEXT NOT NULL,
            title TEXT NOT NULL,
            status TEXT NOT NULL DEFAULT 'pending',
            completed_at TEXT,
            created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
        )
        """
    )
    conn.execute(
        """
        CREATE TABLE IF NOT EXISTS leave_logs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            leave_date TEXT NOT NULL,
            reason TEXT,
            affected_count INTEGER NOT NULL DEFAULT 0,
            created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
        )
        """
    )
    conn.execute(
        """
        CREATE TABLE IF NOT EXISTS app_meta (
            key TEXT PRIMARY KEY,
            value TEXT NOT NULL
        )
        """
    )
    conn.execute(
        """
        CREATE TABLE IF NOT EXISTS notes (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            note_date TEXT NOT NULL,
            title TEXT NOT NULL,
            content TEXT NOT NULL,
            created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
            updated_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
        )
        """
    )
    conn.commit()
    conn.close()


def parse_plan_tasks(plan_text: str) -> list[PlanTask]:
    tasks: list[PlanTask] = []
    current_stage = "未分阶段"
    for raw_line in plan_text.splitlines():
        line = raw_line.strip()
        if line.startswith("## 阶段"):
            current_stage = line.replace("## ", "").strip()
            continue

        m = DAY_LINE_RE.match(line)
        if not m:
            continue

        day_num = int(m.group(1))
        planned_date = m.group(2)
        title = m.group(3).strip()
        tasks.append(
            PlanTask(
                day_num=day_num,
                planned_date=planned_date,
                title=title,
                stage=current_stage,
            )
        )

    return tasks


def seed_tasks_if_empty() -> None:
    conn = get_conn()
    count = conn.execute("SELECT COUNT(*) AS c FROM tasks").fetchone()["c"]
    if count > 0:
        conn.close()
        return

    if not PLAN_PATH.exists():
        conn.close()
        raise FileNotFoundError(f"找不到计划文件: {PLAN_PATH}")

    text = PLAN_PATH.read_text(encoding="utf-8")
    tasks = parse_plan_tasks(text)
    if not tasks:
        conn.close()
        raise ValueError("未从计划文件中解析到 Day 任务")

    conn.executemany(
        """
        INSERT INTO tasks (day_num, stage, planned_date, title, status)
        VALUES (?, ?, ?, ?, 'pending')
        """,
        [(t.day_num, t.stage, t.planned_date, t.title) for t in tasks],
    )
    conn.commit()
    conn.close()


def leetcode_target(day_num: int) -> int:
    if day_num <= 20:
        return 0
    # Day21 起：3,2,3,2... 交替
    return 3 if (day_num - 21) % 2 == 0 else 2


def china_now() -> dt.datetime:
    return dt.datetime.now(CHINA_TZ)


def china_today() -> dt.date:
    return china_now().date()


def ensure_ready() -> None:
    init_db()
    seed_tasks_if_empty()
    ensure_meta_defaults()


def ensure_meta_defaults() -> None:
    conn = get_conn()
    today = china_today().isoformat()
    conn.execute(
        """
        INSERT INTO app_meta (key, value)
        VALUES ('auto_rollover_enabled', '1')
        ON CONFLICT(key) DO NOTHING
        """
    )
    conn.execute(
        """
        INSERT INTO app_meta (key, value)
        VALUES ('last_auto_rollover_date', ?)
        ON CONFLICT(key) DO NOTHING
        """,
        (today,),
    )
    conn.commit()
    conn.close()


def get_meta(conn: sqlite3.Connection, key: str, default: str) -> str:
    row = conn.execute("SELECT value FROM app_meta WHERE key = ?", (key,)).fetchone()
    return row["value"] if row else default


def set_meta(conn: sqlite3.Connection, key: str, value: str) -> None:
    conn.execute(
        """
        INSERT INTO app_meta (key, value)
        VALUES (?, ?)
        ON CONFLICT(key) DO UPDATE SET value = excluded.value
        """,
        (key, value),
    )


def run_auto_rollover_if_needed() -> tuple[int, int]:
    conn = get_conn()
    today = china_today()
    enabled = get_meta(conn, "auto_rollover_enabled", "1") == "1"
    if not enabled:
        conn.close()
        return 0, 0

    last_value = get_meta(conn, "last_auto_rollover_date", today.isoformat())
    try:
        last_date = dt.date.fromisoformat(last_value)
    except ValueError:
        last_date = today

    if last_date >= today:
        conn.close()
        return 0, 0

    days_processed = 0
    affected_total = 0
    current = last_date
    while current < today:
        current_str = current.isoformat()
        affected = conn.execute(
            """
            SELECT COUNT(*) AS c
            FROM tasks
            WHERE status = 'pending' AND date(planned_date) = date(?)
            """,
            (current_str,),
        ).fetchone()["c"]

        if affected > 0:
            conn.execute(
                """
                UPDATE tasks
                SET planned_date = date(planned_date, '+1 day')
                WHERE status = 'pending' AND date(planned_date) = date(?)
                """,
                (current_str,),
            )
            conn.execute(
                """
                INSERT INTO leave_logs (leave_date, reason, affected_count)
                VALUES (?, ?, ?)
                """,
                (current_str, "自动日结", affected),
            )
            affected_total += affected

        current += dt.timedelta(days=1)
        days_processed += 1

    set_meta(conn, "last_auto_rollover_date", today.isoformat())
    conn.commit()
    conn.close()
    return days_processed, affected_total


def parse_view_date(raw_value: str | None, fallback: dt.date | None = None) -> dt.date:
    default_day = fallback or china_today()
    if not raw_value:
        return default_day
    try:
        return dt.date.fromisoformat(raw_value)
    except ValueError:
        return default_day


def parse_sim_today(raw_value: str | None) -> dt.date | None:
    if not raw_value:
        return None
    try:
        return dt.date.fromisoformat(raw_value)
    except ValueError:
        return None


def pick_default_view_date(conn: sqlite3.Connection, today: dt.date) -> dt.date:
    # 找到最早未完成任务日期。
    row = conn.execute(
        """
        SELECT planned_date
        FROM tasks
        WHERE status = 'pending'
        ORDER BY date(planned_date) ASC, day_num ASC
        LIMIT 1
        """,
    ).fetchone()

    # 默认视图规则（按真实日期推进）：
    # 1) 若有逾期/今天未完成，显示最早未完成日期。
    # 2) 若下一条未完成在未来（例如 16/17），当天是否立即跳转取决于“这些超前完成是否发生在今天”：
    #    - 若是今天刚超前完成：保持显示今天，不在当天跳太远。
    #    - 到下一真实日期后：显示下一条未完成任务（可直接跳到 16/17/...）。
    if not row:
        return today
    try:
        pending_date = dt.date.fromisoformat(row["planned_date"])
    except ValueError:
        return today

    if pending_date <= today:
        return pending_date
    if pending_date == today + dt.timedelta(days=1):
        return pending_date

    today_str = today.isoformat()
    completed_future_today = conn.execute(
        """
        SELECT COUNT(*) AS c
        FROM tasks
        WHERE status = 'completed'
          AND completed_at IS NOT NULL
          AND date(completed_at) = date(?)
          AND date(planned_date) > date(?)
        """,
        (today_str, today_str),
    ).fetchone()["c"]

    if completed_future_today > 0:
        return today
    return pending_date


def pick_anchor_task_date(conn: sqlite3.Connection, today: dt.date) -> dt.date:
    """
    计算“今天视角”的任务锚点日期：
    - 若有逾期/今天待完成任务，优先从最早 pending 开始；
    - 若今天已超前完成未来任务，锚点保持在今天完成过的最早任务日期，
      避免页面在同一天内跳来跳去，导致未来日期撤销按钮消失。
    """
    today_str = today.isoformat()
    pending_row = conn.execute(
        """
        SELECT planned_date
        FROM tasks
        WHERE status = 'pending'
        ORDER BY date(planned_date) ASC, day_num ASC
        LIMIT 1
        """,
    ).fetchone()
    completed_today_row = conn.execute(
        """
        SELECT planned_date
        FROM tasks
        WHERE status = 'completed'
          AND completed_at IS NOT NULL
          AND date(completed_at) = date(?)
        ORDER BY date(planned_date) ASC, day_num ASC
        LIMIT 1
        """,
        (today_str,),
    ).fetchone()

    pending_date = None
    if pending_row:
        try:
            pending_date = dt.date.fromisoformat(pending_row["planned_date"])
        except ValueError:
            pending_date = None

    completed_today_date = None
    if completed_today_row:
        try:
            completed_today_date = dt.date.fromisoformat(
                completed_today_row["planned_date"]
            )
        except ValueError:
            completed_today_date = None

    # 逾期/今天未完成优先处理。
    if pending_date and pending_date <= today:
        return pending_date

    candidates = [d for d in (completed_today_date, pending_date) if d is not None]
    if not candidates:
        return today
    return min(candidates)


@app.route("/")
def index():
    ensure_ready()
    sim_today = parse_sim_today(request.args.get("sim_today"))
    preview_mode = sim_today is not None
    effective_today = sim_today or china_today()

    if not preview_mode:
        auto_days, auto_affected = run_auto_rollover_if_needed()
        if auto_days > 0:
            flash(
                f"自动日结已执行：处理 {auto_days} 天，顺延 {auto_affected} 个未完成任务。",
                "info",
            )
    else:
        flash("当前为预览模式：使用临时日期，仅查看不保存。", "info")

    conn = get_conn()
    today = effective_today
    anchor_date = pick_anchor_task_date(conn, today)
    raw_date = request.args.get("date")
    if raw_date:
        display_date = parse_view_date(raw_date, effective_today)
        # 规则：
        # 1) 过去日期：用于回看历史，任务按该日期显示。
        # 2) 今天及未来日期：以“今天锚点任务”为基准，每天顺推 1 个任务。
        if display_date < today:
            auto_focus = False
            task_date = display_date
        else:
            delta_days = (display_date - today).days
            task_date = anchor_date + dt.timedelta(days=delta_days)
            auto_focus = True
    else:
        display_date = effective_today
        task_date = anchor_date
        auto_focus = True

    display_date_str = display_date.isoformat()
    task_date_str = task_date.isoformat()
    prev_date = (display_date - dt.timedelta(days=1)).isoformat()
    next_date = (display_date + dt.timedelta(days=1)).isoformat()
    is_today = display_date == today

    planned_tasks = conn.execute(
        """
        SELECT id, day_num, stage, planned_date, title, status, completed_at
        FROM tasks
        WHERE date(planned_date) = date(?)
        ORDER BY day_num ASC
        """,
        (task_date_str,),
    ).fetchall()
    planned_pending_tasks = [t for t in planned_tasks if t["status"] == "pending"]
    completed_on_display_date = conn.execute(
        """
        SELECT id, day_num, stage, planned_date, title, status, completed_at
        FROM tasks
        WHERE status = 'completed'
          AND completed_at IS NOT NULL
          AND date(completed_at) = date(?)
        ORDER BY datetime(completed_at) ASC, day_num ASC
        """,
        (display_date_str,),
    ).fetchall()

    # 当天任务列表规则：
    # 1) 当天应做但未完成（按 task_date 映射）；
    # 2) 当天实际完成的任务（按 completed_at 的日期归属）。
    # 已完成任务只出现在“完成那天”，避免在后续日期重复出现。
    daily_task_map: dict[int, sqlite3.Row] = {}
    for t in planned_pending_tasks:
        daily_task_map[t["id"]] = t
    for t in completed_on_display_date:
        daily_task_map[t["id"]] = t
    daily_tasks = sorted(
        daily_task_map.values(),
        key=lambda t: (t["day_num"], t["completed_at"] or ""),
    )
    all_tasks = conn.execute(
        """
        SELECT id, day_num, stage, planned_date, title, status, completed_at
        FROM tasks
        ORDER BY date(planned_date) ASC, day_num ASC
        """
    ).fetchall()

    leave_logs = conn.execute(
        """
        SELECT leave_date, reason, affected_count, created_at
        FROM leave_logs
        ORDER BY id DESC
        LIMIT 15
        """
    ).fetchall()
    notes = conn.execute(
        """
        SELECT id, note_date, title, content, created_at, updated_at
        FROM notes
        WHERE date(note_date) = date(?)
        ORDER BY datetime(updated_at) DESC, id DESC
        """,
        (display_date_str,),
    ).fetchall()
    auto_rollover_enabled = (
        conn.execute(
            "SELECT value FROM app_meta WHERE key = 'auto_rollover_enabled'"
        ).fetchone()["value"]
        == "1"
    )
    conn.close()

    edit_note_raw = request.args.get("edit_note", "").strip()
    edit_note_id = int(edit_note_raw) if edit_note_raw.isdigit() else None

    total = len(all_tasks)
    completed = sum(1 for t in all_tasks if t["status"] == "completed")
    pending = total - completed
    progress = round((completed / total) * 100, 1) if total else 0

    overdue_tasks = [
        t for t in all_tasks if t["status"] == "pending" and t["planned_date"] < display_date_str
    ]
    daily_total = len(planned_tasks)
    daily_completed = len(completed_on_display_date)
    daily_pending = len(planned_pending_tasks)
    daily_progress = (
        round((daily_completed / daily_total) * 100, 1) if daily_total else 0
    )
    daily_progress_bar = min(daily_progress, 100) if daily_total else 0
    extra_completed_count = sum(
        1 for t in completed_on_display_date if t["planned_date"] > display_date_str
    )
    daily_leetcode = sum(leetcode_target(t["day_num"]) for t in daily_tasks)
    daily_leetcode_pending = sum(
        leetcode_target(t["day_num"]) for t in daily_tasks if t["status"] == "pending"
    )

    sim_today_str = sim_today.isoformat() if sim_today else ""
    if preview_mode:
        prev_url = url_for("index", date=prev_date, sim_today=sim_today_str)
        next_url = url_for("index", date=next_date, sim_today=sim_today_str)
        overview_url = url_for("overview", sim_today=sim_today_str)
        clear_sim_url = url_for("index")
    else:
        prev_url = url_for("index", date=prev_date)
        next_url = url_for("index", date=next_date)
        overview_url = url_for("overview")
        clear_sim_url = url_for("index")

    return render_template(
        "index.html",
        tasks=daily_tasks,
        notes=notes,
        edit_note_id=edit_note_id,
        leave_logs=leave_logs,
        display_date=display_date_str,
        task_date=task_date_str,
        real_today=china_today().isoformat(),
        effective_today=effective_today.isoformat(),
        preview_mode=preview_mode,
        sim_today=sim_today_str,
        prev_url=prev_url,
        next_url=next_url,
        overview_url=overview_url,
        clear_sim_url=clear_sim_url,
        prev_date=prev_date,
        next_date=next_date,
        is_today=is_today,
        auto_focus=auto_focus,
        total=total,
        completed=completed,
        pending=pending,
        progress=progress,
        daily_total=daily_total,
        daily_completed=daily_completed,
        daily_pending=daily_pending,
        daily_progress=daily_progress,
        daily_progress_bar=daily_progress_bar,
        extra_completed_count=extra_completed_count,
        overdue_count=len(overdue_tasks),
        daily_leetcode=daily_leetcode,
        daily_leetcode_pending=daily_leetcode_pending,
        leetcode_target=leetcode_target,
        auto_rollover_enabled=auto_rollover_enabled,
    )


@app.post("/checkin/<int:task_id>")
def checkin(task_id: int):
    ensure_ready()
    display_date = parse_view_date(request.form.get("display_date")).isoformat()
    sim_today = request.form.get("sim_today", "").strip()
    if sim_today:
        flash("预览模式下不能修改数据，请先恢复真实今天。", "info")
        return redirect(url_for("index", date=display_date, sim_today=sim_today))
    now = china_now().strftime("%Y-%m-%d %H:%M:%S")
    conn = get_conn()
    row = conn.execute("SELECT status FROM tasks WHERE id = ?", (task_id,)).fetchone()
    if row is None:
        conn.close()
        flash("任务不存在。", "error")
        return redirect(url_for("index", date=display_date))
    if row["status"] == "completed":
        conn.close()
        flash("该任务已打卡完成。", "info")
        return redirect(url_for("index", date=display_date))

    conn.execute(
        "UPDATE tasks SET status = 'completed', completed_at = ? WHERE id = ?",
        (now, task_id),
    )
    conn.commit()
    conn.close()
    flash("打卡成功。", "success")
    return redirect(url_for("index", date=display_date))


@app.post("/undo/<int:task_id>")
def undo(task_id: int):
    ensure_ready()
    display_date = parse_view_date(request.form.get("display_date")).isoformat()
    sim_today = request.form.get("sim_today", "").strip()
    if sim_today:
        flash("预览模式下不能修改数据，请先恢复真实今天。", "info")
        return redirect(url_for("index", date=display_date, sim_today=sim_today))
    conn = get_conn()
    row = conn.execute("SELECT status FROM tasks WHERE id = ?", (task_id,)).fetchone()
    if row is None:
        conn.close()
        flash("任务不存在。", "error")
        return redirect(url_for("index", date=display_date))

    conn.execute(
        "UPDATE tasks SET status = 'pending', completed_at = NULL WHERE id = ?",
        (task_id,),
    )
    conn.commit()
    conn.close()
    flash("已撤销打卡。", "info")
    return redirect(url_for("index", date=display_date))


@app.post("/leave")
def leave():
    ensure_ready()
    leave_date = parse_view_date(request.form.get("display_date")).isoformat()
    sim_today = request.form.get("sim_today", "").strip()
    if sim_today:
        flash("预览模式下不能修改数据，请先恢复真实今天。", "info")
        return redirect(url_for("index", date=leave_date, sim_today=sim_today))
    reason = request.form.get("reason", "").strip()

    conn = get_conn()
    affected = conn.execute(
        """
        SELECT COUNT(*) AS c
        FROM tasks
        WHERE status = 'pending' AND date(planned_date) >= date(?)
        """,
        (leave_date,),
    ).fetchone()["c"]

    conn.execute(
        """
        UPDATE tasks
        SET planned_date = date(planned_date, '+1 day')
        WHERE status = 'pending' AND date(planned_date) >= date(?)
        """,
        (leave_date,),
    )
    conn.execute(
        """
        INSERT INTO leave_logs (leave_date, reason, affected_count)
        VALUES (?, ?, ?)
        """,
        (leave_date, reason, affected),
    )
    conn.commit()
    conn.close()

    flash(f"请假成功：从 {leave_date} 开始顺延 {affected} 个未完成任务。", "success")
    return redirect(url_for("index", date=leave_date))


@app.post("/rollover_day")
def rollover_day():
    ensure_ready()
    view_date = parse_view_date(request.form.get("display_date"))
    sim_today = request.form.get("sim_today", "").strip()
    if sim_today:
        flash("预览模式下不能修改数据，请先恢复真实今天。", "info")
        return redirect(url_for("index", date=view_date.isoformat(), sim_today=sim_today))
    view_date_str = view_date.isoformat()
    next_date_str = (view_date + dt.timedelta(days=1)).isoformat()

    conn = get_conn()
    affected = conn.execute(
        """
        SELECT COUNT(*) AS c
        FROM tasks
        WHERE status = 'pending' AND date(planned_date) = date(?)
        """,
        (view_date_str,),
    ).fetchone()["c"]

    conn.execute(
        """
        UPDATE tasks
        SET planned_date = date(planned_date, '+1 day')
        WHERE status = 'pending' AND date(planned_date) = date(?)
        """,
        (view_date_str,),
    )
    conn.execute(
        """
        INSERT INTO leave_logs (leave_date, reason, affected_count)
        VALUES (?, ?, ?)
        """,
        (view_date_str, "未完成项顺延到明天", affected),
    )
    conn.commit()
    conn.close()

    flash(f"已将当天 {affected} 个未完成项顺延到 {next_date_str}。", "success")
    return redirect(url_for("index", date=next_date_str))


@app.post("/toggle_auto_rollover")
def toggle_auto_rollover():
    ensure_ready()
    display_date = parse_view_date(request.form.get("display_date")).isoformat()
    sim_today = request.form.get("sim_today", "").strip()
    if sim_today:
        flash("预览模式下不能修改数据，请先恢复真实今天。", "info")
        return redirect(url_for("index", date=display_date, sim_today=sim_today))
    conn = get_conn()
    current = get_meta(conn, "auto_rollover_enabled", "1")
    new_value = "0" if current == "1" else "1"
    set_meta(conn, "auto_rollover_enabled", new_value)
    set_meta(conn, "last_auto_rollover_date", china_today().isoformat())
    conn.commit()
    conn.close()
    if new_value == "1":
        flash("自动日结已开启。", "success")
    else:
        flash("自动日结已关闭。", "info")
    return redirect(url_for("index", date=display_date))


@app.post("/notes/create")
def create_note():
    ensure_ready()
    display_date = parse_view_date(request.form.get("display_date")).isoformat()
    sim_today = request.form.get("sim_today", "").strip()
    if sim_today:
        flash("预览模式下不能修改数据，请先恢复真实今天。", "info")
        return redirect(url_for("index", date=display_date, sim_today=sim_today))

    title = request.form.get("title", "").strip()
    content = request.form.get("content", "").strip()
    if not title:
        flash("笔记标题不能为空。", "error")
        return redirect(url_for("index", date=display_date))
    if not content:
        flash("笔记内容不能为空。", "error")
        return redirect(url_for("index", date=display_date))

    now = china_now().strftime("%Y-%m-%d %H:%M:%S")
    conn = get_conn()
    conn.execute(
        """
        INSERT INTO notes (note_date, title, content, created_at, updated_at)
        VALUES (?, ?, ?, ?, ?)
        """,
        (display_date, title, content, now, now),
    )
    conn.commit()
    conn.close()
    flash("笔记已保存。", "success")
    return redirect(url_for("index", date=display_date))


@app.post("/notes/<int:note_id>/update")
def update_note(note_id: int):
    ensure_ready()
    display_date = parse_view_date(request.form.get("display_date")).isoformat()
    sim_today = request.form.get("sim_today", "").strip()
    if sim_today:
        flash("预览模式下不能修改数据，请先恢复真实今天。", "info")
        return redirect(url_for("index", date=display_date, sim_today=sim_today))

    title = request.form.get("title", "").strip()
    content = request.form.get("content", "").strip()
    if not title:
        flash("笔记标题不能为空。", "error")
        return redirect(url_for("index", date=display_date, edit_note=note_id))
    if not content:
        flash("笔记内容不能为空。", "error")
        return redirect(url_for("index", date=display_date, edit_note=note_id))

    now = china_now().strftime("%Y-%m-%d %H:%M:%S")
    conn = get_conn()
    row = conn.execute("SELECT note_date FROM notes WHERE id = ?", (note_id,)).fetchone()
    if row is None:
        conn.close()
        flash("笔记不存在。", "error")
        return redirect(url_for("index", date=display_date))

    conn.execute(
        """
        UPDATE notes
        SET title = ?, content = ?, updated_at = ?
        WHERE id = ?
        """,
        (title, content, now, note_id),
    )
    conn.commit()
    conn.close()
    flash("笔记已更新。", "success")
    return redirect(url_for("index", date=display_date))


@app.post("/notes/<int:note_id>/delete")
def delete_note(note_id: int):
    ensure_ready()
    display_date = parse_view_date(request.form.get("display_date")).isoformat()
    sim_today = request.form.get("sim_today", "").strip()
    if sim_today:
        flash("预览模式下不能修改数据，请先恢复真实今天。", "info")
        return redirect(url_for("index", date=display_date, sim_today=sim_today))

    conn = get_conn()
    row = conn.execute("SELECT id FROM notes WHERE id = ?", (note_id,)).fetchone()
    if row is None:
        conn.close()
        flash("笔记不存在。", "error")
        return redirect(url_for("index", date=display_date))

    conn.execute("DELETE FROM notes WHERE id = ?", (note_id,))
    conn.commit()
    conn.close()
    flash("笔记已删除。", "info")
    return redirect(url_for("index", date=display_date))


@app.route("/notes")
def notes_list():
    ensure_ready()
    sim_today = parse_sim_today(request.args.get("sim_today"))
    preview_mode = sim_today is not None
    effective_today = sim_today or china_today()

    conn = get_conn()
    notes = conn.execute(
        """
        SELECT id, note_date, title, created_at, updated_at
        FROM notes
        ORDER BY date(note_date) ASC, datetime(updated_at) DESC, id DESC
        """
    ).fetchall()
    conn.close()

    return render_template(
        "notes_list.html",
        notes=notes,
        preview_mode=preview_mode,
        today=effective_today.isoformat(),
        sim_today=sim_today.isoformat() if sim_today else "",
        real_today=china_today().isoformat(),
    )


@app.route("/notes/<int:note_id>")
def note_detail(note_id: int):
    ensure_ready()
    sim_today = parse_sim_today(request.args.get("sim_today"))
    preview_mode = sim_today is not None
    effective_today = sim_today or china_today()

    conn = get_conn()
    note = conn.execute(
        """
        SELECT id, note_date, title, content, created_at, updated_at
        FROM notes
        WHERE id = ?
        """,
        (note_id,),
    ).fetchone()
    conn.close()
    if note is None:
        flash("笔记不存在。", "error")
        if preview_mode:
            return redirect(url_for("notes_list", sim_today=sim_today.isoformat()))
        return redirect(url_for("notes_list"))

    return render_template(
        "note_detail.html",
        note=note,
        preview_mode=preview_mode,
        today=effective_today.isoformat(),
        sim_today=sim_today.isoformat() if sim_today else "",
        real_today=china_today().isoformat(),
    )


@app.route("/overview")
def overview():
    ensure_ready()
    sim_today = parse_sim_today(request.args.get("sim_today"))
    preview_mode = sim_today is not None
    if not preview_mode:
        run_auto_rollover_if_needed()
    effective_today = sim_today or china_today()
    conn = get_conn()
    tasks = conn.execute(
        """
        SELECT id, day_num, stage, planned_date, title, status, completed_at
        FROM tasks
        ORDER BY date(planned_date) ASC, day_num ASC
        """
    ).fetchall()
    leave_logs = conn.execute(
        """
        SELECT leave_date, reason, affected_count, created_at
        FROM leave_logs
        ORDER BY id DESC
        LIMIT 30
        """
    ).fetchall()
    conn.close()

    total = len(tasks)
    completed = sum(1 for t in tasks if t["status"] == "completed")
    pending = total - completed
    progress = round((completed / total) * 100, 1) if total else 0

    # 全局页按“完成日期”展示：
    # 完成/总数 = 当天实际完成数 / 当天计划任务数。
    planned_count_by_date: dict[str, int] = {}
    planned_tasks_by_date: dict[str, list[sqlite3.Row]] = {}
    completed_tasks_by_date: dict[str, list[sqlite3.Row]] = {}
    all_dates: set[str] = set()

    for t in tasks:
        planned_date = t["planned_date"]
        all_dates.add(planned_date)
        planned_count_by_date[planned_date] = planned_count_by_date.get(planned_date, 0) + 1
        planned_tasks_by_date.setdefault(planned_date, []).append(t)
        if t["status"] == "completed" and t["completed_at"]:
            completed_date = t["completed_at"][:10]
            all_dates.add(completed_date)
            completed_tasks_by_date.setdefault(completed_date, []).append(t)

    date_details: list[dict[str, object]] = []
    for date_key in sorted(all_dates):
        day_completed_tasks = completed_tasks_by_date.get(date_key, [])
        day_completed_tasks = sorted(
            day_completed_tasks,
            key=lambda t: (t["completed_at"] or "", t["day_num"]),
        )
        total_count = planned_count_by_date.get(date_key, 0)
        completed_count = len(day_completed_tasks)
        rate = round((completed_count / total_count) * 100, 1) if total_count else 0
        planned_day_tasks = sorted(
            planned_tasks_by_date.get(date_key, []),
            key=lambda t: t["day_num"],
        )
        date_details.append(
            {
                "date_key": date_key,
                "total_count": total_count,
                "completed_count": completed_count,
                "rate": rate,
                "completed_tasks": [
                    {
                        "day_num": t["day_num"],
                        "title": t["title"],
                        "completed_at": t["completed_at"],
                    }
                    for t in day_completed_tasks
                ],
                "planned_tasks": [
                    {
                        "day_num": t["day_num"],
                        "title": t["title"],
                    }
                    for t in planned_day_tasks
                ],
            }
        )

    return render_template(
        "overview.html",
        tasks=tasks,
        date_details=date_details,
        leave_logs=leave_logs,
        total=total,
        completed=completed,
        pending=pending,
        progress=progress,
        today=effective_today.isoformat(),
        real_today=china_today().isoformat(),
        preview_mode=preview_mode,
        sim_today=sim_today.isoformat() if sim_today else "",
    )


if __name__ == "__main__":
    ensure_ready()
    host = os.environ.get("HOST", "127.0.0.1")
    port = int(os.environ.get("PORT", "5000"))
    debug = os.environ.get("DEBUG", "1") == "1"
    app.run(host=host, port=port, debug=debug, use_reloader=debug)
