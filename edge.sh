set -e
gcc edge-detect.c
./a.out
convert -size "1920x1080" -depth 8 line.rgb line.png