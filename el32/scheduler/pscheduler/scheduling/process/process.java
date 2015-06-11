package scheduling.process;

/* By: Harjot Bains
   For: MS Project, Computer Science Department, CSUS
   Term: Spring, 2002
   Advisor: Dr. Du Zhang

   Applet for Processor Scheduling
*/

public class process
{

    String name;
    int ArrivalTime;
    int ServiceTime;
    int FinishTime;
    int TimeLeft;
    int Tq;
    int processNum;
    double Tqs;
    public process(String n, int a, int s, int processNum)
    {
        this.processNum = processNum;
        name = n;
        ArrivalTime = a;
        ServiceTime = TimeLeft = s;
    }
    public int getArrival()
    {
        return ArrivalTime;
    }
    public int getFinish()
    {
        return FinishTime;
    }
    public String getName()
    {
        return name;
    }
    public int getProcessNum()
    {
        return processNum;
    }
    public int getService()
    {
        return ServiceTime;
    }
    public int getTminus()
    {
        return TimeLeft;
    }
    public double getTq()
    {
        return (double)Tq;
    }
    public double getTqs()
    {
        return Tqs;
    }
    public void report(int t)
    {
        FinishTime = t;
        Tq = FinishTime - ArrivalTime;
        Tqs = Math.round(Tq / ServiceTime);
    }
    public void servicing()
    {
        TimeLeft--;
    }
}
