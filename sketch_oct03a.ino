#include<Servo.h>


//parametre reglable
int longe=5;  //i
int large=3;   //j
int ideb=1;
int jdeb=1;
int jfin=3;
int ifin=5;
int T[9][9];



//ne pas touché
int i=1;
int j=1;
int wR,wL;
int test;
int x=4;
int K[]={0,1,0,-1};
int tempPas=60;
int npart=25;
int tempTour=550;
int tempstop=25;
Servo servoL,servoR;

void setup() {
  Serial.begin(9600);
  remplirMat();
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  servoL.attach(12);
  servoR.attach(11);
}

void loop() {
  regleDir();
  test=avanceCase();
  changeCase();
}

void regleDir(){
  int count=0;
  int c=-1;
  int s=1;
   if (j==jfin&&i==ifin){
    theEnd();    
  }
  while(c!=-3){
    if (T[i+getK(x)][j+getK(x+3)]==s)
    {
      Serial.println(T[i+getK(x)][j+getK(x+3)]);
      tourne(c);
      c=-3;  
    }
    else{
      count++;
      x++;
      c++;
      if(count==7){
        theEnd();
      }
      if(count==3){
        s=2;
        c=-2;
        T[i][j]=0;
      }
    }
  }
  x--;
}

void theEnd(){
  Serial.println("the end");
  //stopSer();
  servoL.detach();
  servoR.detach();
  while(1){
  digitalWrite(2,HIGH);
  delay(650);
  digitalWrite(2,LOW);
  digitalWrite(10,HIGH);
  delay(650);
  digitalWrite(10,LOW);
  }
}

int avanceCase(){
  int R;
  int count=0;
  
  while(!(R = testCol())&& count < npart){
    avance();
    count++;
}
  
  if (count < npart) {
    digitalWrite(2,HIGH);
    digitalWrite(10,HIGH);
    stopSer();
    
    while(count > 0){
      arrier();
      count--;
    }
    digitalWrite(2,LOW);
    digitalWrite(10,LOW);
  }
  stopSer();
  return(R);
}

void changeCase(){

  if(test) {
    T[i+getK(x+1)][j+getK(x)]=0;
  }
  else {
    i+=getK(x+1);
    j+=getK(x);
    if(T[i][j]==1) {
      T[i][j]=2;
    }
  }
  String position = String("position:(");
  Serial.println(position + i + "," + j + ")");
  Serial.println(getAsciiMatrice());
}

int testCol(){
    wL =digitalRead(4);
    wR =digitalRead(5);
    //Serial.print(wL);
    //Serial.print(wR);
    if(wL==0||wR==0)return(1);
    return(0);
}
  
int getK(int e){
  int y;
  y=e%4;
  //String directio = String("y=") + y + String(", x=") + x;
  //Serial.println(directio);
  return (K[y]);
}
  
void tourne(int c){
  if (c==-1){turnLeft();}
  if(c==1){turnRight();}
  if(c==-2){
    turnRight();
    turnRight();
  }
}

void remplirMat(){
  int p;
  int q;
  for(p=1; p<=longe;p++)
  for(q=1;q<=large;q++)
  T[p][q]=1;
  /*for(p=0;p<=large;p++)
  {
  T[0][p]=0;
  T[large+1][p]=0;
    }
    for(p=0;p<=longe;p++){
    T[p][0]=0;
      T[p][longe+1]=0;
    
  }*/
    T[ideb][jdeb]=2;//position du debut
}

void avance()
{
  servoL.writeMicroseconds(1600);
  servoR.writeMicroseconds(1400);
  delay(tempPas);
}

void arrier()
{
  
  servoL.writeMicroseconds(1400);
  servoR.writeMicroseconds(1600);
  delay(tempPas);
}

void turnRight()
{
  servoR.writeMicroseconds(1700);
  servoL.writeMicroseconds(1700);
digitalWrite(2,HIGH);
  delay(tempTour);
  digitalWrite(2,LOW);
}

void turnLeft()
{
  servoL.writeMicroseconds(1300);
  servoR.writeMicroseconds(1300);
  digitalWrite(10,HIGH);
  delay(tempTour);
  digitalWrite(10,LOW);
}

void stopSer()
{
  Serial.println("stop");
  servoL.writeMicroseconds(1500);
  servoR.writeMicroseconds(1500);
  delay(tempstop);
}

String getAsciiMatrice()
{
  int i,j;
  String mat;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      mat = mat + "[" + T[j][i] + "] ";
    }  
    mat = mat + "\n";
  }
  return mat;
}
