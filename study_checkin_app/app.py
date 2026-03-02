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
NET_PLAN_PATH = APP_DIR.parent / "net.md"

DAY_LINE_RE = re.compile(r"- Day(\d+)\s+\((\d{4}-\d{2}-\d{2})\):\s*(.+)$")
NET_DAY_LINE_RE = re.compile(
    r"^\*\s+\*\*(\d{1,2})月(\d{1,2})日 今日任务\*\*：(.+)$"
)
CHINA_TZ = ZoneInfo("Asia/Shanghai")
FITNESS_TITLE = "健身任务：若干健身动作25*4"
FITNESS_STAGE = "每日健身"
LEETCODE_STAGE = "每日刷题"
LEETCODE_TITLE_PREFIX = "LeetCode任务："
LEETCODE_DAY_OFFSET = 100000
NETWORK_STAGE = "网络学习"
NETWORK_TITLE_PREFIX = "计网任务："
NETWORK_DAY_OFFSET = 200000
SQL_BASE_DAY = f"""
CASE
    WHEN day_num >= {NETWORK_DAY_OFFSET} THEN day_num - {NETWORK_DAY_OFFSET}
    WHEN day_num >= {LEETCODE_DAY_OFFSET} THEN day_num - {LEETCODE_DAY_OFFSET}
    WHEN day_num < 0 THEN -day_num
    ELSE day_num
END
""".strip()
SQL_TASK_ORDER = f"""
CASE
    WHEN day_num > 0 AND day_num < {LEETCODE_DAY_OFFSET} THEN 0
    WHEN day_num < 0 THEN 1
    WHEN day_num >= {LEETCODE_DAY_OFFSET} AND day_num < {NETWORK_DAY_OFFSET} THEN 2
    ELSE 3
END
""".strip()

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
    base_day = day_base_num(day_num)
    if base_day <= 20:
        return 0
    # Day21 起：3,2,3,2... 交替
    return 3 if (base_day - 21) % 2 == 0 else 2


def leetcode_day_num(day_num: int) -> int:
    return LEETCODE_DAY_OFFSET + day_num


def task_kind(day_num: int) -> str:
    if day_num >= NETWORK_DAY_OFFSET:
        return "network"
    if day_num >= LEETCODE_DAY_OFFSET:
        return "leetcode"
    if day_num < 0:
        return "fitness"
    return "study"


def task_sort_order(day_num: int) -> int:
    kind = task_kind(day_num)
    if kind == "study":
        return 0
    if kind == "fitness":
        return 1
    if kind == "leetcode":
        return 2
    return 3


def day_base_num(day_num: int) -> int:
    if day_num >= NETWORK_DAY_OFFSET:
        return day_num - NETWORK_DAY_OFFSET
    if day_num >= LEETCODE_DAY_OFFSET:
        return day_num - LEETCODE_DAY_OFFSET
    return -day_num if day_num < 0 else day_num


def day_label(day_num: int) -> str:
    kind = task_kind(day_num)
    if kind == "fitness":
        return "健身"
    if kind == "leetcode":
        return "LeetCode"
    if kind == "network":
        return "计网"
    if day_num < 0:
        return "健身"
    return f"Day{day_num}"


def china_now() -> dt.datetime:
    return dt.datetime.now(CHINA_TZ)


def china_today() -> dt.date:
    return china_now().date()


def ensure_ready() -> None:
    init_db()
    seed_tasks_if_empty()
    ensure_meta_defaults()
    ensure_daily_fitness_tasks()
    ensure_daily_leetcode_tasks()
    ensure_daily_network_tasks()


