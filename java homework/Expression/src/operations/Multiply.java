package operations;

import exceptions.OverflowArithmeticException;
import operations.BinaryOperation;
import operations.Expression3;
import types.INumber;

/**
 * Created by pva701 on 3/15/14.
 */
public class Multiply<T extends INumber<T> > extends BinaryOperation<T> {

    public Multiply(Expression3<T> xx, Expression3<T> yy) {
        super(xx, yy);
    }

    @Override
    T op(T valX, T valY) throws OverflowArithmeticException {
        return valX.mul(valY);
    }

    @Override
    public String signOperation() {
        return "*";
    }
}
