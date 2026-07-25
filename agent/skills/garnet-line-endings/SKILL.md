---
name: garnet-line-endings
description: Line-ending convention for files created by agents. Use when creating new files, when git warns "LF will be replaced by CRLF", or when fixing mixed line endings.
---

# Garnet Line Endings

When creating a new file, match the hosting OS's convention in the working
directory:

- Windows: CRLF
- Linux/macOS: LF

Git normalizes text files to LF in the repository (`core.autocrlf=true` on
Windows checkouts), so the committed content is identical either way. The
convention is about the working copy staying consistent with the rest of the
checkout and the tools that touch it.

## Rules

- New files follow the hosting OS, not the tool's default. File-writing tools
  often emit LF regardless of platform — verify and convert after creating.
- Never change the line endings of existing files while editing them; keep
  whatever the file already uses.
- Do not add per-file `eol` overrides to `.gitattributes` for this; the
  existing autocrlf setup already handles repository normalization.

## Symptom and fix

A git warning like `LF will be replaced by CRLF in <file>` on add/commit means
the working copy does not match the checkout convention. It is harmless to the
repository but leaves an inconsistent working copy.

To fix a file whose committed content is already correct, re-smudge it:

```bash
rm <file> && git checkout -- <file>
```

Git rewrites the working copy with the correct endings. For an uncommitted
file, rewrite it in place with the correct endings instead, e.g. on Windows:

```powershell
[IO.File]::WriteAllText($f, ([IO.File]::ReadAllText($f) -replace "(?<!\r)\n", "`r`n"))
```

Verify with `file <path>`: it should report `CRLF line terminators` on Windows
and plain text (LF) on Linux/macOS.
