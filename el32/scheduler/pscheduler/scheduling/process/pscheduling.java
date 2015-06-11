package scheduling.process;

/* By: Harjot Bains
For: MS Project, Computer Science Department, CSUS
Term: Spring, 2002
Advisor: Dr. Du Zhang

Applet for Processor Scheduling
 */
import java.applet.*;
import java.awt.*;
import java.util.Vector;
import java.awt.event.*;
import java.util.*;

import javax.swing.*;

/**
 * Insert the type's description here.
 * Creation date: (2/20/2002 1:06:20 PM)
 * @author: 
 */
public class pscheduling extends Applet implements ActionListener
{

    TextArea proc;
    TextArea arriv;
    TextArea serv;
    Panel knobs, organize1, organize2, organize3;
    Panel blayout;
    Panel panel1, panel2, panel2a, panel2b, panel3, panel3a, panel3b, panel4, panel5;
    Choice alg;
    CheckboxGroup functions;
    Checkbox fun[];
    Packet info;
    GUI vigil;
    String PrevBut;
    Label title;
    graphCanvas board;
    TextArea statusLine;
    Label algTitle, label1, label2;
    Graphics gr;
    Vector oldL;
    Vector grid;
    int inc1;
    int inc2;
    int hbuf;
    int hinc;
    int vinc;
    TextArea pad;
    Vector out;
    String P;
    String A;
    String S;
    String F;
    String Tq;
    String Tqs;
    Vector Q;
    Scheduler scheduleAlgorithm;
    Scrollbar barSpeed;
    Label labelSpeed;
    Label labelSpeedStatus;
    int nSpeed = 50;
    Button run;

    public void actionPerformed (ActionEvent evt)
    {
        if (getParms())
        {
            cleargrid();
            int click = setgrid(Q);
            run.setEnabled(false);

            if (evt.getActionCommand().equals("run"))
            {
                if (alg.getSelectedItem().equals("FCFS"))
                {
                    setTitle("First Come First Serve Scheduling");
                    //scheduleAlgorithm = new FCFS(Q, this, click);
                    scheduleAlgorithm = new runAlgorithm(Q, this, click, "FCFS");
                }
                if (alg.getSelectedItem().equals("RR1"))
                {
                    setTitle("Round Robin, q = 1");
                    //scheduleAlgorithm = new RR1(Q, this, click);
                    scheduleAlgorithm = new runAlgorithm(Q, this, click, "RR1");
                }
                if (alg.getSelectedItem().equals("RR4"))
                {
                    setTitle("Round Robin, q = 4");
                    //scheduleAlgorithm = new RR4(Q, this, click);
                    scheduleAlgorithm = new runAlgorithm(Q, this, click, "RR4");
                }
                if (alg.getSelectedItem().equals("SPN"))
                {
                    setTitle("Shortest Process Next");
                    //scheduleAlgorithm = new SPN(Q, this, click);
                    scheduleAlgorithm = new runAlgorithm(Q, this, click, "SPN");
                }
                if (alg.getSelectedItem().equals("SRT"))
                {
                    setTitle("Shortest Remaining Time");
                    //scheduleAlgorithm = new SRT(Q, this, click);
                    scheduleAlgorithm = new runAlgorithm(Q, this, click, "SRT");
                }

            }
        }
        else
        {
            resetGUI();
            Q.removeAllElements();
        }


    }//end actionPerformed

    private void buffer (String p, String a, String s, String f, String tq, String tqs)
    {
        int max = Math.max(P.length(), Math.max(A.length(), Math.max(S.length(), Math.max(F.length(), Math.max(Tq.length(), Tqs.length())))));
        max += 5;
        P = space(P, max);
        A = space(A, max);
        S = space(S, max);
        F = space(F, max);
        Tq = space(Tq, max);
        Tqs = space(Tqs, max);
    }

    public void cleargrid ()
    {
        gr = board.getGraphics();
        gr.clearRect(0, 0, board.getSize().width, board.getSize().height);
    }

    private void display ()
    {
        P = "Process";
        A = "Arrival Time";
        S = "Service Time";
        F = "Finish Time";
        Tq = "Turnaround Time";
        Tqs = "Tq/Ts";
        buffer(P, A, S, F, Tq, Tqs);
        for (int j = 0; j < out.size(); j++)
        {
            process temp = (process) out.elementAt(j);
            P += temp.getName();
            A += temp.getArrival();
            S += temp.getService();
            F += temp.getFinish();
            Tq += temp.getTq();
            Tqs += temp.getTqs();
            buffer(P, A, S, F, Tq, Tqs);
        }

        pad.append(P + "\n" + A + "\n" + S + "\n" + F + "\n" + Tq + "\n" + Tqs + "\n");
    }

    public void drawbar (process P, int t)
    {
        grid.addElement(new Integer(P.getProcessNum()));
        board.repaint();
    }

    /**
     * Returns information about this applet.
     * @return a string of information about this applet
     */
    public String getAppletInfo ()
    {
        return "pscheduling\n" +
                "\n" +
                "Insert the type's description here.\n" +
                "Creation date: (2/20/2002 1:06:20 PM)\n" +
                "@author: \n" +
                "";
    }