def ensure_daily_fitness_tasks() -> None:
    """
    保障“从今天开始每天一条独立健身任务”：
    - 清理历史上把健身文案拼接到学习标题后的旧格式；
    - 为未来学习任务补齐同日健身任务（day_num 使用负号区分）。
    """
    conn = get_conn()
    today_str = china_today().isoformat()

    # 清理旧格式："...；健身任务：若干健身动作25*4"
    conn.execute(
        """
        UPDATE tasks
        SET title = REPLACE(title, ?, '')
        WHERE title LIKE '%' || ? || '%'
        """,
        ("；" + FITNESS_TITLE, "；" + FITNESS_TITLE),
    )

    study_rows = conn.execute(
        """
        SELECT day_num, planned_date
        FROM tasks
        WHERE day_num > 0
          AND day_num < ?
          AND date(planned_date) >= date(?)
        ORDER BY day_num ASC
        """,
        (LEETCODE_DAY_OFFSET, today_str),
    ).fetchall()
    fitness_rows = conn.execute(
        """
        SELECT day_num, planned_date, status
        FROM tasks
        WHERE day_num < 0
        """
    ).fetchall()
    fitness_map = {abs(r["day_num"]): r for r in fitness_rows}

    for row in study_rows:
        study_day_num = int(row["day_num"])
        study_date = row["planned_date"]
        fit = fitness_map.get(study_day_num)
        if fit is None:
            conn.execute(
                """
                INSERT INTO tasks (day_num, stage, planned_date, title, status)
                VALUES (?, ?, ?, ?, 'pending')
                """,
                (-study_day_num, FITNESS_STAGE, study_date, FITNESS_TITLE),
            )
            continue

        # 已存在则修正标题/阶段；若未完成则对齐计划日期。
        conn.execute(
            """
            UPDATE tasks
            SET stage = ?, title = ?
            WHERE day_num = ?
            """,
            (FITNESS_STAGE, FITNESS_TITLE, -study_day_num),
        )
        if fit["status"] == "pending" and fit["planned_date"] != study_date:
            conn.execute(
                "UPDATE tasks SET planned_date = ? WHERE day_num = ?",
                (study_date, -study_day_num),
            )

    conn.commit()
    conn.close()


def ensure_daily_leetcode_tasks() -> None:
    """
    保障“从今天开始，LeetCode 题量>0 的天有独立任务行”：
    - 0 题天不创建 LeetCode 行；
    - 已有行会自动修正标题/阶段；
    - 未完成行会与对应学习任务日期对齐。
    """
    conn = get_conn()
    today_str = china_today().isoformat()

    study_rows = conn.execute(
        """
        SELECT day_num, planned_date
        FROM tasks
        WHERE day_num > 0
          AND day_num < ?
          AND date(planned_date) >= date(?)
        ORDER BY day_num ASC
        """,
        (LEETCODE_DAY_OFFSET, today_str),
    ).fetchall()
    lc_rows = conn.execute(
        """
        SELECT day_num, planned_date, status
        FROM tasks
        WHERE day_num >= ?
        """,
        (LEETCODE_DAY_OFFSET,),
    ).fetchall()
    lc_map = {int(r["day_num"]) - LEETCODE_DAY_OFFSET: r for r in lc_rows}

    valid_days: set[int] = set()
    for row in study_rows:
        day_num = int(row["day_num"])
        study_date = row["planned_date"]
        target = leetcode_target(day_num)
        lc_day_num = leetcode_day_num(day_num)
        lc_title = f"{LEETCODE_TITLE_PREFIX}{target}题"
        existing = lc_map.get(day_num)

        if target <= 0:
            if existing and existing["status"] == "pending":
                conn.execute("DELETE FROM tasks WHERE day_num = ?", (lc_day_num,))
            continue

        valid_days.add(day_num)
        if existing is None:
            conn.execute(
                """
                INSERT INTO tasks (day_num, stage, planned_date, title, status)
                VALUES (?, ?, ?, ?, 'pending')
                """,
                (lc_day_num, LEETCODE_STAGE, study_date, lc_title),
            )
            continue

        conn.execute(
            """
            UPDATE tasks
            SET stage = ?, title = ?
            WHERE day_num = ?
            """,
            (LEETCODE_STAGE, lc_title, lc_day_num),
        )
        if existing["status"] == "pending" and existing["planned_date"] != study_date:
            conn.execute(
                "UPDATE tasks SET planned_date = ? WHERE day_num = ?",
                (study_date, lc_day_num),
            )

    # 清理无效且未完成的 LeetCode 行（如 0 题日或计划已调整）。
    for row in lc_rows:
        row_day_num = int(row["day_num"])
        base_day = row_day_num - LEETCODE_DAY_OFFSET
        if base_day not in valid_days and row["status"] == "pending":
            conn.execute("DELETE FROM tasks WHERE day_num = ?", (row_day_num,))

    conn.commit()
    conn.close()


def parse_network_tasks(text: str, year: int) -> dict[str, str]:
    """
    解析 net.md：
    * **3月1日 今日任务**：......
    返回 {YYYY-MM-DD: title}
    """
    date_to_title: dict[str, str] = {}
    for raw in text.splitlines():
        line = raw.strip()
        m = NET_DAY_LINE_RE.match(line)
        if not m:
            continue
        month = int(m.group(1))
        day = int(m.group(2))
        title = m.group(3).strip()
        # 某些行把“复习计划”写在同一行，这里只保留“今日任务”部分。
        title = re.split(r"\s+\*\s+\*\*复习计划\*\*：", title)[0].strip()
        try:
            d = dt.date(year, month, day)
        except ValueError:
            continue
        date_to_title[d.isoformat()] = title
    return date_to_title


