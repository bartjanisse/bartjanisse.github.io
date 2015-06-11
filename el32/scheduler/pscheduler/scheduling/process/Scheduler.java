package scheduling.process;

/* By: Harjot Bains
   For: MS Project, Computer Science Department, CSUS
   Term: Spring, 2002
   Advisor: Dr. Du Zhang

   Applet for Processor Scheduling
*/

import java.util.Vector;

public class Scheduler
{

    Vector readyQ;
    Vector finishQ;
    Vector Q;
    int clock;
    process P;
    process T;
    boolean idle;
    public Thread thread;
    String algorithm;
   
    pscheduling in;
    
     public Scheduler(Vector q, pscheduling i, int c)
    {
        Q = q;
        in = i;
        clock = c - 1;
        idle = true;
        readyQ = new Vector(1, 1);
        finishQ = new Vector(1, 1);
    }
     public Scheduler(Vector q, pscheduling i, int c, String algo)
    {
        Q = q;
        in = i;
        clock = c - 1;
        idle = true;
        algorithm = algo;
        readyQ = new Vector(1, 1);
        finishQ = new Vector(1, 1);
    }
    public process processready(int tick)
    {
        for(int j = 0; j < Q.size(); j++)
        {
            if (((process) Q.elementAt(j)).getArrival() <= tick)
            {
                return (process) Q.elementAt(j);
            }
        }

        return null;
    }
    public void resetQ()
    {
        readyQ.setSize(0);
        finishQ.setSize(0);
        Q.setSize(0);
        in.resetGUI();
    }
}
