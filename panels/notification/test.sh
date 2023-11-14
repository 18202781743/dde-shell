#!/bin/bash

# Listen to the `org.deepin.dde.Osd1` received by `dde-osd` and forward it to `org.deepin.dde.Shell`
#busctl --user monitor --match="type='method_call',eavesdrop='true',destination='org.deepin.dde.Osd1'" -j |jq --unbuffered -r '.payload.data[0]' |xargs -I {}  dbus-send --session --type=method_call --print-reply --dest=org.deepin.dde.Shell /org/deepin/osdService org.deepin.osdService.showText string:{}

notify-send 1 -a dde-control-center -t 3000
notify-send 2 -a dde-control-center -t 4000
notify-send 3 -a dde-control-center -t 5000
notify-send 4 -a dde-control-center -t 5000

sleep 3

#notify-send 1 -a dde-control-center
#sleep 1
#notify-send 2 -a dde-control-center
#sleep 1
#notify-send 3 -a dde-control-center
#sleep 1
#notify-send 4 -a dde-control-center

#sleep 4

#notify-send 5 -a dde-control-center
#notify-send 6 -a dde-control-center
#notify-send 7 -a dde-control-center
#notify-send 8 -a dde-control-center
#notify-send 9 -a dde-control-center
#notify-send 10 -a dde-control-center
#notify-send 11 -a dde-control-center
#notify-send 12 -a dde-control-center
#notify-send 13 -a dde-control-center

#sleep 4

#notify-send 1 -a element
#sleep 1
#notify-send 2 -a element
#sleep 1
#notify-send 3 -a element

dbus-send --session --type=method_call --dest=org.deepin.dde.Notification1 /org/deepin/dde/Notification1 org.deepin.dde.Notification1.ReplaceBubble boolean:false
