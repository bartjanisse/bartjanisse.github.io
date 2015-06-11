package scheduling.process;

/* By: Harjot Bains
   For: MS Project, Computer Science Department, CSUS
   Term: Spring, 2002
   Advisor: Dr. Du Zhang

   Applet for Processor Scheduling
*/

public class Packet
{

    String proc;
    String arriv;
    String serv;
    String alg;
    public Packet(String p, String a, String s, String al)
    {
        proc = p;
        arriv = a;
        serv = s;
        alg = al;
    }
    public String getAlg()
    {
        return alg;
    }
    public String getArriv()
    {
        return arriv;
    }
    public String getProc()
    {
        return proc;
    }
    public String getServ()
    {
        return serv;
    }
}
