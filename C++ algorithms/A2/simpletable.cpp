/*********************************************************/
/*                                                       */
/*   Written by: Catherine Leung                         */
/*   Version 1.0                                         */
/*                                                       */
/*                                                       */
/*   A reminder that all template code must be placed in */
/*   header file. You will have trouble compiling and    */
/*   linking otherwise.                                  */
/*                                                       */
/*   This program uses a text file with a list of        */
/*   words and creates the tables using the words as     */
/*   keys.                                               */
/*   You will need the file: dictionary.txt (in repo)    */
/*                                                       */
/*                                                       */
/*   To compile this program:                            */
/*          g++ simpletable.cpp timer.cpp -std=c++0x     */
/*                                                       */
/*********************************************************/

#include "table.h"
#include "timer.h"
#include <string>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;
const int NUMTESTS=13;
const int MAX=1000;
const int  NUMOVP= 200;   //number of words to overpopulate tables with


const char* filename="dictionary.txt";

//A struct to simplify error recording process.
struct Error{
  char msg_[150];
  int passed_;
  Error(const char* msg, int p){
    strcpy(msg_,msg);
    passed_=p;
  }
  void set(const char* msg, int p){
    strcpy(msg_,msg);
    passed_=p;
  }
};


void cleanWordList(char* wordlist[]);
void setData(int data[],int sz);
void printTiming(double timing[],int numrec, int numover);

