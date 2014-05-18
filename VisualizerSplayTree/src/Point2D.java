import java.awt.*;

/**
 * Created by pva701 on 4/25/14.
 */
public class Point2D extends Point {
    public Point2D(int x, int y) {
        super(x, y);
    }

    public Point2D add(Point2D b) {
        return new Point2D(x + b.x, y + b.y);
    }

    public Point2D sub(Point2D b) {
        return new Point2D(x - b.x, y - b.y);
    }

    public Point2D setX(int valX) {
        return new Point2D(valX, y);
    }

    public Point2D setY(int valY) {
        return new Point2D(x, valY);
    }

    public int diffXY() {
        return y - x;
    }

    public boolean eq(Point2D b) {
        return x == b.x && y == b.y;
    }

    public double len() {
        return Math.sqrt(x * x + y * y);
    }
}
