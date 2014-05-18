public class SplayTree {
    //static final int INF = 1000000000;
    //public static final int RADIUS = GraphicsNode.RADIUS;
    private final int HEIGHT_BETWEEN_LEVELS = 3 * Node.RADIUS;
    private final Point2D ROOT_CENTER = new Point2D(Information.WIDTH / 2, 50);

    private NodeTablet tablet;

    public SplayTree(NodeTablet t) {
        tablet = t;
    }

    protected Node root;
    protected void rotate(Node v2) {
        Node v1 = v2.parent;
        if (v1.left == v2) {
            v2.setParent(v1.parent);
            if (v2.right != null)
                v2.right.setParent(v1);
            else
                v1.left = null;
            v1.setParent(v2);
        } else {
            v2.setParent(v1.parent);
            if (v2.left != null)
                v2.left.setParent(v1);
            else
                v1.right = null;
            v1.setParent(v2);
        }
    }

    protected void zig(Node v) {
        rotate(v);
    }

    protected void zigzag(Node v) {
        rotate(v);
        rotate(v);
    }

    protected void zigzig(Node v) {
        rotate(v.parent);
        rotate(v);
    }

    protected Node splay(Node v) {
        return v;//TODO write splay
        /*if (v.parent == null)
            return v;
        if (v.parent.parent == null)
            zig(v);
        else if (v.parent.parent.left != null && v.parent.parent.left.left == v ||
                v.parent.parent.right != null && v.parent.parent.right.right == v)
            zigzig(v);
        else
            zigzag(v);
        return splay(v);*/
    }

    protected Node insert(Node v, int value) {
        Node result = null;
        if (v == null)
            return root = new Node(value, ROOT_CENTER, new Point2D(0, Information.WIDTH), tablet);//good

        v.repaintNode(v.activeVertexColor);
        try {
            Thread.sleep(500);
        } catch (InterruptedException e) {
            System.out.println("interrupted main thread");
        }

        if (value < v.value) {
            if (v.left == null) {
                if (v.bounds.x == 0)
                    result = v.left = new Node(value, v, v.center.add(new Point2D(-Node.RADIUS, HEIGHT_BETWEEN_LEVELS)), v.bounds.add(new Point2D(0, -Node.RADIUS)), tablet);
                else {
                    Node basicVertex = v.getBasicVertex();
                    System.out.println("MAIN THREAD = " + Thread.currentThread());
                    basicVertex.shift(v.center.x - 1);

                    /*try {
                        basicVertex.t.join();
                    } catch (InterruptedException e) {
                        System.out.println("interrupted left son");
                    }*/

                    Point2D newCenter = v.center.add(new Point2D(-Node.RADIUS, HEIGHT_BETWEEN_LEVELS));
                    Point2D newBounds = new Point2D(v.center.x - 2 * Node.RADIUS, v.center.x);
                    result = v.left = new Node(value, v, newCenter, newBounds, tablet);
                }
            } else
                result = insert(v.left, value);
        } else if (value > v.value) {
            if (v.right == null) {
                if (v.bounds.y == Information.WIDTH) {
                    result = v.right = new Node(value, v, v.center.add(new Point2D(Node.RADIUS, HEIGHT_BETWEEN_LEVELS)), v.bounds.add(new Point2D(Node.RADIUS, 0)), tablet);
                } else {
                    Node basicVertex = v.getBasicVertex();
                    basicVertex.shift(v.center.x + 1);
                    /*try {
                        basicVertex.t.join();
                    } catch (InterruptedException e) {
                        System.out.println("interrupted right son");
                    }*/
                    Point2D newCenter = v.center.add(new Point2D(Node.RADIUS, HEIGHT_BETWEEN_LEVELS));
                    Point2D newBounds = new Point2D(v.center.x, v.center.x + 2 * Node.RADIUS);
                    result = v.right = new Node(value, v, newCenter, newBounds, tablet);
                }
            } else
                result = insert(v.right, value);
        }
        v.repaintNode(v.defaultColorNode);
        return result;//TODO here result
    }

    protected Node find(Node v, int value) {
        if (v == null)
            return null;
        if (value < v.value)
            return find(v.left, value);
        if (value > v.value)
            return find(v.right, value);
        return v;
    }

    protected Node merge(Node l, Node r) {
        if (l != null) l.parent = null;
        if (r != null) r.parent = null;

        if (l == null)
            return r;
        if (r == null)
            return l;

        Node v = l;
        while (v.right != null)
            v = v.right;
        splay(v);
        r.setParent(v);
        return v;
    }

    protected Pair<Node> split(Node v, int key) {
        Node l = null, r = null;
        Node fndKey = find(v, key);
        boolean inserted = false;
        if (fndKey == null) {
            //fndKey = insert(v, key); TODO writing
            inserted = true;
        }
        splay(fndKey);
        r = fndKey.right;
        fndKey.right = null;
        l = fndKey;
        if (inserted) {
            l = l.left;
        }

        if (r != null)
            r.parent = null;
        if (l != null)
            l.parent = null;
        return new Pair<Node>(l, r);
    }

    SplayTree() {
        root = null;
    }

    public void insert(int x) {
        ///root = splay(insert(root, x)); TODO root = splay(insert(root, x));
        insert(root, x);
    }

    public boolean find(int x) {
        Node vert = find(root, x);
        if (vert != null)
            root = splay(vert);
        return vert != null;
    }

    public void remove(int x) {
        Node v = find(root, x);
        if (v == null)
            return;
        root = splay(v);
        if (v.left != null)
            v.left.parent = null;
        if (v.right != null)
            v.right.parent = null;
        root = merge(v.left, v.right);
    }

    /*TODO add this code

    public int prev(int value) {
        --value;
        Pair <Node> retSplit = split(root, value);
        Node L = retSplit.first;
        Node R = retSplit.second;

        Node cur = L;
        int res = -INF;
        if (cur != null) {
            while (cur.right != null)
                cur = cur.right;
            res = cur.value;
        }
        root = merge(L, R);
        return res;
    }

    public int next(int value) {
        Pair <Node> retSplit = split(root, value);
        Node L = retSplit.first;
        Node R = retSplit.second;
        Node cur = R;
        int res = -INF;
        if (cur != null) {
            while (cur.left != null)
                cur = cur.left;
            res = cur.value;
        }
        root = merge(L, R);
        return res;
    }*/
}