#!/usr/bin/env python3
"""Assignment progress monitor — multi-assignment dashboard with real-time git log."""

import json
import re
import os
import subprocess
import sys
import time
from http.server import HTTPServer, BaseHTTPRequestHandler
from pathlib import Path

AGENT_DIR = Path(__file__).parent
REPO_DIR  = AGENT_DIR.parent

# ---------------------------------------------------------------------------
# Assignment parser
# ---------------------------------------------------------------------------

PHASE_RE = re.compile(r"^Phase (\d+):\s*(.+?)(?:\s*─+\s*)?$")
TASK_RE  = re.compile(r"^\s{2}Task (\d+\.\d+):\s*(.+)$")

def _short_name(path: Path) -> str:
    """ASSIGNMENT_FOO_BAR.txt  →  Foo Bar"""
    stem = path.stem  # e.g. ASSIGNMENT_GPU_RESOURCE_BINDING
    stem = re.sub(r"^ASSIGNMENT_", "", stem)
    return stem.replace("_", " ").title()


def parse_assignment(path: Path) -> dict:
    text = path.read_text(encoding="utf-8")

    # ---- title from first # heading ------------------------------------------
    title_m = re.search(r"^# Assignment:\s*(.+)", text, re.MULTILINE)
    title   = title_m.group(1).strip() if title_m else path.stem

    # ---- progress block -------------------------------------------------------
    prog_m  = re.search(
        r"--- PROGRESS.*?---\n(.*?)--- END PROGRESS ---", text, re.DOTALL
    )
    prog    = prog_m.group(1) if prog_m else ""

    lc_m    = re.search(r"Last completed:\s*(.+)", prog)
    ip_m    = re.search(r"In progress:\s*(.+)",    prog)
    last_completed_str = lc_m.group(1).strip() if lc_m else ""
    in_progress_str    = ip_m.group(1).strip() if ip_m else ""

    # Explicit per-phase/per-task completion lines inside the progress block
    completed_task_ids  = set()   # "0.1", "1.3", …
    completed_phase_ids = set()   # 0, 1, 2, …  (whole phase done)
    for m in re.finditer(r"^\s+Phase ([0-9]+(?:\.[0-9]+)?):", prog, re.MULTILINE):
        tid = m.group(1)
        if "." in tid:
            completed_task_ids.add(tid)
        else:
            completed_phase_ids.add(int(tid))

    # In-progress single task
    ip_task = None
    ip_task_m = re.search(r"In progress:\s*Phase ([0-9]+\.[0-9]+)", prog)
    if ip_task_m:
        ip_task = ip_task_m.group(1)

    # Highest completed phase number from "Last completed: Phase X" headline
    lc_phase_m       = re.search(r"Last completed:.*?Phase (\d+)", last_completed_str)
    last_done_phase  = int(lc_phase_m.group(1)) if lc_phase_m else -1
    if completed_phase_ids:
        last_done_phase = max(last_done_phase, max(completed_phase_ids))

    # ---- task breakdown -------------------------------------------------------
    td_m   = re.search(r"--- TASK BREAKDOWN ---\n(.*?)--- SUMMARY", text, re.DOTALL)
    td     = td_m.group(1) if td_m else ""

    phases, cur = [], None
    for line in td.splitlines():
        # skip separator lines (─────)
        if re.match(r"^[─\-]{4,}$", line.strip()):
            continue
        pm = PHASE_RE.match(line)
        if pm:
            cur = {"id": int(pm.group(1)), "label": pm.group(2).strip(), "tasks": []}
            phases.append(cur)
            continue
        tm = TASK_RE.match(line)
        if tm and cur is not None:
            tid       = tm.group(1)
            phase_num = int(tid.split(".")[0])
            if (tid in completed_task_ids
                    or phase_num in completed_phase_ids
                    or phase_num < last_done_phase):
                status = "done"
            elif tid == ip_task:
                status = "in_progress"
            else:
                status = "pending"
            cur["tasks"].append({"id": tid, "label": tm.group(2).strip(), "status": status})

    for p in phases:
        statuses = [t["status"] for t in p["tasks"]]
        if not statuses:
            p["status"] = "pending"
        elif all(s == "done" for s in statuses):
            p["status"] = "done"
        elif any(s in ("done", "in_progress") for s in statuses):
            p["status"] = "in_progress"
        else:
            p["status"] = "pending"

    all_tasks  = [t for p in phases for t in p["tasks"]]
    done_count = sum(1 for t in all_tasks if t["status"] == "done")
    total      = len(all_tasks)

    return {
        "key":            path.stem,
        "short_name":     _short_name(path),
        "title":          title,
        "file":           str(path),
        "mtime":          os.path.getmtime(path),
        "last_completed": last_completed_str,
        "in_progress":    in_progress_str,
        "phases":         phases,
        "done":           done_count,
        "total":          total,
        "pct":            round(100 * done_count / total) if total else 0,
    }


