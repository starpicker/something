# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

# If not running interactively, don't do anything
#[ -z "$PS1" ] && exit 

# don't put duplicate lines in the history. See bash(1) for more options
# don't overwrite GNU Midnight Commander's setting of `ignorespace'.
export HISTCONTROL=$HISTCONTROL${HISTCONTROL+,}ignoredups
# ... or force ignoredups and ignorespace
export HISTCONTROL=ignoreboth

# append to the history file, don't overwrite it
shopt -s histappend

# for setting history length see HISTSIZE and HISTFILESIZE in bash(1)

# check the window size after each command and, if necessary,
# update the values of LINES and COLUMNS.
shopt -s checkwinsize

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
if [ -z "$debian_chroot" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# set a fancy prompt (non-color, unless we know we "want" color)
case "$TERM" in
    xterm-color) color_prompt=yes;;
esac

# uncomment for a colored prompt, if the terminal has the capability; turned
# off by default to not distract the user: the focus in a terminal window
# should be on the output of commands, not on the prompt
#force_color_prompt=yes

if [ -n "$force_color_prompt" ]; then
    if [ -x /usr/bin/tput ] && tput setaf 1 >&/dev/null; then
	# We have color support; assume it's compliant with Ecma-48
	# (ISO/IEC-6429). (Lack of such support is extremely rare, and such
	# a case would tend to support setf rather than setaf.)
	color_prompt=yes
    else
	color_prompt=
    fi
fi

if [ "$color_prompt" = yes ]; then
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
unset color_prompt force_color_prompt

# If this is an xterm set the title to user@host:dir
case "$TERM" in
xterm*|rxvt*)
    PS1="\[\e]0;${debian_chroot:+($debian_chroot)}\u@\h: \w\a\]$PS1"
    ;;
*)
    ;;
esac

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

#if [ -f ~/.bash_aliases ]; then
#    . ~/.bash_aliases
#fi

# enable color support of ls and also add handy aliases
if [ -x /usr/bin/dircolors ]; then
    eval "`dircolors -b`"
    alias ls='ls --color=auto'
    #alias dir='dir --color=auto'
    #alias vdir='vdir --color=auto'

    #alias grep='grep --color=auto'
    #alias fgrep='fgrep --color=auto'
    #alias egrep='egrep --color=auto'
fi

# some more ls aliases
#alias ll='ls -l'
#alias la='ls -A'
#alias l='ls -CF'
#alias ld-a2.1='/home/lma/android/prebuilt/linux-x86/toolchain/arm-eabi-4.4.0/bin/arm-eabi-ld'
#alias nm-a2.1='/home/lma/android/prebuilt/linux-x86/toolchain/arm-eabi-4.4.0/bin/arm-eabi-nm'
#alias objdump-a2.1='/home/lma/android/prebuilt/linux-x86/toolchain/arm-eabi-4.4.0/bin/arm-eabi-objdump'
#alias readelf-a2.1='/home/lma/inbrics_64/android-1214/prebuilt/linux-x86/toolchain/arm-eabi-4.4.0/bin/arm-eabi-readelf'
#alias size-a2.1='/home/lma/android/prebuilt/linux-x86/toolchain/arm-eabi-4.4.0/bin/arm-eabi-size'
#alias strip-a2.1='/home/lma/android/prebuilt/linux-x86/toolchain/arm-eabi-4.4.0/bin/arm-eabi-strip'
#alias ar-a2.1='/home/lma/inbrics/s2_c110_eclair_v05_1.374/prebuilt/linux-x86/toolchain/arm-eabi-4.2.1/bin/arm-eabi-ar'
# enable programmable completion features (you don't need to enable
# this, if it's already enabled in /etc/bash.bashrc and /etc/profile
# sources /etc/bash.bashrc).
if [ -f /etc/bash_completion ]; then
    . /etc/bash_completion
fi

#export LC_ALL=C

#umask 0027
#export PATH=$PATH:/mnt/500G/OEMIV/lma/lge/android-ndk-r7/
#export PATH=$PATH:/mnt/500G/OEMIV/lma/android-ndk-r9:/mnt/500G/OEMIV/lma/bin/bin:/mnt/500G/OEMIV/lma/bin/depot_tools
#export ZZ=/mnt/500G/OEMIV/lma/mount_mount/
#alias cdd='cd `cat /home/lma/myfifo`'
#alias vimm='vim `cat /home/lma/myfifo`'
#alias pwdd='pwd >/home/lma/myfifo'
#alias ema='emacs'
# sudo mount -o username=lma,uid=lma,gid=lma,nounix,noserverino //lma-win7/gif_encoder mount_mount
#sudo mount -o username=lma,uid=lma,gid=lma,nounix,noserverino //172.17.200.21/gif_my mount_mount
export LS_COLORS=$LS_COLORS:'di=01;94':'ex=01;91'
#alias android_addr2line=/mnt/500G/public/NDK/android-ndk64-r10-x86_64/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin/aarch64-linux-android-addr2line
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.:/usr/lib/x86_64-linux-gnu:/lma/mount_for_olympus/opengles/qcomsdk/Bin/x64:/usr/local/lib/:/usr/local/lib64/:/opt/Qualcomm/Symphony/1.1.2/x86_64-linux-gnu/lib

[[ -s //home/lma/.autojump/etc/profile.d/autojump.sh ]] && source /home/lma/.autojump/etc/profile.d/autojump.sh

export PATH="$GOROOT/bin:/mnt/500G/public/NDK/android-ndk-r10d-linux-x86_64:/home/lma/skia_android/depot_tools:/opt/poky/1.7/sysroots/i686-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi:/usr/local/bin:/usr/bin:/usr/lib/x86_64-linux-gnu:${PATH}"

export ANDROID_NDK=/mnt/500G/public/NDK/android-ndk-r10d-linux-x86_64
export ANDROID_SDK_ROOT=/mnt/500G/public/SDK/android-sdk-linux-6.0
export ANDROID_HOME=/mnt/500G/public/SDK/android-sdk-linux-6.0
export ANDROID_SDK_HOME=/mnt/500G/public/SDK/android-sdk-linux-6.0
export ANDROID_NDK_ROOT=/mnt/500G/public/NDK/android-ndk-r10d-linux-x86_64
export ANDROID_MAKE_CCACHE=ccache
export TERM=xterm-256color
#toilet "FUCK U" | pv -qL 100
alias gcc="color_compile gcc"
alias g++="color_compile g++ -std=c++11"
#alias make="color_compile make -j4"
alias make="color_compile make"

alias ..="cd .."
alias ...="cd ../../"
alias ....="cd ../../../"
alias .....="cd ../../../../"
alias l="ls"
alias v="vim"
#motherfuck, if caused shell if problem, cost ME LOTS of time
#shit, rubbish
#must pay attention to alias
#alias if="ifconfig"
#return
#alias make="make | lolcat"
#alias clang="clang-3.5"
#alias make="make -j4"
#export LC_ALL=en_US.utf8
export EDITOR=vim
export ANDROID_JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64/
#bind Space:magic-space
#stty -ixon
alias vpn='/opt/cisco/anyconnect/bin/vpn'
alias vpnui='/opt/cisco/anyconnect/bin/vpnui'

export PATH=$HOME/bin:$PATH
alias bci='bitcoin-cli'
source $HOME/.tmuxinator/.tmuxinator.bash
