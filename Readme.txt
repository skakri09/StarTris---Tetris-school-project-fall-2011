Jeg har i stor grad benyttet meg av de minste mulige data typene, men bruker ikke mindre en short for integers så det er mulig å resize spillet 
til ekstreme dimensjoner for morro-skyld.

Jeg gjør all utskrift i gameManager. Er klart over at jeg kunne brukt en referanse til outputManager objekt og gjort individuell utskrift i piece/playArea
for deres respektive funksjoner, men synes det gir bedre oversikt å gjøre all graphics output i gameManager istedenfor.

Jeg legger landede pieces til i samme array som background for å slippe å ha en egen array for landede pieces som også må tegnes ut. En mindre og unødvendig for-loop.
Hadde jeg trengt å gjøre forskjell mellom de to pga en eventuell special-feature til spillet ville jeg heller da vurdert å lage en egen landedPieces array.

Slik spillet er nå, hadde det vært mulig å slå sammen en eller to av collision checkene for å ha færre for-loops. Jeg har likevel valgt å beholde det slik det er nå, 
da det vil gi en bedre mulighet til å bygge ut med eventuelle extra features eller lignende. Det skal også nevnes at når hver for-loop kun kjøres 16 ganger, er det ekstremt
minimalt hvor mye det hadde hatt å si om jeg hadde en mindre. Hadde loopene vært større, ville jeg absolutt vurdert å slå dem sammen.

GameManager klassen min er satt sammen av en del små funksjoner. Slik det er nå kunne noen av dem vært slått sammen til større funksjoner, men jeg liker oversikten det gir
meg sånn det er nå, samtidig som det gjør det veldig enkelt å legge til funksjonskall på riktige steder slik det er nå.

Har valgt å starte TowerPiecen ligende i motsettning til stående slik den er vist i oppgaveteksten. Det gir mer mening for meg, da man ville fått gameOver med 4 
linjer til overs med towerPiece da jeg starter piecene mine inne på brettet og ikke utenfor. Også sett mange andre tetriser gjør det samme.

Jeg valgte å skrive om loopTimeren til å være en baseklasse (LoopTimerBase), med tre barneklasser (generalTimer, keyHeldDownTimer og PieceTimer). Jeg får da vist noen enkle arv teknikker,
samtidig som de tre timerne ikke trenger å få med funksjonene fra den orginale loopTimeren de ikke trenger. Eksempelvis Trenger ikke generalTimer
setUpdatesPerInterval/setIntervalDuration, da denne skal være statisk hele tiden. pieceTimer trenger ikke sleepUntillUpdate. Jeg la også til en "pause" funksjon i piecetimeren,
som "snapshotter" noen timer variabler, for så å kunne kalle en resumeFromPause funksjon som setter timer variablene til nøyaktig riktig istedenfor hva start() ville gjort. 
Start() er også deklarert virtual, så jeg i keyHeldDownTimer kan skrive den om til å inkludere en liten delay på første IsTimeForUpdate. Den virker ikke helt som den skal, da
timeren prøver å ta igjen den utsatte første oppdateringen, men jeg har ikke fått tid til å fikse dette 100%. Det føles bra i spillet.

Jeg har laget en egen SoundManager klasse hvor det er enkelt å kalle spesifk musikk og lyder fra. Har også laget en egen score klasse som holder på current score, highscore
og combopoints. Highscoren blir lest av en ekstern .txt fil, og kan også skrives til denne txt filen om en ny highscore blir satt.

Jeg har lagt til en try-catch blokk på initialiseringen av spillet. Den vil skrive til consoll vinduet når den starter og avslutter initialiseringen av de kritiske delene av spillet 
(lyd, grafikk o.l). Om en feil skulle oppstå under allokeringen av minnet til spillet vil en bad_alloc utskrift komme, og skjer det en feil med annen initialisering
vil det skrives ut en exception.what()

X og Y for pieces kan i enkelte tilfeller være forvirrende da det i noen tilfeller blir speilvendt når det taes ut igjen fra en array med peker. 
De skal uansett stemme i alltid skrive ut riktig.