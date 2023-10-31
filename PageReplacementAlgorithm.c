#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int fifo(int numFrames, int *reference, int numPageReferences) {
  // initialise values for page faults and pointer
  int faults = 0;
  int pointer = 0;

  // initialize the array for the frames (empty frame has value -1)
  int *frames = (int *)malloc(numFrames * sizeof(int));
  for (int i = 0; i < numFrames; i++) {
    frames[i] = -1;
  }

  //loop through all the pages in the page reference string
  for (int j = 0; j < numPageReferences; j++) {
    //loop through all the frames to see if the page is in any of the frames
    for (int k = 0; k < numFrames; k++) {

      // if the page is in the frame
      if (frames[k] == reference[j]) {
        break;
      }

      // if the page wasn't in any of the frames
      else if (k == (numFrames - 1)) {
        // increment fault counter
        faults++;

        // move the pointer
        frames[pointer] = reference[j];
        if (pointer ==
            (numFrames - 1)) { // if the pointer is pointing the the last frame
          pointer = 0;         // move pointer back to the first frame
        } else {
          pointer++; // if not, increment the pointer
        }
      }
    }
  }

  return faults;
}

int lru(int numFrames, int *reference, int numPageReferences) {
  int faults = 0;

  // initialize the array for the frames (empty frame has value -1) and for the
  // array with the counters
  int *frames = (int *)malloc(numFrames * sizeof(int));
  int *lruCounters = (int *)malloc(numFrames * sizeof(int));
  for (int i = 0; i < numFrames; i++) {
    frames[i] = -1;
    lruCounters[i] = numFrames + 1;
  }

  //loop through all the pages in the page reference string
  for (int j = 0; j < numPageReferences; j++) {
    //loop through all the frames to see if the page is in any of the frames
    for (int k = 0; k < numFrames; k++) {

      // if the page is in the frame
      if (frames[k] == reference[j]) {
        // the counter for the page returns to 0
        lruCounters[k] = 0;

        // all the other counters that aren't associated with empty frames are
        // incremented
        for (int a = 0; a < numFrames; a++) {
          if ((a != k) && (lruCounters[a] != numFrames + 1)) {
            lruCounters[a]++;
          }
        }

        break;
      }

      // if the page wasn't in any of the frames
      else if (k == (numFrames - 1)) {
        // increment fault counter
        faults++;

        // max represents the value of the lru page's counter
        int max = lruCounters[0];

        // position represents the position of the lru page's counter
        int position = 0;

        // searching for the lru page
        for (int b = numFrames - 1; b > 0; b--) {
          if (lruCounters[b] > max) {
            max = lruCounters[b];
            position = b;
          }
        }

        // the lru page is replaced with the new page
        frames[position] = reference[j];

        // the counter for the page returns to 0
        lruCounters[position] = 0;

        // all the other counters that aren't associated with empty frames are
        // incremented
        for (int c = 0; c < numFrames; c++) {
          if ((c != position) && (lruCounters[c] != -1)) {
            lruCounters[c]++;
          }
        }
      }
    }
  }

  return faults;
}

int *randomPageReference(int size) {
  srand(time(NULL));

  //generate string with random page references
  int *string = (int *)malloc(size * sizeof(int));
  for (int i = 0; i < size; i++) {
    string[i] = rand() % 10;
  }

  return string;
}

int main(void) {
  //number of page references
  int numPageReferences = 20;

  //generate random number of frames between 1 and 7
  srand(time(NULL));
  int numFrames = (rand() % 7) + 1;

  // get array of random page references
  int *referenceString = randomPageReference(numPageReferences);

  // run the methods for each algorithm
  int fifoFaults = fifo(numFrames, referenceString, numPageReferences);
  int lruFaults = lru(numFrames, referenceString, numPageReferences);

  // print the page-reference string
  printf("Page-reference string: ");
  printf("%d", referenceString[0]);
  for (int i = 1; i < numPageReferences; i++) {
    printf(", ");
    printf("%d", referenceString[i]);
  }
  printf("\nNumber of frames: ");
  printf("%d", numFrames);
  printf("\n\n");

  // print the number of faults for each algorithm
  char *strFifoFaults = (char *)malloc(12);
  char *strLruFaults = (char *)malloc(12);
  sprintf(strFifoFaults, "%d", fifoFaults);
  sprintf(strLruFaults, "%d", lruFaults);

  printf("FIFO: ");
  printf("%s", strFifoFaults);
  printf("\n\n");

  printf("LRU: ");
  printf("%s", strLruFaults);
  printf("\n\n");
}