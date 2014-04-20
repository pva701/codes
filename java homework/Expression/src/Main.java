import parsing.ExpressionParser;
import types.BigIntegerNumber;
import types.DoubleNumber;
import types.IntegerNumber;

/**
 * Created by pva701 on 3/15/14.
 */

public class Main {
    final static int NUMBER_OF_ARGUMENT = 5;
    public static void main(String[] args) {
        if (args.length != NUMBER_OF_ARGUMENT) {
            System.out.println(String.format("You must input %d arguments!", NUMBER_OF_ARGUMENT));
            System.exit(0);
        }
        String option = args[0];
        String s = args[1];
        try {
            if (option.equals("-i")) {
                IntegerNumber x = new IntegerNumber(args[2]);
                IntegerNumber y = new IntegerNumber(args[3]);
                IntegerNumber z = new IntegerNumber(args[4]);
                System.out.println(new ExpressionParser<IntegerNumber>().parse(s, new IntegerNumber()).evaluate(x, y, z).value);
            } else if (option.equals("-d")) {
                DoubleNumber x = new DoubleNumber(args[2]);
                DoubleNumber y = new DoubleNumber(args[3]);
                DoubleNumber z = new DoubleNumber(args[4]);
                System.out.println(new ExpressionParser<DoubleNumber>().parse(s, new DoubleNumber()).evaluate(x, y, z).value);
            } else if (option.equals("-bi")) {
                BigIntegerNumber x = new BigIntegerNumber(args[2]);
                BigIntegerNumber y = new BigIntegerNumber(args[3]);
                BigIntegerNumber z = new BigIntegerNumber(args[4]);
                System.out.println(new ExpressionParser<BigIntegerNumber>().parse(s, new BigIntegerNumber()).evaluate(x, y, z).value);
            } else {
                System.out.println("Unexpected option!");
                System.exit(0);
            }
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
}
/*
test("x", 1, 1, 1);
        test("", 0, 0, 0);
        test("1 1", 1, 1, 1);
        test("x  *  \n  z + -y", 1, 2, 3);
        test("1000*1000*1000*x", 1, 0, 0);
        test("1000*1000*1000*x", 2, 0, 0);
        test("1000*1000*1000*x", 3, 0, 0);
        test("x / ----y", 0, 0, 1);
        test("x / y", Integer.MIN_VALUE, -1, 0);
        test("x*x*x      *   x*x*x*x * x * x *x", 0, 1, 1);
        test("x*x*x      *   x*x*x*x * x * x *x", 1, 1, 1);
        test("x*x*x      *   x*x*x*x * x * x *x", 2, 1, 1);
        test("x*x*x      *   x*x*x*x * x * x *x", 8, 1, 1);
        test("x*x*x      *   x*x*x*x * x * x *x", 9, 1, 1);
        test("x*x*x      *   x*x*x*x * x * x *x", 10, 1, 1);
        test("x*x*x      *   x*x*x*x * x * x * 16 + 0 * z", -8, 1, Integer.MIN_VALUE);
        test("x+y +      0 * x * x * x * x * y + -z", Integer.MIN_VALUE, Integer.MAX_VALUE, -1);
        test("3000000000", 0, 0, 0);
        test("00-0000000000000010", 0, 0, 0);

1
Expression contains incorrect tokens!
Unexpected end of expression!
1
1000000000
2000000000
overflow
division by zero
overflow
0
1
1024
1073741824
overflow
overflow
-2147483648
0
overflow
-10
 */
