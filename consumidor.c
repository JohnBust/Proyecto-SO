#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include "enviolib.h"

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg) {
    printf("Mensaje recibido: %s\n", (char *)msg->payload);
    conectar_bd("localhost", "root", "tesoem", "Tesoem");
    escribir_bd((char *)msg->payload);
}

int main() {
    mosquitto_lib_init();

    struct mosquitto *mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
        fprintf(stderr, "Error inicializando Mosquitto\n");
        return 1;
    }

    mosquitto_message_callback_set(mosq, on_message);

    if (mosquitto_connect(mosq, "localhost", 1883, 60) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error conectando al broker Mosquitto\n");
        return 1;
    }

    mosquitto_subscribe(mosq, NULL, "estudiantes", 0);

    mosquitto_loop_forever(mosq, -1, 1);

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
