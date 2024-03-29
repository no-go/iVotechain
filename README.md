# iVotechain

Fazit: Das Projekt habe ich ersmal aufgegeben, da ich logisch
eine geheime elektronische Wahl unter Teilnehmern, die sich
nicht trauen, für nicht durchführbar halte.

Überlegungen zur Unmöglichkeit

- ich traue der Institution, die einer Person ein Wahlrecht einräumt.
  An dieser Stelle ist möglich, dass diese Institution einfach
  die Stimmen der Nicht-Wähler nimmt, und damit wählt.
- Obiges ist nur von der Öffentlichkeit erkennbar, wenn das Wahlrecht
  an eine echte, eindeutige Person (kein übertragbarer Key/Token) gebunden
  ist (elektronisch nur schwer möglich - Stimme gut verkaufbar)
- Trennung Voting und Wahlberechtigung: Eine getrennte Speicherung
  geschieht im guten Glauben. Die Öffentlichkeit kann eine getrennte
  Speicherung nicht überprüfen. Mit einem homomorpher Kryptograpie und
  Mixnet ließe sich zwar Voting+Wahlrecht zusammen erfassen, aber eine
  Erkennbarkeit, ob meine Wahl korrekt gezählt wurde, entfällt.
- IP-Adresse, Person, Voratsdatenspeicherung: auch mit TOR oder einem
  vergleichbaren Netzwerk, welches zur Wahl der Staat aufbauen würde,
  ist die Abgabe einer elektronischen Stimme nicht anonym oder geheim.
  Dies könnte man durch ein vorgelagertes Sammeln von Stimmen anderer
  Personen abfedern. Man lädt in dem Fall dann gleich mehrere
  mit einem Pseudonym markierte verschlüsselte Votings hoch, so dass
  das eigene Voting "untergeht". Bootstraping ist hier aber ein Problem:
  bekomme ich initial ein Voting von einem anderen, dann kann ich diese
  Stimme ihm zuordnen. Ohne Pseudonym und mit einem Mixnet, wäre
  allerdings diese Sammelidee nicht möglich, da sonst Stimmen
  mehrfach gezählt werden.
- Die Wahl muss geheim ausgezählt werden: Aus meiner Sicht gilt dies
  auch für den Staat. Wenn dieser einen public-Key zum Verschlüssel
  des Votings veröffentlicht, so muss ich eigentlich auch zusätzlich
  noch mit meinem Key oder einer "Nonce" mein Voting vor einem vorzeitigen
  auszählen des Staats schützen. Leider existiert noch kein Konzept,
  bei dem ein zeitliches Ereignis zur automatischen Entschlüsselung
  führt (spannendes Thema!). Daher muss ich genauso wie der Staat
  am Ende der Wahl meinen private Key/Schlüssel/Nonce veröffentlichen.
  Genau wie bei dem IP-Adressen Punkt: mein Pseudonym im Voting wird
  bei dieser Veröffentlichung an meine Person gebunden/gelinkt.


Aus allen mir damit verbundenen Überlegungen führe ich das Projekt
hier nicht weiter. Durch die Möglichkeit einer Vorratsdatenspeicherung
ist der Zugang zu einem elektronischen Wahlsystem nicht geheim/anonym
und Techniken wie "Mixnet" führen zu einem Mangel der Überprüfbarkeit
bei der Abgabe. Alternative/Kombinierte Systeme, die dies lösen würden,
würden ein großes Vertrauen in eine an der Wahl beteiligten Institution
oder Service-Provider vorraussetzen. Dies mache aber eine sichere,
geheime elektronische Wahl unter "Paranoiden" die sich alle nicht trauen
zu einem unmöglichen Konzept.

# Aufbau bisheriger Code

Das Projekt nutzt Crypto++ um ein Full Domain Hash, base64 Kodierung
sowie eine RSA blind-Signierung (inkl. Verschlüsselung) zu ermöglichen. Als
Übertragung wird Apache Thrift (= Remote Procedure Calls) zwischen den beteiligen
Knoten verwendet. Die Kommunikation ist in der Datei `src/SimpleEvote.thrift` beschrieben
und mit `make idl` wird daraus ein Basiscode generiert (ist schon geschehen!). Dieser wird dann
in folgenden Dateien verwendet und mit `make` kompiliert:

- **rOffice.cpp**: Der Registration Office Server erzeugt Keys und signiert blind.
- **bBox.cpp**: Der Ballot Box Server nimmt valide Votes auf.
- **voter.cpp**: Ein Client, der sich erst eine Signatur holt und damit
  beim Ballot Box Server ein Vote abgibt.
- **schoenenborn.cpp**: Ein Client, der mit Hilfe eines veröffentlichten private Keys
  beim Ballot Box Server die verschlüsselten Votes entschlüsseln lässt.

