#include <SPI.h>
#include <DW1000Ranging.h>
#include "link.h"
#include"FunctionLibrary/UWB_Math.h"
#include"FunctionLibrary/UWB_nC3.h"
#include"FunctionLibrary/UWB_Matrix.h"
#include"FunctionLibrary/UWB_Newton.h"

#define ADDRESS "00:01:00:00:00:00:00:00"

#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4
#define PIN_RST 27
#define PIN_IRQ 34

struct MyLink *uwb_data;
int index_num = 0;
long runtime = 0;

#define anchor_count 4
#define matrix_size 3

struct nC3 *structure;
// {X, Y, Z} units:milimeter
float anchor_location[4][matrix_size] = {
        {519.618, 876.596, -51.963}, // Anchor A
        {19.6184, 10.5662, -51.963}, // Anchor B
        {1019.62, 10.5648, -51.963}, // Anchor C
        {519.618, 299.242, 764.531}  // Anchor D
    };
char anchor_label[anchor_count] = {'A', 'B', 'C', 'D'} ;
uint16_t anchor_address[anchor_count] = {10, 11, 12, 13} ;

void setup()
{
    Serial.begin(115200);

    delay(1000);

    structure = nC3_Creator(anchor_count);

    //init the configuration
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    DW1000Ranging.initCommunication(PIN_RST, DW_CS, PIN_IRQ);
    DW1000Ranging.attachNewRange(newRange);
    DW1000Ranging.attachNewDevice(newDevice);
    DW1000Ranging.attachInactiveDevice(inactiveDevice);

    //we start the module as a tag
    DW1000Ranging.startAsTag(ADDRESS, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);

    uwb_data = init_link();
}

void loop()
{
    DW1000Ranging.loop();
    if ((millis() - runtime) > 1000)
    {
        long start_time = millis();
        float tag_distance[4] = {1081.42, 1146.05, 983.262, 187.758};
        float tag_signal[4] = {0, 0, 0, 0};
        for(int i = 0; i < 4; i++){
          struct MyLink *link = find_link(uwb_data, anchor_address[i]);
          if(link == NULL) continue;
          tag_distance[i] = link->range[0]*1000.0;
          tag_signal[i] = link->dbm;
        }
        float guess[matrix_size] = {0.0, 0.0, 0.0}; // initial guess

        float current_anchor_location[matrix_size][matrix_size] = {0.0};
        float current_distance[matrix_size] = {0.0};
        float tags_location[4][matrix_size] = {0.0};
        for(int i = 0; i < structure->nC3_size; i++){
        // newton_3b3_3 loader
        for(int j = 0; j < matrix_size; j++){
              int choose_row = *(structure->nC3_array+(matrix_size*i)+j);
              memcpy(current_anchor_location[j], anchor_location[choose_row], sizeof(float)*matrix_size);
              current_distance[j] = tag_distance[choose_row];
          }

          // current newton method round
          for(int j = 0; j < 5; j++){
              float *guess_location = newton_3b3_3(guess, current_anchor_location, current_distance);
              memcpy(guess, guess_location, sizeof(float)*matrix_size);
              free(guess_location);
          }

          // put result to array
          memcpy(tags_location[i], guess, sizeof(float)*matrix_size);
        }

        for(int i = 0; i < structure->nC3_size; i++)
                Serial.printf("Anchor %c:Distance %.2f, Signal %.2f\n", anchor_label[i], tag_distance[i], tag_signal[i]);

        // print out
        char anchor_label[anchor_count] = {'A', 'B', 'C', 'D'} ;
        for(int i = 0; i < structure->nC3_size; i++){
            for(int j = 0; j < matrix_size; j++)
                Serial.printf("%c ", anchor_label[*(structure->nC3_array+(matrix_size*i)+j)]);
            for(int j = 0; j < matrix_size; j++)
                Serial.printf("%.2f ", tags_location[i][j]);
            Serial.printf("\n");
        }

        runtime = millis();
        Serial.printf("ComputeTime: %d millisecond\n", millis()-start_time);
        Serial.printf("\n");
     }
}

void newRange()
{
    char c[30];

    //Serial.print("from: ");
    //Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
    //Serial.print("\t Range: ");
    //Serial.print(DW1000Ranging.getDistantDevice()->getRange());
    //Serial.print(" m");
    //Serial.print("\t RX power: ");
    //Serial.print(DW1000Ranging.getDistantDevice()->getRXPower());
    //Serial.println(" dBm");
    fresh_link(uwb_data, DW1000Ranging.getDistantDevice()->getShortAddress(), DW1000Ranging.getDistantDevice()->getRange(), DW1000Ranging.getDistantDevice()->getRXPower());
}

void newDevice(DW1000Device *device)
{
    //Serial.print("ranging init; 1 device added ! -> ");
    //Serial.print(" short:");
    //Serial.println(device->getShortAddress(), HEX);

    add_link(uwb_data, device->getShortAddress());
}

void inactiveDevice(DW1000Device *device)
{
    //Serial.print("delete inactive device: ");
    //Serial.println(device->getShortAddress(), HEX);

    delete_link(uwb_data, device->getShortAddress());
}
