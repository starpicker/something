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

export GOPATH=/home/lma/.go/

export PATH="$GOROOT/bin:$GOPATH/bin:/mnt/500G/public/NDK/android-ndk-r16b:/home/lma/skia_android/depot_tools:/opt/poky/1.7/sysroots/i686-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi:/usr/local/bin:/usr/bin:/usr/lib/x86_64-linux-gnu:${PATH}"

export ANDROID_NDK=/mnt/500G/public/NDK/android-ndk-r10d-linux-x86_64
export ANDROID_SDK_ROOT=/mnt/500G/public/SDK/android-sdk-linux-6.0
export ANDROID_HOME=/mnt/500G/public/SDK/android-sdk-linux-6.0
export ANDROID_SDK_HOME=/mnt/500G/public/SDK/android-sdk-linux-6.0
export ANDROID_NDK_ROOT=/mnt/500G/public/NDK/android-ndk-r10d-linux-x86_64
export ANDROID_MAKE_CCACHE=ccache
export TERM=xterm-256color
#toilet "FUCK U" | pv -qL 100
alias gcc="color_compile g++ -std=c++17"
alias g++="color_compile g++ -std=c++17"
#alias make="color_compile make -j4"
alias make="color_compile make"

alias ..="cd .."
alias ...="cd ../../"
alias ....="cd ../../../"
alias .....="cd ../../../../"
alias l="ls"
#alias v="vim"
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

export PATH=$HOME/.bin:$PATH
alias bci='bitcoin-cli'
export s="sheng@172.17.199.159"
source $HOME/.tmuxinator/.tmuxinator.bash

# Colors
default=$(tput sgr0)
red=$(tput setaf 1)
green=$(tput setaf 2)
purple=$(tput setaf 5)
orange=$(tput setaf 9)

# Less colors for man pages
export PAGER=less
# Begin blinking
#export LESS_TERMCAP_mb=$red
# Begin bold
#export LESS_TERMCAP_md=$orange
# End mode
#export LESS_TERMCAP_me=$default
# End standout-mode
#export LESS_TERMCAP_se=$default
# Begin standout-mode - info box
#export LESS_TERMCAP_so=$purple
# End underline
#export LESS_TERMCAP_ue=$default
# Begin underline
#export LESS_TERMCAP_us=$green

alias vi='vim'
alias im='vim'
alias gi='git'
alias gti='git'
alias gs='git status'
alias gd='git diff'
alias gl='git log'
alias ga='git add -A'
alias gr='git remote -v '
alias gp='git push '
alias gm='git commit -m '
alias mux='tmuxinator'
alias cl='clear'

export FZF_DEFAULT_ COMMAND='ag -g ""'
[ -f ~/.fzf.bash ] && source ~/.fzf.bash
export FZF_CTRL_T_OPTS="--preview '(highlight -O ansi -l {} 2> /dev/null || cat {} || tree -C {}) 2> /dev/null | head -200'"

fasd_cache="$HOME/.fasd-init-bash"
if [ "$(command -v fasd)" -nt "$fasd_cache" -o ! -s "$fasd_cache" ]; then
  fasd --init posix-alias bash-hook bash-ccomp bash-ccomp-install >| "$fasd_cache"
fi
source "$fasd_cache"
unset fasd_cache
unalias a
unalias d
unalias f
unalias z
unalias zz

