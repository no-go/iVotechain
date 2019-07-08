# Überlegungen zu eVoting

Ich denke, seitdem die USA Lochkarten zum Zählen der Bevölkerung einsetzte,
kam die Idee auf, irgendwie Computer zum Zählen von Stimmzetteln zu nutzen.
Jeder kennt bestimmt die amerikanischen Wahlmaschinen (und ihre Skandale),
welche Löcher in den Stimmzettel stanzten... aber leicht versetzt. An dieser
Stelle möchte ich mich gleich von solchen Maschinen distanzieren: Wer
vielleicht schon kein Vertrauen in alle an der Wahl beteiligten Institutionen
hat, der wird sein Vertrauen auch nicht zusätzlich weiterer beteiligter
IT-Firmen und Hersteller dieser Maschinen geben. An dieser Stelle findet
sich ein weiteres Problem beim elektronischen Wählen: Das Vertrauen in
die zugrundeliegende Technik sowie die Komplexität des Wahlvorgangs ist
für Otto-Normal-Verbraucher nur mit Skepzis zu akzeptieren.

## eVoting: Warum sollte man das tun?

Das ist eine gute Frage. Top Argumente sind:

- weniger Papier (Kosten, Umwelt)
- weniger Personal beim Auszählen (Kosten)
- schnelleres und genaueres auszählen (kurz: Ja!)
- sicherer(?), anonymer(?) und komfortabler(?)
- weil billig und komfortabel: Überlegung mehr Bürgerbeteiligung durch mehr Wahlen umsetzen

Punkt 1 und 2 ist schon mal invalid. Der Grund dafür ist, dass man nicht um
eine Papierwahl herum kommt. Menschen ohne Handy oder Internet darf eine
Wahl nicht vorenthalten werden. Gutes Beispiel ist Estland: hier kann man
immerhin in einer Behörde die Geräte verwenden, um elektronisch zu wählen.
Hier stellt sich dann wieder die Frage des Vertrauens in die Geräte. Trotzdem
kann man in Estland final in Papierform noch mal wählen, wodurch die eletronische
Wahl zuvor invalid wird. Die Menge an Papierstimmzetteln sollte daher nicht
reduziert werden (man sollte diese bei einem Engpass nicht plötzlich fälschungsicher
nachdrucken müssen).

Der letzte Punkt ist etwas fragwürdig: Es erinnert an einen Garfield Witz
wo auf der Verpackung des Katzenfutters "Jetzt: neu und verbessert!" steht.
War vorher also das Katzenfutter "alt und schlecht"? Die Aussage, weil
elektonische Wahlen schneller und günstiger sind könne man im Grunde endlich
"feinnmasichiger" das politische Meinungsbild in realer Politik abbilden
hört sich nach einem Armutszeugnis an: genauso kann man sagen, es sei
zu teuer alle 4 Jahre wählen zu lassen. Wenn man den Teufel an die Wand
malen will, kann man auch die Wahlen aufgrund der Staatsverschuldung ganz
aussetzen oder man darf nur 1x im Leben zur Wahl gehen, wenn man z.B.
60 Jahre alt wird.

Die Punkte "sicherer, anonymer, komfortabler" sind absolut streitbar.
Wenn ich mit dem Handy oder einem altmodischem Modem meine Stimme abgeben
kann, ohne vorher 15km durch eine radioaktive alpine Wüstenlandschaft
zu einem Wahllokal oder davor zur einer Behörde laufen zu müssen, dann
stimme ich jedem dieser Punkte teilweise zu. Wie gut sind in diesem Fall
aber folgende Punkte:

- Woher wissen die, dass ich nicht schon mal gewählt habe?
- Woher weiss ich, dass ich die richtige Telefonummer anngerfen habe?
- Was, wenn die beim Wunschergebnis einfach die Leitungen dicht machen?
- Wie anonym und abhörsicher ist meine Telefonleitung?

Das Beispiel mag sich altmodisch anhören, aber es finden sich auch heute
vergleichbare Szenarien. Splittet man diese primitive "Telefonleitungswahl" auf in
die **Information** und in das **Verbreitungsmedium**, so sieht man sich
mit diesen Monstern konfrontiert:

- Informatik bzw. Mathematik
- realen Hardware-Welt (und ihren Protagonisten)

## Ein bunter Blumenstrauß an Anforderungen

