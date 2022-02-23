//--------/
//   設定 ★がついてる場所の値を変更して調整をお願いします。
//---------/
#define SELECT 3//ここでパターンを指定 // ★

/*点滅パターン 1がON、0がOFF */
int64_t pattern[] ={//64bitまで格納
  0b0000101101010,//パターン１　右から読む
  0b000101101101100111101101100,
  0b01110111011100010111000111011101110001110111,
  0b111011101110111011101110111011101110111011101110,
  };

int interval[] = {1000,500,300,300};
#define MOTER 2//使用するピン番号 ★
#define LED A5 //★

/*点滅パターン 1がON、0がOFF */
#define CYCLE 10 //pwmの１周期にかける時間　単位 ms ★
#define MAXPOWER 10//pwmの最大出力時に１周期のうち何m秒をONにするか ★
#define KASOKUTIME 4000//加速,減速にかける時間 ms ★
int KASOKUCYCLE = KASOKUTIME/MAXPOWER;
#define MOTERTIME 10000 //モータの稼働時間 単位ms ★

//-------------------/
//  制御
//------------------/
int power = 0;//初期化
int arySize = 0;//初期化
unsigned count = 0;
bool lockedLED = false;
void ON(int pin){
  digitalWrite(pin, HIGH);
}
void OFF(int pin){
  digitalWrite(pin, LOW);
}

void Blink(){

  if((pattern[SELECT-1]>>count)&1){
    ON(LED);
  }
  else{
    OFF(LED);
  }
}
void setup()
{
  Serial.begin(9600);  
  pinMode(MOTER, OUTPUT);
  pinMode(LED, OUTPUT);
}
void controlMoter(int time){
  if(time % CYCLE < power){
    ON(MOTER);
  }
  else{
    OFF(MOTER);
  }
  if(time % KASOKUCYCLE == 0){
    if(time <= KASOKUTIME){
      if(power < MAXPOWER){
        power++;
      }
    }
    if(MOTERTIME - time <= KASOKUTIME){
      if(power > 0){
        power --;
      }
    }
  }
}
void controlLED(int time){
  if(time % interval[SELECT-1] <= 5){//誤差を5msだけ許容
    if(!(lockedLED)){
      Blink();
      count++;
      lockedLED = true;
    }
  }
  else{
    lockedLED = false;
  }
}

void loop()
{
  unsigned long time = 0;

  while(time <= MOTERTIME){
    time = (unsigned long)millis();//プログラム開始時点からの経過時間を取得　※約50日でオーバーフローし0に戻るので注意
    controlMoter(time); //モータをpwm制御
  }
  while(1){
    time = (unsigned long)millis();//プログラム開始時点からの経過時間を取得　※約50日でオーバーフローし0に戻るので注意
    OFF(MOTER);
    controlLED(time-MOTERTIME);// LEDのONOFFをパターンに基づいて指定
  }
}