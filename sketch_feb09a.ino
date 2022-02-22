//--------/
//   設定 ★がついてる場所の値を変更して調整をお願いします。
//---------/
#define SELECT 4//ここでパターンを指定 // ★
/*点滅パターン 1がON、0がOFF */
#define PATTERN_NUM 3

int pattern1[] = {0,1,0,1,0,1,1,0,1,0,0,0,0}; // ★
int pattern2[] = {0,0,1,1,0,1,1,0,1,1,1,1,0,0,1,1,0,1,1,0,1,0,0,0}; // ★
int pattern3[] = {1,1,1,0,1,1,1,0,0,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0,1,1,1,0,1,0,0,0,1,1,1,0,1,1,1,0,1,1,1,0};// ★
int pattern4[] = {//動作確認用
  0,1,0,1,0,0,1,0,0,
  0,1,0,1,0,0,1,0,0,
  0,1,0,1,0,0,1,1,1,0,0,
  0,1,0,1,0,0,1,0,0,
  0,1,0,1,0,0,1,0,0,
  0,1,0,1,0,0,1,1,1,0,0,
  0,1,0,1,0,0,1,0,0,
  0,1,0,1,0,0,1,0,0,
  0,1,0,1,0,0,1,1,1,1,1,1,
  };
#define INTERVAL1 1000//1文字にかける時間 単位：ms ★
#define INTERVAL2 500//単位：ms ★
#define INTERVAL3 300//単位：ms ★
#define INTERVAL4 150//単位：ms ★

#define MOTER 2//使用するピン番号 ★
#define LED A5 //★
#define INPUT_SET 5 //★
#define START_BUTTON 6//★

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
int interval = 0;//初期化
unsigned count = 0;
bool lockedLED = false;
bool startFlag = false;
int prevTime = 0;
bool state = false;
bool oldValStart = false;
bool valStart = false;
void initialize(){
  OFF(MOTER);
  OFF(LED);
  count = 0;
  power = 0;
  lockedLED = false;
  prevTime = (unsigned long)millis();
}
void ON(int pin){
  digitalWrite(pin, HIGH);
}
void OFF(int pin){
  digitalWrite(pin, LOW);
}
void _Blink(int ary[]){
  
  if(ary[count]==1){
    ON(LED);
  }
  else{
    OFF(LED);
  }
}
void Blink(){
  switch(SELECT){
    case 1:_Blink(pattern1);break;
    case 2:_Blink(pattern2);break;
    case 3:_Blink(pattern3);break;
    case 4:_Blink(pattern4);break;
  }
}
void setup()
{
  pinMode(MOTER, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(INPUT_SET, INPUT);
  pinMode(START_BUTTON, INPUT);

  switch(SELECT){
    case 1:
      arySize = sizeof(pattern1)/sizeof(int);
      interval = INTERVAL1;
      break;
    case 2:
      arySize = sizeof(pattern2)/sizeof(int);
      interval = INTERVAL2;
      break;
    case 3:
      arySize = sizeof(pattern3)/sizeof(int);
      interval = INTERVAL3;
      break;
    case 4:
      arySize = sizeof(pattern4)/sizeof(int);
      interval = INTERVAL4;
      break;
  }
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
  if(time % interval <= 10){//誤差を10msだけ許容する
    if(!(lockedLED)){
      if(count<arySize){
        Blink();
      }
      else{
        OFF(LED);
      }
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
  
  unsigned long time = (unsigned long)millis()-prevTime;//スタートボタン押下からの経過時間を取得　※約50日でオーバーフローし0に戻るので注意
  valStart =  digitalRead(START_BUTTON);
  if(valStart == HIGH && oldValStart == LOW){
    state = !state;
    initialize();
  }
  oldValStart = valStart;
  
  if(state){
    if(time <= MOTERTIME){
     controlMoter(time); //モータをpwm制御
    }
    else{
      OFF(MOTER);
      controlLED(time-MOTERTIME);// LEDのONOFFをパターンに基づいて指定
    }
  }
  else{
    
  }
  
  
  delay(1);
}