def parse_all_assignments() -> list:
    files = sorted(AGENT_DIR.glob("ASSIGNMENT_*.txt"))
    return [parse_assignment(f) for f in files]


# ---------------------------------------------------------------------------
# Git log reader
# ---------------------------------------------------------------------------

ASSIGNMENT_RE = re.compile(r"rdg\(upload\)|rdg\(binding\)|Phase \d", re.IGNORECASE)

def fetch_commits(n=60):
    fmt = "%x00".join(["%H", "%h", "%an", "%at", "%D", "%s"])
    try:
        out = subprocess.check_output(
            ["git", "log", f"--format={fmt}", f"-{n}"],
            cwd=REPO_DIR, stderr=subprocess.DEVNULL, text=True
        )
    except subprocess.CalledProcessError:
        return []

    commits, now = [], time.time()
    for line in out.splitlines():
        if not line.strip():
            continue
        parts = line.split("\x00")
        if len(parts) < 6:
            continue
        sha_full, sha_short, author, ts_str, refs, subject = parts[:6]
        try:
            ts = int(ts_str)
        except ValueError:
            ts = 0
        commits.append({
            "sha":       sha_short,
            "sha_full":  sha_full,
            "author":    author,
            "ts":        ts,
            "age":       _human_age(now - ts),
            "branch":    _parse_refs(refs),
            "subject":   subject,
            "highlight": bool(ASSIGNMENT_RE.search(subject)),
        })
    return commits


def _human_age(s):
    if s < 60:    return "just now"
    if s < 3600:  return f"{int(s/60)}m ago"
    if s < 86400: return f"{int(s/3600)}h ago"
    return f"{int(s/86400)}d ago"


def _parse_refs(refs_str):
    if not refs_str.strip():
        return ""
    parts = [r.strip() for r in refs_str.split(",")]
    for p in parts:
        if p.startswith("HEAD -> "): return p[len("HEAD -> "):]
        if p.startswith("tag: "):    return p[len("tag: "):]
    for p in parts:
        if "HEAD" not in p and "origin/" not in p: return p
    return parts[0] if parts else ""


# ---------------------------------------------------------------------------
# HTML (embedded)
# ---------------------------------------------------------------------------

