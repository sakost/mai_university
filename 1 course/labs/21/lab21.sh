#!/bin/bash

usage(){
    echo "usage: ${0##*/} size [-h | --help]"
    echo -e "\t-s size | --size size - size of file"
    echo -e "\t-h | --help - print to the output this message"
    echo -e "\t-d dir | --directory dir - in what directory check"
}

error() {
    echo -e "error occurred: $1"
    usage
}

# https://stackoverflow.com/a/31625253/8506506
dehumanise_size() {
  for v in "${@:-$(</dev/stdin)}"
  do  
    echo $v | awk \
      'BEGIN{IGNORECASE = 1}
       function printpower(n,b,p) {printf "%u\n", n*b^p; next}
       /[0-9]$/{print $1;next};
       /K(iB)?$/{printpower($1,  2, 10)};
       /M(iB)?$/{printpower($1,  2, 20)};
       /G(iB)?$/{printpower($1,  2, 30)};
       /T(iB)?$/{printpower($1,  2, 40)};
       /KB$/{    printpower($1, 10,  3)};
       /MB$/{    printpower($1, 10,  6)};
       /GB$/{    printpower($1, 10,  9)};
       /TB$/{    printpower($1, 10, 12)}'
  done
} 

dirname=$PWD
size=
verbose=0
while [ "$1" != "" ]
do
    case $1 in
        -d | --directory )      shift
                                dirname=$1
                                ;;
        -s | --size )           shift
                                size=$1
                                ;;
        -h | --help )           usage
                                exit 0
                                ;;
        * )                     error "unknown argument"
                                exit 1
    esac
    shift
done
size=$(dehumanise_size $size)
if [ -z "$size" ]
then 
    error "bad size format"
    exit 1
fi

files=$(find $dirname -type f -size -${size}c)
for FILE in $files
do
    SFILE=${FILE##*/}
    ext="${FILE#*.}"
    len=${#ext}
    to_paste=$(echo "$SFILE" | cut -c1-${len})
    WEFILE="${FILE%.*}"
    mv -u $FILE ${WEFILE}.${to_paste}  >/dev/null
done

