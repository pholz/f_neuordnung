#include <Neuroduino.h>

#define NUM_LAYERS 2
#define ETA 0.2
#define THETA 0.0
#define DEBUG true

#include "WProgram.h"
int get_free_memory();
void printArray(double arr[], int len);
int checkMem();
void setup();
void loop();
extern int __bss_end;
extern void *__brkval;

double netArray[NUM_LAYERS] = {8,1};
double inputArray[] = {1, -1, 1, -1, -1, 1, -1, 1};
double trainArray[] = {0};

int netMem = get_free_memory();

// Neuroduino params: (network array, number of layers, Eta, Theta, debug)
Neuroduino myNet(netArray, NUM_LAYERS, ETA, THETA, DEBUG);

// free memory check
// from: http://forum.pololu.com/viewtopic.php?f=10&t=989#p4218
int get_free_memory(){
  int free_memory;
  if((int)__brkval == 0)
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  else
    free_memory = ((int)&free_memory) - ((int)__brkval);

  return free_memory;
}

void printArray(double arr[], int len){
  int i;
  Serial.print("[");
  for(i=0; i<len; i++){
    Serial.print(arr[i], DEC);
    if(i != len-1) Serial.print(", ");
  }
  Serial.println("]");
}

int checkMem(){
  Serial.print("Free memory: ");
  Serial.println(get_free_memory(), DEC);
}

void setup(){
  netMem -= get_free_memory();
  Serial.begin(9600);
  srand(analogRead(0)); 
  
  myNet.randomizeWeights();
  
  Serial.print("Net size: ");
  Serial.print(netMem);
  Serial.print(" / Free memory: ");
  Serial.println(get_free_memory());
  
  myNet.printNet();
  printArray(myNet.simulate(inputArray), netArray[1]);
  
}

void loop(){
  for(int i = 0; i < 8; i++)
  {
    inputArray[i] = (double)(rand() % 100 -50);
  }
  
  double sumEven = inputArray[0] + inputArray[2] + inputArray[4] + inputArray[6];
  double clas = sumEven < 0 ? -1 : 1;
  trainArray[0] = clas;
  
  myNet.train(inputArray, trainArray);
  myNet.printNet();
  printArray(myNet.simulate(inputArray), netArray[1]);
 
  delay(500); 
}




int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

