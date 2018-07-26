if [ $# -lt 1 ]
then
	echo "no memo provided"
	exit
fi

memo="(while sleep 1;
do
	tput sc;
    	tput cup 0 $(($(tput cols)-20));
        echo "$@";
	tput rc;
done)&"

pidmark='CLOCK_PID=$!;
	PARENT_SHELL=$(ps -p "$$" -o ppid= | xargs);
	echo $PARENT_SHELL
	echo "$CLOCK_PID" >> "/tmp/clock.$PARENT_SHELL";
	'

for _window in $(tmux list-windows -F '#I'); do
    tmux send-keys -t ${_window}.1 "$memo"
    tmux send-keys -t ${_window}.1 C-m
    tmux send-keys -t ${_window}.1 "$pidmark"
    tmux send-keys -t ${_window}.1 C-m
done
