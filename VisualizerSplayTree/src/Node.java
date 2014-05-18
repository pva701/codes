import java.awt.*;

/**
 * Created by pva701 on 4/29/14.
 */
class Node {//implements Runnable{
    //Graphic
    static final int RADIUS = 20;
    public Thread t;
    private int radius = RADIUS;
    public Point2D center;
    public int weight;
    public Color defaultColorNode = Color.YELLOW;
    public Color activeVertexColor = Color.RED;
    public Color currentColor;
    public Point2D bounds;
    public Point2D previosCenter;
    //private Image currentPicture;
    enum TypeRun {Shift, Fading}
    TypeRun state;
    Point2D toPoint;

    //Structure tree
    private static final Point2D RIGHT_SHIFT_CENTER = new Point2D(RADIUS, 0);
    private static final Point2D LEFT_SHIFT_CENTER = new Point2D(-RADIUS, 0);
    protected Node parent, left, right;
    protected int value;
    protected NodeTablet table;

    public Node(int val, Point2D c, Point2D bnd, NodeTablet tbl) {
        center = c;
        table = tbl;
        value = val;
        weight = 2 * RADIUS;
        bounds = bnd;
        //t = new Thread(this);
        previosCenter = c;
        repaintNode(defaultColorNode);
    }

    public Node(int val, Node par, Node lef, Node rig, Point2D c, Point2D bnd, NodeTablet tbl) {
        center = c;
        table = tbl;
        value = val;
        left = lef;
        right = rig;
        parent = par;
        weight = 2 * RADIUS;
        bounds = bnd;
        //t = new Thread(this);
        previosCenter = c;
        repaintNode(defaultColorNode);
    }

    public Node(int val, Node par, Point2D c, Point2D bnd, NodeTablet tbl) {
        center = c;
        table = tbl;
        value = val;
        parent = par;
        weight = 2 * RADIUS;
        bounds = bnd;
        //t = new Thread(this);
        previosCenter = c;
        repaintNode(defaultColorNode);
    }

    public void setParent(Node p) {
        this.parent = p;
        if (p != null) {
            if (this.value <= p.value)
                p.left = this;
            else
                p.right = this;
        }
    }

    public Node getBasicVertex() {
        Node cur = this;
        while (cur.bounds.x != 0 && cur.bounds.y != table.getWidth())
            cur = cur.parent;
        return cur;
    }

    public void shift(int newPosX) {
        if (bounds.x == 0)
            Shift(LEFT_SHIFT_CENTER, newPosX);
        else
            Shift(RIGHT_SHIFT_CENTER, newPosX);
    }

    private void Shift(Point2D s, int newPosX) {
        Point2D currentS;
        if (s.x * (center.x - newPosX) > 0)
            currentS = s;
        else
            currentS = new Point2D(0, 0);

        bounds = bounds.add(new Point2D(currentS.x, currentS.x));
        if (bounds.x < 0) bounds.x = 0;
        if (bounds.y > table.getWidth()) bounds.y = table.getWidth();
        runMotionLine(currentS);

        if (s.x < 0) {
            if (left != null)
                left.Shift(s, newPosX);
            if (right != null)
                right.Shift(s, newPosX);
        } else {
            if (right != null)
                right.Shift(s, newPosX);
            if (left != null)
                left.Shift(s, newPosX);
        }

        previosCenter = center;
    }

    private void drawEdge(Point2D a, Point2D b, Graphics g1) {
        Point2D vc = a.sub(b);
        double dx = 1.0 * Node.RADIUS / vc.len() * vc.x;
        double dy = 1.0 * Node.RADIUS / vc.len() * vc.y;

        Graphics2D g = (Graphics2D)g1;
        g.setStroke(new BasicStroke(2));
        g.setColor(Color.BLACK);

        g.drawLine((int)(b.x + dx), (int)(b.y + dy), (int)(a.x - dx), (int)(a.y - dy));
    }

