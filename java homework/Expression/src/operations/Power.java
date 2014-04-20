package operations;

import exceptions.NegativePowerArithmeticException;
import exceptions.OverflowArithmeticException;
import types.INumber;

/**
 * Created by pva701 on 4/15/14.
 */
public class Power<T extends INumber<T> > extends BinaryOperation<T> {
    public Power(Expression3<T> xx, Expression3<T> yy) {
        super(xx, yy);
    }

    @Override
    T op(T valX, T valY) throws OverflowArithmeticException, NegativePowerArithmeticException {
        return valX.power(valY);
    }

    @Override
    public String signOperation() {
        return "+";
    }
}
