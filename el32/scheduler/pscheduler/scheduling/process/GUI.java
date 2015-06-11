package scheduling.process;

/* By: Harjot Bains
   For: MS Project, Computer Science Department, CSUS
   Term: Spring, 2002
   Advisor: Dr. Du Zhang

   Applet for Processor Scheduling
*/

import java.util.Observable;

public class GUI extends Observable
{

    public GUI()
    {
       clearChanged();
    }
    public void input(Object info)
    {
        setChanged();
        notifyObservers(info);
    }
}
