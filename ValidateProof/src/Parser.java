import java.util.ArrayList;

/**
 * Created by pva701 on 9/8/14.
 */
public class Parser {
    private String nextLexeme;
    private int i = 0;
    private String s;

    public Parser(String s) {
        this.s = s;
        nextLexeme = getNextLexeme();
    }

    public ExpressionTree parse() {
        return expr();
    }

    private String getNextLexeme() {
        if (i == s.length())
            return "";
        if (s.charAt(i) == '-') {
            i += 2;
            return "->";
        }

        i++;
        return "" + s.charAt(i - 1);
    }

    private ExpressionTree expr() {
        ArrayList<ExpressionTree> sums = new ArrayList<ExpressionTree>();
        sums.add(sum());
        while (nextLexeme.equals("->")) {
            nextLexeme = getNextLexeme();
            sums.add(sum());
        }
        ExpressionTree current = sums.get(sums.size() - 1);
        for (int i = sums.size() - 2; i >= 0; --i)
            current = new ExpressionTree(sums.get(i), current, "->");
        return current;

    }

    private ExpressionTree sum() {
        ArrayList <ExpressionTree> items = new ArrayList<ExpressionTree>();
        items.add(item());
        while (nextLexeme.equals("|")) {
            nextLexeme = getNextLexeme();
            items.add(item());
        }
        ExpressionTree current = items.get(items.size() - 1);
        for (int i = items.size() - 2; i >= 0; --i)
            current = new ExpressionTree(items.get(i), current, "|");
        return current;
    }

    private ExpressionTree item() {
        ArrayList <ExpressionTree> mults = new ArrayList<ExpressionTree>();
        mults.add(mult());
        while (nextLexeme.equals("&")) {
            nextLexeme = getNextLexeme();
            mults.add(item());
        }
        ExpressionTree current = mults.get(mults.size() - 1);
        for (int i = mults.size() - 2; i >= 0; --i)
            current = new ExpressionTree(mults.get(i), current, "&");
        return current;
    }

    private ExpressionTree mult() {
        ExpressionTree res;
        if (nextLexeme.equals("!")) {
            nextLexeme = getNextLexeme();
            res = new ExpressionTree(mult(), null, "!");
        } else if (nextLexeme.equals("(")) {
            nextLexeme = getNextLexeme();
            res = expr();
            if (nextLexeme.equals(")"))
                nextLexeme = getNextLexeme();
            else
                throw new RuntimeException("Expression contains unpaired brackets!");
        } else if (Character.isLetter(nextLexeme.charAt(0))) {
            res = new ExpressionTree(nextLexeme.charAt(0));
            nextLexeme = getNextLexeme();
        } else
            throw new RuntimeException("Unexpected symbol!");
        return res;
    }
}
