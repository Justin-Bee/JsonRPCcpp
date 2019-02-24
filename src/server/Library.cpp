#include "Library.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <json/json.h>

using namespace std;

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

Library::Library(){
}

void Library::initLibraryFromJsonFile(string fileName){
  
  Json::Reader reader;
  Json::Value root;
  std::ifstream json(fileName.c_str(), std::ifstream::binary);
 bool parseSuccess = reader.parse(json,root,false);
      Json::Value::Members mbr = root.getMemberNames();
       for(vector<string>::const_iterator i = mbr.begin(); i!= mbr.end(); i++){
         Json::Value jsonUser = root[*i];
         string nameStr = "name";
         if(nameStr.compare(*i)==0){
            name = jsonUser.asString();
         }else{
            string toAUser = jsonUser["to"].asString();
            string fromAUser = jsonUser["from"].asString();
	    string aSubject = jsonUser["subject"].asString();
	    string aDate = jsonUser["date"].asString();
	    string aMsg = jsonUser["message"].asString();
            Message * message = new Message(toAUser, fromAUser, aSubject,
					    aDate, aMsg);
            messages[*i] = *message;
	    cout<<messages[*i].toUser<<endl;
         }
      }     
}
      


void Library::toJsonFile(string fileName){
  Json::Value jsonLib;
  jsonLib["name"] = name;
  int count =1;
  for(std::map<string,Message>::iterator i = messages.begin();
      i!= messages.end(); i++){
    string key = i->first;
    Json::Value jsonUser;
    Message msg = messages[key];
    jsonUser["to"] = msg.toUser;
    jsonUser["from"] = msg.fromUser;
    jsonUser["subject"]=msg.subject;
    jsonUser["date"]=msg.date;
    jsonUser["message"]=msg.msgBody;
    jsonLib[to_string(count)] = jsonUser;
    count++;
  }
  Json::StyledStreamWriter ssw("  ");
  std::ofstream jsonOutFile(fileName.c_str(), std::ofstream::binary);
  ssw.write(jsonOutFile, jsonLib);

}

void Library::add(Message msg){
  string key = to_string(messages.size()+1);
  messages[key]=msg;

}

void Library::remove(int value){
  messages.erase(to_string(value));
}