# fasd & fzf change directory - open best matched file using `fasd` if given argument, filter output of `fasd` using `fzf` else
#open file only from recent history
v() {
    [ $# -gt 0 ] && fasd -f -e ${EDITOR} "$*" && return
    local file
    file="$(fasd -Rfl "$1" | fzf -1 -0 --no-sort +m)" && vi "${file}" || return 1
}

# fe [FUZZY PATTERN] - Open the selected file with the default editor
#   - Bypass fuzzy finder if there's only one match (--select-1)
#   - Exit if there's no match (--exit-0)
#can add directory pattern
fe() {
  local files
  IFS=$'\n' files=($(fzf-tmux --query="$1" --multi --select-1 --exit-0))
  [[ -n "$files" ]] && ${EDITOR:-vim} "${files[@]}"
}

# fuzzy grep open via ag
vg() {
  local file

  file="$(ag --nobreak --noheading $@ | fzf -0 -1 | awk -F: '{print $1 " +" $2}')"

  if [[ -n $file ]]
  then
     vim $file
  fi
}

#under current directory
fzfp() {
fzf --preview '[[ $(file --mime {}) =~ binary ]] && echo {} is a binary file || (rougify {}  || highlight -O ansi -l {} || coderay {} || cat {}) 2> /dev/null | head -500'
alias tt='fzf --preview '"'"'[[ $(file --mime {}) =~ binary ]] && echo {} is a binary file || (rougify {}  || highlight -O ansi -l {} || coderay {} || cat {}) 2> /dev/null | head -500'"'"
}

# fasd & fzf change directory - jump using `fasd` if given argument, filter output of `fasd` using `fzf` else
#jump from everywhere
z() {
    [ $# -gt 0 ] && fasd_cd -d "$*" && return
    local dir
    dir="$(fasd -Rdl "$1" | fzf -1 -0 --no-sort +m)" && cd "${dir}" || return 1
}

# fd - cd to selected directory
#open folder under current directory
fd() {
  local dir
  dir=$(find ${1:-.} -path '*/\.*' -prune \
                  -o -type d -print 2> /dev/null | fzf +m) &&
  cd "$dir"
}

#cd which file is in
cdf() {
   local file
   local dir
   file=$(fzf +m -q "$1") && dir=$(dirname "$file") && cd "$dir"
}

#lma close it
# fda - including hidden directories
#fda() {
  #local dir
  #dir=$(find ${1:-.} -type d 2> /dev/null | fzf +m) && cd "$dir"
#}

#lma close it
# fdr - cd to selected parent directory
#fdr() {
  #local declare dirs=()
  #get_parent_dirs() {
    #if [[ -d "${1}" ]]; then dirs+=("$1"); else return; fi
    #if [[ "${1}" == '/' ]]; then
      #for _dir in "${dirs[@]}"; do echo $_dir; done
    #else
      #get_parent_dirs $(dirname "$1")
    #fi
  #}
  #local DIR=$(get_parent_dirs $(realpath "${1:-$PWD}") | fzf-tmux --tac)
  #cd "$DIR"
#}

#lma close it
# cf - fuzzy cd from anywhere
# ex: cf word1 word2 ... (even part of a file name)
# zsh autoload function
#cf() {
  #local file

  #file="$(locate -Ai -0 $@ | grep -z -vE '~$' | fzf --read0 -0 -1)"

  #if [[ -n $file ]]
  #then
    #if [[ -d $file ]]
    #then
        #cd -- $file
    #else
        #cd -- ${file:h}
    #fi
  #fi
#}

#lma close it
# Modified version where you can press
#   - CTRL-O to open with `open` command,
#   - CTRL-E or Enter key to open with the $EDITOR
#fo() {
  #local out file key
  #IFS=$'\n' out=($(fzf-tmux --query="$1" --exit-0 --expect=ctrl-o,ctrl-e))
  #key=$(head -1 <<< "$out")
  #file=$(head -2 <<< "$out" | tail -1)
  #if [ -n "$file" ]; then
    #[ "$key" = ctrl-o ] && open "$file" || ${EDITOR:-vim} "$file"
  #fi
#}

export CDIFF_OPTIONS='-s -w0 --wrap'
alias grep='grep --color -n'
eval $(thefuck --alias 2> /dev/null)

# source /etc/bash_completion.d/climate_completion
#[[ -s "$HOME/.qfc/bin/qfc.sh" ]] && source "$HOME/.qfc/bin/qfc.sh"
alias r='ranger'
source ~/.commacd.bash
#function _update_ps1() {
    #PS1=$(powerline-shell $?)
#}

#if [[ $TERM != linux && ! $PROMPT_COMMAND =~ _update_ps1 ]]; then
    #PROMPT_COMMAND="_update_ps1; $PROMPT_COMMAND"
#fi
alias ff='find . -type f -name '
alias ffd='find . -type d -name '
alias agg='ag -g '

alias c='bcal -c '
#if [ -f ~/.bashrc.local ]; then
    #. ~/.bashrc.local
#fi
alias cidff='cdiff'

if hash ag 2>/dev/null; then
  export TAG_SEARCH_PROG=ag  # replace with rg for ripgrep
  tag() { command tag "$@"; source ${TAG_ALIAS_FILE:-/tmp/tag_aliases} 2>/dev/null; }
  alias ag=tag  # replace with rg for ripgrep
fi

alias lsc='colorls -lA --sd'
alias cdd='cd `cat ~/.vpath`'
alias lss='ls -1 |fpp -ai'
alias rms='ls -1 |fpp -ai -c "rm -rf"'
alias d='dict -d langdao-ec'
alias xx='cat ~/.vbuf |xargs'
alias rm="trash-put"
# use trash-empty instead
# alias trash-rm="sudo rm -rf $HOME/.local/share/Trash/files/"
