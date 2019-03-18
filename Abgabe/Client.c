#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <string.h> //für memset()

#define BUFLEN 512
#define PORT 9930

using namespace std;

void getHelp();

int main(void)
{
  struct sockaddr_in si_other={0};
  int s, slen=sizeof(si_other);
  char buf[BUFLEN],bufCode[3];
  string buffer,bufCom,bufferCode,res,SRV_IP;

  s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);

  cout << "IP of Server: ";
  getline(cin,SRV_IP);

  inet_aton(SRV_IP.c_str(), &si_other.sin_addr);
  while(true){
        cout << "<NOTR>-: ";
        getline(cin, bufCom);
        bufCom.resize(3); //Auf die ersten 3 Zeichen reduzieren
        if(bufCom == "hlp")getHelp(); //Hilfe suchen
        else if(bufCom == "clr")system("clear"); //Den Bildschirm wischen das nich dreckig is
        else if(bufCom == "bye")break; //Aus Programm aussteigen
        else if(bufCom == "mdr"){ //Ein neues Verzeichnis
                sendto(s, bufCom.c_str(), bufCom.length(), 0, (sockaddr *)&si_other, slen); //Befehl senden
                recv(s, bufCode, 3, 0); //Auf antwort warten
                bufferCode = bufCode; //Von C-String in string umwandeln
                if(bufferCode == "200"){ //Falls Nachricht erfolgreich erhalten wurde
                        cout << "Give directory a name: "; //Verzeichnisname eingeben
                        getline(cin,buffer);
                        if(buffer.find("..") == 0)buffer.clear(); //falls zwei Punkte gefunden werden wird der buffer gelöscht
                        sendto(s, buffer.c_str(), buffer.length(), 0, (sockaddr *)&si_other, slen); //Verzeichnisname senden
                        memset(bufCode, 0, sizeof(buf)); //buffer clearen
                        recv(s, bufCode, 3, 0); //Auf Antwort hören
                        bufferCode = bufCode; //Von C-String in string Umwandeln
                        if(bufferCode == "200")cout << "Directory created"; //Erfolgreich
                        else if(bufferCode == "400")cout << "Directory could not be created"; //Nicht erfolgreich
                        else cout << "Unexpected error"; //Unerwartetes ist passiert
                }else cout << "Connection Error"; //Verbindung unerfolgreich
        }else if(bufCom == "cdr"){ //Verzeichnispfad ändern
                sendto(s, bufCom.c_str(), bufCom.length(), 0, (sockaddr *)&si_other, slen);
                recv(s, bufCode, 3, 0);
                bufferCode = bufCode;
                if(bufferCode == "200"){
                        cout << "Directory name: ";
                        getline(cin,buffer);
                        sendto(s, buffer.c_str(), buffer.length(), 0, (sockaddr *)&si_other, slen);
                        memset(bufCode, 0, sizeof(buf));
                        recv(s, bufCode, 3, 0);
                        bufferCode = bufCode;
                        if(bufferCode == "200")cout << "Successfully moved";
                        else if (bufferCode == "400")cout << "Did not move";
                        else cout << "Unexpected error";
                }else cout << "Connection Error";
        }else if(bufCom == "idr"){ //Verzeichnispfad zurücksetzen
                sendto(s, bufCom.c_str(), bufCom.length(), 0, (sockaddr *)&si_other, slen);
        }else if(bufCom == "del"){ //Datei löschen
                sendto(s, bufCom.c_str(), bufCom.length(), 0, (sockaddr *)&si_other, slen);
                recv(s, bufCode, 3, 0);
                bufferCode = bufCode;
                if(bufferCode == "200"){
                        cout << "File name: ";
                        getline(cin,buffer);
                        sendto(s, buffer.c_str(), buffer.length(), 0, (sockaddr *)&si_other, slen);
                        memset(bufCode, 0, sizeof(buf));
                        recv(s, bufCode, 3, 0);
                        bufferCode = bufCode;
                        if(bufferCode == "200")cout << "File deleted";
                        else if (bufferCode == "404")cout << "File does not exist";
                        else cout << "File could not be deleted due to an unknown error";
                }else cout << "Connection Error";
        }else if(bufCom == "mve"){ //Datei Umbenennen oder verschieben
                sendto(s, bufCom.c_str(), bufCom.length(), 0, (sockaddr *)&si_other, slen);
                recv(s, bufCode, 3, 0);
                bufferCode = bufCode;
                if(bufferCode == "200"){
                        cout << "File name: ";
                        getline(cin,buffer);
                        sendto(s, buffer.c_str(), buffer.length(), 0, (sockaddr *)&si_other, slen);
                        memset(bufCode, 0, sizeof(buf));
                        recv(s, bufCode, 3, 0);
                        bufferCode = bufCode;
                        if(bufferCode == "200"){
                                cout << "Dir. or File Name: ";
                                getline(cin,buffer);
                                sendto(s, buffer.c_str(), buffer.length(), 0, (sockaddr *)&si_other, slen);
                                memset(bufCode, 0, sizeof(buf));
                                recv(s, bufCode, 3, 0);
                                bufferCode = bufCode;
                                if(bufferCode == "200")cout << "Move or Rename was Succesful";
                                else if(bufferCode == "400") cout << "Move or Rename was Unsuccessful";
                                else cout << "Unexpected Error";
                        }else cout << "Connection Error";
                }else cout << "Connection Error";
        }else if(bufCom == "dir"){ //Funktioniert nicht
                sendto(s, bufCom.c_str(), bufCom.length(), 0, (sockaddr *)&si_other, slen);
                recv(s, bufCode, 3, 0);
                bufferCode = bufCode;
                if(bufferCode == "200"){
                        sendto(s, bufCom.c_str(), bufCom.length(), 0, (sockaddr *)&si_other, slen);
                        recv(s, buf, BUFLEN, 0);
                        buffer = buf;
                        cout << buffer;
                }else cout << "Connection Error";
        }else if(bufCom == "wrt"){ //Datei beschreiben
                sendto(s, bufCom.c_str(), bufCom.length(), 0, (sockaddr *)&si_other, slen);
                recv(s, bufCode, 3, 0);
                bufferCode = bufCode;
                if(bufferCode == "200"){
                        cout << "Write here: ";
                        getline(cin,buffer);
                        sendto(s, buffer.c_str(), buffer.length(), 0, (sockaddr *)&si_other, slen);
                        recv(s, bufCode, 3, 0);
                        bufferCode = bufCode;
                        if(bufferCode == "200"){
                                cout << "Give the file a name: ";
                                getline(cin,buffer);
                                sendto(s, buffer.c_str(), buffer.length(), 0, (sockaddr *)&si_other, slen);
                                recv(s, bufCode, 3, 0);
                                bufferCode = bufCode;
                                if(bufferCode == "200") cout << "File created";
                                else if(bufferCode == "400") cout << "File created but not filled";
                                else if(bufferCode == "402") cout << "File already exists";
                                else cout << "Unexpected Error";
                        }else cout << "Connection Error";
                }else cout << "Connection Error";
        }else cout << "Not a Command, try \"hlp\"";
        cout << endl;
  }
  close(s);
  return 0;
}

void getHelp(){ //Hilfe Ausgabe
        cout << endl;
        cout << "hlp - shows this screen" << endl;
        cout << "clr - clears screen" << endl;
        cout << "bye - stops client" << endl;
        cout << "mdr - create a directory" << endl;
        cout << "cdr - move to a directory" << endl;
        cout << "idr - set path to initial directory" << endl;
        cout << "mve - move or rename a file";
        cout << "del - delete a file" << endl;
        cout << "dir - get the names of all items in current directory (doesn't work)" << endl;
        cout << "wrt - Write something into a file and give it a name (WIP)" << endl;
        cout << "--------------------------------------------------------------------------------------------------------------------------------------------------";
}
