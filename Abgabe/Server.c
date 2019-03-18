#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <fstream>

#define BUFLEN 512
#define PORT 9930

using namespace std;

int main(void)
{
  struct sockaddr_in si_me={0};
  struct sockaddr_in si_other={0};
  int s, slen=sizeof(si_other), numOfStepbacks = 0;
  char buf[BUFLEN],bufCom[3];
  string buffer,buffer2,bufferCode, bufferCommand;
  string CURRENT_PATH = "repo/";
  ifstream stream;

  s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(PORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(s, (sockaddr*)&si_me, sizeof(si_me));
  system("clear");
  while (true)
  {
    recvfrom(s, bufCom, 3, 0, (sockaddr *)&si_other, (socklen_t*)&slen);
    bufferCommand = bufCom;
    bufferCommand.resize(3); //Benötigt weil buffer am ende immer ein 'v' setzt, keine Ahnung warum
    cout << "Received Command: " <<bufferCommand << endl;
    if(bufferCommand == "mdr"){ //Ordner erstellen
        bufferCode="200"; //Code zum zurücksenden
        sendto(s, bufferCode.c_str(), bufferCode.length(), 0, (sockaddr *)&si_other, slen); //Code senden (Server sagt das er da ist)
        memset(buf, 0, sizeof(buf)); //Buffer clearen
        recvfrom(s, buf, BUFLEN, 0, (sockaddr *)&si_other, (socklen_t*)&slen); //Buffer erhalten (Verzeichnisname)
        buffer = buf; //C-String -> string
        buffer = "mkdir " + CURRENT_PATH + buffer; //Befehl zusammenstellen
        if(system(buffer.c_str()) == 0)bufferCode = "200"; //Befehl ausführen und prüfen ob es funktionierte
        else bufferCode = "400";
        sendto(s, bufferCode.c_str(), buffer.length(), 0, (sockaddr *)&si_other, slen); //Schicken ob es funktionierte oder nicht
    }else if(bufferCommand == "cdr"){ //Verzeichnispfad ändern
        bufferCode="200";
        sendto(s, bufferCode.c_str(), bufferCode.length(), 0, (sockaddr *)&si_other, slen);
        memset(buf, 0, sizeof(buf));
        recvfrom(s, buf, BUFLEN, 0, (sockaddr *)&si_other, (socklen_t*)&slen);
        buffer = buf;
        if(buffer == ".."){
                if(numOfStepbacks > 0){
                        CURRENT_PATH = CURRENT_PATH + buffer + "/";
                        numOfStepbacks--;
                }else bufferCode = "400";
        }else {
                CURRENT_PATH = CURRENT_PATH + buffer + "/";
                numOfStepbacks++;
        }
        sendto(s, bufferCode.c_str(), bufferCode.length(), 0, (sockaddr *)&si_other, slen);
    }else if(bufferCommand == "idr"){ //Verzeichnispfad zurücksetzen
        CURRENT_PATH = "repo/";
        numOfStepbacks = 0;
    }else if(bufferCommand == "del"){ //Datei löschen
        bufferCode="200";
        sendto(s, bufferCode.c_str(), bufferCode.length(), 0, (sockaddr *)&si_other, slen);
        memset(buf, 0, sizeof(buf));
        recvfrom(s, buf, BUFLEN, 0, (sockaddr *)&si_other, (socklen_t*)&slen);
        buffer = buf;
        buffer = "rm " + CURRENT_PATH + buffer;
        if(system(buffer.c_str()) == 0)bufferCode = "200";
        else bufferCode = "400";
        sendto(s, bufferCode.c_str(), bufferCode.length(), 0, (sockaddr *)&si_other, slen);
    }else if(bufferCommand == "mve"){ //Datei umbenennen oder verschieben
        bufferCode = "200";
        sendto(s, bufferCode.c_str(), bufferCode.length(), 0, (sockaddr *)&si_other, slen);
        memset(buf, 0, sizeof(buf));
        recvfrom(s, buf, BUFLEN, 0, (sockaddr *)&si_other, (socklen_t*)&slen);
        buffer = buf;
        sendto(s, bufferCode.c_str(), bufferCode.length(), 0, (sockaddr *)&si_other, slen);
        memset(buf, 0, sizeof(buf));
        recvfrom(s, buf, BUFLEN, 0, (sockaddr *)&si_other, (socklen_t*)&slen);
        buffer = "mv " + CURRENT_PATH + buffer + " " + CURRENT_PATH + buf;
        if(system(buffer.c_str()) != 0)bufferCode = "400";
        sendto(s, bufferCode.c_str(), bufferCode.length(), 0, (sockaddr *)&si_other, slen);
    }else if(bufferCommand == "dir"){ //Funktioniert nicht
        bufferCode="200";
        sendto(s, bufferCode.c_str(), bufferCode.length(), 0, (sockaddr *)&si_other, slen);
        memset(buf, 0, sizeof(buf));
        recvfrom(s, buf, BUFLEN, 0, (sockaddr *)&si_other, (socklen_t*)&slen);
        buffer = "ls " + CURRENT_PATH + " > .dir";
        system(buffer.c_str());
        stream.open(".dir");

        stream >> buf;

        sendto(s, buf, BUFLEN, 0, (sockaddr *)&si_other, slen);
    }else if(bufferCommand == "wrt"){ //Datei beschreiben
        bufferCode="200";
        sendto(s, bufferCode.c_str(), bufferCode.length(), 0, (sockaddr *)&si_other, slen);
        memset(buf, 0, sizeof(buf));
        recvfrom(s, buf, BUFLEN, 0, (sockaddr *)&si_other, (socklen_t*)&slen);
        buffer2 = buf;
        sendto(s, bufferCode.c_str(), bufferCode.length(), 0, (sockaddr *)&si_other, slen);
        memset(buf, 0, sizeof(buf));
        recvfrom(s, buf, BUFLEN, 0, (sockaddr *)&si_other, (socklen_t*)&slen);
        buffer = buf;
        buffer = "touch " + CURRENT_PATH + buffer;
        if(system(buffer.c_str()) == 0){
                buffer2 = "echo " + buffer2 + " >> " + CURRENT_PATH + buf;
                if(system(buffer2.c_str()) != 0) bufferCode = "400";
        }else bufferCode = "402";
        sendto(s, bufferCode.c_str(), bufferCode.length(), 0, (sockaddr *)&si_other, slen);
    }else{
        bufferCode="400";
        sendto(s, bufferCode.c_str(), bufferCode.length(), 0, (sockaddr *)&si_other, slen);
    }
  }
  close(s);
  return 0;
}
