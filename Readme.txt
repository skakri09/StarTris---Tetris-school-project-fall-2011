Jeg har i stor grad benyttet meg av de minste mulige data typene, men bruker ikke mindre en short for integers s� det er mulig � resize spillet 
til ekstreme dimensjoner for morro-skyld.

Jeg gj�r all utskrift i gameManager. Er klart over at jeg kunne brukt en referanse til outputManager objekt og gjort individuell utskrift i piece/playArea
for deres respektive funksjoner, men synes det gir bedre oversikt � gj�re all graphics output i gameManager istedenfor.

Jeg legger landede pieces til i samme array som background for � slippe � ha en egen array for landede pieces som ogs� m� tegnes ut. En mindre og un�dvendig for-loop.
Hadde jeg trengt � gj�re forskjell mellom de to pga en eventuell special-feature til spillet ville jeg heller da vurdert � lage en egen landedPieces array.

Slik spillet er n�, hadde det v�rt mulig � sl� sammen en eller to av collision checkene for � ha f�rre for-loops. Jeg har likevel valgt � beholde det slik det er n�, 
da det vil gi en bedre mulighet til � bygge ut med eventuelle extra features eller lignende. Det skal ogs� nevnes at n�r hver for-loop kun kj�res 16 ganger, er det ekstremt
minimalt hvor mye det hadde hatt � si om jeg hadde en mindre. Hadde loopene v�rt st�rre, ville jeg absolutt vurdert � sl� dem sammen.

GameManager klassen min er satt sammen av en del sm� funksjoner. Slik det er n� kunne noen av dem v�rt sl�tt sammen til st�rre funksjoner, men jeg liker oversikten det gir
meg s�nn det er n�, samtidig som det gj�r det veldig enkelt � legge til funksjonskall p� riktige steder slik det er n�.

Har valgt � starte TowerPiecen ligende i motsettning til st�ende slik den er vist i oppgaveteksten. Det gir mer mening for meg, da man ville f�tt gameOver med 4 
linjer til overs med towerPiece da jeg starter piecene mine inne p� brettet og ikke utenfor. Ogs� sett mange andre tetriser gj�r det samme.

Jeg valgte � skrive om loopTimeren til � v�re en baseklasse (LoopTimerBase), med tre barneklasser (generalTimer, keyHeldDownTimer og PieceTimer). Jeg f�r da vist noen enkle arv teknikker,
samtidig som de tre timerne ikke trenger � f� med funksjonene fra den orginale loopTimeren de ikke trenger. Eksempelvis Trenger ikke generalTimer
setUpdatesPerInterval/setIntervalDuration, da denne skal v�re statisk hele tiden. pieceTimer trenger ikke sleepUntillUpdate. Jeg la ogs� til en "pause" funksjon i piecetimeren,
som "snapshotter" noen timer variabler, for s� � kunne kalle en resumeFromPause funksjon som setter timer variablene til n�yaktig riktig istedenfor hva start() ville gjort. 
Start() er ogs� deklarert virtual, s� jeg i keyHeldDownTimer kan skrive den om til � inkludere en liten delay p� f�rste IsTimeForUpdate. Den virker ikke helt som den skal, da
timeren pr�ver � ta igjen den utsatte f�rste oppdateringen, men jeg har ikke f�tt tid til � fikse dette 100%. Det f�les bra i spillet.

Jeg har laget en egen SoundManager klasse hvor det er enkelt � kalle spesifk musikk og lyder fra. Har ogs� laget en egen score klasse som holder p� current score, highscore
og combopoints. Highscoren blir lest av en ekstern .txt fil, og kan ogs� skrives til denne txt filen om en ny highscore blir satt.

Jeg har lagt til en try-catch blokk p� initialiseringen av spillet. Den vil skrive til consoll vinduet n�r den starter og avslutter initialiseringen av de kritiske delene av spillet 
(lyd, grafikk o.l). Om en feil skulle oppst� under allokeringen av minnet til spillet vil en bad_alloc utskrift komme, og skjer det en feil med annen initialisering
vil det skrives ut en exception.what()

X og Y for pieces kan i enkelte tilfeller v�re forvirrende da det i noen tilfeller blir speilvendt n�r det taes ut igjen fra en array med peker. 
De skal uansett stemme i alltid skrive ut riktig.