HTML = r"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8"/>
<meta name="viewport" content="width=device-width,initial-scale=1"/>
<title>Assignment Monitor</title>
<style>
  :root {
    --bg:        #0f1117;
    --surface:   #1a1d27;
    --surface2:  #20243a;
    --border:    #2a2d3e;
    --text:      #e2e4ef;
    --muted:     #6b7094;
    --done:      #22c55e;
    --ip:        #f59e0b;
    --pending:   #6b7094;
    --accent:    #6366f1;
    --highlight: #818cf8;
    --tag-done:  rgba(34,197,94,.15);
    --tag-ip:    rgba(245,158,11,.15);
    --tag-pen:   rgba(107,112,148,.12);
    --tag-hi:    rgba(129,140,248,.18);
  }
  * { box-sizing: border-box; margin: 0; padding: 0; }
  body { background: var(--bg); color: var(--text);
         font-family: 'Segoe UI', system-ui, sans-serif;
         min-height: 100vh; padding: 2rem 1rem; max-width: 1400px; margin: 0 auto; }

  /* ---- header ---- */
  .top-header { display: flex; align-items: baseline; gap: 1rem; margin-bottom: .3rem; flex-wrap: wrap; }
  h1 { font-size: 1.4rem; font-weight: 700; letter-spacing: -.01em; }
  .live-dot { display: inline-block; width: 8px; height: 8px; border-radius: 50%;
              background: var(--done); margin-right: 6px; animation: pulse 1.8s infinite; }
  @keyframes pulse { 0%,100%{opacity:1} 50%{opacity:.35} }
  .subtitle { font-size: .78rem; color: var(--muted); margin-bottom: 1.5rem; }

  /* ---- tabs ---- */
  .tabs { display: flex; gap: .4rem; flex-wrap: wrap; margin-bottom: 1.5rem;
          border-bottom: 1px solid var(--border); padding-bottom: .75rem; }
  .tab { padding: .45rem 1rem; border-radius: 8px; font-size: .82rem; font-weight: 600;
         cursor: pointer; border: 1px solid transparent; color: var(--muted);
         transition: all .15s; white-space: nowrap; }
  .tab:hover { background: var(--surface); color: var(--text); }
  .tab.active { background: var(--accent); color: #fff; border-color: transparent; }
  .tab .tab-pct { font-size: .7rem; opacity: .75; margin-left: .35rem; }
  .tab .tab-dot { display: inline-block; width: 6px; height: 6px; border-radius: 50%;
                  margin-right: .35rem; }
  .tab .tab-dot.done     { background: var(--done); }
  .tab .tab-dot.in_progress { background: var(--ip); animation: pulse 1.8s infinite; }
  .tab .tab-dot.pending  { background: var(--muted); }

  /* ---- stats ---- */
  .stats { display: flex; gap: .75rem; flex-wrap: wrap; margin-bottom: 1.25rem; }
  .stat-card { background: var(--surface); border: 1px solid var(--border); border-radius: 10px;
               padding: .8rem 1.1rem; flex: 1; min-width: 120px; }
  .stat-card .label { font-size: .67rem; text-transform: uppercase; letter-spacing: .06em;
                      color: var(--muted); margin-bottom: .3rem; }
  .stat-card .value { font-size: 1.35rem; font-weight: 700; }
  .stat-card .value.green { color: var(--done); }
  .stat-card .value.amber { color: var(--ip);   }

  /* ---- progress bar ---- */
  .progress-wrap { background: var(--border); border-radius: 99px; height: 8px;
                   margin-bottom: 1.5rem; overflow: hidden; }
  .progress-fill { height: 100%; border-radius: 99px;
                   background: linear-gradient(90deg, var(--accent), var(--done));
                   transition: width .6s ease; }

  /* ---- layout ---- */
  .main-grid { display: grid; grid-template-columns: 1fr 370px; gap: 1.5rem; align-items: start; }
  @media (max-width: 900px) { .main-grid { grid-template-columns: 1fr; } }
  h2 { font-size: .8rem; font-weight: 700; text-transform: uppercase; letter-spacing: .06em;
       color: var(--muted); margin-bottom: .7rem; }

  /* ---- badges ---- */
  .badge { display: inline-flex; align-items: center; gap: 4px; font-size: .67rem;
           font-weight: 600; padding: 2px 7px; border-radius: 99px; white-space: nowrap; }
  .badge.done        { background: var(--tag-done); color: var(--done); }
  .badge.in_progress { background: var(--tag-ip);   color: var(--ip);   }
  .badge.pending     { background: var(--tag-pen);  color: var(--muted); }
  .dot { width: 6px; height: 6px; border-radius: 50%; flex-shrink: 0; }
  .dot.done        { background: var(--done); }
  .dot.in_progress { background: var(--ip); animation: pulse 1.8s infinite; }
  .dot.pending     { background: var(--pending); }

  /* ---- phase cards ---- */
  .phases { display: flex; flex-direction: column; gap: .55rem; }
  .phase-card { background: var(--surface); border: 1px solid var(--border);
                border-radius: 12px; overflow: hidden; }
  .phase-card.done        { border-color: rgba(34,197,94,.2); }
  .phase-card.in_progress { border-color: rgba(245,158,11,.35); }
  .phase-header { display: flex; align-items: center; gap: .6rem;
                  padding: .65rem .9rem; cursor: pointer; user-select: none; }
  .phase-header:hover { background: rgba(255,255,255,.03); }
  .phase-num   { font-size: .67rem; color: var(--muted); font-weight: 600;
                 background: var(--border); border-radius: 5px; padding: 2px 6px; flex-shrink: 0; }
  .phase-label { flex: 1; font-size: .86rem; font-weight: 600; }
  .phase-cnt   { font-size: .7rem; color: var(--muted); flex-shrink: 0; }
  .chevron     { width: 14px; height: 14px; transition: transform .2s; flex-shrink: 0;
                 stroke: var(--muted); }
  .phase-card.open .chevron { transform: rotate(180deg); }
  .task-list   { display: none; padding: 0 .9rem .65rem; border-top: 1px solid var(--border); }
  .phase-card.open .task-list { display: flex; flex-direction: column; gap: .38rem; padding-top: .5rem; }
  .task-row  { display: flex; align-items: flex-start; gap: .5rem; font-size: .79rem; }
  .task-dot  { margin-top: 4px; flex-shrink: 0; }
  .task-id   { color: var(--muted); font-size: .69rem; flex-shrink: 0; margin-top: 2px; min-width: 24px; }
  .task-text { color: var(--text); }
  .task-text.done        { color: var(--muted); text-decoration: line-through; }
  .task-text.in_progress { color: var(--ip); font-weight: 600; }

  /* ---- error ---- */
  .error { color: #f87171; font-size: .82rem; padding: .8rem 1rem;
           background: rgba(248,113,113,.1); border-radius: 8px; margin-bottom: 1rem; }

  /* ---- commit log ---- */
  .commit-panel { position: sticky; top: 1.5rem; }
  .commit-list  { display: flex; flex-direction: column; }
  .commit-item  { display: flex; gap: .7rem; padding: .6rem 0;
                  border-bottom: 1px solid var(--border); position: relative; }
  .commit-item:last-child { border-bottom: none; }
  .commit-item::before { content: ''; position: absolute; left: 8px; top: 28px;
    width: 1px; bottom: -8px; background: var(--border); }
  .commit-item:last-child::before { display: none; }
  .commit-dot { width: 18px; height: 18px; border-radius: 50%; flex-shrink: 0;
                display: flex; align-items: center; justify-content: center;
                margin-top: 1px; position: relative; z-index: 1; }
  .commit-dot.hi  { background: var(--tag-hi);    border: 1.5px solid var(--highlight); }
  .commit-dot.std { background: var(--surface2);  border: 1.5px solid var(--border);   }
  .commit-dot svg { width: 8px; height: 8px; }
  .commit-body  { flex: 1; min-width: 0; }
  .commit-meta  { display: flex; align-items: center; gap: .4rem; flex-wrap: wrap; margin-bottom: 2px; }
  .commit-sha   { font-family: monospace; font-size: .67rem; color: var(--accent);
                  background: rgba(99,102,241,.12); padding: 1px 5px; border-radius: 4px; flex-shrink: 0; }
  .commit-age   { font-size: .67rem; color: var(--muted); }
  .commit-author { font-size: .67rem; color: var(--muted); }
  .commit-branch { font-size: .64rem; color: var(--highlight);
                   background: var(--tag-hi); padding: 1px 5px; border-radius: 4px; }
  .commit-subject { font-size: .77rem; color: var(--text); line-height: 1.4;
                    overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }
  .commit-subject.hi { color: var(--highlight); font-weight: 600; }
  .new-flash { animation: flash .6s ease; }
  @keyframes flash { 0%{background:rgba(129,140,248,.22)} 100%{background:transparent} }

  /* ---- footer ---- */
  .footer { margin-top: 2rem; font-size: .68rem; color: var(--muted); text-align: center; }
</style>
</head>
<body>

<div class="top-header">
  <h1><span class="live-dot"></span>Assignment Monitor</h1>
</div>
<div class="subtitle" id="subtitle">Loading…</div>

<div id="error-box" class="error" style="display:none"></div>

<!-- Assignment tabs -->
<div class="tabs" id="tabs"></div>

<!-- Per-assignment stats -->
<div class="stats">
  <div class="stat-card">
    <div class="label">Tasks Done</div>
    <div class="value green" id="stat-done">—</div>
  </div>
  <div class="stat-card">
    <div class="label">Total</div>
    <div class="value" id="stat-total">—</div>
  </div>
  <div class="stat-card">
    <div class="label">Progress</div>
    <div class="value amber" id="stat-pct">—</div>
  </div>
  <div class="stat-card" style="flex:2; min-width:180px;">
    <div class="label">In Progress</div>
    <div class="value" id="stat-ip" style="font-size:.88rem;line-height:1.4;font-weight:500">—</div>
  </div>
  <div class="stat-card">
    <div class="label">Commits</div>
    <div class="value" id="stat-commits">—</div>
  </div>
</div>

<div class="progress-wrap">
  <div class="progress-fill" id="progress-fill" style="width:0%"></div>
</div>

<div class="main-grid">
  <!-- left: phases -->
  <div>
    <h2 id="phases-header">Task Breakdown</h2>
    <div class="phases" id="phases"></div>
  </div>
  <!-- right: commit log -->
  <div class="commit-panel">
    <h2>Commit History</h2>
    <div class="commit-list" id="commit-list"></div>
  </div>
</div>

<div class="footer" id="footer"></div>

<script>
const STATUS_LABEL = { done: 'Done', in_progress: 'In Progress', pending: 'Pending' };
let allData      = null;
let activeKey    = null;
let openPhases   = {};   // { key: Set<phaseId> }
let prevTopSha   = null;

function esc(s) {
  return String(s).replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;');
}

/* ---- overall status of all assignments for a tab dot ---- */
function assignmentOverallStatus(a) {
  if (a.pct === 100) return 'done';
  if (a.done > 0 || a.in_progress) return 'in_progress';
  return 'pending';
}

/* ---- tabs ---- */
function renderTabs(assignments) {
  if (!activeKey && assignments.length) activeKey = assignments[0].key;
  document.getElementById('tabs').innerHTML = assignments.map(a => {
    const st  = assignmentOverallStatus(a);
    const act = a.key === activeKey ? ' active' : '';
    return `<div class="tab${act}" onclick="selectTab('${esc(a.key)}')">
      <span class="tab-dot ${st}"></span>${esc(a.short_name)}<span class="tab-pct">${a.pct}%</span>
    </div>`;
  }).join('');
}

function selectTab(key) {
  activeKey = key;
  if (allData) renderAssignment(allData.assignments.find(a => a.key === key));
  renderTabs(allData ? allData.assignments : []);
}

/* ---- phase cards ---- */
function badge(s) {
  const dot = s === 'done' ? '●' : s === 'in_progress' ? '◎' : '○';
  return `<span class="badge ${s}">${dot} ${STATUS_LABEL[s]}</span>`;
}

function renderPhase(p, key) {
  if (!openPhases[key]) openPhases[key] = new Set();
  const set   = openPhases[key];
  const done  = p.tasks.filter(t => t.status === 'done').length;
  const total = p.tasks.length;
  const open  = set.has(p.id) || p.status === 'in_progress';
  const tasks = p.tasks.map(t => `
    <div class="task-row">
      <span class="dot task-dot ${t.status}"></span>
      <span class="task-id">${t.id}</span>
      <span class="task-text ${t.status}">${esc(t.label)}</span>
    </div>`).join('');
  return `
    <div class="phase-card ${p.status} ${open?'open':''}" data-phase="${p.id}" data-key="${esc(key)}">
      <div class="phase-header" onclick="togglePhase('${esc(key)}',${p.id})">
        <span class="phase-num">Phase ${p.id}</span>
        <span class="phase-label">${esc(p.label)}</span>
        <span class="phase-cnt">${done}/${total}</span>
        ${badge(p.status)}
        <svg class="chevron" viewBox="0 0 14 14" fill="none" stroke-width="2">
          <polyline points="3 5 7 9 11 5"/>
        </svg>
      </div>
      <div class="task-list">${tasks}</div>
    </div>`;
}

function togglePhase(key, id) {
  if (!openPhases[key]) openPhases[key] = new Set();
  const set = openPhases[key];
  const el  = document.querySelector(`[data-key="${key}"][data-phase="${id}"]`);
  if (!el) return;
  el.classList.toggle('open');
  if (el.classList.contains('open')) set.add(id); else set.delete(id);
}

/* ---- render one assignment ---- */
function renderAssignment(a) {
  if (!a) return;
  if (!openPhases[a.key]) openPhases[a.key] = new Set();
  a.phases.forEach(p => { if (p.status === 'in_progress') openPhases[a.key].add(p.id); });

  document.getElementById('stat-done').textContent  = a.done;
  document.getElementById('stat-total').textContent = a.total;
  document.getElementById('stat-pct').textContent   = a.pct + '%';
  document.getElementById('stat-ip').textContent    = a.in_progress || '—';
  document.getElementById('progress-fill').style.width = a.pct + '%';
  document.getElementById('phases-header').textContent =
    `Task Breakdown — ${a.short_name}`;

  const mt = new Date(a.mtime * 1000);
  document.getElementById('subtitle').textContent =
    `${a.title}  ·  file updated ${mt.toLocaleTimeString()}`;

  document.getElementById('phases').innerHTML =
    a.phases.map(p => renderPhase(p, a.key)).join('');
}

/* ---- commits ---- */
function commitIcon(hi) {
  if (hi) return `<svg viewBox="0 0 8 8" fill="var(--highlight)"><circle cx="4" cy="4" r="3.5"/></svg>`;
  return `<svg viewBox="0 0 8 8" fill="none" stroke="var(--muted)" stroke-width="1.5"><circle cx="4" cy="4" r="3"/></svg>`;
}

function renderCommit(c, flash) {
  const branch = c.branch ? `<span class="commit-branch">${esc(c.branch)}</span>` : '';
  return `
    <div class="commit-item${flash?' new-flash':''}" title="${esc(c.sha_full)}">
      <div class="commit-dot ${c.highlight?'hi':'std'}">${commitIcon(c.highlight)}</div>
      <div class="commit-body">
        <div class="commit-meta">
          <span class="commit-sha">${esc(c.sha)}</span>
          <span class="commit-age">${esc(c.age)}</span>
          <span class="commit-author">${esc(c.author)}</span>
          ${branch}
        </div>
        <div class="commit-subject${c.highlight?' hi':''}">${esc(c.subject)}</div>
      </div>
    </div>`;
}

/* ---- refresh loop ---- */
async function refresh() {
  try {
    const r = await fetch('/api/data?t=' + Date.now());
    if (!r.ok) throw new Error('HTTP ' + r.status);
    const d = await r.json();
    allData = d;

    document.getElementById('error-box').style.display = 'none';

    /* commits */
    const commits = d.commits || [];
    const newTop  = commits.length ? commits[0].sha : null;
    const isNew   = newTop && newTop !== prevTopSha;
    prevTopSha    = newTop;
    document.getElementById('stat-commits').textContent = commits.length || '—';
    document.getElementById('commit-list').innerHTML =
      commits.map((c, i) => renderCommit(c, isNew && i === 0)).join('');

    /* tabs + current assignment */
    renderTabs(d.assignments);
    const current = d.assignments.find(a => a.key === activeKey) || d.assignments[0];
    if (current) { activeKey = current.key; renderAssignment(current); }

    document.getElementById('footer').textContent =
      `Polls every 3 s  ·  Last: ${new Date().toLocaleTimeString()}  ·  ${AGENT_DIR}`;
  } catch(e) {
    document.getElementById('error-box').textContent = 'Fetch error: ' + e.message;
    document.getElementById('error-box').style.display = '';
  }
}

const AGENT_DIR = '${AGENT_DIR_PLACEHOLDER}';
refresh();
setInterval(refresh, 3000);
</script>
</body>
</html>
"""

# ---------------------------------------------------------------------------
# HTTP handler
# ---------------------------------------------------------------------------

class Handler(BaseHTTPRequestHandler):
    def log_message(self, fmt, *args):
        pass

    def _send(self, status, ct, body):
        if isinstance(body, str):
            body = body.encode()
        self.send_response(status)
        self.send_header("Content-Type", ct)
        self.send_header("Content-Length", len(body))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self):
        path = self.path.split("?")[0]
        if path == "/api/data":
            try:
                data = {
                    "assignments": parse_all_assignments(),
                    "commits":     fetch_commits(60),
                }
                self._send(200, "application/json", json.dumps(data))
            except Exception as e:
                self._send(500, "application/json", json.dumps({"error": str(e)}))
        else:
            html = HTML.replace("${AGENT_DIR_PLACEHOLDER}", str(AGENT_DIR))
            self._send(200, "text/html; charset=utf-8", html)


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    port = int(sys.argv[1]) if len(sys.argv) > 1 else 7788
    print(f"Assignment Monitor → http://localhost:{port}", flush=True)
    httpd = HTTPServer(("", port), Handler)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\nStopped.")