Vielleicht sollte man sich die Frage stellen, ob man nur eine bessere alternative
zur Briefwahl will: auch diese ist nicht unumstritten. Eigentlich sollte das
Wahllokal ein Hort der Sicherheit sein und man dort ohne Druck oder Zwang
geheim Wählen können. Bei der Briefwahl kann deer Staat dies nicht sicher stellen.
Die Welt und ihre Wahlen zeigten jedoch, dass es genau anders sein kann, und
man gut daran tat, nicht in die Nähe eines Wahllokals zu kommen oder gar dort
(womöglich das "Falsche") zu wählen.

Folgende Punkte sollte eine Wahl (nach meiner Meinung) ermöglichen:

- eine "Zentrale" vergibt Wahlrecht (Eine Stimme pro Person)
- jeder oder zumindest die Mehrheit der "Betroffenen" sollte das Ergebnis prüfen können
- das Ergebnis sollte idealerweise erst zum Ende bekannt werden
- die Wahl/Stimmabgabe sollte fälschungssicher sein (ggf. ein Nichtwählerverzeichnis)
- ich muss mir sicher sein können, dass meine Stimme gezählt wurde
- was ich gewählt habe sollte (auch in ferner Zukunft, Diebstahl von Geräten
  oder nach Folter) unbekannt bleiben

Ein paar der Punkte habe ich als mathematisch und informationstechnisch lösbar, aber
in "real-life" nicht praktikabel oder möglich klassifiziert. Bei anderen Punkten verhält
es sich glücklicherweise genau umgekehrt. Ich kam zu dem Schluss, dass nur eine
sinnvolle Mischung eine Lösung für eine akzeptable elektronische Wahl möglich ist.

## Mein Modell

Mein Modell ist eine Mischung aus Blind-Signierung, Pokemon Go, kommunaler Verbund
einer öffentlichen Blockchain und dem Ablauf der Wahl in Estland.

Der Ablauf ist wie folgt:

### Staatlicher Public Key

Der Staat oder die Institution, die das Wahlrecht und die Personalausweise vergibt,
erstellt einen öffentlichen Schlüssel und einen privaten Schlüssel für die Wahl.

### Vote verschlüsseln (blind)

Ich mache mir ein Tupel aus Vote, einer Nonce, ggf. einem Pseudonym und machen
daraus einen hash-Wert. Auf der Basis des Staats Public Keys baue ich mit eine
symetrische Verschlüsselung und mache den Hashwert damit "blind". Als Pseudonym
hatte ich über ein Bild meiner Blutbahnen in der Hand nachgedacht, aber so
könnte man rausfinden, dass ich (physisch, nicht Name/Identität) diese Wahl getroffen
habe. Wichtig ist, dass Nonce/Pseudonym eindeutig ist, da sonst eine Kollision
beim Auszählen auftritt. Inwiefern das verhindert werden kann (ggf. Anwendung
eines eigenen pubkeys + den des Staates vorm Hashen) muss noch geklärt/getestet werden.

### Blind Vote signieren

Ich gehe mit meinem Pass zu Vater Staat, der mich als Wahlberechtigt erkennt. Daraufhin
signiert er mir meinen verschlüsselten Hashwert des Votes.

### Unblind

Ich nutze meine symetrische Verschlüsselung, um eine staatliche Signierung des
Hashwertes meines Tupels (Vote, Nonce, Pseudonym) zu bekommen.

### Bildung meines Votes

Nun bilde ich mir folgendes Tupel:

- pub((Vote, Nonce, Pseudonym))
- hash((Vote, Nonce, Pseudonym))
- signatur(hash((Vote, Nonce, Pseudonym)))

### eVote-Sharing

Bis kurz vor der Wahl teile ich mit anderen Leuten diese Tupel und werde damit
so etwas wie eine persönliche Wahlurne. Da die Städte und Gemeinden (und ggf.
auch Privatpersonen) einen öffentlichen Cloudspeicher (P2P, Redundanz, Tabelle)
aufgebaut haben, kann ich die gesammelten Tupel dort hochspielen. Eine Prüfung
von hash und signatur kann jeder leicht machen und sich auch sein eigenes
Tupel anschauen. Durch den Upload mehrerer Tupel wird verschleiert, welches von
mir selbst ist.

### Auszählung

Der Staat könnte nun den private key veröffentlichen, um allen das Auszählen
und die Korrektheit zu gewähren. Zusätzlich könnte man auch noch seinen
eigenen public key beim ersten Tupel mit vverbraten... eine anonymität ginge hier
aber möglicherweise verloren (meine IP bei veröffentlichung). beim sammeln der
Tupel könnte man sich aber auch die Leute merken, wenn sie einem nur eines
(ihr eigenes) Tupel geben. cool wären hier natürlich ungültige fake-tuple.
