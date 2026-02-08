il progetto è stato realizzato partendo dalla versione più recente trovata sulla piattaforma e-learning, seguendo i passi per il setup su windows (da cui la cartella "base").

per compilare:
rimpiazzare nel makefile in 17-progetto_HDR_lights:
inserire in "CC = " il proprio compilatore (CC = g++) è il default dato su elearning.
se non si usa un debugger cambiare le flag in (CCFLAGS = -O3 -s -DNDEBUG) è il default dato su elearning

in seguito eseguire il comando "make" nella directory 17-progetto_HDR_lights, ed eseguire program.exe

per usare:
i tasti 0,9 selezionano quale luce si sta cambiando
u,i,o,p,j,l cambiano vari attributi delle luci stesse

di default il numero di luci è limitato a 2 per tipo, ma si può incrementare nel codice, [!] cambiare MAX_***_LIGHT sia in common.h che nella fragment shader 14.frag, in modo tale che i due numeri coincidano