    private void clearEdge(Point2D a, Point2D b, Graphics g1) {
        Graphics2D g = (Graphics2D)g1;
        g.setStroke(new BasicStroke(2));
        g.setColor(Color.WHITE);
        Point2D vc = a.sub(b);
        double dx = 1.0 * Node.RADIUS / vc.len() * vc.x;
        double dy = 1.0 * Node.RADIUS / vc.len() * vc.y;
        g.drawLine((int)(b.x + dx), (int)(b.y + dy), (int)(a.x - dx), (int)(a.y - dy));
    }

    public void repaintNode(Color colorNode) {
        currentColor = colorNode;

        Image im = table.createImage(2 * radius, 2 * radius);
        im.getGraphics().setColor(new Color(0, 0, 0, 0));
        drawOnContext(im.getGraphics(), 0, 0);
        table.draw(im, center.x - radius, center.y - radius);

        Graphics tg = table.getGraphics();
        if (parent != null)
            drawEdge(parent.center, center, tg);
        if (left != null)
            drawEdge(left.center, center, tg);
        if (right != null)
            drawEdge(right.center, center, tg);
    }

    public void repaintNodeWithoutChildren(Color colorNode) {
        currentColor = colorNode;

        Image im = table.createImage(2 * radius, 2 * radius);
        im.getGraphics().setColor(new Color(0, 0, 0, 0));
        drawOnContext(im.getGraphics(), 0, 0);
        table.draw(im, center.x - radius, center.y - radius);

        Graphics tg = table.getGraphics();
        if (parent != null)
            drawEdge(parent.center, center, tg);
    }

    private void drawOnContext(Graphics g, int shiftx, int shifty) {
        g.fillOval(shiftx, shifty, 2 * radius, 2 * radius);
        g.setColor(currentColor);
        g.fillOval(shiftx + 2, shifty + 2, 2 * (radius - 2), 2 * (radius - 2));
        g.setColor(Color.BLACK);
        g.setFont(new Font("Number on node", Font.ITALIC, 20));
        if (value >= 10)
            g.drawString(Integer.toString(value), 7 + shiftx, 27 + shifty);
        else
            g.drawString(Integer.toString(value), 13 + shiftx, 28 + shifty);
    }

    private void move(int sx, int sy) {
        Graphics g = table.getGraphics();
        g.setColor(Color.WHITE);
        g.fillOval(center.x - radius, center.y - radius, 2 * radius, 2 * radius);
        if (parent != null)
            clearEdge(parent.previosCenter, center, g);

        center = center.add(new Point2D(sx, sy));
        /*try {
            parent.t.join();
        } catch (InterruptedException e) {
            System.out.println("interrupted join parent");
        }*/
        repaintNodeWithoutChildren(currentColor);
    }

    public void runMotionLine(Point2D s) {
        toPoint = center.add(s);
        state = TypeRun.Shift;
        run();
        //t = new Thread(this);
        //t.start();
    }

    public void runFading() {
        state = TypeRun.Fading;
        t.run();
    }

    //@Override
    public void run() {

        if (state == TypeRun.Shift) {
            final int PERIOD_SLEEP = 1000;//ms
            final int TOTAL_TIME = 100;//ms
            previosCenter = center;
            int iters = 1;//TOTAL_TIME / PERIOD_SLEEP;
            int sx = (toPoint.x - center.x) / iters;
            int sy = (toPoint.y - center.y) / iters;
            for (int i = 0; i < iters; ++i) {
                move(sx, sy);

                /*try {
                    System.out.println(Thread.currentThread());
                    Thread.sleep(PERIOD_SLEEP);
                } catch (InterruptedException e) {
                    System.out.println("interrupted shift vertex");
                }*/
            }
            //t.interrupt();////TODO ???
        }
    }

//    @Override
//    public void run() {
//        while (true) {
//            try {
//                Thread.sleep(50);
//            } catch (InterruptedException e) {
//                System.out.println("interrupted " + t.getName());
//            }
//            center = center.add(new Point2D(10, 10));
//            //setBounds(center.x, center.y, 2 * radius, 2 * radius);
//            table.draw(currentPicture, center);
//            //System.out.println(center.x + " " + center.y);
//        }
//    }

}