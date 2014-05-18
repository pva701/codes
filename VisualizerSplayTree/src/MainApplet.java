/*
 * Created by pva701 on 4/14/14.
 */

/*
<applet code="MainApplet" width=500 height=500>
</applet>
 */

import java.applet.Applet;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Random;


public class MainApplet extends Applet {
    SplayTree tree;
    Random rnd = new Random(340);//343

    public MainApplet() {
        setLayout(null);
    }

    @Override
    public void init() {
        //setLayout(null);
        resize(Information.WIDTH, Information.HEIGHT);
        NodeTablet tablet = new NodeTablet();
        tree = new SplayTree(tablet);
        add(tablet);
        tablet.init();

        tablet.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                System.out.println("tablet.mouseClicked");
                int x = rnd.nextInt() % 100;
                if (x < 0) x *= -1;
                System.out.println(x);
                tree.insert(x);
            }
        });

        tablet.setBackground(new Color(255, 255, 255));
        Information.mainApplet = this;
    }

    @Override
    public void start() {

    }

    @Override
    public void paint(Graphics g) {
        System.out.println("MainApplet.paint");
        showStatus("hello, niger!");
    }

    public static void main(String[] args) {}
}
