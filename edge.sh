set -e
gcc edge-detect.c -o edge-detect.bin -Wall -Wextra
./edge-detect.bin input.rgb
convert -size "1920x1080" -depth 8 line.rgb line.png