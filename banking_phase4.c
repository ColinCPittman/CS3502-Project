#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h> // Required for timespec and clock_gettime
#include <errno.h> // Required for ETIMEDOUT

static int accountBalance1 = 10000;
static int accountBalance2 = 5000;
static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void *transferAccount1toAccount2(void *transferAmount) {
    int amount = *((int *) transferAmount);
    printf("Thread 1 about to attempt to acquire access to lock on account1.\n");
    fflush(stdout);

    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += 1; 

    int accessMutex1 = pthread_mutex_timedlock(&mutex1, &timeout);
    if (accessMutex1 != 0) {
        if (accessMutex1 == ETIMEDOUT) {
            printf("Thread 1 timed out waiting for lock on account 1.\n");
            fflush(stdout);
        } else {
            printf("Error locking mutex1 in transferAccount1toAccount2: %d\n", accessMutex1);
            fflush(stdout);
        }
        return NULL;
    }
    printf("Thread 1 acquired lock on account 1.\n");
    fflush(stdout);
    usleep(100);

    printf("Thread 1 about to attempt to acquire access to lock on account 2.\n");
    fflush(stdout);

    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += 1; 

    int accessMutex2 = pthread_mutex_timedlock(&mutex2, &timeout);
    if (accessMutex2 != 0) {
        if (accessMutex2 == ETIMEDOUT) {
            printf("Thread 1 timed out waiting for lock on account 2.\n");
            fflush(stdout);
        } else {
            printf("Error locking mutex2 in transferAccount1toAccount 2: %d\n", accessMutex2);
            fflush(stdout);
        }
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

    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += 1; 

    int accessMutex2 = pthread_mutex_timedlock(&mutex2, &timeout);
    if (accessMutex2 != 0) {
        if (accessMutex2 == ETIMEDOUT) {
            printf("Thread 2 timed out waiting for lock on account 2.\n");
            fflush(stdout);
        } else {
            printf("Error locking mutex2 in transferAccount2toAccount1: %d\n", accessMutex2);
            fflush(stdout);
        }
        return NULL;
    }
    printf("Thread 2 acquired lock on account 2.\n");
    fflush(stdout);
    usleep(100);

    printf("Thread 2 about to attempt to acquire access to lock on account 1.\n");
    fflush(stdout);

    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += 1; 

    int accessMutex1 = pthread_mutex_timedlock(&mutex1, &timeout);
    if (accessMutex1 != 0) {
        if (accessMutex1 == ETIMEDOUT) {
            printf("Thread 2 timed out waiting for lock on account 1.\n");
            fflush(stdout);
        } else {
            printf("Error locking mutex1 in transferAccount2toAccount1: %d\n", accessMutex1);
            fflush(stdout);
        }
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
void *fixedTransferAccount2toAccount1(void *transferAmount) {
    int amount = *((int *) transferAmount);
    printf("Thread 2 about to attempt to acquire access to lock on account 2.\n");
    fflush(stdout);

    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += 1; 

    int accessMutex1 = pthread_mutex_timedlock(&mutex1, &timeout);
    if (accessMutex1 != 0) {
        if (accessMutex1 == ETIMEDOUT) {
            printf("Thread 2 timed out waiting for lock on account 2.\n");
            fflush(stdout);
        } else {
            printf("Error locking mutex2 in transferAccount2toAccount1: %d\n", accessMutex1);
            fflush(stdout);
        }
        return NULL;
    }
    printf("Thread 2 acquired lock on account 2.\n");
    fflush(stdout);
    usleep(100);

    printf("Thread 2 about to attempt to acquire access to lock on account 1.\n");
    fflush(stdout);

    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += 1; 

    int accessMutex2 = pthread_mutex_timedlock(&mutex2, &timeout);
    if (accessMutex2 != 0) {
        if (accessMutex2 == ETIMEDOUT) {
            printf("Thread 2 timed out waiting for lock on account 1.\n");
            fflush(stdout);
        } else {
            printf("Error locking mutex1 in transferAccount2toAccount1: %d\n", accessMutex2);
            fflush(stdout);
        }
        pthread_mutex_unlock(&mutex1);
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

    
    pthread_create(&thread1, NULL, transferAccount1toAccount2, &transferAmount);
    pthread_create(&thread2, NULL, fixedTransferAccount2toAccount1, &transferAmount);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Final account 1 balance is %.2f.\n", accountBalance1 / 100.00);
    printf("Final account 2 balance is %.2f.\n", accountBalance2 / 100.00);

    return 0;
}
