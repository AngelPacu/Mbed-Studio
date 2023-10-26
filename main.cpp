#include "mbed.h"
#include "DigitalOut.h"
#include "algorithm"
#include "numeric"

 /*
Timer timer_fast;
Timer timer_slow;

DigitalOut ledA(LED1);
DigitalOut ledB(LED2);

void task_fast(void);
void task_slow(void);
// main() runs in its own thread in the OS


int main()
{
    timer_fast.start();
    timer_slow.start();

    while (true){
        if (timer_fast.read_ms() > 200){
            task_fast();
            timer_fast.reset();
        }

        if (timer_slow.read_ms() > 1000){
            task_slow();
            timer_slow.reset();
        }
    }
}

void task_fast(void){
    ledA = !ledA;
}

void task_slow(void){
    ledB = !ledB;
}


Timeout response;
DigitalIn btn(D4);

DigitalOut ledA(LED1);
DigitalOut ledB(LED2);
DigitalOut ledC(LED3);

void blink(){
    ledB = 1;
    ThisThread::sleep_for(500ms);
    ledB = 0;
}

int main(){
    while (true) {
        if(btn == 1){
            response.attach(&blink,3000);
            ledC = 1;
        } else{
            ledC = 0;
        }
        ledA = !ledA;
        ThisThread::sleep_for(200ms);
    }
}

Ticker time_up;

void led_switch(void);
DigitalOut ledB = LED2;

void led_switch() {
    ledB = !ledB;
}

int main() {
    time_up.attach(&led_switch,200ms);
    while (1){
        ThisThread::sleep_for(1s);
    }
}


InterruptIn btn(D4);
DigitalOut led(LED1);
DigitalOut flash(LED2);
Timer debounce;


void lsr(void){
    if(debounce.read_ms() > 2000){
        led = !led;
        debounce.reset();
    }
}

int main (){
    debounce.start();
    btn.rise(&lsr);
    while (true){
        flash = !flash;
        ThisThread::sleep_for(200ms);

    }
}


*/
#define MAX_PROCESS 3

int num_of_process=3, count, remain, time_quantum;
int remain_time[MAX_PROCESS] = {2,3,4};
// T=((2,8);(3,12);(4,16)) 
int execution_time[MAX_PROCESS] = {2,3,4};
int period[MAX_PROCESS] = {8,12,16};

//get maximum of three numbers
int max(int a,int b, int c){
        long max;
        if( a>=b && a>=c)
                max=a;
        else if(b >=a && b>=c)
                max=b;
        else if(c>=a && c>=b)
                max=c;
        return max;
}

void print_schedule(int process_list[], int cycles) {
    printf("\nScheduling:-\n\n");
    printf("Time: ");
    for (int i = 0; i < cycles; i++) {
        if (i < 10)
            printf("| 0%d ", i);
        else
            printf("| %d ", i);
    }
    printf("|\n");

    for (int i = 0; i < num_of_process; i++) {
        printf("P[%d]: ", i + 1);
        for (int j = 0; j < cycles; j++) {
            if (process_list[j] == i + 1)
                printf("|####");
            else
                printf("|    ");
        }
        printf("|\n");
    }
}


void rate_monotonic(int time) {
    float utilization = 0;
    for (int i = 0; i < num_of_process; i++) {
        utilization += (1.0 * execution_time[i]) / period[i];
    }
    int n = num_of_process;
    if (utilization > n * (pow(2, 1.0 / n) - 1)) {
        printf("\nGiven problem is not schedulable under said scheduling algorithm.\n");
        return;  // Puedes ajustar la lógica aquí para manejar este caso en Mbed.
    }

    int process_list[MAX_PROCESS] = {0};  // Asumiendo que el tamaño máximo de procesos es MAX_PROCESS
    int min = 999;
    int next_process = 0;
    for (int i = 0; i < time; i++) {
        min = 1000;
        for (int j = 0; j < num_of_process; j++) {
            if (remain_time[j] > 0) {
                if (min > period[j]) {
                    min = period[j];
                    next_process = j;
                }
            }
        }

        if (remain_time[next_process] > 0) {
            process_list[i] = next_process + 1;  // +1 for catering 0 array index.
            remain_time[next_process] -= 1;
        }

        for (int k = 0; k < num_of_process; k++) {
            if ((i + 1) % period[k] == 0) {
                remain_time[k] = execution_time[k];
                next_process = k;
            }
        }
    }
    // Muestra el resultado utilizando la comunicación serial
    printf("\nScheduling:-\n\n");
    printf("Time: ");
    for (int i = 0; i < time; i++) {
        if (i < 10)
            printf("| 0%d ", i);
        else
            printf("| %d ", i);
    }
    printf("|\n");

    for (int i = 0; i < num_of_process; i++) {
        printf("P[%d]: ", i + 1);
        for (int j = 0; j < time; j++) {
            if (process_list[j] == i + 1)
                printf("|####");
            else
                printf("|    ");
        }
        printf("|\n");
    }
}

int main(int argc, char* argv[]) {
	int option = 0;
    // T=((2,8);(3,12);(4,16)) 
	int observation_time = max(period[0],period[1],period[2]);
	rate_monotonic(observation_time);
	return 0;
}