    private boolean getParms ()
    {
        Packet in;
        info = new Packet(proc.getText(), arriv.getText(), serv.getText(), "run");
        String mark = "\n";
        boolean empty = false;
        String t1 = info.getProc();
        String t2 = info.getArriv();
        int len = t2.length();
        int proceed = 0;
        for (int i = 0; i < len; i++)
        {
            if (t2.charAt(i) == '0')
            {
                proceed = 1;
            }
        }

        if (proceed == 0)
        {
            upstatus("There should be atleast one case with arrival time = 0");
            return false;
        }
        String t3 = info.getServ();
        StringTokenizer t1tokens = new StringTokenizer(t1, "\n");
        StringTokenizer t2tokens = new StringTokenizer(t2, "\n");
        StringTokenizer t3tokens = new StringTokenizer(t3, "\n");
        try
        {
            int count = 0;
            do
            {
                String name = t1tokens.nextToken();
                int a = Integer.parseInt(t2tokens.nextToken());
                int s = Integer.parseInt(t3tokens.nextToken());
                process temp = new process(name, a, s, count);
                Q.addElement(temp);
                count++;
            }
            while (t1tokens.hasMoreTokens() && t2tokens.hasMoreTokens() && t3tokens.hasMoreTokens());
        }
        catch (NumberFormatException nfe)
        {
            upstatus("ERROR: Time " + nfe.getMessage() + " is not an integer.");
            return false;
        }
        if (t1tokens.hasMoreTokens() || t2tokens.hasMoreTokens() || t3tokens.hasMoreTokens())
        {
            upstatus("ERROR: One or more missing parameters.");
            return false;
        }
        else
        {
            return true;
        }
    }

    public boolean handleEvent (Event evt)
    {

        if (evt.id == Event.WINDOW_DESTROY)
        {
            System.exit(0);
        }
        else
        {
            if (evt.id == Event.SCROLL_ABSOLUTE ||
                    evt.id == Event.SCROLL_LINE_DOWN ||
                    evt.id == Event.SCROLL_LINE_UP ||
                    evt.id == Event.SCROLL_PAGE_DOWN ||
                    evt.id == Event.SCROLL_PAGE_UP)
            {

                if (evt.target == barSpeed)
                {
                    nSpeed = barSpeed.getValue() + 1;
                    labelSpeedStatus.setText(String.valueOf(nSpeed - 1));

                }
                return true;
            }
        }
        return super.handleEvent(evt);
    }	//end of handle event	

    /**
     * Initializes the applet.
     * 
     * @see #start
     * @see #stop
     * @see #destroy
     */
    public void init ()
    {
        super.init();
        setBackground(java.awt.Color.lightGray);
        setSize(800, 500);
        setForeground(java.awt.Color.blue);
        setLayout(new BorderLayout());
        scheduleAlgorithm = null;
        Q = new Vector(1, 1);
        title = new java.awt.Label();
        title.setFont(new java.awt.Font("dialog", 0, 18));
        title.setText("Processor Scheduling");

        String sampleP = "1\n2\n3\n4\n5";
        String sampleA = "0\n2\n4\n6\n8";
        String sampleS = "3\n6\n4\n5\n2";
        proc = new TextArea(5, 5);
        proc.setEditable(true);
        proc.append(sampleP);

        arriv = new TextArea(5, 5);
        arriv.setEditable(true);
        arriv.append(sampleA);

        serv = new TextArea(5, 5);
        serv.setEditable(true);
        serv.append(sampleS);

        alg = new Choice();
        alg.addItem("FCFS");
        alg.addItem("RR1");
        alg.addItem("RR4");
        alg.addItem("SPN");
        alg.addItem("SRT");

        barSpeed = new Scrollbar(Scrollbar.HORIZONTAL, 50, 0, 0, 101);
        barSpeed.setUnitIncrement(10);

        labelSpeedStatus = new Label("");
        labelSpeedStatus.setText("50");
        labelSpeed = new Label("Speed Bar");

        PrevBut = "";

        knobs = new Panel();
        knobs.setLayout(new FlowLayout(1));
        knobs.add(labelSpeed);
        knobs.add(labelSpeedStatus);
        knobs.add(barSpeed);
        knobs.add(alg);
        run = new Button("run");
        run.addActionListener(this);
        knobs.add(run);

        panel1 = new Panel();
        panel1.setLayout(new FlowLayout(1));
        panel1.add(title);

        panel2a = new Panel();
        panel2a.setLayout(new GridLayout(1, 3));
        panel2a.add(new Label("Process name"));
        panel2a.add(new Label("Arrival time"));
        panel2a.add(new Label("Service time"));

        panel2b = new Panel();
        panel2b.setLayout(new GridLayout(1, 1));
        panel2b.add(new Label("Statistics"));

        panel2 = new Panel();
        panel2.setLayout(new GridLayout(1, 2));
        panel2.add(panel2a);
        panel2.add(panel2b);

        panel3a = new Panel();
        panel3a.setLayout(new GridLayout(1, 3));
        panel3a.add(proc);
        panel3a.add(arriv);
        panel3a.add(serv);

        panel3b = new Panel();
        panel3b.setLayout(new GridLayout(1, 1));
        pad = new TextArea(5, 5);
        pad.setEditable(true);
        pad.setFont(new Font("Monospaced", 0, 10));
        panel3b.add(pad);

        panel3 = new Panel();
        panel3.setLayout(new GridLayout(1, 2));
        panel3.add(panel3a);
        panel3.add(panel3b);

        panel4 = new Panel();
        panel4.setLayout(new FlowLayout(3));
        panel4.add(new Label("Algorithm:"));
        algTitle = new Label("                                                            ");
        panel4.add(algTitle);

        panel5 = new Panel();
        panel5.setLayout(new FlowLayout(3));
        panel5.add(new Label("Status:   "));
        statusLine = new TextArea(4, 50);
        statusLine.setEditable(false);
        panel5.add(statusLine);


        Panel organize1 = new Panel();
        organize1.setLayout(new GridLayout(2, 0));
        organize1.add(panel1);
        organize1.add(panel2);

        Panel organize2 = new Panel();
        organize2.setLayout(new GridLayout(1, 2));
        organize2.add(panel4);
        organize2.add(panel5);

        Panel organize3 = new Panel();
        organize3.setLayout(new BorderLayout());
        organize3.add("North", organize1);
        organize3.add("Center", panel3);
        organize3.add("South", organize2);


        board = new graphCanvas(this);

        gr = board.getGraphics();

        add("North", organize3);
        add("Center", board);
        add("South", knobs);





    // insert code to initialize the applet here
    }

