mkfifo /tmp/mlfifo
sudo mplayer -slave -quiet -idle -input file=/tmp/mlfifo d/ubuntu/mp3/* &
