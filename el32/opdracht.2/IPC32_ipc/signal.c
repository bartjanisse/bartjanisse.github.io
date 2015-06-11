#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

// sigaction() is used to install a signal handler
// this can be in two different ways, dependent of a flag SA_SIGINFO in one of the parameters
// with this define we can choose between the two possibilities
#define	WITH_SA_SIGINFO

static char * 	progname;

#ifdef WITH_SA_SIGINFO

    static void
    my_sig_handler (int signum, siginfo_t * info, void * context)
    {
        char *	cause;

        printf ("\n%s(%d): signal %d received\n", progname, getpid(), signum);
        switch (signum)
        {
            case SIGRTMIN:
                break;
            case (SIGRTMAX):
                break;
        }
        switch (info->si_code)
        {
            case SI_USER:
                cause = "SI_USER";
                break;
            case SI_KERNEL:
                cause = "SI_KERNEL";
                break;
            case SI_QUEUE:
                cause = "SI_QUEUE";
                break;
            case SI_TIMER:
                cause = "SI_TIMER";
                break;
            case SI_ASYNCIO:
                cause = "SI_ASYNCIO";
                break;
            case SI_MESGQ:
                cause = "SI_MESGQ";
                break;
            case SI_SIGIO:
                cause = "SI_SIGIO";
                break;
            default:
                cause = "???";
                break;
        }
        printf ("   siginfo: si_code: %d '%s'\n"
                "            si_errno:%d\n"
                "            si_signo:%d\n"
                "            si_pid:  %d\n"
                "            si_uid:  %d\n"
                "            si_value:%d (%#x)\n",
                info->si_code, cause,
                info->si_errno,
                info->si_signo,
                info->si_pid,
                info->si_uid,
                info->si_value.sival_int, info->si_value.sival_int);
        // si_signo:%d (signal number being delivered)
        // si_pid:  %d (process id of sending process)
        // si_uid:  %d (real user id of sending process)
        // si_value:auxiliary value coming from sigqueue()
    }

#else

    static void
    my_sig_handler (int signum)
    {
        printf ("\n%s(%d): signal %d received\n", progname, getpid(), signum);
    }

#endif

static void
install_sigaction (int signo)
{
    struct sigaction 	new_action;
    struct sigaction	old_action;

    printf ("install signal handler for signal %d\n", signo);

    sigemptyset (&new_action.sa_mask);
    // 'sa_mask': additional set of signals to be blocked during execution of signal-catching function
    // so here: no additional signals will be blocked during execution
#ifdef WITH_SA_SIGINFO
    new_action.sa_sigaction = my_sig_handler;
    new_action.sa_flags = SA_RESTART | SA_SIGINFO;
    // SA_SIGINFO: use 'sa_sigaction' instead of 'sa_handler'
    // SA_RESTART: interruptible functions will restart (when signal arrives)
#else
    new_action.sa_handler = my_sig_handler;
    new_action.sa_flags   = SA_RESTART;
#endif

    if (sigaction (signo, &new_action, NULL) == -1)
    {
        perror ("sigaction(set-my-sig)");
    }
}

