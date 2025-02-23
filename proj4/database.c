/*
File: database.c
Authors: Jessica Chen & Shadiya Akhter
CSC 713 | George Ferguson | Project 4
Last Modified: 11/15/24
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

unsigned int hash(char *key) {
  unsigned int hash = 0;
  while (*key) {
      hash = (hash << 5) + *key++;
  }
  return hash % TABLE_SIZE;
}

HashTable *createHashTable() {
  HashTable *table = (HashTable *)malloc(sizeof(HashTable));
  for (int i = 0; i < TABLE_SIZE; i++) {
      table->buckets[i] = NULL;
  }
  return table;
}

// insert method to populate table
void insert(HashTable *table, char *key, void *data) {
  unsigned int index = hash(key);
  HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
  newNode->data = data;
  newNode->next = table->buckets[index];
  table->buckets[index] = newNode;
}

// lookup function for table
void *lookup(HashTable *table, char *key, int (*cmp)(void *, void *), void *query) {
    unsigned int index = hash(key);
    HashNode *node = table->buckets[index];
    while (node) {
        if (cmp == NULL || cmp(node->data, query)) {
            return node->data;
        }
        node = node->next;
    }
    return NULL;
}

// delete function for table
void delete(HashTable *table, char *key, int (*cmp)(void *, void *), void *query) {
    unsigned int index = hash(key);
    HashNode *node = table->buckets[index];
    HashNode *prev = NULL;

    while (node) {
        if (cmp(node->data, query)) {
            if (prev) {
                prev->next = node->next;
            } else {
                table->buckets[index] = node->next; 
            }

            free(node->data);
            free(node);

            return;
        }

        prev = node;
        node = node->next;
    }
}

// free hash table
void freeHashTable(HashTable *table) {
  for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *node = table->buckets[i];
      while (node) {
          HashNode *temp = node;
          node = node->next;
          free(temp->data);
          free(temp);       
      }
  }
  free(table);
}

// match functions - allow more flexible lookup and deletion
int matchPNCZ(void *data, int PId, char *Name, char *City, char *Zip) {
  PNCZ *entry = (PNCZ *)data;
  return (PId == -1 || entry->PId == PId) &&
         (Name == NULL || strcmp(entry->Name, Name) == 0) &&
         (City == NULL || strcmp(entry->City, City) == 0) &&
         (Zip == NULL || strcmp(entry->Zip, Zip) == 0);
}

int matchRPT(void *data, char *Race, int PId, char *Time) {
  RPT *entry = (RPT *)data;
  return (Race == NULL || strcmp(entry->Race, Race) == 0) &&
         (PId == -1 || entry->PId == PId) &&
         (Time == NULL || strcmp(entry->Time, Time) == 0);
}

int matchRD(void *data, char *Race, char *Date) {
  RD *entry = (RD *)data;
  return (Race == NULL || strcmp(entry->Race, Race) == 0) &&
         (Date == NULL || strcmp(entry->Date, Date) == 0);
}

int matchRS(void *data, char *Race, char *Sponsor) {
  RS *entry = (RS *)data;
  return (Race == NULL || strcmp(entry->Race, Race) == 0) &&
         (Sponsor == NULL || strcmp(entry->Sponsor, Sponsor) == 0);
}

int matchRQ(void *data, char *Race, char *Qualifier) {
  RQ *entry = (RQ *)data;
  return (Race == NULL || strcmp(entry->Race, Race) == 0) &&
         (Qualifier == NULL || strcmp(entry->Qualifier, Qualifier) == 0);
}

// PART 1: single-relation operations

// insert functions
void insert_PNCZ(HashTable *table, int PId, char *Name, char *City, char *Zip) {
  if (lookup_PNCZ(table, PId, Name, City, Zip) != NULL) {
      printf("Duplicate PNCZ entry found: PId=%d, Name=%s, City=%s, Zip=%s. Insertion skipped.\n", PId, Name, City, Zip);
      return;
  }
  PNCZ *entry = (PNCZ *)malloc(sizeof(PNCZ));
  if (!entry) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(1);
  }
  entry->PId = PId;
  strcpy(entry->Name, Name);
  strcpy(entry->City, City);
  strcpy(entry->Zip, Zip);
  char key[20];
  sprintf(key, "%d", PId);
  insert(table, key, entry);
}

void insert_RPT(HashTable *table, char *Race, int PId, char *Time) {
  if (lookup_RPT(table, Race, PId, Time) != NULL) {
      printf("Duplicate RPT entry found: Race=%s, PId=%d, Time=%s. Insertion skipped.\n", Race, PId, Time);
      return;
  }
  RPT *entry = (RPT *)malloc(sizeof(RPT));
  if (!entry) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(1);
  }
  strcpy(entry->Race, Race);
  entry->PId = PId;
  strcpy(entry->Time, Time);




  char key[100];
  sprintf(key, "%s%d", Race, PId);
  insert(table, key, entry);
}

void insert_RD(HashTable *table, char *Race, char *Date) {
  if (lookup_RD(table, Race, Date) != NULL) {
      printf("Duplicate RD entry found: Race=%s, Date=%s. Insertion skipped.\n", Race, Date);
      return;
  }
  RD *entry = (RD *)malloc(sizeof(RD));
  if (!entry) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(1);
  }
  strcpy(entry->Race, Race);
  strcpy(entry->Date, Date);
  insert(table, Race, entry);
}

void insert_RS(HashTable *table, char *Race, char *Sponsor) {
  if (lookup_RS(table, Race, Sponsor) != NULL) {
      printf("Duplicate RS entry found: Race=%s, Sponsor=%s. Insertion skipped.\n", Race, Sponsor);
      return;
  }
  RS *entry = (RS *)malloc(sizeof(RS));
  if (!entry) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(1);
  }
  strcpy(entry->Race, Race);
  strcpy(entry->Sponsor, Sponsor);
  insert(table, Race, entry);
}

void insert_RQ(HashTable *table, char *Race, char *Qualifier) {
  if (lookup_RQ(table, Race, Qualifier) != NULL) {
      printf("Duplicate RQ entry found: Race=%s, Qualifier=%s. Insertion skipped.\n", Race, Qualifier);
      return;
  }
  RQ *entry = (RQ *)malloc(sizeof(RQ));
  if (!entry) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(1);
  }
  strcpy(entry->Race, Race);
  strcpy(entry->Qualifier, Qualifier);
  insert(table, Race, entry);
}

// delete functions
void delete_PNCZ(HashTable *table, int PId, char *Name, char *City, char *Zip) {
  for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *node = table->buckets[i];
      HashNode *prev = NULL;
      while (node) {
          if (matchPNCZ(node->data, PId, Name, City, Zip)) {
              if (prev) {
                  prev->next = node->next;
              } else {
                  table->buckets[i] = node->next;
              }
              free(node->data);
              free(node);
              return;
          }
          prev = node;
          node = node->next;
      }
  }
}

void delete_RPT(HashTable *table, char *Race, int PId, char *Time) {
  for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *node = table->buckets[i];
      HashNode *prev = NULL;
      while (node) {
          if (matchRPT(node->data, Race, PId, Time)) {
              if (prev) {
                  prev->next = node->next;
              } else {
                  table->buckets[i] = node->next;
              }
              free(node->data);
              free(node);
              return;
          }
          prev = node;
          node = node->next;
      }
  }
}

void delete_RD(HashTable *table, char *Race, char *Date) {
  for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *node = table->buckets[i];
      HashNode *prev = NULL;
      while (node) {
          if (matchRD(node->data, Race, Date)) {
              if (prev) {
                  prev->next = node->next;
              } else {
                  table->buckets[i] = node->next;
              }
              free(node->data);
              free(node);
              return;
          }
          prev = node;
          node = node->next;
      }
  }
}

void delete_RS(HashTable *table, char *Race, char *Sponsor) {
  for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *node = table->buckets[i];
      HashNode *prev = NULL;
      while (node) {
          if (matchRS(node->data, Race, Sponsor)) {
              if (prev) {
                  prev->next = node->next;
              } else {
                  table->buckets[i] = node->next;
              }
              free(node->data);
              free(node);
              return;
          }
          prev = node;
          node = node->next;
      }
  }
}

void delete_RQ(HashTable *table, char *Race, char *Qualifier) {
  for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *node = table->buckets[i];
      HashNode *prev = NULL;
      while (node) {
          if (matchRQ(node->data, Race, Qualifier)) {
              if (prev) {
                  prev->next = node->next;
              } else {
                  table->buckets[i] = node->next;
              }
              free(node->data);
              free(node);
              return;
          }
          prev = node;
          node = node->next;
      }
  }
}

// lookup functions
PNCZ *lookup_PNCZ(HashTable *table, int PId, char *Name, char *City, char *Zip) {
   for (int i = 0; i < TABLE_SIZE; i++) { 
       HashNode *node = table->buckets[i];
       while (node) { 
           PNCZ *entry = (PNCZ *)node->data;
           if ((PId == -1 || entry->PId == PId) &&
               (!Name || strcmp(entry->Name, Name) == 0) &&
               (!City || strcmp(entry->City, City) == 0) &&
               (!Zip || strcmp(entry->Zip, Zip) == 0)) {
              
               printPNCZEntry(entry); 
           }
           node = node->next;
       }
   }
   return NULL; 
}

RPT *lookup_RPT(HashTable *table, char *Race, int PId, char *Time) {
  for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *node = table->buckets[i];
      while (node) {
          if (matchRPT(node->data, Race, PId, Time)) {
              return (RPT *)node->data;
          }
          node = node->next;
      }
  }
  return NULL;
}

RD *lookup_RD(HashTable *table, char *Race, char *Date) {
  for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *node = table->buckets[i];
      while (node) {
          if (matchRD(node->data, Race, Date)) {
              return (RD *)node->data;
          }
          node = node->next;
      }
  }
  return NULL;
}

RS *lookup_RS(HashTable *table, char *Race, char *Sponsor) {
  for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *node = table->buckets[i];
      while (node) {
          if (matchRS(node->data, Race, Sponsor)) {
              return (RS *)node->data;
          }
          node = node->next;
      }
  }
  return NULL;
}

RQ *lookup_RQ(HashTable *table, char *Race, char *Qualifier) {
  for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *node = table->buckets[i];
      while (node) {
          if (matchRQ(node->data, Race, Qualifier)) {
              return (RQ *)node->data;
          }
          node = node->next;
      }
  }
  return NULL;
}

// PART 2:

// Task 1: function to find the time based on Name and Race - following FOCS 8.8
void findTimeByParticipantAndRace(HashTable *pncTable, HashTable *rptTable, char *name, char *race) {
    int participantIds[TABLE_SIZE];
    int participantCount = 0;
    int participantId = -1;
    int found = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = pncTable->buckets[i];
        while (node) {
            PNCZ *pnczEntry = (PNCZ *)node->data;
            if (strcmp(pnczEntry->Name, name) == 0) {
                participantIds[participantCount++] = pnczEntry->PId;
            }
            node = node->next;
        }
    }

    if (participantCount == 0) {
        printf("No participant found with name %s.\n", name);
        return;
    }

    if (participantCount > 1) {
        printf("Multiple participants found with the name '%s'.\n", name);
        printf("Please select a participant by entering their PId:\n");
        for (int i = 0; i < participantCount; i++) {
            printf("- PId: %d\n", participantIds[i]);
        }
        
        printf("Enter the PId: ");
        scanf("%d", &participantId);
        getchar(); 

        int valid = 0;
        for (int i = 0; i < participantCount; i++) {
            if (participantIds[i] == participantId) {
                valid = 1;
                break;
            }
        }
        if (!valid) {
            printf("Invalid PId entered. Query canceled.\n");
            return;
        }
    } else {
        participantId = participantIds[0];
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = rptTable->buckets[i];
        while (node) {
            RPT *rptEntry = (RPT *)node->data;
            if (rptEntry->PId == participantId && strcmp(rptEntry->Race, race) == 0) {
                printf("%s (PId: %d) ran the race '%s' in %s.\n", name, participantId, race, rptEntry->Time);
                found = 1;
                break;
            }
            node = node->next;
        }
        if (found) break;
    }

    if (!found) {
        printf("No matching entry found for %s (PId: %d) in race '%s'.\n", name, participantId, race);
    }
}

// REPL for Part 2, Task 1
void replForRaceTimeQuery(HashTable *pncTable, HashTable *rptTable) {
    char name[50];
    char race[50];
    char continueQuery[5];

    printf("\n--- Race Time Query ---\n");
    do {
        printf("Enter participant's name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';  

        printf("Enter race name: ");
        fgets(race, sizeof(race), stdin);
        race[strcspn(race, "\n")] = '\0';  

        findTimeByParticipantAndRace(pncTable, rptTable, name, race);

        printf("\nWould you like to perform another query? (yes/no): ");
        fgets(continueQuery, sizeof(continueQuery), stdin);
        continueQuery[strcspn(continueQuery, "\n")] = '\0';  
    } while (strcmp(continueQuery, "yes") == 0);

    printf("Exiting Race Time Query.\n");
}

// Task 2: function to find the date(s) based on Name and Sponsor - following FOCS 8.10
void findRaceDatesByParticipantAndSponsor(HashTable *pncTable, HashTable *rptTable, HashTable *rsTable, HashTable *rdTable, char *name, char *sponsor) {
    int participantIds[TABLE_SIZE];
    int participantCount = 0;
    int participantId = -1;
    int found = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = pncTable->buckets[i];
        while (node) {
            PNCZ *pnczEntry = (PNCZ *)node->data;
            if (strcmp(pnczEntry->Name, name) == 0) {
                participantIds[participantCount++] = pnczEntry->PId;
            }
            node = node->next;
        }
    }

    if (participantCount == 0) {
        printf("No participant found with name %s.\n", name);
        return;
    }

    if (participantCount > 1) {
        printf("Multiple participants found with the name '%s'.\n", name);
        printf("Please select a participant by entering their PId:\n");
        for (int i = 0; i < participantCount; i++) {
            printf("- PId: %d\n", participantIds[i]);
        }
        
        printf("Enter the PId: ");
        scanf("%d", &participantId);
        getchar(); 

        int valid = 0;
        for (int i = 0; i < participantCount; i++) {
            if (participantIds[i] == participantId) {
                valid = 1;
                break;
            }
        }
        if (!valid) {
            printf("Invalid PId entered. Query canceled.\n");
            return;
        }
    } else {
        participantId = participantIds[0];
    }

    char *races[TABLE_SIZE];
    int raceCount = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = rptTable->buckets[i];
        while (node) {
            RPT *rptEntry = (RPT *)node->data;
            if (rptEntry->PId == participantId) {
                races[raceCount++] = rptEntry->Race;
            }
            node = node->next;
        }
    }

    char *sponsoredRaces[TABLE_SIZE];
    int sponsoredRaceCount = 0;
    for (int i = 0; i < raceCount; i++) {
        for (int j = 0; j < TABLE_SIZE; j++) {
            HashNode *node = rsTable->buckets[j];
            while (node) {
                RS *rsEntry = (RS *)node->data;
                if (strcmp(rsEntry->Race, races[i]) == 0 && strcmp(rsEntry->Sponsor, sponsor) == 0) {
                    sponsoredRaces[sponsoredRaceCount++] = rsEntry->Race;
                    break;
                }
                node = node->next;
            }
        }
    }

    printf("Dates when %s (PId: %d) ran a race sponsored by %s:\n", name, participantId, sponsor);
    for (int i = 0; i < sponsoredRaceCount; i++) {
        for (int j = 0; j < TABLE_SIZE; j++) {
            HashNode *node = rdTable->buckets[j];
            while (node) {
                RD *rdEntry = (RD *)node->data;
                if (strcmp(rdEntry->Race, sponsoredRaces[i]) == 0) {
                    printf("- %s on %s\n", sponsoredRaces[i], rdEntry->Date);
                    found = 1;
                }
                node = node->next;
            }
        }
    }

    if (!found) {
        printf("No races found for %s (PId: %d) sponsored by %s.\n", name, participantId, sponsor);
    }
}

// REPL for Part 2, Task 2
void replForRaceDateQuery(HashTable *pncTable, HashTable *rptTable, HashTable *rsTable, HashTable *rdTable) {
    char name[50];
    char sponsor[50];
    char continueQuery[5];

    printf("\n--- Race Date Query ---\n");
    do {
        printf("Enter participant's name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0'; 

        printf("Enter sponsor name: ");
        fgets(sponsor, sizeof(sponsor), stdin);
        sponsor[strcspn(sponsor, "\n")] = '\0';  

        findRaceDatesByParticipantAndSponsor(pncTable, rptTable, rsTable, rdTable, name, sponsor);

        printf("\nWould you like to perform another query? (yes/no): ");
        fgets(continueQuery, sizeof(continueQuery), stdin);
        continueQuery[strcspn(continueQuery, "\n")] = '\0';  
    } while (strcmp(continueQuery, "yes") == 0);

    printf("Exiting Race Date Query.\n");
}

// PART 3: 

// selection operation: σRace=“GVP Classic”(RPT)
HashTable *select_RPT_Race(HashTable *rptTable, const char *race) {
    HashTable *resultTable = createHashTable(); 

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = rptTable->buckets[i];
        while (node) {
            RPT *entry = (RPT *)node->data;
            if (strcmp(entry->Race, race) == 0) {
                RPT *selectedEntry = (RPT *)malloc(sizeof(RPT));
                if (!selectedEntry) {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                *selectedEntry = *entry; 

                char key[100];
                sprintf(key, "%s%d", selectedEntry->Race, selectedEntry->PId);
                insert(resultTable, key, selectedEntry);
            }
            node = node->next;
        }
    }
    return resultTable;
}

// helper function to print result of selection 
void printSelectionResult(HashTable *table, const char *description) {
    printf("\n%s\n", description);
    printf("Race              PId     Time\n");
    printf("-----------------------------------\n");

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = table->buckets[i];
        while (node) {
            RPT *entry = (RPT *)node->data;
            printf("%-18s%-8d%-10s\n", entry->Race, entry->PId, entry->Time);
            node = node->next;
        }
    }
    printf("\n");
}

// projection operation: πPId(RPT) after selection σRace=“GVP Classic”(RPT)
HashTable *project_PId_from_selected_RPT(HashTable *rptTable, const char *race) {
    HashTable *resultTable = createHashTable(); 

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = rptTable->buckets[i];
        while (node) {
            RPT *entry = (RPT *)node->data;
            if (strcmp(entry->Race, race) == 0) {
                int PId = entry->PId;

                char key[20];
                sprintf(key, "%d", PId);
                if (!lookup(resultTable, key, NULL, NULL)) {
                    int *projectedPId = (int *)malloc(sizeof(int));
                    if (!projectedPId) {
                        fprintf(stderr, "Memory allocation failed\n");
                        exit(1);
                    }
                    *projectedPId = PId;
                    insert(resultTable, key, projectedPId);
                }
            }
            node = node->next;
        }
    }
    return resultTable;
}

// helper function to print reuslt of projection
void printProjectionResult(HashTable *table, const char *description) {
    printf("\n%s\n", description);
    printf("PId\n");
    printf("----\n");

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = table->buckets[i];
        while (node) {
            int *projectedPId = (int *)node->data;
            printf("%d\n", *projectedPId);
            node = node->next;
        }
    }
    printf("\n");
}

// joining operation: RD ▷◁ RS relations
HashTable *join_RD_RS(HashTable *rdTable, HashTable *rsTable) {
    HashTable *resultTable = createHashTable();

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *rdNode = rdTable->buckets[i];
        while (rdNode) {
            RD *rdEntry = (RD *)rdNode->data;

            unsigned int rsIndex = hash(rdEntry->Race);
            HashNode *rsNode = rsTable->buckets[rsIndex];
            while (rsNode) {
                RS *rsEntry = (RS *)rsNode->data;

                if (strcmp(rdEntry->Race, rsEntry->Race) == 0) {
                    RD_RS_Joined *joinedEntry = (RD_RS_Joined *)malloc(sizeof(RD_RS_Joined));
                    strcpy(joinedEntry->Race, rdEntry->Race);
                    strcpy(joinedEntry->Date, rdEntry->Date);
                    strcpy(joinedEntry->Sponsor, rsEntry->Sponsor);

                    insert(resultTable, joinedEntry->Race, joinedEntry);
                }
                rsNode = rsNode->next;
            }
            rdNode = rdNode->next;
        }
    }
    return resultTable;
}

// helper function to print resulting join
void print_RD_RS_JoinedTable(HashTable *table) {
    printf("RD ▷◁ RS\n");
    printf("Race              Date       Sponsor\n");
    printf("---------------------------------------\n");

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = table->buckets[i];
        while (node) {
            RD_RS_Joined *entry = (RD_RS_Joined *)node->data;
            printf("%-18s%-12s%-15s\n", entry->Race, entry->Date, entry->Sponsor); 
            node = node->next;
        }
    }
    printf("\n");
}

// all operations: select tuples with Date = "2023-10-02" from the result of RD ▷◁ RS
HashTable *select_by_date_from_joined_RD_RS(HashTable *joinedTable, const char *date) {
    HashTable *resultTable = createHashTable();

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = joinedTable->buckets[i];
        while (node) {
            RD_RS_Joined *entry = (RD_RS_Joined *)node->data;
            if (strcmp(entry->Date, date) == 0) {
                RD_RS_Joined *selectedEntry = (RD_RS_Joined *)malloc(sizeof(RD_RS_Joined));
                if (!selectedEntry) {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                *selectedEntry = *entry;

                insert(resultTable, selectedEntry->Race, selectedEntry);
            }
            node = node->next;
        }
    }
    return resultTable;
}

HashTable *project_Sponsor_from_selected_joined_RD_RS(HashTable *selectedTable) {
    HashTable *resultTable = createHashTable();

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = selectedTable->buckets[i];
        while (node) {
            RD_RS_Joined *entry = (RD_RS_Joined *)node->data;

            if (!lookup(resultTable, entry->Sponsor, NULL, NULL)) {
                char *projectedSponsor = (char *)malloc(strlen(entry->Sponsor) + 1);
                if (!projectedSponsor) {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                strcpy(projectedSponsor, entry->Sponsor);
                insert(resultTable, projectedSponsor, projectedSponsor);
            }
            node = node->next;
        }
    }
    return resultTable;
}

void printProjectedSponsorResult(HashTable *table, const char *description) {
    printf("\n%s\n", description);
    printf("Sponsor\n");
    printf("---------\n");

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = table->buckets[i];
        while (node) {
            char *sponsor = (char *)node->data;
            printf("%s\n", sponsor);
            node = node->next;
        }
    }
    printf("\n");
}

// populating relations with appropriate tuples based off Appendix A
void populateTables(HashTable *pncTable, HashTable *rptTable, HashTable *rdTable, HashTable *rsTable, HashTable *rqTable) {
  insert_PNCZ(pncTable, 67815, "E. Allen", "Clarkson", "14642");
  insert_PNCZ(pncTable, 37435, "J. Thomas", "Churchville", "14643");
  insert_PNCZ(pncTable, 41222, "H. Evans", "North Gates", "14621");
  insert_PNCZ(pncTable, 31444, "E. Allen", "Pittsford", "14559");
  insert_PNCZ(pncTable, 89602, "J. Thomas", "Hamlin", "14651");
  insert_PNCZ(pncTable, 69441, "O. Price", "Hamlin", "14506");

  insert_RPT(rptTable, "UTri", 69441, "01:45:12");
  insert_RPT(rptTable, "CSC Marathon", 31444, "01:22:22");
  insert_RPT(rptTable, "GVP Classic", 89602, "01:52:26");
  insert_RPT(rptTable, "CSC Marathon", 67815, "00:32:32");
  insert_RPT(rptTable, "Buffalo Stampede", 41222, "01:53:57");
  insert_RPT(rptTable, "GVP Classic", 37435, "01:20:23");

  insert_RD(rdTable, "RocRace", "2023-07-27");
  insert_RD(rdTable, "Buffalo Stampede", "2023-05-16");
  insert_RD(rdTable, "UTri", "2023-11-02");
  insert_RD(rdTable, "GVP Classic", "2023-05-16");
  insert_RD(rdTable, "CSC Marathon", "2023-11-11");

  insert_RS(rsTable, "Buffalo Stampede", "Sadida");
  insert_RS(rsTable, "CSC Marathon", "Psyke");
  insert_RS(rsTable, "CSC Marathon", "URMC Ortho");
  insert_RS(rsTable, "GVP Classic", "LizardAde");
  insert_RS(rsTable, "Buffalo Stampede", "Run Club");
  insert_RS(rsTable, "RocRace", "Crooks");

  insert_RQ(rqTable, "RocRace", "Buffalo Stampede");
  insert_RQ(rqTable, "GVP Classic", "CSC Marathon");
  insert_RQ(rqTable, "GVP Classic", "RocRace");
}

// printing hash tables
void printPNCZTable(HashTable *table) {
  PNCZ entries[TABLE_SIZE];
  int count = 0;
  for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *node = table->buckets[i];
      while (node) {
          entries[count++] = *(PNCZ *)node->data;
          node = node->next;
      }
  }
  for (int i = 0; i < count - 1; i++) {
      for (int j = 0; j < count - i - 1; j++) {
          if (entries[j].PId > entries[j + 1].PId) {
              PNCZ temp = entries[j];
              entries[j] = entries[j + 1];
              entries[j + 1] = temp;
          }
      }
  }
  printf("PNCZ\n");
  printf("PId     Name            City           Zip\n");
  printf("-------------------------------------------\n");
  for (int i = 0; i < count; i++) {
      printf("%-8d%-15s%-15s%-5s\n", entries[i].PId, entries[i].Name, entries[i].City, entries[i].Zip);
  }
  printf("\n");
}

void printRPTTable(HashTable *table) {
  RPT entries[TABLE_SIZE];
  int count = 0;
  for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *node = table->buckets[i];
      while (node) {
          entries[count++] = *(RPT *)node->data;
          node = node->next;
      }
  }
  for (int i = 0; i < count - 1; i++) {
      for (int j = 0; j < count - i - 1; j++) {
          if (strcmp(entries[j].Race, entries[j + 1].Race) > 0 ||
              (strcmp(entries[j].Race, entries[j + 1].Race) == 0 && entries[j].PId > entries[j + 1].PId)) {
              RPT temp = entries[j];
              entries[j] = entries[j + 1];
              entries[j + 1] = temp;
          }
      }
  }
  printf("RPT\n");
  printf("Race              PId     Time\n");
  printf("-----------------------------------\n");
  for (int i = 0; i < count; i++) {
      printf("%-18s%-8d%-10s\n", entries[i].Race, entries[i].PId, entries[i].Time);
  }
  printf("\n");
}

void printRDTable(HashTable *table) {
  RD entries[TABLE_SIZE];
  int count = 0;
  for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *node = table->buckets[i];
      while (node) {
          entries[count++] = *(RD *)node->data;
          node = node->next;
      }
  }
  for (int i = 0; i < count - 1; i++) {
      for (int j = 0; j < count - i - 1; j++) {
          if (strcmp(entries[j].Race, entries[j + 1].Race) > 0) {
              RD temp = entries[j];
              entries[j] = entries[j + 1];
              entries[j + 1] = temp;
          }
      }
  }
  printf("RD\n");
  printf("Race              Date\n");
  printf("------------------------\n");
  for (int i = 0; i < count; i++) {
      printf("%-18s%-10s\n", entries[i].Race, entries[i].Date);
  }
  printf("\n");
}

void printRSTable(HashTable *table) {
  RS entries[TABLE_SIZE];
  int count = 0;
  for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *node = table->buckets[i];
      while (node) {
          entries[count++] = *(RS *)node->data;
          node = node->next;
      }
  }
  for (int i = 0; i < count - 1; i++) {
      for (int j = 0; j < count - i - 1; j++) {
          if (strcmp(entries[j].Race, entries[j + 1].Race) > 0) {
              RS temp = entries[j];
              entries[j] = entries[j + 1];
              entries[j + 1] = temp;
          }
      }
  }
  printf("RS\n");
  printf("Race              Sponsor\n");
  printf("----------------------------\n");
  for (int i = 0; i < count; i++) {
      printf("%-18s%-15s\n", entries[i].Race, entries[i].Sponsor);
  }
  printf("\n");
}

void printRQTable(HashTable *table) {
  RQ entries[TABLE_SIZE];
  int count = 0;
  for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *node = table->buckets[i];
      while (node) {
          entries[count++] = *(RQ *)node->data;
          node = node->next;
      }
  }
  for (int i = 0; i < count - 1; i++) {
      for (int j = 0; j < count - i - 1; j++) {
          if (strcmp(entries[j].Race, entries[j + 1].Race) > 0 ||
              (strcmp(entries[j].Race, entries[j + 1].Race) == 0 && strcmp(entries[j].Qualifier, entries[j + 1].Qualifier) > 0)) {
              RQ temp = entries[j];
              entries[j] = entries[j + 1];
              entries[j + 1] = temp;
          }
      }
  }
  printf("RQ\n");
  printf("Race              Qualifier\n");
  printf("-----------------------------\n");
  for (int i = 0; i < count; i++) {
      printf("%-18s%-15s\n", entries[i].Race, entries[i].Qualifier);
  }
  printf("\n");
}
