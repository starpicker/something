# ffmpeg input starttime endtime output
# ffmpeg -i $1 -ss $2 -to $3 -f mp3 -vn $4

# avconv input starttime duration output
avconv -i $1 -ss $2 -t $3 -f mp3 -vn $4