int main(int argc, char * argv[])
{
    char                line[80];
    int                 choice = ' ';
    int                 sig;
    int                 how;
    sigval_t            value;
    int	                target_pid;
    sigset_t            new_sigset;
    sigset_t            old_sigset;
    struct sigevent     new_sigevent;
    struct itimerspec   new_timerspec;
    timer_t             my_timer; /* Contains the ID of the created timer */

    progname = argv[0];
    printf ("%s(%d): started (my_sig=%#x)\n", progname, getpid(), (unsigned int) my_sig_handler);
    //printf ("SIGRTMIN/SIGRTMAX:%d/%d\n", SIGRTMIN, SIGRTMAX);

    install_sigaction (SIGRTMIN);

    while (choice != 'Q')
    {
        printf ("\nEnter option (h for help): ");
        fgets (line, sizeof (line), stdin);
        choice = line[0];
        printf ("\n");

        switch (choice)
        {
            case 'a':
                printf ("Enter signal number: ");
                fgets (line, sizeof (line), stdin);
                sscanf (line, "%i", &sig);

                install_sigaction (sig);
                break;
            case 'k':
                printf ("Enter signal number: ");
                fgets (line, sizeof (line), stdin);
                sscanf (line, "%i", &sig);

                printf ("Enter target pid: ");
                fgets (line, sizeof (line), stdin);
                sscanf (line, "%i", &target_pid);

                printf ("calling kill(%d,%d)\n", target_pid, sig);
                if (kill (target_pid, sig) != 0)
                {
                    perror ("kill()");
                }
                break;
            case 'q':
                printf ("Enter signal number: ");
                fgets (line, sizeof (line), stdin);
                sscanf (line, "%i", &sig);

                printf ("Enter target pid: ");
                fgets (line, sizeof (line), stdin);
                sscanf (line, "%i", &target_pid);

                printf ("Enter auxiliary value: ");
                fgets (line, sizeof (line), stdin);
                sscanf (line, "%i", &value.sival_int);

                printf ("calling sigqueue(%d,%d,%d)\n",
                        target_pid, sig, value.sival_int);
                if (sigqueue (target_pid, sig, value) != 0)
                {
                    perror ("sigqueue()");
                }
                break;
            case 'c':
                printf ("Enter signal number: ");
                fgets (line, sizeof (line), stdin);
                sscanf (line, "%i", &new_sigevent.sigev_signo);

                printf ("Enter auxiliary value: ");
                fgets (line, sizeof (line), stdin);
                sscanf (line, "%i", &new_sigevent.sigev_value.sival_int);

                new_sigevent.sigev_notify = SIGEV_SIGNAL;	// JG,100108: port to Kubuntu
                printf ("calling timer_create(CLOCK_REALTIME,%d,%d)\n",
                        new_sigevent.sigev_signo,
                        new_sigevent.sigev_value.sival_int);
                if (timer_create (CLOCK_REALTIME, &new_sigevent, &my_timer) != 0)
                {
                    perror ("timer_create()");
                }
                break;

            case 's':
                printf ("Enter initial time (in seconds): ");
                fgets (line, sizeof (line), stdin);
                sscanf (line, "%i", (int *) &new_timerspec.it_value.tv_sec);

                printf ("Enter repetition time (in seconds) (0 == one-shot): ");
                fgets (line, sizeof (line), stdin);
                sscanf (line, "%i", (int *) &new_timerspec.it_interval.tv_sec);

                new_timerspec.it_value.tv_nsec = 0;
                new_timerspec.it_interval.tv_nsec = 0;
                printf ("calling timer_settime([%ld,%ld] [%ld,%ld]);\n",
                        new_timerspec.it_value.tv_sec,
                        new_timerspec.it_value.tv_nsec,
                        new_timerspec.it_interval.tv_sec,
                        new_timerspec.it_interval.tv_nsec);
                if (timer_settime(my_timer, 0, &new_timerspec, NULL) != 0)
                {
                    perror ("timer_create()");
                }
                break;

            case 'h':
            case '?':
                printf ("options:\n"
                        "========\n"
                        "    a: sigaction     (install new signal handler)\n"
                        "    q: sigqueue      (send a signal)\n"
                        "    k: kill          (send a signal)\n"
                        "    c: timer_create  (create a timer and attach a signal to it)\n"
                        "    s: timer_settime (let this timer run)\n"
                        "    h: help\n"
                        "    Q: quit\n");
                break;
            case 'Q':
                break;
            default:
                printf ("Invalid option '%c' (%#x)\n", choice, choice);
                break;
        }
    }
    printf ("%s(%d):end\n\n", argv[0], getpid());
    return (0);
}