int main(int argc, char* argv[]){
  int simpleMax=MAX;
  int simpleOver=NUMOVP;

  FILE* fp=fopen(filename,"r");

  int testnumber=0;
  if(!fp){
    cout << "could not open file " << filename << ".  Make sure it is in the" << endl;
    cout << "same directory as this program and try again" << endl;
    return 0;
  }
  char curr[50];          //used to read in words from text file
  string* wordlist;        //list of test cases. 

  bool oprc;              //return value from an operation

  int reccount;     //the count value in the record.  the ones that were removed and added should 
                    //be 1 the ones that were never removed should be 2
  
  int numremoved=0;   //number of words removed from the table;

  int* data;
  int currValue;
  Timer t;
  double timing[10];
  try{
    data=new int[MAX+NUMOVP];
    wordlist= new string[MAX+NUMOVP];
    for(int i=0;i<MAX+NUMOVP;i++){
        fscanf(fp,"%s\n",curr);                 //read in a word from data file
        wordlist[i]=curr;                       //record it
    }/*end for:  populate table*/
    setData(data,MAX+NUMOVP);
    cout << "* * * * * * * * * * * * Testing Simple Table* * * * * * * * * * * * * * * *"<< endl;
    cout << "Test 1: initialization and update" << endl;
    SimpleTable<int> simple(simpleMax);
    t.reset();

    for(int i=0;i<simpleMax;i++){
        t.start();
        oprc=simple.update(wordlist[i],data[i]);
        t.stop();

        if(!oprc){                              //check return code (should be true)
            throw Error("simpletable::update is returning false when table is not full",testnumber);
        }
    }/*end for:  populate table*/
    timing[0]=t.currtime();
    testnumber++;
 


    cout << "Test 2: Add some more records (beyond original max for table)" << endl;
    t.reset();
    for(int i=0;i<simpleOver;i++){
      
        t.start();
        oprc=simple.update(wordlist[simpleMax+i],data[simpleMax+i]);              
        t.stop();
        if(!oprc){                              //check return code (should be false)
            throw Error("SimpleTable::update is returning false.  table should expand with data",testnumber);
        }
    }/*end*/
    timing[1]=t.currtime();
    testnumber++;
    cout << "Test 3: Checking SimpleTable::find() and that records were properly recorded..." << endl;
    t.reset();
    for(int i=0;i<simpleMax+simpleOver;i++){
        int currValue;
        t.start();
        oprc=simple.find(wordlist[i],currValue);
        t.stop();                
        if(!oprc){                                          //check return code (should be true)
            throw Error("simple::find is returning false for a word that should be in the table",testnumber);
        }
        if(currValue!=data[i]){
            cout << "search key:" << wordlist[i] << endl;
            cout << "correct value: "<< data[i] << endl;
            cout << "your record's value: " << currValue << endl;
            throw Error("Simple::find is not passing back the correct data",testnumber);
        }
    }
    timing[2]=t.currtime();
    testnumber++;
    
    cout << "Test 4: Change some records in Simple table and see if it still works" << endl;
    t.reset();   
    for(int i=0;i<simpleMax+simpleOver;i++){
        data[i]+=1;
        t.start();                                              //modify the data (but key is same)
        oprc=simple.update(wordlist[i],data[i]);                          //put it back in the table
        t.stop();
        if(!oprc){
            throw Error("Simple::update is returning false for updating an existing record",testnumber);
        }
    }
    timing[3]=t.currtime();

    testnumber++;
    cout << "Test 5: Checking simple::find() and whether or not records were properly updated..." << endl;
    t.reset();   
    for(int i=0;i<simpleMax+simpleOver;i++){
        t.start();
        oprc=simple.find(wordlist[i],currValue);                //search the linear probing table
        t.stop();
        if(!oprc){                                          //check return code (should be true)
            throw Error("simple::find is returning false for a word that should be in the table",testnumber);
        }
        if(data[i]!=currValue){
            cout << "search key:" << wordlist[i] << endl;
            cout << "correct value: "<< data[i] << endl;
            cout << "your record's value: " << currValue << endl;
            throw Error("Simple::find is not passing back the correct data",testnumber);
        }
    }
    timing[2]+=t.currtime();
    testnumber++;
    cout << "Test 6: Test copy constructor" << endl;
    t.reset();
    t.start();
    SimpleTable<int> simpleCopy=simple;
    t.stop();
    timing[4]=t.currtime();
    t.reset();
    for(int i=0;i<simpleMax+simpleOver;i++){
        t.start();
        oprc=simpleCopy.find(wordlist[i],currValue);                //search the linear probing table
        t.stop();
        if(!oprc){                                          //check return code (should be true)
            throw Error("simpleCopy.find is returning false for a word that should be in the table",testnumber);
        }
        if(data[i]!=currValue){
            cout << "search key:" << wordlist[i] << endl;
            cout << "correct value: "<< data[i] << endl;
            cout << "your record's value: " << currValue << endl;
            throw Error("SimpleCopy.find is not passing back the correct data",testnumber);
        }
    }
    timing[2]+=t.currtime();
    testnumber++;

    cout << "Test 7: Remove some records that exist from simple table..." << endl;
    numremoved=0;
    t.reset(); 
    for(int i=0;i<simpleMax+simpleOver;i+=5){
        t.start();
        oprc=simple.remove(wordlist[i]);                          //put it back in the table
        t.stop();
        numremoved++;
        if(!oprc){
            throw Error("simple::remove is returning false for removing an existing record",testnumber);
        }
    }
    timing[5]=t.currtime();
    testnumber++;
    cout << "Test 8: Check that removed records are gone and other are unaffected in simple table..." << endl;
    t.reset();
    for(int i=0;i<simpleMax;i++){
        t.start();
        oprc=simple.find(wordlist[i],currValue);                          //put it back in the table
        t.stop();
        if(i%5){
            if(!oprc){
                throw Error("simple::find() could not find record that was not removed",testnumber);
            }
            if(data[i]!=currValue){
                cout << "search key:" << wordlist[i] << endl;
                cout << "correct value: "<< data[i] << endl;
                cout << "your record's value: " << currValue << endl;
                throw Error("Simple::find is not passing back the correct data",testnumber);
            } 
        }
        else{
            if(oprc){
                throw Error("simple::find() found a record that was removed",testnumber);
            }
        }
    }
    testnumber++;
    timing[2]+=t.currtime();

    cout << "Test 9: Remove some records that do not exist from the table" << endl;
    for(int i=0;i<simpleMax+simpleOver;i+=5){
        t.start();
        oprc=simple.remove(wordlist[i]);                          //put it back in the table
        t.stop();
        if(oprc){
            throw Error("simple::remove is returning true for removing non-existing record",testnumber);
        }
    }
    timing[6]=t.currtime();
    testnumber++;

    cout << "Test 10: Checking simpleCopy to see that it was unaffected by removing from simple" << endl;
    t.reset();
    for(int i=0;i<simpleMax+simpleOver;i++){
        t.start();
        oprc=simpleCopy.find(wordlist[i],currValue);                //search the linear probing table
        t.stop();
        if(!oprc){                                          //check return code (should be true)
            throw Error("simpleCopy.find is returning false for a word that should be in the table",testnumber);
        }
        if(data[i]!=currValue){
            cout << "search key:" << wordlist[i] << endl;
            cout << "correct value: "<< data[i] << endl;
            cout << "your record's value: " << currValue << endl;
            throw Error("SimpleCopy.find is not passing back the correct data",testnumber);
        }
    }
    timing[2]+=t.currtime();
    testnumber++;

    cout << "Test 11: Test assignment operator" << endl;
    t.reset();
    t.start();
    simpleCopy = simple;
    t.stop();
    timing[7]=t.currtime();
    t.reset();
    for(int i=0;i<simpleMax;i++){
        t.start();
        oprc=simpleCopy.find(wordlist[i],currValue);                          //put it back in the table
        t.stop();
        if(i%5){
            if(!oprc){
                throw Error("table made with = operator did not find word that should have been in table",testnumber);
            }
            if(data[i]!=currValue){
                cout << "search key:" << wordlist[i] << endl;
                cout << "correct value: "<< data[i] << endl;
                cout << "your record's value: " << currValue << endl;
                throw Error("SimpleCopy.find() is not passing back the correct data",testnumber);
            } 
        }
        else{
            if(oprc){
                throw Error("simplecopy.find() found a record that should not be in table",testnumber);
            }
        }
    }
    timing[2]+=t.currtime();
    t.reset();
    testnumber++;

    cout << "Test 12: Add the removed items back to simple table..." << endl;
    for(int i=0;i<simpleMax+simpleOver;i+=5){
        data[i]-=1;
        oprc=simple.update(wordlist[i],data[i]);          //put it in the chaining table
        if(!oprc){                             //check return code (should be true)
            throw Error("simple::update is returning false when table is not full",testnumber);
        }
    }/*end for*/
    testnumber++;


    cout << "Test 13: Checking that everything in simple was properly updated adding adding removed records back" << endl;
    t.reset();
    for(int i=0;i<simpleMax+simpleOver;i++){
        t.start();
        oprc=simple.find(wordlist[i],currValue);                        
        t.stop();
        if(!oprc){
            throw Error("simple::find() could not find record that should be there",testnumber);
        }
        if(data[i]!=currValue){
            cout << "search key:" << wordlist[i] << endl;
            cout << "correct value: "<< data[i] << endl;
            cout << "your record's value: " << currValue << endl;
            throw Error("Simple::find is not passing back the correct data",testnumber);
        } 
    }
    testnumber++;
    timing[2]+=t.currtime();
    cout << testnumber << " out of " << NUMTESTS << " tests passed." << endl;

    if(testnumber == NUMTESTS){
      cout << "Congratulations, looks like your assignment is completed" << endl;
    }
    else{
      cout << "Looks like your assigment still needs some work." << endl;
    }
    cout << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *"<< endl;
    cout << endl;
    cout << "Timing summary: Simple Table" << endl;
    printTiming(timing,simpleMax, simpleOver);
    cout << endl;
    cout << endl;
    cout << endl;

    fclose(fp);
  }
  catch(Error e){
    cout << e.msg_ << endl;
    cout << e.passed_ << " tests of "<< NUMTESTS <<" passed" << endl;
  }
}


void printTiming(double timing[],int numrec, int numover){
  cout << endl << endl;
  cout << "Number of records: " << numrec << endl;
  cout << "Number over initial size: " << numover << endl << endl;
  cout << "     Operation                        |    Time     " << endl;
  cout << "--------------------------------------|-------------" << endl;
  cout << " update, new record                   |  " << timing[0] << endl;
  cout << " update, new rec, over initial size   |  " << timing[1] << endl;
  cout << " update, modify record                |  " << timing[3] << endl;
  cout << " find                                 |  " << timing[2] << endl;
  cout << " remove  (item in list)               |  " << timing[5] << endl;
  cout << " remove  (item not in list)           |  " << timing[6] << endl;
  cout << " copy constructor                     |  " << timing[4] << endl;
  cout << " assignment operator                  |  " << timing[7] << endl;


}

void setData(int data[],int sz){
  for(int i=0;i<sz;i++){
    data[i]=rand()%10;
  }
}