def ensure_daily_network_tasks() -> None:
    """
    按 net.md 为每天生成“计网任务”独立行：
    - 与学习 Day 行按同一 base day 关联；
    - 仅对未完成项同步日期/标题；
    - 不动已完成历史记录。
    """
    if not NET_PLAN_PATH.exists():
        return

    conn = get_conn()
    today_str = china_today().isoformat()
    today_year = china_today().year
    net_text = NET_PLAN_PATH.read_text(encoding="utf-8")
    date_to_title = parse_network_tasks(net_text, today_year)

    study_rows = conn.execute(
        """
        SELECT day_num, planned_date
        FROM tasks
        WHERE day_num > 0
          AND day_num < ?
          AND date(planned_date) >= date(?)
        ORDER BY day_num ASC
        """,
        (LEETCODE_DAY_OFFSET, today_str),
    ).fetchall()
    net_rows = conn.execute(
        """
        SELECT day_num, planned_date, status
        FROM tasks
        WHERE day_num >= ?
        """,
        (NETWORK_DAY_OFFSET,),
    ).fetchall()
    net_map = {int(r["day_num"]) - NETWORK_DAY_OFFSET: r for r in net_rows}

    valid_days: set[int] = set()
    for row in study_rows:
        day_num = int(row["day_num"])
        study_date = row["planned_date"]
        network_title = date_to_title.get(study_date)
        net_day_num = NETWORK_DAY_OFFSET + day_num
        existing = net_map.get(day_num)

        if not network_title:
            if existing and existing["status"] == "pending":
                conn.execute("DELETE FROM tasks WHERE day_num = ?", (net_day_num,))
            continue

        valid_days.add(day_num)
        full_title = f"{NETWORK_TITLE_PREFIX}{network_title}"
        if existing is None:
            conn.execute(
                """
                INSERT INTO tasks (day_num, stage, planned_date, title, status)
                VALUES (?, ?, ?, ?, 'pending')
                """,
                (net_day_num, NETWORK_STAGE, study_date, full_title),
            )
            continue

        conn.execute(
            """
            UPDATE tasks
            SET stage = ?, title = ?
            WHERE day_num = ?
            """,
            (NETWORK_STAGE, full_title, net_day_num),
        )
        if existing["status"] == "pending" and existing["planned_date"] != study_date:
            conn.execute(
                "UPDATE tasks SET planned_date = ? WHERE day_num = ?",
                (study_date, net_day_num),
            )

    # 清理无效且未完成的计网任务（例如计划日期不在 net.md 覆盖范围）。
    for row in net_rows:
        row_day_num = int(row["day_num"])
        base_day = row_day_num - NETWORK_DAY_OFFSET
        if base_day not in valid_days and row["status"] == "pending":
            conn.execute("DELETE FROM tasks WHERE day_num = ?", (row_day_num,))

    conn.commit()
    conn.close()


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
        f"""
        SELECT planned_date
        FROM tasks
        WHERE status = 'pending'
        ORDER BY date(planned_date) ASC, {SQL_BASE_DAY} ASC, {SQL_TASK_ORDER} ASC
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
    pending_study_row = conn.execute(
        """
        SELECT planned_date
        FROM tasks
        WHERE status = 'pending'
          AND day_num > 0
          AND day_num < ?
        ORDER BY date(planned_date) ASC, day_num ASC
        LIMIT 1
        """,
        (LEETCODE_DAY_OFFSET,),
    ).fetchone()
    if pending_study_row:
        try:
            return dt.date.fromisoformat(pending_study_row["planned_date"])
        except ValueError:
            pass

    pending_row = conn.execute(
        f"""
        SELECT planned_date
        FROM tasks
        WHERE status = 'pending'
        ORDER BY date(planned_date) ASC, {SQL_BASE_DAY} ASC, {SQL_TASK_ORDER} ASC
        LIMIT 1
        """,
    ).fetchone()
    completed_today_row = conn.execute(
        f"""
        SELECT planned_date
        FROM tasks
        WHERE status = 'completed'
          AND completed_at IS NOT NULL
          AND date(completed_at) = date(?)
        ORDER BY date(planned_date) ASC, {SQL_BASE_DAY} ASC, {SQL_TASK_ORDER} ASC
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

    # 只要还有未完成任务，就以最早 pending 为锚点，避免今天出现空白列表。
    if pending_date is not None:
        return pending_date

    if completed_today_date is not None:
        return completed_today_date
    return today


