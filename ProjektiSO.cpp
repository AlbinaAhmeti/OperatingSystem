#include <bits/stdc++.h>  //<bits/stdc++.h> është një skedar me kokë. Ky skedar përfshin të gjithë bibliotekën standarde. Ndonjëherë në disa konkurse kodimi, kur duhet të kursejmë kohë gjatë zgjidhjes, atëherë përdorimi i këtij skedari të kokës është i dobishëm. Duke përdorur këtë skedar kokë, ai do të përfshijë shumë skedarë, ndonjëherë që mund të mos kërkohen në program. Kështu që mund të rrisë kohën e përpilimit dhe madhësinë e programit. Për këtë skedar kokë, çdo herë që përpiluesi përpiqet të importojë titujt në mënyrë rekursive sa herë që përpilohet kodi.
#include <pthread.h>	//Skedari i kokës pthread.h përmban deklaratat e funksioneve dhe pasqyrimet për ndërfaqet e filetimit dhe përcakton një numër konstantesh të përdorura nga ato funksione. Kreu përfshin skemën.
#include <unistd.h>		//unistd. h është emri i skedarit të kokës që ofron akses në API të sistemit operativ POSIX. ... P.sh. Në Cygwin, një skedar me kokë mund të gjendet në /usr/include që nën-përfshin një skedar me të njëjtin emër në /usr/include/sys.
using namespace std;
 
#define N 10
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N
 
// Philosopher index
int phil[N];
int times = 200;
class monitor {
 
    // state of the philosopher
    int state[N];
 
    // Philosopher condition variable
    pthread_cond_t phcond[N];
 
    // mutex variable for synchronization
    pthread_mutex_t condLock;
 public:
    // Test for the desired condition
    // i.e. Left and Right philosopher are not reading
    void test(int phnum)
    {
 
        if (state[(phnum + 1) % 5] != EATING
            and state[(phnum + 4) % 5] != EATING
            and state[phnum] == HUNGRY) {
            state[phnum] = EATING;
 
            pthread_cond_signal(&phcond[phnum]);
        }
    }
 
    // Take Fork function
    void take_fork(int phnum)
    {
 
        pthread_mutex_lock(&condLock);
         // Indicates it is hungry
        state[phnum] = HUNGRY;
 
        // test for condition
        test(phnum);
 
        // If unable to eat.. wait for the signal
        if (state[phnum] != EATING) {
            pthread_cond_wait(&phcond[phnum], &condLock);
        }
        cout << "Philosopher " << phnum << " is Eating"
             << endl;
 
        pthread_mutex_unlock(&condLock);
    }
 
    // Put Fork function
    void put_fork(int phnum)
    {
         pthread_mutex_lock(&condLock);
 
        // Indicates that I am thinking
        state[phnum] = THINKING;
 
        test(RIGHT);
        test(LEFT);
 
        pthread_mutex_unlock(&condLock);
    }
 
    // constructor
    monitor()
    {
 
        for (int i = 0; i < N; i++) {
            state[i] = THINKING;
        }
 
        for (int i = 0; i < N; i++) {
            pthread_cond_init(&phcond[i], NULL);
        }
          pthread_mutex_init(&condLock, NULL);
    }
 
    // destructor
    ~monitor()
    {
 
        for (int i = 0; i < N; i++) {
            pthread_cond_destroy(&phcond[i]);
        }
 
        pthread_mutex_destroy(&condLock);
    }
}
 
// Global Object of the monitor
phil_object;
 
void* philosopher(void* arg)
{
    int c = 0;
    while (c < times) {
        int i = *(int*)arg;
         sleep(1);
        phil_object.take_fork(i);
        sleep(0.5);
        phil_object.put_fork(i);
        c++;
    }
}
 
int main()
{
 
    // Declaration...
    pthread_t thread_id[N];
    pthread_attr_t attr;
 
    // Initialization...
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,
                                PTHREAD_CREATE_JOINABLE);
 
    for (int i = 0; i < N; i++) {
        phil[i] = i;
    }
    // Creating...
    for (int i = 0; i < N; i++) {
        pthread_create(&thread_id[i], &attr, philosopher,
                       &phil[i]);
        cout << "Philosopher " << i + 1 << " is thinking..."
             << endl;
    }
 
    // Joining....
    for (int i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }
 
    // Destroying
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
 
    return 0;
}
