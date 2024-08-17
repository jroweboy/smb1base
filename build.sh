#!/usr/bin/env sh

OPTION_LINE_MATCH='^[\s]*[^#\[;]*=.*$'
while IFS= read -r line; do
  ! [[ $line =~ $OPTION_LINE_MATCH ]] && continue
  options="${options} --asm-define ${line} -D ${line}"
  # echo "line: ${line}"
done < build_options.ini

# echo "options: ${options} "

cl65 \
  --cpu 6502x -t nes  -g -Oisr -vm -T \
  -C smb1base.cfg \
  --ld-args --dbgfile,build/smb1base.dbg \
  --include-dir . \
  --asm-include-dir . \
  --bin-include-dir . \
  -l build/smb1base.lst \
  -m build/smb1base.map \
  -o build/smb1base.nes \
  ${options} \
   ./src/main.c ./src/entrypoint.s

# cl65 \
#   --cpu 6502x -t none --no-target-lib -g -Oisr -v -vm -T \
#   -C smb1base.cfg \
#   --ld-args --dbgfile,build/smb1base.dbg \
#   -l build/smb1base.lst \
#   -m build/smb1base.map \
#   -o build/smb1base.nes \
#   ./entrypoint.s