def list_pending_dates_from(
    conn: sqlite3.Connection, start_date: dt.date
) -> list[dt.date]:
    rows = conn.execute(
        """
        SELECT DISTINCT planned_date
        FROM tasks
        WHERE status = 'pending' AND date(planned_date) >= date(?)
        ORDER BY date(planned_date) ASC
        """,
        (start_date.isoformat(),),
    ).fetchall()
    result: list[dt.date] = []
    for r in rows:
        try:
            result.append(dt.date.fromisoformat(r["planned_date"]))
        except ValueError:
            continue
    return result


def list_pending_study_dates_from(
    conn: sqlite3.Connection, start_date: dt.date
) -> list[dt.date]:
    rows = conn.execute(
        """
        SELECT DISTINCT planned_date
        FROM tasks
        WHERE status = 'pending'
          AND day_num > 0
          AND day_num < ?
          AND date(planned_date) >= date(?)
        ORDER BY date(planned_date) ASC, day_num ASC
        """,
        (LEETCODE_DAY_OFFSET, start_date.isoformat()),
    ).fetchall()
    result: list[dt.date] = []
    for r in rows:
        try:
            result.append(dt.date.fromisoformat(r["planned_date"]))
        except ValueError:
            continue
    return result


def map_display_to_task_date(
    conn: sqlite3.Connection,
    today: dt.date,
    display_date: dt.date,
    anchor_date: dt.date,
) -> dt.date:
    """
    将“查看日期”映射到“任务日期”：
    - 当用户提前完成未来任务后，跳过已无 pending 的空档日期；
    - 保持“今天”锚点逻辑不变。
    """
    if display_date <= today:
        return anchor_date

    offset = (display_date - today).days
    pending_dates = list_pending_study_dates_from(conn, anchor_date)
    if not pending_dates:
        pending_dates = list_pending_dates_from(conn, anchor_date)
    if not pending_dates:
        return anchor_date + dt.timedelta(days=offset)

    anchor_has_pending = pending_dates[0] == anchor_date
    if anchor_has_pending:
        # offset=1 -> 下一个 pending 日
        if offset < len(pending_dates):
            return pending_dates[offset]
        return pending_dates[-1] + dt.timedelta(days=offset - (len(pending_dates) - 1))

    # anchor 当天已无 pending，offset=1 对应第一个 pending 日
    if offset <= len(pending_dates):
        return pending_dates[offset - 1]
    return pending_dates[-1] + dt.timedelta(days=offset - len(pending_dates))


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
        # 2) 今天及未来日期：以“今天锚点任务”为基准，按 pending 序列顺推，
        #    跳过因“超前完成”产生的空档日期。
        if display_date < today:
            auto_focus = False
            task_date = display_date
        else:
            task_date = map_display_to_task_date(conn, today, display_date, anchor_date)
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

    study_tasks_on_task_date = conn.execute(
        f"""
        SELECT id, day_num, stage, planned_date, title, status, completed_at
        FROM tasks
        WHERE day_num > 0
          AND day_num < ?
          AND date(planned_date) = date(?)
        ORDER BY {SQL_BASE_DAY} ASC, {SQL_TASK_ORDER} ASC
        """,
        (LEETCODE_DAY_OFFSET, task_date_str),
    ).fetchall()
    study_pending_tasks = [t for t in study_tasks_on_task_date if t["status"] == "pending"]
    non_study_tasks_on_display_date = conn.execute(
        f"""
        SELECT id, day_num, stage, planned_date, title, status, completed_at
        FROM tasks
        WHERE (day_num < 0 OR day_num >= ?)
          AND date(planned_date) = date(?)
        ORDER BY {SQL_BASE_DAY} ASC, {SQL_TASK_ORDER} ASC
        """,
        (LEETCODE_DAY_OFFSET, display_date_str),
    ).fetchall()
    non_study_pending_tasks = [
        t for t in non_study_tasks_on_display_date if t["status"] == "pending"
    ]
    completed_on_display_date = conn.execute(
        f"""
        SELECT id, day_num, stage, planned_date, title, status, completed_at
        FROM tasks
        WHERE status = 'completed'
          AND completed_at IS NOT NULL
          AND date(completed_at) = date(?)
        ORDER BY datetime(completed_at) ASC,
                 {SQL_BASE_DAY} ASC, {SQL_TASK_ORDER} ASC
        """,
        (display_date_str,),
    ).fetchall()

    # 当天任务列表规则：
    # 1) 当天应做但未完成（按 task_date 映射）；
    # 2) 当天实际完成的任务（按 completed_at 的日期归属）。
    # 已完成任务只出现在“完成那天”，避免在后续日期重复出现。
    daily_task_map: dict[int, sqlite3.Row] = {}
    for t in study_pending_tasks:
        daily_task_map[t["id"]] = t
    for t in non_study_pending_tasks:
        daily_task_map[t["id"]] = t
    for t in completed_on_display_date:
        daily_task_map[t["id"]] = t
    daily_tasks = sorted(
        daily_task_map.values(),
        key=lambda t: (
            day_base_num(t["day_num"]),
            task_sort_order(t["day_num"]),
            t["completed_at"] or "",
        ),
    )
    all_tasks = conn.execute(
        f"""
        SELECT id, day_num, stage, planned_date, title, status, completed_at
        FROM tasks
        ORDER BY date(planned_date) ASC, {SQL_BASE_DAY} ASC, {SQL_TASK_ORDER} ASC
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
    daily_total = len(study_tasks_on_task_date) + len(non_study_tasks_on_display_date)
    daily_completed = len(completed_on_display_date)
    daily_pending = len(study_pending_tasks) + len(non_study_pending_tasks)
    daily_progress = (
        round((daily_completed / daily_total) * 100, 1) if daily_total else 0
    )
    daily_progress_bar = min(daily_progress, 100) if daily_total else 0
    extra_completed_count = sum(
        1 for t in completed_on_display_date if t["planned_date"] > display_date_str
    )
    daily_leetcode = sum(
        leetcode_target(t["day_num"])
        for t in daily_tasks
        if task_kind(t["day_num"]) == "leetcode"
    )
    daily_leetcode_pending = sum(
        leetcode_target(t["day_num"])
        for t in daily_tasks
        if t["status"] == "pending" and task_kind(t["day_num"]) == "leetcode"
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
        task_kind=task_kind,
        day_label=day_label,
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
    resolved_task_id = task_id
    row = conn.execute("SELECT id, status FROM tasks WHERE id = ?", (task_id,)).fetchone()
    if row is None:
        day_num_raw = (request.form.get("day_num") or "").strip()
        if day_num_raw:
            try:
                day_num = int(day_num_raw)
            except ValueError:
                day_num = None
            if day_num is not None:
                row = conn.execute(
                    "SELECT id, status FROM tasks WHERE day_num = ?",
                    (day_num,),
                ).fetchone()
                if row is not None:
                    resolved_task_id = int(row["id"])
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
        (now, resolved_task_id),
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
    resolved_task_id = task_id
    row = conn.execute("SELECT id, status FROM tasks WHERE id = ?", (task_id,)).fetchone()
    if row is None:
        day_num_raw = (request.form.get("day_num") or "").strip()
        if day_num_raw:
            try:
                day_num = int(day_num_raw)
            except ValueError:
                day_num = None
            if day_num is not None:
                row = conn.execute(
                    "SELECT id, status FROM tasks WHERE day_num = ?",
                    (day_num,),
                ).fetchone()
                if row is not None:
                    resolved_task_id = int(row["id"])
    if row is None:
        conn.close()
        flash("任务不存在。", "error")
        return redirect(url_for("index", date=display_date))

    conn.execute(
        "UPDATE tasks SET status = 'pending', completed_at = NULL WHERE id = ?",
        (resolved_task_id,),
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
        f"""
        SELECT id, day_num, stage, planned_date, title, status, completed_at
        FROM tasks
        ORDER BY date(planned_date) ASC, {SQL_BASE_DAY} ASC, {SQL_TASK_ORDER} ASC
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
            key=lambda t: (
                t["completed_at"] or "",
                day_base_num(t["day_num"]),
                task_sort_order(t["day_num"]),
            ),
        )
        total_count = planned_count_by_date.get(date_key, 0)
        completed_count = len(day_completed_tasks)
        rate = round((completed_count / total_count) * 100, 1) if total_count else 0
        planned_day_tasks = sorted(
            planned_tasks_by_date.get(date_key, []),
            key=lambda t: (day_base_num(t["day_num"]), task_sort_order(t["day_num"])),
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
        day_label=day_label,
    )


if __name__ == "__main__":
    ensure_ready()
    host = os.environ.get("HOST", "127.0.0.1")
    port = int(os.environ.get("PORT", "5000"))
    debug = os.environ.get("DEBUG", "1") == "1"
    app.run(host=host, port=port, debug=debug, use_reloader=debug)
