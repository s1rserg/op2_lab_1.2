#ifndef MY_MODULE_H
#define MY_MODULE_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <limits>

using namespace std;
#define MAX 85 // normal output limit

struct Time {
    int hour;
    int min;
};

struct Client {
    char name[MAX];
    Time start;
    Time end;
};

int isExist(string &name);

void inputFile(string &name);

int outputFile(string &name);

int validInput(string &name, Client &client);

void processFile(string &name, string &out_name);

#endif
