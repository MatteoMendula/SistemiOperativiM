# Benvenuti! 
In questa repository sono presenti gli esercizi relativi al corso di Sistemi Operativi M dell'università di Bologna.

Le soluzioni degli esami sono scritte in maniera estremamente verbosa per rendere più facile la loro comprensione.

#### Per compilare i programmi C utilizzare:
`gcc -Wall -D_REENTRANT -std=c99 *.c -lpthread && ./a.out`

#### Per compilare i programmi Go utilizzare:
`go run *.go`

##### Riassunto della teoria
[Qui](https://github.com/marmos91/SistemiOperativiM/blob/master/%5BTeoria%5D%20Sistemi%20Operativi%20M.pdf) potrete trovare il pdf con il riassunto della teoria.

##### Utilizzare l'interrogatore
Lanciare il comando `python interrogatore.py` da terminale. Il programma estrarrà delle domande in ordine casuale dalla lista inserita al suo interno, mostrando più spesso quelle con un punteggio assegnato maggiore.

#### Lavorare con Xcode
Il compilatore C di Mac OS considera **deprecati** i semafori standard pthread come utilizzati nel corso. 
Quindi, sebbene chiunque possieda un Mac può clonare questa repository direttamente da XCode, si consiglia sempre di compilare e testare il programma in ambiente Linux. 

**ATTENZIONE**: Il programma compila correttamente anche su Mac, ma i semafori NON si comportano nella maniera che ci si aspetterebbe in quanto non vengono inizializzati dalla `sem_init()` deprecata. 

Nessun problema per quanto riguarda gli esercizi sui monitor

Qualsiasi contributo è ben accetto! Buono studio.
