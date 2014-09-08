import java.util.ArrayList;

/**
 * Created by pva701 on 9/8/14.
 */
public class ExpressionTree {
    private final int P = 57;
    private ExpressionTree left, right;
    private String op;
    private Character val;
    private int hash = 0;

    public ExpressionTree(ExpressionTree l,  ExpressionTree r, String o) {
        left = l;
        right = r;
        op = o;
        calcHash();
    }

    public ExpressionTree(Character vl) {
        val = vl;
        calcHash();
    }

    public String toString() {
        if (op == null)
            return "" + val;
        return "(" + left.toString() + op + right.toString() + ")";
    }
    public int getHash() {
        if (hash == 0)
            calcHash();
        return hash;
    }

    public int size() {
        int cnt = 0;
        if (left != null)
            cnt++;
        if (right != null)
            cnt++;
        return cnt;
    }

    public ExpressionTree son(int id) {
        int sz = size();
        if (id >= sz)
            return null;
        if (id == 0)
            return left;
        return right;
    }

    public char value() {
        return val;
    }

    public String operator() {
        return op;
    }

    private void calcHash() {
        if (left == null && right == null)
            hash = val - 'A' + 1;
        else if (right == null)
            hash = codeOfOperator()  + P * left.getHash();
        else if (left != null)
            hash = left.getHash() + P * codeOfOperator() + P * P * right.getHash();
        else
            throw new RuntimeException("Unexpected state of tree!");
    }

    private int codeOfOperator() {
        if (op.equals("!"))
            return 26;
        if (op.equals("|"))
            return 27;
        if (op.equals("&"))
            return 28;
        if (op.equals("->"))
            return 29;
        return 30;
    }
}
