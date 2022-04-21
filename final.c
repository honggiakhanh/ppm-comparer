#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "neuron/neuron.h"

#define getlength(x)  (sizeof(x) / sizeof((x)[0]))

#define WIDTH 300
#define HEIGHT 400

extern double synaptic_weights_R[3];
extern double synaptic_weights_G[3];
extern double synaptic_weights_B[3];

void getcolor(uint8_t img[], int length, double result[]){
  int r = 0, g = 0, b = 0;
  for(int i = 0; i < length; i=i+3){
    r = r + img[i];
    g = g + img[i+1];
    b = b + img[i+2];
  }
  result[0] = (double)r/(length/3)/255;
  result[1] = (double)g/(length/3)/255;
  result[2] = (double)b/(length/3)/255;
}

void quad(uint8_t main[], int w, int h, uint8_t q1[], uint8_t q2[], uint8_t q3[], uint8_t q4[]){
  uint8_t *fhalf;
  uint8_t *lhalf;
  int length = w*h*3;
  fhalf = (uint8_t *)calloc(length/2, sizeof(uint8_t));
  lhalf = (uint8_t *)calloc(length/2, sizeof(uint8_t));
  for(int i = 0; i < length/2; i++){
    fhalf[i] = main[i];
  }
  for(int j = length/2; j < length; j++){
    lhalf[j - length/2] = main[j];
  }

  int c = 0; 
  for(int i = 0; i < length/2; i+=w*3){
    for(int j = 0; j < w*3/2; j++){
      q1[c] = fhalf[i+j];
      q2[c] = fhalf[i+j+w*3/2];
      q3[c] = lhalf[i+j];
      q4[c] = lhalf[i+j+w*3/2];
      c++;
    }
  }
  free(fhalf);
  free(lhalf);
}

void getinfo(){

}
 
