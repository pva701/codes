package types;

import exceptions.*;

/**
 * Created by pva701 on 4/19/14.
 */
public class DoubleNumber extends INumber<DoubleNumber> {
    public Double value;

    public DoubleNumber() {}
    public DoubleNumber(Double val) {
        value = val;
    }
    public DoubleNumber(String s) throws OverflowArithmeticException, UnexpectedTokenException {
        parseNumber(s);
    }

    public void parseNumber(String s) throws OverflowArithmeticException, UnexpectedTokenException {
        try {
            value = Double.parseDouble(s);
            if (value.isInfinite())
                throw new OverflowArithmeticException("Overflow!");
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

    @Override
    public DoubleNumber add(DoubleNumber x) throws OverflowArithmeticException {
        Double r = value;
        r += x.value;
        if (r.isInfinite())
            throw new OverflowArithmeticException("Overflow in expression " + value + "+" + x + ".");
        return new DoubleNumber(r);
    }

    @Override
    public DoubleNumber sub(DoubleNumber x) throws OverflowArithmeticException {
        Double r = value;
        r -= x.value;
        if (r.isInfinite())
            throw new OverflowArithmeticException("Overflow in expression " + value + "+" + x + ".");
        return new DoubleNumber(r);
    }

    @Override
    public DoubleNumber mul(DoubleNumber x) throws OverflowArithmeticException {
        Double r = value;
        r *= x.value;
        if (r.isInfinite())
            throw new OverflowArithmeticException("Overflow in expression " + value + "+" + x + ".");
        return new DoubleNumber(r);
    }

    @Override
    public DoubleNumber div(DoubleNumber x) throws OverflowArithmeticException, DivisionByZeroArithmeticException {
        Double r = value;
        r /= x.value;
        if (r.isInfinite())
            throw new OverflowArithmeticException("Overflow in expression " + value + "/" + x.value + ".");
        if (r.isNaN())
            throw new DivisionByZeroArithmeticException("Division by zero in expression " + value + "/" + x.value + ".");
        return new DoubleNumber(r);
    }

    @Override
    public DoubleNumber not() throws UnsupportedOperationException {
        throw new UnsupportedOperationException("Unsupported operation ~ for Double.");
    }

    @Override
    public DoubleNumber unary() throws OverflowArithmeticException {
        Double r = value;
        r = -r;
        if (r.isInfinite())
            throw new OverflowArithmeticException("Overflow in expression -" + value + ".");
        return new DoubleNumber(r);
    }

    @Override
    public DoubleNumber power(DoubleNumber x) throws OverflowArithmeticException {
        Double r = Math.pow(value, x.value);
        if (r.isInfinite())
            throw new OverflowArithmeticException("Overflow in expression " + value + "^" + x.value);
        return new DoubleNumber(r);
    }

    @Override
    public DoubleNumber abs() throws OverflowArithmeticException {
        Double r = Math.abs(value);
        if (r.isInfinite())
            throw new OverflowArithmeticException("Overflow in expression abs(" + value + ").");
        return new DoubleNumber(r);
    }

    @Override
    public DoubleNumber lb() throws LbInvalidArgumentException {
        if (value <= 0)
            throw new LbInvalidArgumentException("Invalid lb argument is " + value + ".");
        return new DoubleNumber(Math.log(value) / Math.log(2.0));
    }

    @Override
    public DoubleNumber createInstance(String s) throws OverflowArithmeticException, UnexpectedTokenException{
        return new DoubleNumber(s);
    }
}
