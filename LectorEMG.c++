// I hate RST button
void setup() {
  Serial.begin(115200);
  pinMode(32, INPUT);  //START
  pinMode(4, INPUT);  //MODE
}
const int test_cycles = 25;
int Array_Basal[test_cycles];
int Array_Esfuerzo[test_cycles];
int State = 0;
int Hand_state = 0;
void loop() {


  int MButton = digitalRead(32);
  int SButton = digitalRead(4);


  if (MButton == HIGH) {
    State += 1;
    delay(250);
    if (State > 6) {
      State = 0;
      Serial.println(State);
    }
  }
  if (SButton == HIGH) {
    //Cierra el puño
    float Media_basal = 0.0;
    float Media_esfuerzo = 0.0;
    int Read = 0;
    int VarianzaB = 0;
    int VarianzaE = 0;
    for (int i = 0; i < test_cycles; i++) {
      VarianzaB += pow(Array_Basal[i] - Media_basal, 2);
      VarianzaE += pow(Array_Esfuerzo[i] - Media_esfuerzo, 2);
    }

    VarianzaB /= test_cycles - 1;
    VarianzaE /= test_cycles - 1;


    switch (State) {
      case 0:
        Serial.println("Aprienta el puño!!!");
        delay(300);
        for (int i = 0; i < test_cycles; i++) {
          Array_Esfuerzo[i] = analogRead(A0);
          Media_esfuerzo += Array_Esfuerzo[i];
          delay(int(5000 / test_cycles));
          Serial.println(Array_Esfuerzo[i]);
        }
        Media_esfuerzo /= test_cycles;
        qsort(Array_Esfuerzo, test_cycles, sizeof(Array_Esfuerzo[0]), [](const void *a, const void *b) {
          return (*(int *)a - *(int *)b);
        });
        // Max Array_Esfuerzo[test_cycles - 1];
        // Min Array_Esfuerzo[0];
        break;
      case 1:
        Serial.println("Relaja el brazo");
        delay(300);
        for (int i = 0; i < test_cycles; i++) {
          Array_Basal[i] = analogRead(A0);
          Media_basal += Array_Basal[i];
          delay(int(5000 / test_cycles));
          Serial.println(Array_Basal[i]);
        }
        Media_basal /= test_cycles;
        qsort(Array_Basal, test_cycles, sizeof(Array_Basal[0]), [](const void *a, const void *b) {
          return (*(int *)a - *(int *)b);
        });
        //Min Array_Basal[0];
        //Max Array_Basal[test_cycles - 1];
        break;


        
      case 2:
        {
         
            int Range = Array_Esfuerzo[test_cycles - 1] - Media_esfuerzo;
            int lower_boundary = Media_esfuerzo - Range;
            Read = analogRead(A0);
            if (Read >= lower_boundary && Read <= Array_Esfuerzo[test_cycles - 1]) {
              Hand_state = 2;
              Serial.println(1);
            } else if (Read > Array_Basal[0] && Read < lower_boundary) {
              Hand_state = 1;
              Serial.println(0);
            }
            if (State != 2){
              return;
            }
          
          break;
        }

      case 3:
        {
          
            int RangeE = Array_Esfuerzo[test_cycles - 1] - Media_esfuerzo;
            int lower_boundaryE = RangeE - Media_esfuerzo;
            int Range_B = Array_Basal[0] - Media_basal;
            int upper_boundayB = Media_basal + Range_B;
            Read = analogRead(A0);
            Serial.println(RangeE);
            if (Read >= lower_boundaryE && Read <= Array_Esfuerzo[test_cycles - 1]) {
              Hand_state = 2;
              Serial.println(1);
            } else if (Read > Array_Basal[0] && Read < upper_boundayB) {
              Hand_state = 1;
              Serial.println(0);
            }
            delay(250);
            if (State != 3){
              return;
            }
          
          break;
        }

      case 4:
        
          Read = analogRead(A0);
          Serial.println(Read);
          if (Read >= Media_esfuerzo - VarianzaE && Read <= Media_esfuerzo + VarianzaE) {
            Hand_state = 2;
            Serial.println(1);
          } else if (Read >= Media_basal - VarianzaB && Read <= Media_basal + VarianzaB) {
            Hand_state = 1;
            Serial.println(0);
          }
          if (State != 4){
              return;
            }
        
        break;

      case 5:
        
          Read = analogRead(A0);
          Serial.println(Read);
        
        break;
    }
  }
}
