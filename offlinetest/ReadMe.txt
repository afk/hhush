Vorbereitung
~~~~~~~~~~~~

Damit Sie Ihr Programm schon vor der endgültigen Abgabe überprüfen können, stellen 
wir Ihnen automatische Tests zur Verfügung, die Sie offline auf Ihrem Rechner durchführen 
können. Laden Sie sich dazu auf der Seite zur C-Programmieraufgabe 
(http://www.cn.hhu.de/lehre-und-abschlussarbeiten/aktuelle-lehrveranstaltungen/vorlesungen/pflichtmodul-informatik-ii/c-projekt-informatik-ii.html) 
die Datei 'offlinetest.tar.gz' herunter und entpacken Sie diese unter Linux, zum Beispiel mit dem Konsolenbefehl 'tar xvfz offlinetest.tar.gz'.

Stellen Sie sicher, dass Sie GCC und Valgrind installiert haben.
Kopieren Sie ihre C-Datei hhush.c in den Ordner 'offlinetest'.
Wechseln Sie auf der Konsole in das Verzeichnis 'offlinetest'.
Starten Sie den Offlinetest nun mit dem Konsolenbefehl ./offlinetest.sh im Terminal.


Durchführung
~~~~~~~~~~~~

Wir überprüfen folgende Eigenschaften ihres Programms:

- Ist GCC installiert?
- Ist Valgrind installiert?
- Ist die Datei hhush.c vorhanden?
- Lässt sich das Programm kompilieren?
- Lassen sich die einzelnen Befehle Ihrer Shell korrekt ausführen?
- Gibt es Speicherlecks oder andere Fehler, die durch Valgrind gefunden werden?

Die Ergebnisse der Tests werden auf der Konsole mitgeteilt. Die Ausgaben 
Ihres Programms werden zusätzlich im Ordner 'output' gespeichert.

Wenn Ihr Programm die Tests besteht, bekommen Sie das durch die Meldung 
"Conclusion: 0 out of N tests failed" mitgeteilt, wobei N die Anzahl der 
automatischen Tests darstellt.

Wenn Ihr Programm mindestens einen Test nicht besteht, sollten Sie in der Konsolenausgabe 
nach "Test failed!" suchen und sich die jeweilige Ausgabe Ihres Programms, sowie die zum 
Test zugehörige Valgrindausgabe, genauer ansehen. Offenbar stimmt Ihre Ausgabe nicht mit 
der von uns gewünschten Ausgabe überein.

Des Weiteren sollte der gesamte Test nicht länger als zwei Minuten brauchen um zu terminieren. 
Sollte der Test bei Ihnen länger laufen, so ist das ein Indiz für eine Endlosschleife 
(zum Beispiel weil der Exit-Befehl nicht richtig implementiert wurde). Dann müssen Sie 
den Test eigenständig mit STRG+C abbrechen. Vor einem erneuten Start sollten Sie dann 
mit Hilfe des zuletzt ausgeführten Tests die Endlosschleife finden und entfernen.

Besonderheiten
~~~~~~~~~~~~~~

Bedenken Sie, dass ein erfolgreiches Bestehen der Offlinetests nicht automatisch zum Bestehen 
der C-Programmieraufgabe ausreicht. Zum endgültigen Bestehen muss Ihr Programm den Onlinetest 
bestehen, der voraussichtlich ab Mitte Juni zur Verfügung steht. Außerdem müssen Sie ihr 
Programm in einem Kolloquium erklären können.

Achten Sie auch darauf, dass eventuelle Änderungen an den Tests vorgenommen werden und auf 
der Website zur C-Programmieraufgabe (URL: siehe oben) bereitgestellt werden. Fragen Zur 
C-Programmieraufgabe oder zu den automatischen Tests können Sie an info2(at)cs.uni-duesseldorf.de 
richten.


