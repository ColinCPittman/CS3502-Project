#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

static int accountBalance1 = 10000;
static int accountBalance2 = 5000;  
static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; 
static pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER; 

void *transferAccount1toAccount2(void *transferAmount) {
    int amount = *((int *) transferAmount);
    printf("Thread 1 about to attempt to acquire access to lock on account1.\n");
    fflush(stdout); // these flush calls are so that the output can be displayed before the deadlock holds up the program resulting in the output buffer never being flushed out.
    int accessMutex1 = pthread_mutex_lock(&mutex1); 
    if (accessMutex1 != 0) {
        printf("Error locking mutex1 in transferAccount1toAccount2.\n");
        fflush(stdout);
        return NULL;
    }
    printf("Thread 1 acquired lock on account 1.\n");
    fflush(stdout);
    usleep(100); // taking up extra time so deadlock occurs more consistently

    printf("Thread 1 about to attempt to acquire access to lock on account 2.\n");
    fflush(stdout);
    int accessMutex2 = pthread_mutex_lock(&mutex2); 
    if (accessMutex2 != 0) {
        printf("Error locking mutex2 in transferAccount1toAccount 2.\n");
        fflush(stdout);
        pthread_mutex_unlock(&mutex1);
        return NULL;
    }
    printf("Thread 1 acquired lock on account 2.\n");
    fflush(stdout);

    accountBalance1 -= amount;
    accountBalance2 += amount;

    printf("Thread 1 transferred %.2f from account 1 to account 2. Account 1 balance: %.2f, Account 2 balance: %.2f.\n",
           amount / 100.0, accountBalance1 / 100.0, accountBalance2 / 100.0);

    pthread_mutex_unlock(&mutex2); 
    pthread_mutex_unlock(&mutex1); 

    return NULL;
}

void *transferAccount2toAccount1(void *transferAmount) { 
    int amount = *((int *) transferAmount);
    printf("Thread 2 about to attempt to acquire access to lock on account 2.\n");
    fflush(stdout);
    int accessMutex2 = pthread_mutex_lock(&mutex2); // like the example shown in chatper 6.8.1, the orders are swapped here
    if (accessMutex2 != 0) {
        printf("Error locking mutex2 in transferAccount2toAccount1.\n");
        fflush(stdout);
        return NULL;
    }
    printf("Thread 2 acquired lock on account 2.\n");
    fflush(stdout);
    usleep(100);

    printf("Thread 2 about to attempt to acquire access to lock on account 1.\n");
    fflush(stdout);
    int accessMutex1 = pthread_mutex_lock(&mutex1); 
    if (accessMutex1 != 0) {
        printf("Error locking mutex1 in transferAccount2toAccount1.\n");
        fflush(stdout);
        pthread_mutex_unlock(&mutex2); 
        return NULL;
    }
    printf("Thread 2 acquired lock on account 1.\n");
    fflush(stdout);

    accountBalance2 -= amount;
    accountBalance1 += amount;

    printf("Thread 2 transferred %.2f from account2 to account1. Account2 balance: %.2f, Account1 balance: %.2f.\n",
           amount / 100.0, accountBalance2 / 100.0, accountBalance1 / 100.0);

    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2); 

    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int transferAmount = 1000; 

    pthread_create(&thread1, NULL, transferAccount1toAccount2, &transferAmount);
    pthread_create(&thread2, NULL, transferAccount2toAccount1, &transferAmount);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Final account 1 balance is %.2f.\n", accountBalance1 / 100.00);
    printf("Final account 2 balance is %.2f.\n", accountBalance2 / 100.00);

    return 0;
}
