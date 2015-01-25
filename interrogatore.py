# -*- coding: cp1252 -*-
import string
import random
import sys

#La dimensione della lista associata alla domanda permette di deciderne il peso... Maggiore è il numero di elementi più spesso uscirà la domanda associata
#Basta modificare la lista domande 

domande = {
	"[Protezione e sicureza] Politiche e Modelli di protezione":[1,2,3,4,5,6,7],
	"[Protezione e sicureza] Principio del privilegio minimo":[1,2,3,4,5,6],
	"[Protezione e sicureza] Matrice degli accessi":[1,2,3,4,5,6],
	"[Protezione e sicureza] Diritti fondamentali":[1,2,3,4,5,6,7],
	"[Protezione e sicureza] ACL e CL":[1,2,3,4,5,6,7],
	"[Protezione e sicureza] Reference Monitor":[1,2,3],
	"[Virtualizzazione] Definizione":[1,2,3,4,5,6,7],
	"[Virtualizzazione] Tecnologie di virtualizzazione":[1,2,3,4,5,6,7],
	"[Virtualizzazione] Realizzazione del VMM":[1,2,3,4,5,6,7,8],
	"[Virtualizzazione] Possibili problemi del VMM":[1,2,3,4,5,6,7],
	"[Virtualizzazione] Xen":[1,2,3,4,5,6,7],
	"[Virtualizzazione] Gestion VM":[1,2,3,4,5],
	"[Programmazione concorrente] Tipi di architetture":[1,2,3,4,5,6],
	"[Programmazione concorrente] Tipi di applicazione":[1,2,3,4,5,6],
	"[Programmazione concorrente] Architetture e linguaggi per la programmazione concorrente":[1,2,3,4,5,6],
	"[Programmazione concorrente] Costrutti linguistici (fork)":[1,2,3,4,5,6,7,8],
	"[Programmazione concorrente] Costrutti linguistici (join)":[1,2,3,4,5,6,7,8],
	"[Programmazione concorrente] Costrutti linguistici (cobegin/coend)":[1,2,3,4,5],
	"[Programmazione concorrente] Realizzazione delle primitive":[1,2,3,4,5,6],
	"[Programmazione concorrente] Proprietà dei programmi":[1,2,3,4,5,6],
	"[Modello a memoria comune] Definizione":[1,2,3,4,5,6,7,8],
	"[Modello a memoria comune] Gestori di risorse":[1,2,3,4,5,6,7,8],
	"[Modello a memoria comune] Accesso alle risorse":[1,2,3,4,5,6,7],
	"[Modello a memoria comune] Sincronizzazione condizionale":[1,2,3,4,5,6,7],
	"[Modello a memoria comune] Mutua esclusione":[1,2,3,4,5,6,7,8,9],
	"[Strumenti di sincronizzazione] Definizione di semaforo":[1,2,3,4,5,6,7,8,9,10],
	"[Strumenti di sincronizzazione] Proprietà del semaforo":[1,2,3,4,5,6,7],
	"[Strumenti di sincronizzazione] Semafori particolari":[1,2,3,4,5,6],
	"[Strumenti di sincronizzazione] Realizzazione dei semafori":[1,2,3,4,5,6,7],
	"[Threads vs Processi] I threads nelle varie architetture":[1,2,3,4,5,6,7,8],
	"[Threads vs Processi] Realizzazione dei threads":[1,2,3,4,5,6,7,8],
	"La curva dose-risposta di un farmaco e le sue caratteristiche":[1,2,3,4,5,6,7,8],
	"[Kernel] Definizione":[1,2,3,4,5,6,7,8,9,10],
	"[Kernel] Compiti":[1,2,3,4,5,6,7,8],
	"[Kernel] Stati di un processo":[1,2,3,4,5,6,7,8],
	"[Kernel] Strutture dati":[1,2,3,4,5,6,7,8],
	"[Kernel] Funzioni di livello inferiore":[1,2,3,4,5,6,7,8],
	"[Kernel] Passaggio di ambiente":[1,2,3,4,5,6],
	"[Kernel] Sistemi multi-processore":[1,2,3,4,5,6,7,8],
	"[Monitor] Definizione":[1,2,3,4,5,6,7,8,9,10],
	"[Monitor] Uso":[1,2,3,4,5,6,7,8],
	"[Monitor] Realizzazione tramite semafori":[1,2,3,4,5,6,7,8],
	"[Modello a scambio di messaggi] Definizione":[1,2,3,4,5,6,7,8,9,10],
	"[Modello a scambio di messaggi] Il canale":[1,2,3,4,5,6,7,8],
	"[Modello a scambio di messaggi] Primitive di comunicazione (sincrone)":[1,2,3,4,5,6],
	"[Modello a scambio di messaggi] Primitive di comunicazione (asincrone)":[1,2,3,4,5,6],
	"[Modello a scambio di messaggi] Realizzazione delle primitive asincrone":[1,2,3,4,5],
	"[Architetture distribuite] Sistemi NOS":[1,2,3,4,5,6],
	"[Architetture distribuite] Sistemi DOS":[1,2,3,4,5,6],
	"[Architetture distribuite] Pacchetti, interfacce, canali":[1,2,3,4,5,6],
	"[Architetture distribuite] Comunicazione con sincronizzazione estesa":[1,2,3,4,5,6,7,8],
	"[Linguaggio ADA] Specifiche":[1,2,3,4,5],
	"[Azioni atomiche] Definizione":[1,2,3,4,5,6,7,8,9,10],
	"[Azioni atomiche] Consistenza dei dati":[1,2,3,4,5,6,7,8,9],
	"[Azioni atomiche] Malfunzionamenti":[1,2,3,4,5,6,7,8,9],
	"[Azioni atomiche] Realizzazione":[1,2,3,4,5,6,7,8,9,10],
	"[Azioni atomiche] Azioni atomiche multiprocesso":[1,2,3,4,5,6,7,8,9],
	"[Azioni atomiche] Azioni atomiche multiprocesso e sistemi distribuiti":[1,2,3,4,5,6,7,8,9],
	"[Azioni atomiche] Azioni atomiche nidificate":[1,2,3,4,5,6,7,8,9],
	"[Azioni atomiche] RPC in sistemi distribuiti":[1,2,3,4,5,6,7,8],
	"[Azioni atomiche] Transazioni":[1,2,3,4,5,6,7,8]
	}


