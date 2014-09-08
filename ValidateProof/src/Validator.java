import java.util.HashMap;
import java.util.Map;

/**
 * Created by pva701 on 9/8/14.
 */
public class Validator {
    private final String INCORRECT = "Incorrect";
    private ExpressionTree[] rules;
    private ExpressionTree[] proof;
    private HashMap<Integer, Integer> facts = new HashMap<Integer, Integer>();

    public Validator(String[] r, String[] p) {
        rules = new ExpressionTree[r.length];
        proof = new ExpressionTree[p.length];
        for (int i = 0; i < r.length; ++i)
            rules[i] = new Parser(r[i]).parse();
        for (int i = 0; i < p.length; ++i)
            proof[i] = new Parser(p[i]).parse();
        //System.out.println(proof[1].toString());
        //System.out.println(proof[0].operator() + " " + proof[0].son(0).value() + " " + proof[0].son(1).operator());
    }

    public String[] validate() {
        String[] result = new String[proof.length];
        for (int i = 0; i < proof.length; ++i) {
            String res = checkStatement(proof[i]);
            result[i] = res;
            if (res.equals(INCORRECT))
                break;
            addFact(proof[i], i);
        }
        return result;
    }


    private void addFact(ExpressionTree fact, int number) {
        facts.put(fact.getHash(), number);
    }

    private String checkStatement(ExpressionTree statement) {
        if (facts.containsKey(statement.getHash()))
            return "Prev statement #" + facts.get(statement.getHash());

        for (int i = 0; i < rules.length; ++i) {
            HashMap <Character, Integer> hm = new HashMap<Character, Integer>();
            if (equalsTrees(rules[i], statement, hm))
                return "Rule #" + (i + 1);
        }
        String el = modusPon(statement);
        if (el.equals(""))
            return INCORRECT;
        return el;
    }

    private String modusPon(ExpressionTree statement) {
        for (Map.Entry<Integer, Integer> entry : facts.entrySet()) {
            Integer key = entry.getKey();
            Integer id = entry.getValue();
            ExpressionTree curTree = new ExpressionTree(proof[id], statement, "->");
            if (facts.containsKey(curTree.getHash()))
                return "MP " + (facts.get(curTree.getHash()) + 1) + " " + (id + 1);
        }
        return "";
    }

    private boolean equalsTrees(ExpressionTree rule, ExpressionTree line, HashMap <Character, Integer> hm) {
        if (rule.size() == 0) {
            if (!hm.containsKey(rule.value())) {
                hm.put(rule.value(), line.getHash());
            } else if (hm.get(rule.value()) != line.getHash())
                return false;
            return true;
        }

        if (rule.size() != line.size() || !rule.operator().equals(line.operator()))
            return false;
        for (int i = 0; i < rule.size(); ++i)
            if (!equalsTrees(rule.son(i), line.son(i), hm))
                return false;
        return true;
    }
}
