# iVotechain

## todo

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

## Aufbau

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
