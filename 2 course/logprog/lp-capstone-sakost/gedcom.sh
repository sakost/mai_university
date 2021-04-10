#!/usr/bin/env bash
set -ueo pipefail

ged_filename="family.ged"

if [[ "$#" > 0 ]]; then
    ged_filename="$1"
fi

function strip_str {
    echo "${1//[$'\r\n ']}"
}

# read attribute of person (attribute must be set in $1)
function read_attr {
    
    echo ""
}

# convert string of gedcom to integer(string must be set in $1)
function str_to_id { 
    var=$1
    var2=${var:3:-1}
    echo $(( 10#${1:3:-1} ))
}

declare -A people=()

# reading line by line ged file
while IFS="" read -r line || [ -n "$line" ]
do
    params=($line)
    len=${#params[@]}

    # skip line if there are too few arguments
    if (( $len < 3)) ; then
        continue;
    fi

    attr="${params[2]//[$'\r\n']}"
    # if we found a person
    if [[ $attr == "INDI" ]] ; then
        id=$(str_to_id "$(strip_str "${params[1]}")") # convert id to integer

        name=""
        givn_name=""
        surname=""
        marnm=""
        sex=""

        while IFS="" read -r line || [ -n "$line" ]
        do
            params=($line)
            var=$(strip_str ${params[1]})

            case "$var" in
                "NAME")
                name=$(strip_str ${params[2]})
                ;;
                "GIVN")
                givn_name=$(strip_str ${params[2]})
                ;;
                "SURN")
                surname=$(strip_str ${params[2]})
                ;;
                "_MARNM")
                marnm=$(strip_str ${params[2]})
                ;;
                "SEX")
                sex=$(strip_str ${params[2]})
                ;;
                "RIN") break
                ;;
                *);;
            esac
        done

        if [[ -z "$givn_name" ]] ; then
            givn_name="Unknown#$id"
        fi

        if [[ -z "$surname" && -n "$marnm" ]] ; then
            surname="$marnm"
        fi

        if [[ -n "$surname" ]] ; then
            givn_name="\"${givn_name}_${surname}\""
        else
            givn_name="\"${givn_name}\""
        fi

        people[$id]="$givn_name"
        echo "sex(${people[$id]}, ${sex,,})."
    
    # found relation
    elif [[ $attr == "FAM" ]] ; then
        read -r line
        read -r line
        params=($line)
        husb=""
        wife=""
        if [[ "$(strip_str ${params[1]})" == "HUSB" ]]; then
            h_id=$(str_to_id $(strip_str ${params[2]}))
            husb="${people[$h_id]}"
            read -r line
            params=($line)
        fi
        if [[ "$(strip_str ${params[1]})" == "WIFE" ]]; then
            w_id=$(str_to_id $(strip_str ${params[2]}))
            wife="${people[$w_id]}"
            read -r line
            params=($line)
        fi

        while [ -n "$line" ]
        do
            params=($line)
            if [[ "$(strip_str ${params[1]})" != "CHIL" ]]; then
                break;
            fi
            id=$(str_to_id "$(strip_str "${params[2]}")") # convert id to integer
            if [ -n "$wife" ] ; then
                echo "parent(${wife}, ${people[$id]})."
            fi
            if [ -n "$husb" ] ; then
                echo "parent(${husb}, ${people[$id]})."
            fi
            IFS="" read -r line
        done
    fi
done <${ged_filename}
