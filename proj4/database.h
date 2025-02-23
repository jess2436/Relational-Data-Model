/*
File: database.h
Authors: Jessica Chen & Shadiya Akhter
CSC 713 | George Ferguson | Project 4
Last Modified: 11/15/24
*/

#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

typedef struct {
  int PId;
  char Name[50];
  char City[50];
  char Zip[10];
} PNCZ;

typedef struct {
  char Race[50];
  int PId;
  char Time[10];
} RPT;

typedef struct {
  char Race[50];
  char Date[15];
} RD;

typedef struct {
  char Race[50];
  char Sponsor[50];
} RS;

typedef struct {
  char Race[50];
  char Qualifier[50];
} RQ;

typedef struct HashNode {
  void *data;
  struct HashNode *next;
} HashNode;

typedef struct {
  HashNode *buckets[TABLE_SIZE];
} HashTable;

HashTable *createHashTable();
void insert(HashTable *table, char *key, void *data);
void *lookup(HashTable *table, char *key, int (*cmp)(void *, void *), void *query);
void delete(HashTable *table, char *key, int (*cmp)(void *, void *), void *query);

// PART 1
// insert functions
void insert_PNCZ(HashTable *table, int PId, char *Name, char *City, char *Zip);
void insert_RPT(HashTable *table, char *Race, int PId, char *Time);
void insert_RD(HashTable *table, char *Race, char *Date);
void insert_RS(HashTable *table, char *Race, char *Sponsor);
void insert_RQ(HashTable *table, char *Race, char *Qualifier);

// delete functions
void delete_PNCZ(HashTable *table, int PId, char *Name, char *City, char *Zip);
void delete_RPT(HashTable *table, char *Race, int PId, char *Time);
void delete_RD(HashTable *table, char *Race, char *Date);
void delete_RS(HashTable *table, char *Race, char *Sponsor);
void delete_RQ(HashTable *table, char *Race, char *Qualifier);

// lookup functions
PNCZ *lookup_PNCZ(HashTable *table, int PId, char *Name, char *City, char *Zip);
RPT *lookup_RPT(HashTable *table, char *Race, int PId, char *Time);
RD *lookup_RD(HashTable *table, char *Race, char *Date);
RS *lookup_RS(HashTable *table, char *Race, char *Sponsor);
RQ *lookup_RQ(HashTable *table, char *Race, char *Qualifier);

// PART 2
// function to find time by name and race
void findRaceTimeByNameAndRace(HashTable *pncTable, HashTable *rptTable, char *name, char *race);
void replForRaceTimeQuery(HashTable *pncTable, HashTable *rptTable);
// function to find date(s) by name and sponsor
void findRaceDatesByParticipantAndSponsor(HashTable *pncTable, HashTable *rptTable, HashTable *rsTable, HashTable *rdTable, char *name, char *sponsor);
void replForRaceDateQuery(HashTable *pncTable, HashTable *rptTable, HashTable *rsTable, HashTable *rdTable);

// PART 3
// function to select tuples with "GVP Classic" from RPT relation
HashTable *select_RPT_Race(HashTable *rptTable, const char *race);
void printSelectionResult(HashTable *table, const char *description);
// function to project the PId of tuples with "GPT Classic" from RPT relation
HashTable *project_PId_from_selected_RPT(HashTable *rptTable, const char *race);
void printProjectionResult(HashTable *table, const char *description);
// function to join relation RD and RS
HashTable *join_RD_RS(HashTable *rdTable, HashTable *rsTable);
void print_RD_RS_JoinedTable(HashTable *table);
// structure to store resulting relation of RD ▷◁ RS
typedef struct {
    char Race[50];
    char Date[15];
    char Sponsor[50];
} RD_RS_Joined;
// function to project attribute Sponsor from the joint RD & RS relation with the Date "2023-10-02"
HashTable *select_by_date_from_joined_RD_RS(HashTable *joinedTable, const char *date);
HashTable *project_Sponsor_from_selected_joined_RD_RS(HashTable *selectedTable);
void printProjectedSponsorResult(HashTable *table, const char *description);

void populateTables(HashTable *pncTable, HashTable *rptTable, HashTable *rdTable, HashTable *rsTable, HashTable *rqTable);

void printPNCZTable(HashTable *table);
void printRPTTable(HashTable *table);
void printRDTable(HashTable *table);
void printRSTable(HashTable *table);
void printRQTable(HashTable *table);
void printPNCZEntry(PNCZ *entry);

unsigned int hash(char *key);
void freeHashTable(HashTable *table);

#endif
