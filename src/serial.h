#include "Arduino.h"
#include "list"
//#include "freertos\FreeRTOS.h"
//#include "freertos\task.h"
#include "freertos\semphr.h"

#include "thread"


#ifndef _Serial_H
#define _Serial_H

class _Serial{

    private:
        std::list <String> cmd_to_pc;
        SemaphoreHandle_t mutex_to_pc;

        std::list <String> cmd_from_pc;
        SemaphoreHandle_t mutex_from_pc;

        void (*callback)(String str);

        void sent();
        void read();
        void handle_pc_cmd();

    public:
        _Serial();
        void write(String str);
        void begin(int baud_rate=9600);
        void register_pc_callback(void (*func)(String str));
};

#endif