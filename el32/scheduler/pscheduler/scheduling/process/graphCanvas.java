package scheduling.process;

/* By: Harjot Bains
   For: MS Project, Computer Science Department, CSUS
   Term: Spring, 2002
   Advisor: Dr. Du Zhang

   Applet for Processor Scheduling
*/
import java.awt.Canvas;
import java.awt.Graphics;

public class graphCanvas extends Canvas
{

    private pscheduling anime;
    public graphCanvas(pscheduling anime)
    {
        this.anime = anime;
        //setSize(500,250);
    }
    public void paint(Graphics g)
    {
        anime.paintBoard(g);
    }
}