int main(int argc, char *argv[]){

  //Ppm file (main img)
  uint8_t imgheader[15]; //img header
  uint8_t imginfo[360000]; //img info
  //Ppm file (comparing img)
  uint8_t imgheader2[15]; //img header
  uint8_t imginfo2[360000]; //img info
  //Storing quadrants
  uint8_t quads[4][90000];
  uint8_t quads2[4][90000];
  //Storing sub-quadrants
  uint8_t squad[16][22500];
  uint8_t squad2[16][22500];

//-----------------------------

  //Opening file
  FILE *main;
  FILE *comp;
  char *link = calloc(20, 1);
  char *link2 = calloc(20, 1);

  printf("\nMain file directory:\n");
  fgets(link, 20, stdin);
  strtok(link, "\n");
  printf("\nComparing file directory:\n");
  fgets(link2, 20, stdin);
  strtok(link2, "\n");
  printf("\n");
  
  main = fopen(link, "rb");
  comp = fopen(link2, "rb");

  free(link);
  free(link2);

  // Putting the img info into array img
  if(main==NULL){
    printf("Error: can't open main file.\n");
    return 1;
  } 
  else 
  {
    // Seperating header part and actual img part into 2 arr
    fread(imgheader, 1, 15, main);
    fread(imginfo, 1, 360000, main);
    fclose(main);
  }
  if(comp==NULL){
    printf("Error: can't open comparing file.\n");
    return 1;
  } 
  else 
  {
    fread(imgheader2, 1, 15, comp);
    fread(imginfo2, 1, 360000, comp);
    fclose(comp);
  }

  // Getting colors and divide 
  double ctotal[3]; // Total avg color (main img)
  double cquad[4][3]; // 4 Quads avg color
  double csquad[16][3]; // 16 Subquads avg color
  double ctotal2[3]; // Total avg color (comparing img)
  double cquad2[4][3]; // 4 Quads avg color
  double csquad2[16][3]; // 16 Subquads avg color

//-----------------------------
  
  getcolor(imginfo, getlength(imginfo), ctotal); // Getting color of the whole img
  // Divide whole img into 4 and get color
  quad(imginfo, WIDTH, HEIGHT, quads[0], quads[1], quads[2], quads[3]);
  for(int i = 0; i < 4; i++){
    getcolor(quads[i], getlength(quads[i]), cquad[i]);
  }
  // Divide 4 quad into subquads and getcolor
  for(int i = 0; i < 16; i+=4){
    quad(quads[i/4], WIDTH/2, HEIGHT/2, squad[i], squad[i+1], squad[i+2], squad[i+3]);
  } //divide 4 quads into 16 subquads
  for(int i = 0; i < 16; i++){
    getcolor(squad[i], getlength(squad[i]), csquad[i]);
  } //setting 16 subquads color

  // Same thing for the comparing img

  getcolor(imginfo2, getlength(imginfo2), ctotal2); // Getting color of the whole img
  // Divide whole img into 4 and get color
  quad(imginfo2, WIDTH, HEIGHT, quads2[0], quads2[1], quads2[2], quads2[3]);
  for(int i = 0; i < 4; i++){
    getcolor(quads2[i], getlength(quads2[i]), cquad2[i]);
  }
  // Divide 4 quad into subquads and getcolor
  for(int i = 0; i < 16; i+=4){
    quad(quads2[i/4], WIDTH/2, HEIGHT/2, squad2[i], squad2[i+1], squad2[i+2], squad2[i+3]);
  } //divide 4 quads into 16 subquads
  for(int i = 0; i < 16; i++){
    getcolor(squad2[i], getlength(squad2[i]), csquad2[i]);
  } //setting 16 subquads color

//-----------------------------

  //Letting the neuron learns to detect R G B color
  learn();

//-----------------------------

  FILE *result;
  FILE *result2;
  result = fopen("result.txt", "w");
  result2 = fopen("result2.txt", "w");

  //Writing result of the main file to a file
  fprintf(result,"%lf \t",analyze(ctotal, synaptic_weights_R));
  fprintf(result,"%lf \t",analyze(ctotal, synaptic_weights_G));
  fprintf(result,"%lf \n",analyze(ctotal, synaptic_weights_B));
  for(int i = 0; i < 4; i++){
    fprintf(result,"%lf \t",analyze(cquad[i], synaptic_weights_R));
    fprintf(result,"%lf \t",analyze(cquad[i], synaptic_weights_G));
    fprintf(result,"%lf \n",analyze(cquad[i], synaptic_weights_B));
  }
  for(int i = 0; i < 16; i++){
    fprintf(result,"%lf \t",analyze(csquad[i], synaptic_weights_R));
    fprintf(result,"%lf \t",analyze(csquad[i], synaptic_weights_G));
    fprintf(result,"%lf \n",analyze(csquad[i], synaptic_weights_B));
  }

  //Writing result of the comparing file to another file
  fprintf(result2,"%lf \t",analyze(ctotal2, synaptic_weights_R));
  fprintf(result2,"%lf \t",analyze(ctotal2, synaptic_weights_G));
  fprintf(result2,"%lf \n",analyze(ctotal2, synaptic_weights_B));
  for(int i = 0; i < 4; i++){
    fprintf(result2,"%lf \t",analyze(cquad2[i], synaptic_weights_R));
    fprintf(result2,"%lf \t",analyze(cquad2[i], synaptic_weights_G));
    fprintf(result2,"%lf \n",analyze(cquad2[i], synaptic_weights_B));
  }
  for(int i = 0; i < 16; i++){
    fprintf(result2,"%lf \t",analyze(csquad2[i], synaptic_weights_R));
    fprintf(result2,"%lf \t",analyze(csquad2[i], synaptic_weights_G));
    fprintf(result2,"%lf \n",analyze(csquad2[i], synaptic_weights_B));
  }
  fclose(result);
  fclose(result2);

//-----------------------------
  
  //Struct to store the result after it've been analyzed
  typedef struct {
    double R[21];
    double G[21];
    double B[21];
  } Color;
  Color img1, img2;

  //Reopen result file and store them in the struct
  result = fopen("result.txt", "r");
  result2 = fopen("result2.txt", "r");

  for(int i = 0; i < 21; i++){
    fscanf(result,"%lf %lf %lf", &img1.R[i], &img1.G[i], &img1.B[i]);
    fscanf(result2,"%lf %lf %lf", &img2.R[i], &img2.G[i], &img2.B[i]);
  }
  double difference = 0;
  for(int i = 0; i < 21; i++){
    difference += fabs(img1.R[i]-img2.R[i]) + fabs(img1.G[i]-img2.G[i]) + fabs(img1.B[i]-img2.B[i]);
  }

  printf("The compared picture is %0.01lf%c likely the same as the original picture\n", (1-difference/63)*100, '%');

}
