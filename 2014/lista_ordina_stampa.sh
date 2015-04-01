#!/bin/bash

# Considerato il file “Lista”, di seguito riportato, mediante script presentare un MENU a video che, a seconda della scelta digitata dall’utente, realizzi le seguenti operazioni:
# • 1 per ordinare la lista data
# • 2 per stampare la sola riga contenente il nome Ugo
# • 3 per conoscere quante linee contengono la parola Ugo
# • 4 per conoscere il numero di linee contenenti la parola impiegato
# • 5 per stampare le prime 3 linee
# Si richiede che il nome del file (Lista) si dato come parametro in ingresso allo script, e quindi occorre effettuare un controllo sui parametri in ingresso, presentando all’utente un messaggio di errore nel caso in cui il parametro sia omesso.
# Lista
# Urbano Franco, Direttore
# Criaco Antonio, Consulente
# Rossi Ugo, Impiegato 
# Rossi Pasquale, Operaio
# Gallo Ugo, Consulente
# Arbetta Carmela, Direttrice
# Zuculini Sofia, Consulente
# Greco Gerardo, Impiegato

if [[ -z $1 ]]; then
	echo "Inserire nome file lista.txt"
	exit
fi

nomeFile=$1

# MENU
options="ordina stampaUgo nlineeUgo nlineeImpiegato stampa"
select opt in $options; do
	case $opt in
		"ordina")
			sort $nomeFile
			;;
		"stampaUgo")
			cat $nomeFile | grep "Ugo"
			;;
		"nlineeUgo")
			cat $nomeFile | grep -c "Ugo"
			;;
		"nlineeImpiegato")
			cat $nomeFile | grep -c "Impiegato"
			;;
		"stampa")
			sed '3q' $nomeFile
			;;
		*) 
			echo "opzione errata"
			;;
	esac
done