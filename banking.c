#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

static int accountBalance = 10000;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *deposit(void *depositAmt) { // void because pthread expects a void pointer
    int amount = *((int *) depositAmt);
    int access = pthread_mutex_lock(&mutex);
    if(access != 0) {
        printf("Error locking mutex.\n");
        return NULL;
    }
    for (int i = 0; i < 20; i++) {
        accountBalance += amount;
        printf("Depositing %.2f into account. New balance is %.2f.\n", amount / 100.0, accountBalance / 100.0);
        usleep(100); //this increases the likelyhood of race-conditions occuring without thread safety
    }
    access = pthread_mutex_unlock(&mutex);
    return NULL;
}

void *withdraw(void *withdrawAmt) {
    int amount = *((int *) withdrawAmt);
    int access = pthread_mutex_lock(&mutex);
    if(access != 0) {
        printf("Error locking mutex.\n");
        return NULL;
    }
    for(int i = 0; i < 20; i++) {
        accountBalance -= amount;
        printf("Withdrawn %.2f into account. New balance is %.2f.\n", amount / 100.0, accountBalance / 100.0);
        usleep(100);
    }
    access = pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t threads[100];
    int depositAmt = 5000;
    int withdrawAmt = 2000;
    for (int i = 0; i < 50; i++) {
        pthread_create(&threads[i], NULL, deposit, &depositAmt);
    }

    for (int i = 5; i < 100; i++) {
        pthread_create(&threads[i], NULL, withdraw, &withdrawAmt);
    }

    for (int i = 0; i < 100; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final account balance is %.2f.\n", accountBalance / 100.00);
    return 0;
}


