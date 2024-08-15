

tools\Windows\cc65\bin\cl65 ^
  --cpu 6502x -t none --no-target-lib -g -Oisr -v -vm -T ^
  -C smb1base.cfg ^
  --ld-args --dbgfile,build/smb1base.dbg ^
  -l build/smb1base.lst ^
  -m build/smb1base.map ^
  -o build/smb1base.nes ^
  ./entrypoint.s