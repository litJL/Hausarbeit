#/bin/bash

RED=`tput setaf 1`
BLUE=`tput setaf 4`
GREEN=`tput setaf 2`
NC=`tput sgr0`

if [ ! -d "repo" ]; then
 mkdir repo
 touch repo/dummy
 mkdir repo/dummysNest
 echo ${GREEN}Ein Ordner zum austesten des Programms wurde erstellt${NC}
fi

echo --Compiling: ${RED}CLIENT${NC}--
g++ -o client Client.c
echo --Compiled: ${RED}CLIENT${NC}--
echo --Compiling: ${BLUE}SERVER${NC}--
g++ -o server Server.c
echo --Compiled: ${BLUE}SERVER${NC}--

echo --${GREEN}Everything Compiled${NC}--


exit 0
