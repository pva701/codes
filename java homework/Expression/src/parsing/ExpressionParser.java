package parsing;

import exceptions.*;
import operations.*;
import types.INumber;

import java.util.StringTokenizer;

/**
 * Created by pva701 on 3/31/14.
 */

/*
<Expr>   := <Item> | {(+|-) <Item>}
<Item>   :=  <Mult> {(*|/) <Mult>}
<Mult>   := {~} {-} <Number> | <Const> | (<Expr>) | abs(<Expr>) | lb(<Expr>) | {^<Mult>}
<Number> := <Digit> | <Digit><Number>
<Digit>  := 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
<Const>  := x | y | z
 */

public class ExpressionParser <T extends INumber<T> > {
    private static String expressionString;
    private static String currentLexeme = "";
    private static StringTokenizer lexems;
    private static int parsingPosition = 0;
    private T instance;

    private static int getPositionError() {
        return parsingPosition - currentLexeme.length();
    }

    public Expression3<T> parse(String s, T instance)
            throws ExpressionParsingException, ArithmeticException {
        this.instance = instance;
        expressionString = s;
        lexems = new StringTokenizer(expressionString, "+-*/ ()~^", true);
        nextLexeme();
        Expression3<T> result = expr();
        if (!currentLexeme.isEmpty())
            throw new UnexpectedEndOfExpressionException("Unexpected end of expression at position " + getPositionError() + ".");
        return result;
    }

    private void nextLexeme() {
        currentLexeme = "";
        while (lexems.hasMoreTokens()) {
            currentLexeme = lexems.nextToken();
            parsingPosition += currentLexeme.length();
            currentLexeme = currentLexeme.replaceAll("\\s+", "");
            if (!currentLexeme.isEmpty())
                break;
            currentLexeme = "";
        }
        //System.out.println("cur = " + currentLexeme);
    }

    private Expression3<T> expr()
            throws  ExpressionParsingException, ArithmeticException {
        Expression3<T> curValue = item();
        while (currentLexeme.equals("+") || currentLexeme.equals("-")) {
            boolean plus  = currentLexeme.equals("+");
            nextLexeme();
            if (plus)
                curValue = new Add<T>(curValue, item());
            else
                curValue = new Subtract<T>(curValue, item());
        }
        return curValue;
    }

    private  Expression3<T> item()
            throws ExpressionParsingException, ArithmeticException {
        Expression3<T> curValue = mult();
        while (currentLexeme.equals("*") || currentLexeme.equals("/")) {
            boolean mul = currentLexeme.equals("*");
            nextLexeme();
            if (mul)
                curValue = new Multiply<T>(curValue, mult());
            else
                curValue = new Divide<T>(curValue, mult());
        }
        return curValue;
    }

    private Expression3<T> mult()
            throws ExpressionParsingException, ArithmeticException {
        Expression3<T> result;
        if (currentLexeme.equals("abs")) {
            nextLexeme();
            result = new Abs<T>(mult());
        } else if (currentLexeme.equals("lb")) {
            nextLexeme();
            result = new Lb<T>(mult());
        } else if (currentLexeme.equals("~")) {
            nextLexeme();
            result = new Not<T>(mult());
        } else if (currentLexeme.equals("-")) {
            nextLexeme();
            if (isConst(currentLexeme)) {
                result = new Const<T>(instance.createInstance("-" + currentLexeme));
                nextLexeme();
            } else
                result = new UnaryMinus<T>(mult());
        } else if (currentLexeme.equals("(")) {
            nextLexeme();
            result = expr();
            if (currentLexeme.equals(")"))
                nextLexeme();
            else
                throw new UnpairedBracketsException("Expression contains unpaired brackets at position " + getPositionError());
        } else if (isConst(currentLexeme)) {
            result = new Const<T>(instance.createInstance(currentLexeme));
            nextLexeme();
        } else if (isVariable(currentLexeme)) {
            result = new Variable<T>(currentLexeme);
            nextLexeme();
        }  else
            throw new UnexpectedTokenException("Expression contains incorrect token \"" + currentLexeme  + "\" at position " + getPositionError() + ".");

        if (currentLexeme.equals("^")) {
            nextLexeme();
            result = new Power<T>(result, mult());
        }
        return result;
    }

    private boolean isConst(String s) {
        try {
            instance.createInstance(s);
        } catch (UnexpectedTokenException e) {
            return false;
        }
        return true;
    }

    private static boolean isVariable(String s) {
        return s.equals("x") || s.equals("y") || s.equals("z");
    }
}