#######################################################################################################################################################################################################
def menu():
    print "\n"
    print "MENU"
    print "[1]:iniziare"
    print "[2]:lista delle domande"
    print "[3]:Esci"
    print "\n"
    while True:
        choose=raw_input("Scegli un opzione")
        if(choose=="1"):
            start()
            break;
        if(choose=="2"):
            lst()
            break;
        if(choose=="3"):
            esc()
            break;
        else:
            print "valore non riconosciuto"
    
def start():
    history=list()
    nuovadomanda=random.choice([k for k in domande.keys() for x in domande[k]])
    print "\n"
    print "INIZIO DOMANDE"
    print "\n"
    i=1
    while True:
        while True:
            
            if(len(history)==10):
                del history[0]

            nuovadomanda=random.choice([k for k in domande.keys() for x in domande[k]])

            if(nuovadomanda in history):
                print ""
            else:
                break;
            
        print str(i)+")"+nuovadomanda+"?"
        a=raw_input("[invio]:prossima domanda [0]:menu\n")
        history.append(nuovadomanda)
        i=i+1
        if(a=="0"):
            menu()
            break;
        if(a==""):
            print ""
def lst():
    i=1
    print "\n"
    print "LISTA DOMANDE PRESENTI"
    print "\n"
    for key in domande.keys():
        print str(i)+")"+str(key)
        i=i+1
    print "\n"
    menu()


def esc():
    print "ESCO"
    sys.exit()
    
menu()
        
##########################################################################################################################################################################################################