    /**
     * Paints the applet.
     * If the applet does not need to be painted (e.g. if it is only a container for other
     * awt components) then this method can be safely removed.
     * 
     * @param g  the specified Graphics window
     * @see #update
     */
    public void paint (Graphics g)
    {
        super.paint(g);

    // insert code to paint the applet here
    }

    public void paintBoard (Graphics g)
    {
        cleargrid();
        if (oldL != null)
        {
            setgrid(oldL);
            for (int i = 0; i < grid.size() && grid.elementAt(i) != null; i++)
            {
                int x = hbuf + hinc * (i - inc1);
                int y = 30 + ((Integer) grid.elementAt(i)).intValue() * vinc;
                upstatus("zzz "+ x + " " + y);
                gr.setColor(java.awt.Color.red);
                gr.fillRect(x, y, hinc, vinc);
            }

        }
    }

    public void report (Vector R, String title)
    {
        pad.append("\n" + title + "\n\n");
        out = R;
        display();
    }

    public void resetGUI ()
    {
        proc.setEditable(true);
        serv.setEditable(true);
        arriv.setEditable(true);

    }

    public int setgrid (Vector L)
    {
        if (oldL != L)
        {
            oldL = (Vector) L.clone();
        }
        process temp = (process) L.firstElement();
        hbuf = temp.getName().length();
        inc1 = temp.getArrival();
        inc2 = temp.getService();
        for (int j = 1; j < L.size(); j++)
        {
            temp = (process) L.elementAt(j);
            if (hbuf < temp.getName().length())
            {
                hbuf = temp.getName().length();
            }
            if (inc1 > temp.getArrival())
            {
                inc1 = temp.getArrival();
            }
            inc2 += temp.getService();
        }

        if (oldL != L)
        {
            grid = new Vector(inc2);
        }
        hbuf = hbuf * 5 + 10;
        inc2 += inc1;
        Dimension d = board.getSize();
        hinc = (d.width - 10) / ((inc2 - inc1) + 2);
        vinc = (d.height - 10) / (L.size() + 2);

        int c = 0;
        for (int j = inc1; j <= inc2;)
        {
            if ((j - inc1) % 5 == 0)
            {
                gr.drawString(String.valueOf(j), hbuf + c * hinc, 15);
            }
            gr.drawLine(hbuf + c * hinc, 15, hbuf + c * hinc, 20);
            j++;
            c++;
        }

        gr.drawLine(hbuf, 20, hbuf + (c - 1) * hinc, 20);
        for (int j = 0; j < L.size(); j++)
        {
            gr.drawString(((process) L.elementAt(j)).getName(), 5, 40 + j * vinc);
        }

        return inc1;
    }

    public void setTitle (String txt)
    {
        algTitle.setText(txt);
    }

    private String space (String x, int m)
    {
        for (; x.length() < m; x += " ");
        return x;
    }

    public void upstatus (String txt)
    {
        statusLine.append("\n" + txt);
    }

    public static void main (String[] args)
    {
        //... Create an initialize the applet.
        Applet theApplet = new pscheduling();
        theApplet.init();         // Needed if overridden in applet
        theApplet.start();        // Needed if overridden in applet

        //... Create a window (JFrame) and make applet the content pane.
        JFrame window = new JFrame("Sample Applet and Application");
        window.setContentPane(theApplet);
        window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        window.pack();              // Arrange the components.
        //System.out.println(theApplet.getSize());
        window.setVisible(true);    // Make the window visible.
    }
}
