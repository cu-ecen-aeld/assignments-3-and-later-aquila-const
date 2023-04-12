#!/bin/sh
filesdir=$1 #argument one, which should be a path
searchstr=$2 #argument two, which should be a search string
help () {
    echo "\nYou need exactly two arguments:"
    echo "1) The directory you are wanting to search in"
    echo "2) String formatted argument to be searched in the path"
    echo "\ne.g.(for example):\n\n./finder.sh <add path here> <add search term here>\n"

    echo "\nif you are trying to search for a specific term within your user path then do \n"
    echo "\n./finder.sh \$HOME <term_to_search>\n"
}

walk_dir(){
    x="$(grep -l -rnw -i "${searchstr}" "${filesdir}" | wc -l)"
    y="$(grep -rnw "${searchstr}" "${filesdir}" | wc -l)"
    echo "The number of files are ${x} and the number of matching lines are ${y}"
}


if [ $# -eq 2 ];then
    walk_dir
    exit 0
else
    echo "\nInvalid Arguments\n"
    help
    exit 1
fi