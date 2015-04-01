#!/bin/bash


if [[ -z $1 ]]; then
	echo "manca nome file"
	exit 1
fi

# MENU
options="ordina stampa linee limpegato 3linee"
select opt in $options; do
	case $opt in
		"ordina")
			sort $1
			;;
		"stampa")
			sed -n '/Ugo/p' $1
			;;
		"linee")
			sed -n '/Ugo/p' $1 | wc -l
			;;
		"limpegato")
			sed -n '/impiegato/p' $1 | wc -l
			;;
		"3linee")
			sed 3q $1
		;;
		*) 
			echo "opzione errata"
			;;
	esac
done