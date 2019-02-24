#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <json/json.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <csignal>

//#include <jsonrpccpp/server/connectors/tcpsocketserver.h>
#include "../cpp/server/messageserverstub.h"
#include "Library.hpp"
/*
 * Copyright 2019 Justin Bee,
 *
 * This software is the intellectual property of the author,
 * and can not be distributed, used, copied, or
 * reproduced, in whole or in part, for any purpose, commercial
 * or otherwise. The author grants the ASU
 * Software Engineering program the right to copy, execute,
 * and evaluate this work for the purpose of
 * determining performance of the author in coursework, and for
 * Software Engineering program evaluation,
 * so long as this copyright and right-to-use statement is kept
 * in-tact in such use.
 * All other uses are prohibited and reserved to the author.
 *
 * Purpose: A message class.
 *
 * Ser321 Principles of Distributed Software Systems
 * @author Justin Bee jbee2@asu.edu
 * @version February 2019
 */

using namespace jsonrpc;
using namespace std;

class MessageServer : public messageserverstub {
public:
  MessageServer(AbstractServerConnector &connector, int port);
  virtual std::string serviceInfo();
  virtual bool saveToJsonFile();
  virtual bool resetFromJsonFile();
  virtual bool add(const string& to, const string& from, const string& date,
		   const string& subject, const string& message);
  virtual bool remove(int value);
  // virtual Library getLibrary();
  virtual std::string getTo(const string& value);
  virtual std::string getFrom(const string& value);
  virtual std::string getDate(const string& value);
  virtual std::string getSubject(const string& value);
  virtual std::string getMessage(const string& value);
  virtual int getLibrarySize();
  
 
  
private:
  Library * library;
  int portNum;

};

MessageServer::MessageServer(AbstractServerConnector &connector, int port) :
                             messageserverstub(connector){
  library = new Library();
  library->initLibraryFromJsonFile("messages.json");
  portNum = port;
}

string MessageServer::serviceInfo(){
  std::string msg = "Message Library management service.";
  stringstream ms;
  ms<< portNum;
  cout<< "serviceInfo called. Returning: Message Library management service."<<endl;
  return msg.append(ms.str());
}

bool MessageServer::saveToJsonFile(){
  cout<<"saving message library to messages.json"<<endl;
  library->toJsonFile("messages.json");
  return true;
}

bool MessageServer::resetFromJsonFile(){
  cout<< "restoring collection from messages.json"<<endl;
  library->initLibraryFromJsonFile("messages.json");
  return true;
}

bool MessageServer::add(const string& to, const string& from,
			const string& date, const string& subject,
			const string& message) {
 cout<<"Adding message"<<endl;
 Message * m = new Message(to,from,subject,date,message);
  library = new Library();
  library->initLibraryFromJsonFile("messages.json");
  library->add(*m);
  library->toJsonFile("messages.json");
 return true;
}

bool MessageServer::remove(int value){
  cout<<"Removing message"<<endl;
   library = new Library();
   library->initLibraryFromJsonFile("messages.json");
  library->remove(value);
  library->toJsonFile("messages.json");
 return true;
}

int MessageServer::getLibrarySize(){
  int size=0;
  library = new Library();
  library->initLibraryFromJsonFile("messages.json");
   size = library->messages.size();
   cout<<"library size is: "<<size<<endl;
   return size;
}

std::string MessageServer::getTo(const string& value){
   string res = library->messages[value].toUser.c_str();
   return res;
}

std::string MessageServer::getFrom(const string& value){
   string res = library->messages[value].fromUser.c_str();
   return res;
}

std::string MessageServer::getDate(const string& value){
   string res = library->messages[value].date.c_str();
   return res;
}

std::string MessageServer::getSubject(const string& value){
   string res = library->messages[value].subject.c_str();
   return res;
}

std::string MessageServer::getMessage(const string& value){
   string res = library->messages[value].msgBody.c_str();
   return res;
}


void exiting(){
  std::cout<<"Server has been terminated. Exiting normally"<<endl;
  //ss.StopListening();
}

int main(int argc, char * argv[]) {
  int port = 8080;
  if(argc>1){
    port = atoi(argv[1]);
  }
  HttpServer httpserver(port);
  MessageServer ms(httpserver, port);
  std::atexit(exiting);
  auto ex = [] (int i) {cout<<"server terminating with signal"<<i<<endl;
			exit(0);
  };
  std::signal(SIGINT, ex);
  std::signal(SIGABRT, ex);
  std::signal(SIGTERM, ex);
  cout<<"Message library server listening on port"<< port<<
    "use ps to get pid. To quit: kill -9 pid "<<endl;
  ms.StartListening();
  while(true){
  }
  ms.StopListening();
  return 0;
}
