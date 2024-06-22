#include "serial.h"

//https://stackoverflow.com/questions/2545720/error-default-argument-given-for-parameter-1
void _Serial :: begin(int baud_rate/*=9600*/){

    //https://stackoverflow.com/questions/10673585/start-thread-with-member-function
    std::thread (&_Serial::sent, this);
    std::thread (&_Serial::read, this);

    if (this->callback){
        std::thread(&_Serial::handle_pc_cmd, this);
    }

    Serial.begin(baud_rate);
}

_Serial :: _Serial() : 
    cmd_to_pc({}),
    cmd_from_pc({}),
    mutex_to_pc(xSemaphoreCreateMutex()),
    mutex_from_pc(xSemaphoreCreateMutex())
{}

void _Serial :: write(String str){
    if(xSemaphoreTake(mutex_to_pc, portMAX_DELAY) == pdTRUE){
        this->cmd_to_pc.push_back(str);
        xSemaphoreGive(mutex_to_pc);
    }
}

void _Serial :: sent(){
    for (;;){
        if (this->cmd_to_pc.size()>= 1){
            if(xSemaphoreTake(mutex_to_pc, portMAX_DELAY) == pdTRUE){
                Serial.println(this->cmd_to_pc.front());
                this->cmd_to_pc.pop_front();
                xSemaphoreGive(mutex_to_pc);
            }
        }
    }
}

void _Serial :: read(){
    for (;;){
        if (Serial.available()){
            if (xSemaphoreTake(this->mutex_from_pc,portMAX_DELAY) == pdTRUE){
              this->cmd_from_pc.push_back(Serial.readString());  
              xSemaphoreGive(this->mutex_from_pc);
            }
        }
    }
}

void _Serial :: register_pc_callback(void (*func)(String str)){
    this->callback = func;
}

void _Serial :: handle_pc_cmd(){
    for (;;){
        if (this->cmd_from_pc.size() >= 1){
            if (xSemaphoreTake(this->mutex_from_pc, portMAX_DELAY) == pdTRUE){
                this->callback(this->cmd_from_pc.front());
                this->cmd_from_pc.pop_front();
                xSemaphoreGive(this->mutex_from_pc);
            }
        }
    }
}