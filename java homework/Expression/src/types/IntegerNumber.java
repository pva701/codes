package types;

import exceptions.*;

/**
 * Created by pva701 on 4/19/14.
 */

public class IntegerNumber extends INumber<IntegerNumber> {
    public Integer value;

    public IntegerNumber() {}

    public IntegerNumber(String s) throws OverflowArithmeticException, UnexpectedTokenException {
        parseNumber(s);
    }

    public IntegerNumber(Integer x) {
        value = x;
    }

    public void parseNumber(String s) throws OverflowArithmeticException, UnexpectedTokenException {
        try {
            value = Integer.parseInt(s);
        } catch (NumberFormatException e) {

            if (!s.isEmpty() && s.charAt(0) == '-')
                s = s.substring(1);
            boolean allDigits = true;
            for (int i = 0; i < s.length(); ++i)
                allDigits &= Character.isDigit(s.charAt(i));
            if (allDigits && !s.isEmpty())
                throw new OverflowArithmeticException("Overflow!");
            throw new UnexpectedTokenException("Unexpected token.");
        }
    }

    private boolean isOverflow(long res) {
        return Integer.MIN_VALUE > res || res > Integer.MAX_VALUE;
    }

    //OVERIDING
    @Override
    public IntegerNumber add(IntegerNumber x) throws OverflowArithmeticException {
        long res = (long)value + (long)x.value;
        if (!isOverflow(res))
            return new IntegerNumber((int)res);
        throw new OverflowArithmeticException("Overflow in expression " + value + "+" + x + ".");
    }

    @Override
    public IntegerNumber sub(IntegerNumber x) throws OverflowArithmeticException{
        long res = (long)value - (long)x.value;
        if (!isOverflow(res))
            return new IntegerNumber((int)res);
        throw new OverflowArithmeticException("Overflow in expression " + value + "/" + x + ".");
    }

    @Override
    public IntegerNumber mul(IntegerNumber x) throws OverflowArithmeticException{
        long res = (long)value * (long)x.value;
        if (!isOverflow(res))
            return new IntegerNumber((int)res);
        throw new OverflowArithmeticException("Overflow in expression " + value + "*" + x.value + ".");
    }

    @Override
    public IntegerNumber div(IntegerNumber x) throws OverflowArithmeticException, DivisionByZeroArithmeticException{
        if (x.value == 0)
            throw new DivisionByZeroArithmeticException("Division by zero in expression " + value + "/" + x.value + ".");
        long res = (long)value / (long)x.value;
        if (!isOverflow(res))
            return new IntegerNumber((int)res);
        throw new OverflowArithmeticException("Overflow in expression " + value + "/" + x.value + ".");
    }

    @Override
    public IntegerNumber not() {
        return new IntegerNumber(~value);
    }

    @Override
    public IntegerNumber unary() {
        long res = -(long)value;
        if (!isOverflow(res))
            return new IntegerNumber(-value);
        throw new OverflowArithmeticException("Overflow in expression -" + value);
    }

    @Override
    public IntegerNumber power(IntegerNumber x) throws OverflowArithmeticException, NegativePowerArithmeticException{
        if (x.value < 0)
            throw new NegativePowerArithmeticException("Invalid exponent in expression " + value + "^" + x.value + ".");
        long f = 1;
        long t = value;
        int y = x.value;
        while (y > 0) {
            if (y % 2 == 1)
                f = f * t;
            t = t * t;
            if (isOverflow(f))
                throw new OverflowArithmeticException("Overflow in expression " + value + "^" + x.value + ".");
            y >>= 1;
        }
        return new IntegerNumber((int)f);
    }

    @Override
    public IntegerNumber abs() throws OverflowArithmeticException{
        long res = value;
        if (res < 0)
            res = -res;
        if (Integer.MIN_VALUE <= res && res <= Integer.MAX_VALUE)
            return new IntegerNumber((int)res);
        throw new OverflowArithmeticException("Overflow in expression abs(" + value + ").");
    }

    @Override
    public IntegerNumber lb() {
        int cur = value;
        if (cur <= 0)
            throw new LbInvalidArgumentException("Invalid lb argument is " + cur + ".");
        int i = 0;
        while (cur > 1) {
            ++i;
            cur /= 2;
        }
        return new IntegerNumber(i);
    }

    @Override
    public IntegerNumber createInstance(String s) throws OverflowArithmeticException, UnexpectedTokenException {
        return new IntegerNumber(s);
    }
}
