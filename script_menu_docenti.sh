# 13 ottobre 2014
# Fabio Nisci

#!/bin/bash

# (29/06/2010)

function inizio {
	sed '/Ordinario/s/.*/Prof &/' docenti.txt
	sed '/Ricercatore/s/.*/Dott &/' docenti.txt 
}

function stanza {
	read -p "numero stanza?" stanza
	cat docenti.txt | grep "$stanza" 
}

function cancella {
	sed 's/^$/d' docenti.txt
}

function sostituisce {
	sed 's/[0-9][0-9][0-9]/xxx/' docenti.txt
}



# MENU
options="inizio stanza cancella sostituisce"
select opt in $options; do
	case $opt in
		"inizio")
			inizio
			;;
		"stanza")
			stanza
			;;
		"cancella")
			cancella
			;;
		"sostituisce")
			sostituisce
			;;
		*) 
			echo "opzione errata"
			;;
	esac
done