```
jfathman@alpine:~/work/git/avr-io[main]$ bin/picocom.sh 

W:> help
***********************************
**        AVR MCU Monitor        **
***********************************
  adc    init vcc|int|ext <v.v>
  adc    init int1v1|int2v56 <v.v>
  adc    <pin> read
  dig    <pin> in|inpull|out
  dig    <pin> on|off|set|clear|1|0
  dig    <pin> toggle
  dig    <pin> rda|rdc
  pwm    <pin> 0-255
  nop    no operation
  delay  0-10000 (msec)
  debug  on|off
  map    memory map
  mem    memory usage
  heap   heap usage
  stack  stack usage
  flash  flash usage
  crc    flash crc-16
  eeprom erase|usage
  id     erase|r|w <id>
  dump   s|f|e|h <addr>
  cont   continue dump
  uptime since startup
  clear  clear screen
  reset  wdt reset
  system system info
  ver    version info
  help   command menu
  ?      command menu
