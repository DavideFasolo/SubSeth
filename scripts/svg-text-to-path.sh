#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SRC_DIR="$ROOT/res-src"
DST_DIR="$ROOT/res"

mkdir -p "$DST_DIR"

shopt -s nullglob
files=("$SRC_DIR"/*.svg)

if (( ${#files[@]} == 0 )); then
  echo "Nessun file SVG trovato in: $SRC_DIR" >&2
  exit 1
fi

for src in "${files[@]}"; do
  base="$(basename "$src")"
  dst="$DST_DIR/$base"
  echo "Converto: $base"

  inkscape "$src" \
    --actions="export-text-to-path;export-filename:$dst;export-plain-svg;export-overwrite;export-do"
done

echo "Conversione completata."
