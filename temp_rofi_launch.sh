#!/bin/bash

# insert your rofi theme instead of "$HOME/.config/rofi/current-theme/dmenu-theme.rasi"
# change "$HOME/I-Themer/ithemer" if needed
rofi -show theme-menu -modi theme-menu:./build/Rofi-Themer-daemon -theme\
	$HOME/.config/rofi/current-theme/dmenu-theme.rasi -font "cantarell regular 15"\
	-show-icons -i &>/tmp/rofi-theme.txt #&& i3-msg restart
