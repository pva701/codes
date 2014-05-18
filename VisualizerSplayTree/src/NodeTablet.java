import java.awt.*;

/**
 * Created by pva701 on 4/26/14.
 */
public class NodeTablet extends Canvas {

    public NodeTablet() {
    }

    public void init() {
        Dimension sz = getParent().getSize();
        setBounds(0, 0, sz.width, sz.height);
    }

    public void draw(Image im, Point2D p) {
        draw(im, p.x, p.y);
    }

    private int x, y;
    Image im;

    public void draw(Image im, int posX, int posY) {
        x = posX;
        y = posY;
        this.im = im;
        paint(getGraphics());
    }

    @Override
    public void paint(Graphics g) {
        g.drawImage(im, x, y, null);
    }
}
