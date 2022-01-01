#include <iostream>
#include <string>
#include <pthread.h>
#include <fstream>


#define ll long long
void *mult(void *p);
using namespace std;
ll numOfOdd = 0, numOfEven = 0, totalCells = 0,n;

string output="";

ll *A, *B, *C;
pthread_mutex_t lock1, lock2, lock3, lock4;
struct Args
{
    ll start;
    ll len;
    ll id;
} * args;
int main(int argc, char **argv)
{
    ofstream out("output.txt");

    freopen("in.txt", "r", stdin);
    cin >> n;

    A = new ll[n * n];
    B = new ll[n * n];
    C = new ll[n * n];

    ll t = stoll(argv[1]);
    if (t > n * n)
        t = n * n;

    pthread_t *threads = new pthread_t[t];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> A[i * n + j];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> B[i * n + j];

    ll cells = n * n / t, e = n * n % t;
    for (int i = 0; i < t; i++)
    {
        args = new struct Args;
        args->id = i;
        if (i == 0)
        {
            args->start = 0;
            args->len = cells + e;
        }
        else
        {
            args->start = i * cells + e;
            args->len = cells;
        }

        pthread_create(&threads[i], NULL, mult, (void *)args);
    }
    for (int i = 0; i < t; i++)
    {
        pthread_join(threads[i], NULL);
    }

    output+= "numOfEven=" + to_string(numOfEven) + ", numOfOdd=" + to_string(numOfOdd) + ", totalCells=" + to_string(totalCells)+"\n";
    out << output;
}

void *mult(void *p)
{
    ofstream out("output.txt");

    struct Args *r = (struct Args *)p;
    ll start = r->start;
    ll len = r->len;
    pthread_mutex_lock(&lock4);

    output+= "ThreadID=" +to_string (r->id) + ", startLoop="+ to_string(start) + ", endLoop=" +to_string (start+len)  +  "\n";
    pthread_mutex_unlock(&lock4);

    for (int i = start; i < start + len; ++i)
    {
        ll tmp = 0;
        ll current_row = i / n * n;
        ll current_clm = i % n;
        for (int j = 0; j < n; j++)
            tmp += A[current_row + j] * B[current_clm + j * n];
        C[i] = tmp;
        if (tmp % 2 == 0)
        {
            pthread_mutex_lock(&lock1);
            numOfEven++;
            pthread_mutex_unlock(&lock1);
        }
        else
        {
            pthread_mutex_lock(&lock2);
            numOfOdd++;
            pthread_mutex_unlock(&lock2);
        }
        pthread_mutex_lock(&lock3);
        totalCells++;
        pthread_mutex_unlock(&lock3);
    }
}
