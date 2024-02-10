#!/bin/bash


# COMMAND="'./build/Rofi-Themer-client $@'"
# ????? rofi is removing any amount of '/' on the last parameter.??????????????????????????
# anyway I am dumgb and doing it this way does not even make much sense, just ended up putting it in the code, and had to make two separate binaries

# insert your rofi theme instead of "$HOME/.config/rofi/current-theme/dmenu-theme.rasi"
rofi -show theme-menu -modi theme-menu:"./build/Rofi-Themer-menu" -theme\
	$HOME/.config/rofi/current-theme/dmenu-theme.rasi -font "cantarell regular 15"\
	-show-icons -i -log ~/rofi.log # &>/tmp/rofi-theme.txt #&& i3-msg restart
