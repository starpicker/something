pidmark='PARENT_SHELL=$(ps -p "$$" -o ppid= | xargs);
echo $PARENT_SHELL;
if [[ -f /tmp/clock.$PARENT_SHELL ]]; then
    for line in $(cat /tmp/clock.$PARENT_SHELL)
    do
	    echo $line
	    kill -9 $line > /dev/null
    done
    rm "/tmp/clock.$PARENT_SHELL";

fi;'
clearmark='LENGTH=20
    tput sc;
    tput cup 0 $(($(tput cols)-20));
    for i in $(seq 1 $LENGTH); do printf " "; done;
    tput rc'

for _window in $(tmux list-windows -F '#I'); do
    tmux send-keys -t ${_window}.1 "$pidmark"
    tmux send-keys -t ${_window}.1 C-m
    break;
done

for _window in $(tmux list-windows -F '#I'); do
    tmux send-keys -t ${_window}.1 "$clearmark"
    tmux send-keys -t ${_window}.1 C-m
done
