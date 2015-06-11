package scheduling.process;

/* By: Harjot Bains
For: MS Project, Computer Science Department, CSUS
Term: Spring, 2002
Advisor: Dr. Du Zhang

Applet for Processor Scheduling
 */
import java.util.Vector;

public class runAlgorithm extends Scheduler
        implements Runnable
{

    public runAlgorithm (Vector q, pscheduling i, int c, String algo)
    {
        super(q, i, c, algo);
        super.thread = new Thread(this, algo);
        super.thread.start();
    }

    public void run ()
    {
        int all = super.Q.size();
        int interval = 0;
        int next = 0;
        boolean interrupt = false;
        do
        {
            super.clock++;
            for (super.T = processready(super.clock); super.T != null; super.T = processready(super.clock))
            {
                super.readyQ.addElement(super.T);
                super.Q.removeElement(super.T);
                super.in.upstatus("Time " + super.T.getArrival() + ": Process " + super.T.getName() + " ready.");
                try
                {
                    Thread.sleep(10000L / in.nSpeed);
                }
                catch (InterruptedException e)
                {
                }
            }

            if (super.algorithm == "FCFS")
            {
                if (super.idle)
                {
                    if (super.readyQ.size() == 0)
                    {
                        continue;
                    }
                    super.idle = false;
                    super.P = (process) super.readyQ.firstElement();
                    super.readyQ.removeElement(super.P);
                }
                super.P.servicing();
                super.in.drawbar(super.P, super.clock);
                super.in.upstatus("Time " + super.clock + ": Serving process " + super.P.getName() + ".");
                try
                {
                    Thread.sleep(10000L / in.nSpeed);
                }
                catch (InterruptedException ex)
                {
                }
                if (super.P.getTminus() == 0)
                {
                    super.in.upstatus("Time " + (super.clock + 1) + ": Process " + super.P.getName() + " done.");
                    try
                    {
                        Thread.sleep(10000L / in.nSpeed);
                    }
                    catch (InterruptedException e)
                    {
                    }
                    super.P.report(super.clock + 1);
                    super.finishQ.addElement(super.P);
                    super.idle = true;
                }

            } //end if FCFS
            if (super.algorithm == "RR1")
            {
                if (super.idle || interrupt)
                {
                    if (interrupt)
                    {
                        interrupt = false;
                        interval = 0;
                    }
                    if (super.readyQ.size() == 0)
                    {
                        continue;
                    }
                    if (next < super.readyQ.size() - 1)
                    {
                        next++;
                    }
                    else
                    {
                        next = 0;
                    }
                    super.P = (process) super.readyQ.elementAt(next);
                    super.idle = false;
                }
                super.P.servicing();
                interval++;
                super.in.drawbar(super.P, super.clock);
                super.in.upstatus("Time " + super.clock + ":Serving process " + super.P.getName() + ".");
                try
                {
                    Thread.sleep(10000L / in.nSpeed);
                }
                catch (InterruptedException ex)
                {
                }
                if (super.P.getTminus() == 0)
                {
                    super.in.upstatus("Time " + (super.clock + 1) + ":Process " + super.P.getName() + " done.");
                    try
                    {
                        Thread.sleep(10000L / in.nSpeed);
                    }
                    catch (InterruptedException e)
                    {
                    }
                    super.P.report(super.clock + 1);
                    super.finishQ.addElement(super.P);
                    super.readyQ.removeElement(super.P);
                    next--;
                    super.idle = true;
                    interval = 0;
                }
                else
                {
                    if (interval == 1)
                    {
                        interrupt = true;
                    }
                }
            }//end if RR1

            if (super.algorithm == "RR4")
            {
                if (super.idle || interrupt)
                {
                    if (interrupt)
                    {
                        interrupt = false;
                        interval = 0;
                    }
                    if (super.readyQ.size() == 0)
                    {
                        continue;
                    }
                    if (next < super.readyQ.size() - 1)
                    {
                        next++;
                    }
                    else
                    {
                        next = 0;
                    }
                    super.P = (process) super.readyQ.elementAt(next);
                    super.idle = false;
                }
                super.P.servicing();
                interval++;
                super.in.drawbar(super.P, super.clock);
                super.in.upstatus("Time " + super.clock + ":Serving process " + super.P.getName() + ".");
                try
                {
                    Thread.sleep(10000L / in.nSpeed);
                }
                catch (InterruptedException ex)
                {
                }
                if (super.P.getTminus() == 0)
                {
                    super.in.upstatus("Time " + (super.clock + 1) + ":Process " + super.P.getName() + " done.");
                    try
                    {
                        Thread.sleep(10000L / in.nSpeed);
                    }
                    catch (InterruptedException e)
                    {
                    }
                    super.P.report(super.clock + 1);
                    super.finishQ.addElement(super.P);
                    super.readyQ.removeElement(super.P);
                    next--;
                    super.idle = true;
                    interval = 0;
                }
                else
                {
                    if (interval == 4)
                    {
                        interrupt = true;
                    }
                }
            }//end if RR4

            if (super.algorithm == "SPN")
            {
                if (super.idle)
                {
                    if (super.readyQ.size() == 0)
                    {
                        continue;
                    }
                    super.idle = false;
                    super.P = (process) super.readyQ.firstElement();
                    int sp = super.P.getService();
                    for (int j = 1; j < super.readyQ.size(); j++)
                    {
                        if (sp > ((process) super.readyQ.elementAt(j)).getService())
                        {
                            super.P = (process) super.readyQ.elementAt(j);
                            sp = super.P.getService();
                        }
                    }
                    super.readyQ.removeElement(super.P);
                }
                super.P.servicing();
                super.in.drawbar(super.P, super.clock);
                super.in.upstatus("Time " + super.clock + ":Serving process " + super.P.getName() + ".");
                try
                {
                    Thread.sleep(10000L / in.nSpeed);
                }
                catch (InterruptedException ex)
                {
                }
                if (super.P.getTminus() == 0)
                {
                    super.in.upstatus("Time " + (super.clock + 1) + ":Process " + super.P.getName() + " done.");
                    try
                    {
                        Thread.sleep(10000L / in.nSpeed);
                    }
                    catch (InterruptedException e)
                    {
                    }
                    super.P.report(super.clock + 1);
                    super.finishQ.addElement(super.P);
                    super.idle = true;
                }
            } //end if spn

            if (super.algorithm == "SRT")
            {
                if (super.idle || interrupt)
                {
                    if (interrupt)
                    {
                        interrupt = false;
                        interval = 0;
                    }
                    if (super.readyQ.size() == 0)
                    {
                        continue;
                    }
                    if (next < super.readyQ.size() - 1)
                    {
                        next++;
                    }
                    else
                    {
                        next = 0;
                    }
                    super.P = (process) super.readyQ.firstElement();


                    int tl = super.P.getTminus();
                    for (int j = 1; j < super.readyQ.size(); j++)
                    {
                        if (tl > ((process) super.readyQ.elementAt(j)).getTminus())
                        {
                            super.P = (process) super.readyQ.elementAt(j);
                            tl = super.P.getTminus();
                        }
                    }
                    super.idle = false;
                }
                super.P.servicing();
                interval++;
                super.in.drawbar(super.P, super.clock);
                super.in.upstatus("Time " + super.clock + ":Serving process " + super.P.getName() + ".");
                try
                {
                    Thread.sleep(10000L / in.nSpeed);
                }
                catch (InterruptedException ex)
                {
                }
                if (super.P.getTminus() == 0)
                {
                    super.in.upstatus("Time " + (super.clock + 1) + ":Process " + super.P.getName() + " done.");
                    try
                    {
                        Thread.sleep(10000L / in.nSpeed);
                    }
                    catch (InterruptedException e)
                    {
                    }
                    super.P.report(super.clock + 1);
                    super.finishQ.addElement(super.P);
                    super.readyQ.removeElement(super.P);
                    next--;
                    super.idle = true;
                    interval = 0;
                }
                else
                {
                    if (interval == 1)
                    {
                        interrupt = true;
                    }
                }
            }//end if SRT

        }
        while (super.finishQ.size() < all);
        super.in.upstatus("Algorithm finished.");
        super.in.report(super.finishQ, super.algorithm);
        super.in.run.setEnabled(true);
        super.in.resetGUI();
    }
}
