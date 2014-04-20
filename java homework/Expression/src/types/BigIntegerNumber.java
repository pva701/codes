package types;
import exceptions.*;
import java.math.BigInteger;

/**
 * Created by pva701 on 4/19/14.
 */

public class BigIntegerNumber extends INumber<BigIntegerNumber> {
    final static BigInteger TWO = new BigInteger("2", 10);

    public BigInteger value;
    public BigIntegerNumber() {}
    public BigIntegerNumber(BigInteger x) {
        value = x;
    }
    public BigIntegerNumber(String s) throws UnexpectedTokenException {
        parseNumber(s);
    }

    public void parseNumber(String s) throws UnexpectedTokenException {
        boolean isNegative = false;
        if (!s.isEmpty() && s.charAt(0) == '-') {
            s = s.substring(1);
            isNegative = true;
        }

        boolean allDigits = true;
        for (int i = 0; i < s.length(); ++i)
            allDigits &= Character.isDigit(s.charAt(i));

        if (!allDigits || s.isEmpty())
            throw new UnexpectedTokenException("Unexpected token.");
        value = new BigInteger(s, 10);
        if (isNegative)
            value = value.negate();
    }

    @Override
    public BigIntegerNumber add(BigIntegerNumber x)  {
        return new BigIntegerNumber(value.add(x.value));
    }

    @Override
    public BigIntegerNumber sub(BigIntegerNumber x) {
        return new BigIntegerNumber(value.subtract(x.value));
    }

    @Override
    public BigIntegerNumber mul(BigIntegerNumber x) {
        return new BigIntegerNumber(value.multiply(x.value));
    }

    @Override
    public BigIntegerNumber div(BigIntegerNumber x) throws DivisionByZeroArithmeticException {
        if (x.value.compareTo(BigInteger.ZERO) == 0)
            throw new DivisionByZeroArithmeticException("Division by zero in expression " + value + "/" + x.value + ".");
        return new BigIntegerNumber(value.divide(x.value));
    }

    @Override
    public BigIntegerNumber not() {
        return new BigIntegerNumber(value.not());
    }

    @Override
    public BigIntegerNumber unary() {
        return new BigIntegerNumber(value.negate());
    }

    @Override
    public BigIntegerNumber power(BigIntegerNumber x) throws NegativePowerArithmeticException {
        if (x.value.compareTo(BigInteger.ZERO) < 0)
            throw new NegativePowerArithmeticException("Invalid exponent in expression " + value + "^" + x.value + ".");
        BigInteger val = value;
        BigInteger f = BigInteger.ONE;
        while (x.value.compareTo(BigInteger.ZERO) == 1) {
            if (x.value.testBit(0))
                f = f.multiply(val);
            val = val.multiply(val);
            x.value = x.value.divide(TWO);
        }
        return new BigIntegerNumber(f);
    }

    @Override
    public BigIntegerNumber abs() {
        return new BigIntegerNumber(value.abs());
    }

    @Override
    public BigIntegerNumber lb() throws LbInvalidArgumentException {
        BigInteger cur = value;
        if (cur.compareTo(BigInteger.ZERO) <= 0)
            throw new LbInvalidArgumentException("Invalid lb argument is " + cur + ".");
        return new BigIntegerNumber(new BigInteger("" + (value.bitLength() - 1), 10));
    }

    @Override
    public BigIntegerNumber createInstance(String s) throws UnexpectedTokenException {
        return new BigIntegerNumber(s);
    }
}