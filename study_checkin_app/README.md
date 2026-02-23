# 本地学习打卡小程序

## 功能

- 从根目录的 `C_learning_plan.md` 自动读取 100 天任务。
- 日期基准使用中国时区（`Asia/Shanghai`）。
- `日视图`：默认只看某一天任务，支持左右切换前一天/后一天。
- 日视图智能定位：若今天任务已全部完成，首页会自动显示“最近未完成任务日”。
- 支持“临时修改今天”预览：只影响当前查看，不保存到数据库。
- 预览模式下为只读，不允许打卡/请假/顺延等写操作。
- `全局视图`：点击“查看全局完成情况”进入整体统计页。
- 日视图下支持当天多条笔记（标题+纯文本内容），可编辑/删除。
- 支持“查看全部笔记”：按日期排序标题，点击可查看详情。
- 打卡完成/撤销。
- 请假并自动顺延未完成任务（从当前查看日期开始整体 +1 天）。
- 可将“当天未完成项”单独顺延到明天（加到明天任务中）。
- 自动日结（默认开启）：每天首次访问时，自动将前一天未完成项顺延到下一天。
- 自动日结支持开关按钮（在日视图“当日动作”里操作）。
- 显示总进度、当日完成度（例如 3/5）、逾期数量、当日 LeetCode 题量。
- 记录最近请假日志。

## 启动方式

### 一键启动（推荐）

```bash
cd /home/zy/CLearning/study_checkin_app
./start.sh
```

### 手动启动

1. 安装依赖

```bash
cd /home/zy/CLearning/study_checkin_app
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

2. 启动

```bash
python app.py
```

3. 打开浏览器

```text
http://127.0.0.1:5000
```

## 数据文件

- SQLite 数据库：`study_checkin_app/checkin.db`
- 删除该文件后重新启动，会按最新 `C_learning_plan.md` 重新导入任务。

## 说明

- LeetCode 题量规则：
  - Day1-Day20：0题
  - Day21-Day100：3题/2题交替
- 若你修改了 `C_learning_plan.md` 且希望重新导入，请先删除 `checkin.db`。