Zum Kompilieren werden die Bibliotheken von [cryptopp](https://cryptopp.com/) 
und [thrift](https://thrift.apache.org/) benötigt. Beides sollten gute
Linux-Dstributionen als Standard-Packet anbieten.
Ubuntu 2018.0 scheint jedoch die libthrift-cpp nicht mehr zu haben und man
muss deswegen thrift mit open ssl selbst kompilieren, was sehr ärgerlich ist.

Im Kopf dieser 4 Dateien sind Ports, Hostnamen, Dateipfade sowie andere Parameter
definiert. Der laufende `./rOffice` Server verbietet außerdem eine
erneute Signatur. Sollte ein anderer Voter mit dem selben Pseudonym
ein Voting zur Ballot Box senden, so wird die Annahme verweigert. Leider kann
er sich dann keine neue Signierung eines anderen Pseudonyms machen lassen.

Eine sichere Datenübertragung sowie andere Konzepte, welche Wahlbetrug 
verhindern/erschweren soll, sind nicht implementiert. Im Fokus steht:

- Registation Office kann Pseudonyme für "allowed.txt" signieren, ohne die 
  Pseudonyme zu kennen.
- Ballot Box kann Signatur zum Pseudonym prüfen und ein Wahlrecht prüfen, ohne 
  eine "allowed.txt" Liste zu besitzen.
- Erst durch das veröffentlichen des privaten Schlüssels des Registation 
  Office ist eine Stimmauszählung möglich.
- Eine mathematische Verbindung zwischen Voting und Identität in "allowed.txt" 
  existiert nicht.

Außer die Dateien in `src/gen-cpp/` ist der Code relativ übersichtlich und 
relativ gut dokumentiert. Ein paar Crypto++ Teile waren etwas unschön und
sind in `src/Tools.hpp` von mir in Funktionen ausgelagert worden.

Den besten Einstieg zum Lesen des Codes bietet die `main()` Funktion der
Datei `voter.cpp`.

# More Technical Details

- the RegOffice server generates a 4096bit RSA public and private key pair
- the RegOffice server has a list with valid identCardIds+names
- everyone gets this public key (e.g. download or USB stick)
- voter (client) uses this public key to blind (encrypt symetric) a full domain (4096bit) hashed pseudonyme
- voter sends encrypt(fdh(P))) and his/her valid identCardIds to RegOffice and gets a blinded signature
- voter unblind (decrypt symetric) the blinded signature
- voter uses RegOffice public key to build an encrypted vote (pubCryptVote)
- voter sends (makes it public) his/her psydonyme, unblinded signature and pubCryptVote to BallotBox (or Boxes => maybe a p2p network)
- BallotBox can verify the signature via RegOffice public key
- BallotBox can start counting votes after someone sends the RegOffice private key

Many parts are ugly (uncrypted data transfer) and unsave (same psydonyme?), but this is a
simple concept to vote in a public election. Especialy the "random" selection for the
symetric blinding must be "good". Additionaly: how do you prevent RegOffice from voting
with unsigned non-voters if the eVoting system is so anonymous? What happend, if BallotBox
uses pseudonym+signature to generate its own vote? Maybe gossip/broadcast a
hash of psydonyme as commitment. Yada, yada, yada...

## Benefits

- RegistrationOffice did not know voter pseudonyms (and their vote)
- RegistrationOffice check double request from same voter (ident CardId+Name)
- the single BallotBox did not know the voter identCardId/Names or need any voter list
- sended content can be public checked because it is encrypted
- counting the votes can be done by RegistrationOffice or after the private key 
  gets public (election ends)

# todo (veraltet)

tests

- wie zufällig ist pub und priv key
- probieren mit priv key andere pub keys zu machen und ob signierung dann noch valide ist
- wie zufällig ist erzeugter symetrischer schlüssel 

Umbau

- commitment des Pseudonyms um missbrauch vor zu beugen
- hash des letzten votes oder commitments soll verlinkt werden: Verteilte Chain als Zeitstempel
- Lamport-Zeit?
- client und BB soll ein "peer" werden
- gossip funktionen, um BB miteinander zu sync-en
- vote soll ein mix aus Tx und Block einer Blockchain sein
- revoke eines Pseudonyms?
- mixnet? Wie erreiche ich trennung vote-pseudonym?
- mixnet oder Fake Tor Netzwerk, damit vote von IP gelöst ist?

Frage

Wie verschleiere ich in einem P2P Netzwerk die Absender IP-Adresse,
wenn ich eine Nachricht via Gossip broadcaste?

Mixnet

- Gruppen oder Ringsignierung?
- homomorphe Kryptographie?
