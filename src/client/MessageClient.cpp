#include "MessageGui.cpp"
#include "../server/Library.hpp"
#include "../cpp/client/messagelibrarystub.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Check_Browser.H>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <stdlib.h>
#include <sstream>
#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace jsonrpc;
using namespace std;

/**
 * Copyright (c) 2019 Tim Lindquist,
 * Software Engineering,
 * Arizona State University at the Polytechnic campus
 * <p/>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Purpose: C++ FLTK client UI for Message management.
 * This class extends the Gui component class MessageGui and demonstrates
 * sample control functions that respond to button clicks drop-down selects.
 * This software is meant to run on Linux and MacOS using g++.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 *                       IAFSE, ASU at the Polytechnic campus
 * @file    samplemain.cpp
 * @date    January, 2019
 **/

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
class MessageClient : public MessageGui {

  

   /** ClickedX is one of the callbacks for GUI controls.
    * Callbacks need to be static functions. But, static functions
    * cannot directly access instance data. This program uses "userdata"
    * to get around that by passing the instance to the callback
    * function. The callback then accesses whatever GUI control object
    * that it needs for implementing its functionality.
    */
   static void ClickedX(Fl_Widget * w, void * userdata) {
      std::cout << "You clicked Exit" << std::endl;
      exit(1);
   }

  static void ClickedGet(Fl_Widget * w, void * userdata) {
    MessageClient* anInstance = (MessageClient*)userdata;
    string hostName= anInstance->hostName;
    HttpClient httpclient(hostName);
    messagelibrarystub sc(httpclient); 
     std::cout << "You clicked Get" << std::endl;
      Fl_Multiline_Input * messageFMI = anInstance->messageFMI;
       Fl_Input * dateFI = anInstance->dateFI;
       Fl_Input * subjectFI = anInstance->subjectFI;
       Fl_Input * toFI = anInstance->toFI;
       Fl_Input * fromFI = anInstance->fromFI;
       Fl_Check_Browser * headers = anInstance->headerFCB;
       headers->clear();
      
       auto aClk = std::chrono::system_clock::now();
       auto aTime = std::chrono::system_clock::to_time_t(aClk);
        std::string aDateStr(ctime(&aTime));
      
       std::string trimmed(trimMe(aDateStr));
      dateFI->value(ctime(&aTime));
       dateFI->value(trimmed.c_str());
     
      std::string userId = anInstance->userId;
      int size = sc.getLibrarySize();
      for(int i=1; i<size+1; i++){
	headers->add((sc.getTo(to_string(i))+"     "+sc.getDate(to_string(i))).c_str());
      }
   
     headers->checked(1,1); 
      toFI->value(sc.getTo("1").c_str());
      fromFI->value(sc.getFrom("1").c_str());
      dateFI->value(sc.getDate("1").c_str());
      subjectFI->value(sc.getSubject("1").c_str());
      messageFMI->value(sc.getMessage("1").c_str());  
  }

   static void ClickedDelete(Fl_Widget * w, void * userdata) {
      MessageClient* anInstance = (MessageClient*)userdata;
      string hostName= anInstance->hostName;
      Fl_Check_Browser * headers = anInstance->headerFCB;
      HttpClient httpclient(hostName);
      messagelibrarystub sc(httpclient);
     
     sc.remove(headers->value());
     
      headers->clear();
      int size = sc.getLibrarySize();
      for(int i=1; i<size+1; i++){
	headers->add((sc.getTo(to_string(i))+"     "+sc.getDate(to_string(i))).c_str());
      }
     
      headers->checked(1,1);
     
     std::cout << "You clicked Delete" << std::endl;
   }
   
   static std::string& trimMe (std::string& str) {
      // right trim
      while (str.length() > 0 && (str[str.length()-1] == ' ' || str[str.length()-1] == '\t'
                                  || str[str.length()-1] == '\n' ||  str[str.length()-1] == '\r')){
         str.erase(str.length ()-1, 1);
      }
      // left trim
      while (str.length () > 0 && (str[0] == ' ' || str[0] == '\t')){
         str.erase(0, 1);
      }
      return str;
   }

  static void ClickedReply(Fl_Widget * w, void * userdata) {
    cout<<"Clicked Reply"<<endl;
    MessageClient* anInstance = (MessageClient*)userdata;
      Fl_Check_Browser * headers = anInstance->headerFCB;


      Fl_Multiline_Input * messageFMI = anInstance->messageFMI;
     Fl_Input * dateFI = anInstance->dateFI;
     Fl_Input * subjectFI = anInstance->subjectFI;
     Fl_Input * toFI = anInstance->toFI;
     Fl_Input * fromFI = anInstance->fromFI;

    string to, from, subject, message,date;
    to=toFI->value();
     from=fromFI->value();
     subject=subjectFI->value();
     auto aClk = std::chrono::system_clock::now();
     auto aTime = std::chrono::system_clock::to_time_t(aClk);
     std::string aDateStr(ctime(&aTime));
     std::string trimmed(trimMe(aDateStr));
     
      dateFI->value(trimmed.c_str());
      message=messageFMI->value();
      string re="Re:" , newline="\n\n\n";

      toFI->value(from.c_str());
      fromFI->value(to.c_str());
      subjectFI->value((re+subject).c_str());
      messageFMI->value((newline+message).c_str());
      
  }

