/*
File: main.c
Authors: Jessica Chen & Shadiya Akhter
CSC 713 | George Ferguson | Project 4
Last Modified: 11/15/24
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

void printRPTEntry(RPT *entry) {
  if (entry) {
      printf("Found RPT entry: Race=%s, PId=%d, Time=%s\n", entry->Race, entry->PId, entry->Time);
  } else {
      printf("No matching RPT entry found.\n");
  }
}

void printPNCZEntry(PNCZ *entry) {
  if (entry) {
      printf("Found PNCZ entry: PId=%d, Name=%s, City=%s, Zip=%s\n", entry->PId, entry->Name, entry->City, entry->Zip);
  } else {
      printf("No matching PNCZ entry found.\n");
  }
}

void printRSEntry(RS *entry) {
  if (entry) {
      printf("Found RS entry: Race=%s, Sponsor=%s\n", entry->Race, entry->Sponsor);
  } else {
      printf("No matching RS entry found.\n");
  }
}

void printRQEntry(RQ *entry) {
  if (entry) {
      printf("Found RQ entry: Race=%s, Qualifier=%s\n", entry->Race, entry->Qualifier);
  } else {
      printf("No matching RQ entry found.\n");
  }
}

// cloning functions to create copies of relations
HashTable *cloneHashTable(HashTable *original, void *(*copyData)(void *));
void *copyPNCZ(void *data);
void *copyRPT(void *data);
void *copyRD(void *data);
void *copyRS(void *data);
void *copyRQ(void *data);

int main (int argc, char *argv[]){
  HashTable *pnczTable = createHashTable();
  HashTable *rptTable = createHashTable();
  HashTable *rdTable = createHashTable();
  HashTable *rsTable = createHashTable();
  HashTable *rqTable = createHashTable();

  printf("\nCSC173 Project 4 by Jessica Chen & Shadiya Akhter\n");

  populateTables(pnczTable, rptTable, rdTable, rsTable, rqTable);
   // testing - printing original relations (no changes made)
  printf("\nPRINTING ORIGINAL RELATION TABLES...\n");
  printPNCZTable(pnczTable);
  printRPTTable(rptTable);
  printRDTable(rdTable);
  printRSTable(rsTable);
  printRQTable(rqTable);

  // PART 1: demonstrating implementation
  printf("TESTING IMPLEMENTATION FOR PART 1...\n\n");

  // Operation (a): lookup(⟨"CSC Marathon", 67815, *⟩, Race-PId-Time)
  HashTable *rptClone = cloneHashTable(rptTable, copyRPT);
  printf("Operation (a): lookup(⟨\"CSC Marathon\", 67815, *⟩, Race-PId-Time)\n");
  RPT *resultRPT = lookup_RPT(rptClone, "CSC Marathon", 67815, NULL);
  printRPTEntry(resultRPT);
  freeHashTable(rptClone);
  printf("\n");

  // Operation (b): lookup(⟨*, "J. Thomas", *, *⟩, PId-Name-City-Zip)
  HashTable *pnczClone = cloneHashTable(pnczTable, copyPNCZ);
  printf("Operation (b): lookup(⟨*, \"J. Thomas\", *, *⟩, PId-Name-City-Zip)\n");
  lookup_PNCZ(pnczClone, -1, "J. Thomas", NULL, NULL);
  freeHashTable(pnczClone);
  printf("\n");

  // Operation (c): lookup(⟨"CSC Marathon", "GVP Classic"⟩, Race-Qualifier)
  HashTable *rqClone = cloneHashTable(rqTable, copyRQ);
  printf("Operation (c): lookup(⟨\"CSC Marathon\", \"GVP Classic\"⟩, Race-Qualifier)\n");
  RQ *resultRQ = lookup_RQ(rqClone, "CSC Marathon", "GVP Classic");
  printRQEntry(resultRQ);
  freeHashTable(rqClone);
  printf("\n");

  // Operation (d): delete(⟨"UTri", "2023-11-02"⟩, Race-Date)
  HashTable *rdClone = cloneHashTable(rdTable, copyRD);
  printf("Operation (d): delete(⟨\"UTri\", \"2023-11-02\"⟩, Race-Date)\n");
  delete_RD(rdClone, "UTri", "2023-11-02");
  printf("Updated RD Table:\n");
  printRDTable(rdClone);
  freeHashTable(rdClone);
  printf("\n");

  // Operation (e): delete(⟨"GVP Classic", "McDavid's"⟩, Race-Sponsor)
  HashTable *rsClone1 = cloneHashTable(rsTable, copyRS);
  printf("Operation (e): delete(⟨\"GVP Classic\", \"McDavid's\"⟩, Race-Sponsor)\n");
  delete_RS(rsClone1, "GVP Classic", "McDavid's");
  printf("Updated RS Table:\n");
  printRSTable(rsClone1);
  freeHashTable(rsClone1);
  printf("\n");

  // Operation (f): delete(⟨*, "LizardAde"⟩, Race-Sponsor)
  HashTable *rsClone2 = cloneHashTable(rsTable, copyRS);
  printf("Operation (f): delete(⟨*, \"LizardAde\"⟩, Race-Sponsor)\n");
  delete_RS(rsClone2, NULL, "LizardAde");
  printf("Updated RS Table:\n");
  printRSTable(rsClone2);
  freeHashTable(rsClone2);
  printf("\n");

  // Operation (g): insert(⟨"GVP Classic", "Newbie Classic"⟩, Race-Qualifier)
  HashTable *rqClone1 = cloneHashTable(rqTable, copyRQ);
  printf("Operation (g): insert(⟨\"GVP Classic\", \"Newbie Classic\"⟩, Race-Qualifier)\n");
  insert_RQ(rqClone1, "GVP Classic", "Newbie Classic");
  printf("Updated RQ Table:\n");
  printRQTable(rqClone1);
  freeHashTable(rqClone1);
  printf("\n");

  // Operation (h): insert(⟨"GVP Classic", "CSC Marathon"⟩, Race-Qualifier)
  HashTable *rqClone2 = cloneHashTable(rqTable, copyRQ);
  printf("Operation (h): insert(⟨\"GVP Classic\", \"CSC Marathon\"⟩, Race-Qualifier)\n");
  insert_RQ(rqClone2, "GVP Classic", "CSC Marathon");
  printf("Updated RQ Table:\n");
  printRQTable(rqClone2);
  freeHashTable(rqClone2);
  printf("\n");
  
  // PART 2: querying
  printf("TESTING QUERIES FOR PART 2...\n\n");
  // PART 2: Task 1
  printf("Answering query: “What time did Name get in Race?” ");
  replForRaceTimeQuery(pnczTable, rptTable);

  // PART 2: Task 2
  printf("\nAnswering query: “On what date(s) did Name run a race sponsored by Sponsor?” ");
  replForRaceDateQuery(pnczTable, rptTable, rsTable, rdTable);

  // PART 3: implementing relational algebra
  printf("\nTESTING RELATIONAL ALGEBRA IMPLEMENTATION FOR PART 3...\n");
  // PART 3: Task 1
  const char *selectionDescription = "Operation 1 (Selection): σRace=\"GVP Classic\"(RPT)";
  HashTable *selectionResult = select_RPT_Race(rptTable, "GVP Classic");
  printSelectionResult(selectionResult, selectionDescription);
  // PART 3: Task 2
  const char *projectionDescription = "Operation 2 (Projection): πPId(σRace=\"GVP Classic\"(RPT))";
  HashTable *projectionResult = project_PId_from_selected_RPT(rptTable, "GVP Classic");
  printProjectionResult(projectionResult, projectionDescription);
  // PART 3: Task 3
  printf("Operation 3 (Join): Join RD ▷◁ RS\n");
  HashTable *joinedTable = join_RD_RS(rdTable, rsTable);
  print_RD_RS_JoinedTable(joinedTable);
  freeHashTable(joinedTable);
  // PART 3: Task 4
  printf("Operation 4 (all operations): πSponsor(σDate=“2023-10-02”(RD ▷◁ RS))");
  HashTable *joinedTableForTask4 = join_RD_RS(rdTable, rsTable);
  HashTable *dateSelectedTable = select_by_date_from_joined_RD_RS(joinedTableForTask4, "2023-10-02");
  HashTable *projectedSponsorTable = project_Sponsor_from_selected_joined_RD_RS(dateSelectedTable);
  printProjectedSponsorResult(projectedSponsorTable, "Projection of Sponsor where Date=2023-10-02 in RD ▷◁ RS");

  // clean up original tables
  freeHashTable(pnczTable);
  freeHashTable(rptTable);
  freeHashTable(rdTable);
  freeHashTable(rsTable);
  freeHashTable(rqTable);

  return 0;
}

// cloning functions for each type of relation
HashTable *cloneHashTable(HashTable *original, void *(*copyData)(void *)) {
  HashTable *clone = createHashTable();
  for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *node = original->buckets[i];
      while (node) {
          void *dataCopy = copyData(node->data);
          insert(clone, ((PNCZ *)dataCopy)->Name, dataCopy); 
          node = node->next;
      }
  }
  return clone;
}

void *copyPNCZ(void *data) {
  PNCZ *original = (PNCZ *)data;
  PNCZ *copy = (PNCZ *)malloc(sizeof(PNCZ));
  if (!copy) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(1);
  }
  *copy = *original;
  return copy;
}

void *copyRPT(void *data) {
  RPT *original = (RPT *)data;
  RPT *copy = (RPT *)malloc(sizeof(RPT));
  if (!copy) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(1);
  }
  *copy = *original;
  return copy;
}

void *copyRD(void *data) {
  RD *original = (RD *)data;
  RD *copy = (RD *)malloc(sizeof(RD));
  if (!copy) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(1);
  }
  *copy = *original;
  return copy;
}

void *copyRS(void *data) {
  RS *original = (RS *)data;
  RS *copy = (RS *)malloc(sizeof(RS));
  if (!copy) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(1);
  }
  *copy = *original;
  return copy;
}

void *copyRQ(void *data) {
  RQ *original = (RQ *)data;
  RQ *copy = (RQ *)malloc(sizeof(RQ));
  if (!copy) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(1);
  }
  *copy = *original;
  return copy;
}