  static void ClickedExport(Fl_Widget * w, void * userdata) {
   
    cout<<"You clicked export"<<endl;

  }

   static void SelectedHeader(Fl_Widget * w, void * userdata) {
      MessageClient* anInstance = (MessageClient*)userdata;
      string hostName= anInstance->hostName;
      Fl_Check_Browser * headers = anInstance->headerFCB;
      HttpClient httpclient(hostName);
      messagelibrarystub sc(httpclient);

       Fl_Multiline_Input * messageFMI = anInstance->messageFMI;
       Fl_Input * dateFI = anInstance->dateFI;
       Fl_Input * subjectFI = anInstance->subjectFI;
       Fl_Input * toFI = anInstance->toFI;
       Fl_Input * fromFI = anInstance->fromFI;
       string value = to_string(headers->value());
        toFI->value(sc.getTo(value).c_str());
	fromFI->value(sc.getFrom(value).c_str());
	dateFI->value(sc.getDate(value).c_str());
	subjectFI->value(sc.getSubject(value).c_str());
        messageFMI->value(sc.getMessage(value).c_str());
       
     int howMany = headers->nitems();
     int which = headers->value();
    
       for(int i=1; i<howMany+1; i++){
         if(i != which ){
          headers->checked(i,0);
	   
        }
        }
           
   }

  static void fillMenu(void * userdata){
      MessageClient* anInstance = (MessageClient*)userdata;
       string hostName= anInstance->hostName;
    HttpClient httpclient(hostName);
    messagelibrarystub sc(httpclient); 
      Fl_Check_Browser * headers = anInstance->headerFCB;
      int size = sc.getLibrarySize();
      for(int i=1; i<size+1; i++){
	headers->add((sc.getTo(to_string(i))+"     "+sc.getDate(to_string(i))).c_str());
      }
   
     headers->checked(1,1); 
     Fl_Multiline_Input * messageFMI = anInstance->messageFMI;
      Fl_Input * dateFI = anInstance->dateFI;
      Fl_Input * subjectFI = anInstance->subjectFI;
     Fl_Input * toFI = anInstance->toFI;
      Fl_Input * fromFI = anInstance->fromFI;
      toFI->value(sc.getTo("1").c_str());
      fromFI->value(sc.getFrom("1").c_str());
      dateFI->value(sc.getDate("1").c_str());
      subjectFI->value(sc.getSubject("1").c_str());
      messageFMI->value(sc.getMessage("1").c_str());
      
  }

  static void ClickedSend(Fl_Widget * w, void * userdata){
      cout<<"You clicked send"<<endl;
    MessageClient* anInstance = (MessageClient*)userdata;
       string hostName= anInstance->hostName;
    HttpClient httpclient(hostName);
    messagelibrarystub sc(httpclient); 
    
    
     Fl_Check_Browser * headers = anInstance->headerFCB;
      Fl_Multiline_Input * messageFMI = anInstance->messageFMI;
     Fl_Input * dateFI = anInstance->dateFI;
     Fl_Input * subjectFI = anInstance->subjectFI;
     Fl_Input * toFI = anInstance->toFI;
      Fl_Input * fromFI = anInstance->fromFI;
      string to, from, subject, message,date;
        to=toFI->value();
       from=fromFI->value();
       subject=subjectFI->value();
      auto aClk = std::chrono::system_clock::now();
      auto aTime = std::chrono::system_clock::to_time_t(aClk);
      std::string aDateStr(ctime(&aTime));
      std::string trimmed(trimMe(aDateStr));
     
     dateFI->value(trimmed.c_str());
    message=messageFMI->value();
     sc.add(to,from, trimmed, subject, message);
            
  }


public:
   std::string userId;
   std::string hostName;
 
  MessageClient(const char * name = 0, string host=0) : MessageGui(name) {
    hostName=host;
    getButt->callback(ClickedGet, (void*)this);
      deleteButt->callback(ClickedDelete, (void*)this);
      replyButt->callback(ClickedReply, (void*)this);
      sendButt->callback(ClickedSend, (void*)this);
      exportButt->callback(ClickedExport, (void*)this);
      headerFCB->callback(SelectedHeader, (void*)this);
      headerFCB->when(FL_WHEN_CHANGED);
      fillMenu((void*)this);
      
      callback(ClickedX);

      userId = "jbee2";
      
   }

  
};

int main(int argc, char*argv[]) {
  string host = "http://127.0.0.1:8080";
  if(argc>1){
    host = string(argv[1]);
  }
  cout<<"Connecting to host "<<host<<endl;
  try{
    
    MessageClient cm("Justin Bee's Message Browser", host);

   return (Fl::run());
  }catch (JsonRpcException e) {
    cerr<<e.what()<<endl;
  }
  
}
